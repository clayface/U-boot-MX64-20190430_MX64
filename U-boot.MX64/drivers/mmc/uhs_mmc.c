/*
 * uhs_mmc.c
 *
 *  Created on: Aug 11, 2013
 *      Author: shengw
 */

/*
 * Copyright (C) 2008-2011 Freescale Semiconductor, Inc.
 * Terry Lv
 *
 * Copyright 2008, Freescale Semiconductor, Inc
 * Andy Fleming
 *
 * Based vaguely on the Linux code
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <uhs_mmc.h>
#include <part.h>
#include <malloc.h>
#include <linux/list.h>
#include <div64.h>

/* Set block count limit because of 16 bit register limit on some hardware*/
#ifndef CONFIG_SYS_MMC_MAX_BLK_COUNT
#define CONFIG_SYS_MMC_MAX_BLK_COUNT 65535
#endif

static struct list_head mmc_devices;
static int cur_dev_num = -1;

int __board_mmc_getcd(u8 *cd, struct mmc *mmc)
{
	return -1;
}

int board_mmc_getcd(u8 *cd, struct mmc *mmc)__attribute__((weak,
		alias("__board_mmc_getcd")));

int mmc_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd, struct mmc_data *data)
{
#ifdef CONFIG_MMC_TRACE
	int ret;
	int i;
	u8 *ptr;

	printf("CMD_SEND:%d\n", cmd->cmdidx);
	printf("\t\tARG\t\t\t 0x%08X\n", cmd->cmdarg);
	printf("\t\tFLAG\t\t\t %d\n", cmd->flags);
	ret = mmc->send_cmd(mmc, cmd, data);
	switch (cmd->resp_type) {
	case MMC_RSP_NONE:
		printf("\t\tMMC_RSP_NONE\n");
		break;
	case MMC_RSP_R1:
		printf("\t\tMMC_RSP_R1,5,6,7 \t 0x%08X\n",
				cmd->response[0]);
		break;
	case MMC_RSP_R1b:
		printf("\t\tMMC_RSP_R1b\t\t 0x%08X\n",
				cmd->response[0]);
		break;
	case MMC_RSP_R2:
		printf("\t\tMMC_RSP_R2\t\t 0x%08X\n",
				cmd->response[0]);
		printf("\t\t \t\t 0x%08X\n",
				cmd->response[1]);
		printf("\t\t \t\t 0x%08X\n",
				cmd->response[2]);
		printf("\t\t \t\t 0x%08X\n",
				cmd->response[3]);
		printf("\n");
		printf("\t\t\t\t\tDUMPING DATA\n");
		for (i = 0; i < 4; i++) {
			int j;
			printf("\t\t\t\t\t%03d - ", i*4);
			ptr = &cmd->response[i];
			ptr += 3;
			for (j = 0; j < 4; j++)
				printf("%02X ", *ptr--);
			printf("\n");
		}
		break;
	case MMC_RSP_R3:
		printf("\t\tMMC_RSP_R3,4\t\t 0x%08X\n",
				cmd->response[0]);
		break;
	default:
		printf("\t\tERROR MMC rsp not supported\n");
		break;
	}
	return ret;
#else
	return mmc->send_cmd(mmc, cmd, data);
#endif
}

int mmc_send_status(struct mmc *mmc, int timeout)
{
	struct mmc_cmd cmd;
	int err;
#ifdef CONFIG_MMC_TRACE
	int status;
#endif

	cmd.cmdidx = MMC_CMD_SEND_STATUS;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = mmc->rca << 16;
	cmd.flags = 0;

	do {
		err = mmc_send_cmd(mmc, &cmd, NULL);
		if (err)
			return err;
		else if (cmd.response[0] & MMC_STATUS_RDY_FOR_DATA)
			break;

		udelay(1000);

		if (cmd.response[0] & MMC_STATUS_MASK) {
			printf("Status Error: 0x%08X\n", cmd.response[0]);
			return COMM_ERR;
		}
	} while (timeout--);

#ifdef CONFIG_MMC_TRACE
	status = (cmd.response[0] & MMC_STATUS_CURR_STATE) >> 9;
	printf("CURR STATE:%d\n", status);
#endif
	if (!timeout) {
		printf("Timeout waiting card ready\n");
		return TIMEOUT;
	}

	return 0;
}

int mmc_set_blocklen(struct mmc *mmc, int len)
{
	struct mmc_cmd cmd;

	cmd.cmdidx = MMC_CMD_SET_BLOCKLEN;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = len;
	cmd.flags = 0;

	return mmc_send_cmd(mmc, &cmd, NULL);
}

struct mmc *find_mmc_device(int dev_num)
{
	struct mmc *m;
	struct list_head *entry;

	list_for_each(entry, &mmc_devices) {
		m = list_entry(entry, struct mmc, link);

		if (m->block_dev.dev == dev_num)
			return m;
	}

	printf("MMC Device %d not found\n", dev_num);

	return NULL;
}

static ulong mmc_erase_t(struct mmc *mmc, ulong start, lbaint_t blkcnt)
{
	struct mmc_cmd cmd;
	ulong end;
	int err, start_cmd, end_cmd;

	if (mmc->high_capacity)
		end = start + blkcnt - 1;
	else {
		end = (start + blkcnt - 1) * mmc->write_bl_len;
		start *= mmc->write_bl_len;
	}

	if (IS_SD(mmc)) {
		start_cmd = SD_CMD_ERASE_WR_BLK_START;
		end_cmd = SD_CMD_ERASE_WR_BLK_END;
	} else {
		start_cmd = MMC_CMD_ERASE_GROUP_START;
		end_cmd = MMC_CMD_ERASE_GROUP_END;
	}

	cmd.cmdidx = start_cmd;
	cmd.cmdarg = start;
	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		goto err_out;

	cmd.cmdidx = end_cmd;
	cmd.cmdarg = end;

	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		goto err_out;

	cmd.cmdidx = MMC_CMD_ERASE;
	cmd.cmdarg = SECURE_ERASE;
	cmd.resp_type = MMC_RSP_R1b;

	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		goto err_out;

	return 0;

	err_out:
	puts("mmc erase failed\n");
	return err;
}

static unsigned long
mmc_berase(int dev_num, unsigned long start, lbaint_t blkcnt)
{
	int err = 0;
	struct mmc *mmc = find_mmc_device(dev_num);
	lbaint_t blk = 0, blk_r = 0;

	if (!mmc)
		return -1;

	if ((start % mmc->erase_grp_size) || (blkcnt % mmc->erase_grp_size))
		printf("\n\nCaution! Your devices Erase group is 0x%x\n"
				"The erase range would be change to 0x%lx~0x%lx\n\n",
				mmc->erase_grp_size, start & ~(mmc->erase_grp_size - 1),
				((start + blkcnt + mmc->erase_grp_size)
						& ~(mmc->erase_grp_size - 1)) - 1);

	while (blk < blkcnt) {
		blk_r = ((blkcnt - blk) > mmc->erase_grp_size) ?
				mmc->erase_grp_size : (blkcnt - blk);
		err = mmc_erase_t(mmc, start + blk, blk_r);
		if (err)
			break;

		blk += blk_r;
	}

	return blk;
}

static ulong
mmc_write_blocks(struct mmc *mmc, ulong start, lbaint_t blkcnt, const void*src)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	int timeout = 1000;

	if ((start + blkcnt) > mmc->block_dev.lba) {
		printf("MMC: block number 0x%lx exceeds max(0x%lx)\n",
				start + blkcnt, mmc->block_dev.lba);
		return 0;
	}

	if (blkcnt > 1)
		cmd.cmdidx = MMC_CMD_WRITE_MULTIPLE_BLOCK;
	else
		cmd.cmdidx = MMC_CMD_WRITE_SINGLE_BLOCK;

	if (mmc->high_capacity)
		cmd.cmdarg = start;
	else
		cmd.cmdarg = start * mmc->write_bl_len;

	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;

	data.src = src;
	data.blocks = blkcnt;
	data.blocksize = mmc->write_bl_len;
	data.flags = MMC_DATA_WRITE;

	if (mmc_send_cmd(mmc, &cmd, &data)) {
		printf("mmc write failed\n");
		return 0;
	}

	/* SPI multiblock writes terminate using a special
	 * token, not a STOP_TRANSMISSION request.
	 */
	if (!mmc_host_is_spi(mmc) && blkcnt > 1) {
		cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
		cmd.cmdarg = 0;
		cmd.resp_type = MMC_RSP_R1b;
		cmd.flags = 0;
		if (mmc_send_cmd(mmc, &cmd, NULL)) {
			printf("mmc fail to send stop cmd\n");
			return 0;
		}

		/* Waiting for the ready status */
		mmc_send_status(mmc, timeout);
	}

	return blkcnt;
}

static ulong
mmc_bwrite(int dev_num, ulong start, lbaint_t blkcnt, const void *src)
{
	lbaint_t cur, blocks_todo = blkcnt;
	int err = 0, blklen;

	struct mmc *mmc = find_mmc_device(dev_num);
	if (!mmc)
		return 0;

	blklen = mmc->write_bl_len;

	if ((mmc->card_caps & EMMC_MODE_4BIT_DDR ||
			mmc->card_caps & EMMC_MODE_8BIT_DDR) ||
			(IS_SD(mmc) && mmc->high_capacity)) {
		err = 0;
		blklen = 512;
	} else
		err = mmc_set_blocklen(mmc, mmc->write_bl_len);

	if (err) {
		puts("set write bl len failed\n\r");
		return err;
	}

	do {
		cur = (blocks_todo > mmc->b_max) ? mmc->b_max : blocks_todo;
		if (mmc_write_blocks(mmc, start, cur, src) != cur)
			return 0;
		blocks_todo -= cur;
		start += cur;
		src += cur * blklen;
	} while (blocks_todo > 0);

	return blkcnt;
}

static int mmc_read_blocks(struct mmc *mmc, void *dst,
		ulong start, lbaint_t blkcnt)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	int timeout = 1000;

	if (blkcnt > 1)
		cmd.cmdidx = MMC_CMD_READ_MULTIPLE_BLOCK;
	else
		cmd.cmdidx = MMC_CMD_READ_SINGLE_BLOCK;

	if (mmc->high_capacity)
		cmd.cmdarg = start;
	else
		cmd.cmdarg = start * mmc->read_bl_len;

	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;

	data.dest = dst;
	data.blocks = blkcnt;
	data.blocksize = mmc->read_bl_len;
	data.flags = MMC_DATA_READ;

	if (mmc_send_cmd(mmc, &cmd, &data))
		return 0;

	if (blkcnt > 1) {
		cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
		cmd.cmdarg = 0;
		cmd.resp_type = MMC_RSP_R1b;
		cmd.flags = 0;
		if (mmc_send_cmd(mmc, &cmd, NULL)) {
			printf("mmc fail to send stop cmd\n");
			return 0;
		}

		/* Waiting for the ready status */
		mmc_send_status(mmc, timeout);
	}

	return blkcnt;
}

static ulong mmc_bread(int dev_num, ulong start, lbaint_t blkcnt, void *dst)
{
	lbaint_t cur, blocks_todo = blkcnt;
	int err = 0, blklen;

	if (blkcnt == 0)
		return 0;

	struct mmc *mmc = find_mmc_device(dev_num);
	if (!mmc)
		return 0;

	if ((start + blkcnt) > mmc->block_dev.lba) {
		printf("MMC: block number 0x%lx exceeds max(0x%lx)\n",
				start + blkcnt, mmc->block_dev.lba);
		return 0;
	}

	if ((mmc->card_caps & EMMC_MODE_4BIT_DDR ||
			mmc->card_caps & EMMC_MODE_8BIT_DDR) ||
			(IS_SD(mmc) && mmc->high_capacity)) {
		blklen = 512;
		err = 0;
	} else {
		blklen = mmc->read_bl_len;
		err = mmc_set_blocklen(mmc, blklen);
	}

	if (err) {
		puts("set read bl len failed\n\r");
		return err;
	}

	do {
		cur = (blocks_todo > mmc->b_max) ? mmc->b_max : blocks_todo;
		if (mmc_read_blocks(mmc, dst, start, cur) != cur)
			return 0;
		blocks_todo -= cur;
		start += cur;
		dst += cur * blklen;
	} while (blocks_todo > 0);

	return blkcnt;
}

static int mmc_go_idle(struct mmc *mmc)
{
	struct mmc_cmd cmd;
	int err;

	udelay(1000);

	cmd.cmdidx = MMC_CMD_GO_IDLE_STATE;
	cmd.cmdarg = 0;
	cmd.resp_type = MMC_RSP_NONE;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	//printf("\nmmc_go_idle ::  err %d ", err);

	udelay(2000);

	return 0;
}

int
sd_send_op_cond(struct mmc *mmc)
{
	int timeout = 1000;
	int err;
	struct mmc_cmd cmd;

	do {
		cmd.cmdidx = MMC_CMD_APP_CMD;
		cmd.resp_type = MMC_RSP_R1;
		cmd.cmdarg = 0;
		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;

		cmd.cmdidx = SD_CMD_APP_SEND_OP_COND;
		cmd.resp_type = MMC_RSP_R3;

		/*
		 * Most cards do not answer if some reserved bits
		 * in the ocr are set. However, Some controller
		 * can set bit 7 (reserved for low voltages), but
		 * how to manage low voltages SD card is not yet
		 * specified.
		 */
		cmd.cmdarg = mmc_host_is_spi(mmc) ? 0 :
				(mmc->voltages & 0xff8000);

		/* Check for high capacity or UHS-I 1.8V signalling */
		if (mmc->version == SD_VERSION_2) {
			cmd.cmdarg |= OCR_HCS;

			if (mmc->host_caps & SD_UHSI_CAP_ALL_MODES)
				cmd.cmdarg |= SD_SWITCH_18V;
		}

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;

		udelay(1000);
	} while ((!(cmd.response[0] & OCR_BUSY)) && timeout--);

	if (timeout <= 0)
		return UNUSABLE_ERR;

	if (mmc->version != SD_VERSION_2)
		mmc->version = SD_VERSION_1_0;

	if (mmc_host_is_spi(mmc)) { /* read OCR for spi */
		cmd.cmdidx = MMC_CMD_SPI_READ_OCR;
		cmd.resp_type = MMC_RSP_R3;
		cmd.cmdarg = 0;
		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;
	}

	mmc->ocr = cmd.response[0];

	mmc->high_capacity = ((mmc->ocr & OCR_HCS) == OCR_HCS);

	/* Is card UHS-I compliant? */
	if (mmc->host_caps & SD_UHSI_CAP_ALL_MODES)
		mmc->uhs18v = ((mmc->ocr & SD_SWITCH_18V) == SD_SWITCH_18V);

	mmc->rca = 0;

	return 0;
}

static int mmc_send_op_cond(struct mmc *mmc)
{
	int timeout = 10000;
	struct mmc_cmd cmd;
	int err;

	/* Some cards seem to need this */
	mmc_go_idle(mmc);

	/* Asking to the card its capabilities */
	cmd.cmdidx = MMC_CMD_SEND_OP_COND;
	cmd.resp_type = MMC_RSP_R3;
	cmd.cmdarg = 0;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	udelay(1000);

	do {
		cmd.cmdidx = MMC_CMD_SEND_OP_COND;
		cmd.resp_type = MMC_RSP_R3;
		cmd.cmdarg = (mmc_host_is_spi(mmc) ? 0 :
				(mmc->voltages &
						(cmd.response[0] & OCR_VOLTAGE_MASK)) |
						(cmd.response[0] & OCR_ACCESS_MODE));

		if (mmc->host_caps & MMC_MODE_HC)
			cmd.cmdarg |= OCR_HCS;

		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;

		udelay(1000);
	} while (!(cmd.response[0] & OCR_BUSY) && timeout--);

	if (timeout <= 0)
		return UNUSABLE_ERR;

	if (mmc_host_is_spi(mmc)) { /* read OCR for spi */
		cmd.cmdidx = MMC_CMD_SPI_READ_OCR;
		cmd.resp_type = MMC_RSP_R3;
		cmd.cmdarg = 0;
		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;
	}

	mmc->version = MMC_VERSION_UNKNOWN;
	mmc->ocr = cmd.response[0];

	mmc->high_capacity = ((mmc->ocr & OCR_HCS) == OCR_HCS);
	mmc->rca = 0;

	return 0;
}


int mmc_send_ext_csd(struct mmc *mmc, char *ext_csd)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	int err;

	/* Get the Card Status Register */
	cmd.cmdidx = MMC_CMD_SEND_EXT_CSD;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = 0;
	cmd.flags = 0;

	data.dest = ext_csd;
	data.blocks = 1;
	data.blocksize = 512;
	data.flags = MMC_DATA_READ;

	err = mmc_send_cmd(mmc, &cmd, &data);

	return err;
}


int mmc_switch(struct mmc *mmc, u8 set, u8 index, u8 value)
{
	struct mmc_cmd cmd;
	int timeout = 1000;
	int ret;

	cmd.cmdidx = MMC_CMD_SWITCH;
	cmd.resp_type = MMC_RSP_R1b;
	cmd.cmdarg = (MMC_SWITCH_MODE_WRITE_BYTE << 24) |
			(index << 16) |
			(value << 8);
	cmd.flags = 0;

	ret = mmc_send_cmd(mmc, &cmd, NULL);

	/* Waiting for the ready status */
	mmc_send_status(mmc, timeout);

	return ret;

}

int mmc_change_freq(struct mmc *mmc)
{
	char ext_csd[512];
	char cardtype;
	int err;

	mmc->card_caps = 0;

	if (mmc_host_is_spi(mmc))
		return 0;

	/* Only version 4 supports high-speed */
	if (mmc->version < MMC_VERSION_4)
		return 0;

	mmc->card_caps |= ((mmc->host_caps & MMC_MODE_8BIT)
			? MMC_MODE_8BIT : MMC_MODE_4BIT);

	err = mmc_send_ext_csd(mmc, ext_csd);

	if (err)
		return err;

	cardtype = ext_csd[196] & 0xf;

	err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 1);

	if (err)
		return err;

	/* Now check to see that it worked */
	err = mmc_send_ext_csd(mmc, ext_csd);

	if (err)
		return err;

	/* No high-speed support */
	if (!ext_csd[185])
		return 0;

	/* High Speed is set, there are two types: 52MHz and 26MHz */
	if (cardtype & MMC_HS_52MHZ)
		mmc->card_caps |= MMC_MODE_HS_52MHz | MMC_MODE_HS;
	else
		mmc->card_caps |= MMC_MODE_HS;

	if (cardtype & EMMC_MODE_DDR_3V) {
		if (mmc->card_caps & MMC_MODE_8BIT)
			mmc->card_caps |= EMMC_MODE_8BIT_DDR;
		else
			mmc->card_caps |= EMMC_MODE_4BIT_DDR;
	}

	return 0;
}

int mmc_switch_part(int dev_num, unsigned int part_num)
{
	struct mmc *mmc = find_mmc_device(dev_num);

	if (!mmc)
		return -1;

	return mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_PART_CONF,
			(mmc->part_config & ~PART_ACCESS_MASK)
			| (part_num & PART_ACCESS_MASK));
}

int sd_switch_part(int dev_num, unsigned int part_num)
{
	struct mmc *mmc = find_mmc_device(dev_num);
	struct mmc_cmd cmd;
	int err;

	if (!mmc)
		return -1;

	cmd.cmdidx = SD_CMD_SELECT_PARTITION;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = (part_num & PART_ACCESS_MASK) << 24;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return -1;

	return 0;
}

int mmc_switch_boot_part(int dev_num, unsigned int part_num)
{
	struct mmc *mmc = find_mmc_device(dev_num);
	char ext_csd[512] = { 0 };
	int err;
	char boot_config;
	char boot_bus_width, card_boot_bus_width;

	/* Partition must be -
0 - user area
1 - boot partition 1
2 - boot partition 2
	 */
	if (part_num > 2) {
		printf("Wrong partition id - "
				"0 (user area), 1 (boot1), 2 (boot2)\n");
		return 1;
	}

	/* Before calling this func, "mmc" struct must have been initialized */
	if (mmc->version < MMC_VERSION_4) {
		printf("Error: invalid mmc version! "
				"mmc version is below version 4!");
		return -1;
	}

	err = mmc_send_ext_csd(mmc, ext_csd);
	if (err) {
		printf("Warning: fail to get ext csd for MMC!\n");
		goto err_rtn;
	}

	/* Leave access to current partition as is */
	boot_config = ext_csd[EXT_CSD_PART_CONF] &
			EXT_CSD_BOOT_PARTITION_ACCESS_MASK;

	/* Enable boot from that partition and boot_ack bit */
	boot_config |= (char)(part_num << 3 | 1 << 6);

	err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
			EXT_CSD_PART_CONF, boot_config);
	if (err) {
		printf("Error: fail to send SWITCH command to card "
				"to swich partition for access!\n");
		goto err_rtn;
	}

	/* Now check whether it works */
	err = mmc_send_ext_csd(mmc, ext_csd);
	if (err) {
		printf("Warning: fail to get ext csd for MMC!\n");
		goto err_rtn;
	}

	if (boot_config != ext_csd[EXT_CSD_PART_CONF]) {
		printf("Warning: Boot partition switch failed!\n");
		goto err_rtn;
	} else
		mmc->part_config = ext_csd[EXT_CSD_PART_CONF];

	/* Program boot_bus_width field for eMMC fastboot mode
	 * according to this card's capabilities
	 */
	if (mmc->card_caps & EMMC_MODE_8BIT_DDR)
		boot_bus_width = EXT_CSD_BOOT_BUS_WIDTH_DDR |
		EXT_CSD_BOOT_BUS_WIDTH_8BIT;
	else if (mmc->card_caps & EMMC_MODE_4BIT_DDR)
		boot_bus_width = EXT_CSD_BOOT_BUS_WIDTH_DDR |
		EXT_CSD_BOOT_BUS_WIDTH_4BIT;
	else if (mmc->card_caps & MMC_MODE_8BIT)
		boot_bus_width = EXT_CSD_BOOT_BUS_WIDTH_8BIT;
	else if (mmc->card_caps & MMC_MODE_4BIT)
		boot_bus_width = EXT_CSD_BOOT_BUS_WIDTH_4BIT;
	else
		boot_bus_width = 0;

	err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
			EXT_CSD_BOOT_BUS_WIDTH, boot_bus_width);

	/* Ensure that it programmed properly */
	err = mmc_send_ext_csd(mmc, ext_csd);
	if (err) {
		printf("Warning: fail to get ext csd for MMC!\n");
		goto err_rtn;
	}

	card_boot_bus_width = ext_csd[EXT_CSD_BOOT_BUS_WIDTH];
	if (card_boot_bus_width != boot_bus_width) {
		printf("Warning: current boot_bus_width, 0x%x, is "
				"not same as requested boot_bus_width 0x%x!\n",
				card_boot_bus_width, boot_bus_width);
		goto err_rtn;
	}

	return 0;
	err_rtn:
	return -1;
}

static int sd_send_switch_uhs18v(struct mmc *mmc)
{
	struct mmc_cmd cmd;
	int err;

	cmd.cmdidx = SD_CMD_SWITCH_UHS18V;
	cmd.cmdarg = 0;
	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err) {
		printf("mmc: CMD11 to switch to 1.8V UHS mode failed. \
Card will require power cycle\n");
		return err;
	}

	switchcoreVoltage();
	printf("\nSwitched to 1.8v IO");

	return 0;
}

int sd_switch_boot_part(int dev_num, unsigned int part_num)
{
	return 0;
}
int sd_switch(struct mmc *mmc, int mode, int group, u8 value, u8 *resp)
{
	struct mmc_cmd cmd;
	struct mmc_data data;

	/* Switch the frequency */
	cmd.cmdidx = SD_CMD_SWITCH_FUNC;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = (mode << 31) | 0xffffff;
	cmd.cmdarg &= ~(0xf << (group * 4));
	cmd.cmdarg |= value << (group * 4);
	cmd.flags = 0;

	data.dest = (char *)resp;
	data.blocksize = 64;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;

	return mmc_send_cmd(mmc, &cmd, &data);
}


int sd_change_freq(struct mmc *mmc)
{
	int err;
	struct mmc_cmd cmd;
	//uint scr[2];
	//uint switch_status[16];
	ALLOC_CACHE_ALIGN_BUFFER(uint, scr, 2);
	ALLOC_CACHE_ALIGN_BUFFER(uint, switch_status, 16);
	struct mmc_data data;
	int timeout;

	mmc->card_caps = 0;

	if (mmc_host_is_spi(mmc))
		return 0;

	/* Read the SCR to find out if this card supports higher speeds */
	cmd.cmdidx = MMC_CMD_APP_CMD;
	cmd.resp_type = MMC_RSP_R1;

	//printf("\n Command RCA   = 0x%x\n ", mmc->rca );

	cmd.cmdarg =mmc->rca << 16;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	mdelay(2000);


	//printf("\n before SCR err = 0x%x !!! ", err);
	if (err)
		return err;

	cmd.cmdidx = SD_CMD_APP_SEND_SCR;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = 0;
	//cmd.flags = 0;

	timeout = 3;




	retry_scr:
	data.dest = (char *)scr;
	data.blocksize = 8;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;

	err = mmc_send_cmd(mmc, &cmd, &data);

	mdelay(2000);

	if (err) {
		if (timeout--)
			goto retry_scr;

		return err;
	}



	mmc->scr[0] = __be32_to_cpu(scr[0]);
	mmc->scr[1] = __be32_to_cpu(scr[1]);

	switch ((mmc->scr[0] >> 24) & 0xf) {
	case 0:
		mmc->version = SD_VERSION_1_0;
		break;
	case 1:
		mmc->version = SD_VERSION_1_10;
		break;
	case 2:
		if ((mmc->scr[0] >> 15) & 0x1)
			mmc->version = SD_VERSION_3;
		/* else, it is already initialized as SD_VERSION_2 */
		break;
	default:
		mmc->version = SD_VERSION_1_0;
		break;
	}

	if (mmc->scr[0] & SD_DATA_4BIT)
		mmc->card_caps |= MMC_MODE_4BIT;

	/* Version 1.0 doesn't support switching */
	if (mmc->version == SD_VERSION_1_0)
		return 0;

	timeout = 4;
	while (timeout--) {
		err = sd_switch(mmc, SD_SWITCH_CHECK, 0, 1,
				(u8 *)switch_status);

		if (err)
			return err;

		/* The high-speed function is busy. Try again */
		if (!(__be32_to_cpu(switch_status[7]) & SD_HIGHSPEED_BUSY))
			break;
	}

	/* If high-speed isn't supported, we return */
	if (!(__be32_to_cpu(switch_status[3]) & SD_HIGHSPEED_SUPPORTED))
		return 0;

	if (mmc->uhs18v) {

		printf("\nChanged SD card to high freq, SD card supports :");
		/* which UHS-I modes are supported by card? */
		if (__be32_to_cpu(switch_status[3]) & SD_UHSI_CAP_SDR104)
		{
			printf("\nSDR104 mode ");
			mmc->card_caps |= SD_UHSI_CAP_SDR104;
		}
		if (__be32_to_cpu(switch_status[3]) & SD_UHSI_CAP_SDR50)
		{
			printf("\nSDR50 mode ");
			mmc->card_caps |= SD_UHSI_CAP_SDR50;
		}
		if (__be32_to_cpu(switch_status[3]) & SD_UHSI_CAP_DDR50)
		{
			printf("\nDDR50 mode ");
			mmc->card_caps |= SD_UHSI_CAP_DDR50;
		}
		if (__be32_to_cpu(switch_status[3]) & SD_UHSI_CAP_SDR25)
		{
			printf("\nSDR25 mode ");
			mmc->card_caps |= SD_UHSI_CAP_SDR25;
		}
		if (__be32_to_cpu(switch_status[3]) & SD_UHSI_CAP_SDR12)
		{
			printf("\nSDR12 mode ");
			mmc->card_caps |= SD_UHSI_CAP_SDR12;
		}
	} else {
		/* Switch non-UHS card to high speed mode (50 MHz) */
		err = sd_switch(mmc, SD_SWITCH_SWITCH, 0, 1,
				(u8 *)switch_status);

		if (err)
			return err;

		if ((__be32_to_cpu(switch_status[4]) & 0x0f000000) ==
				0x01000000)
			mmc->card_caps |= MMC_MODE_HS;
	}

	return 0;
}

/* Put the card in SDR50, DDR50, or SDR104 mode of UHS-I */
int sd_uhsi_mode_select(struct mmc *mmc)
{
	int timeout, err;
	uint switch_status[16];
	uint func_num;

	mmc->card_uhs_mode = SD_UHSI_FUNC_SDR12;

	/* Order of checking important to pick fastest mode */
	if (mmc->card_caps & SD_UHSI_CAP_SDR104)
		func_num = SD_UHSI_FUNC_SDR104;
	else if (mmc->card_caps & SD_UHSI_CAP_SDR50)
		func_num = SD_UHSI_FUNC_SDR50;
	else if (mmc->card_caps & SD_UHSI_CAP_DDR50)
		func_num = SD_UHSI_FUNC_DDR50;
	else if (mmc->card_caps & SD_UHSI_CAP_SDR25)
		func_num = SD_UHSI_FUNC_SDR25;
	else
		return 0;

	if ( func_num == SD_UHSI_FUNC_SDR50 )
	{
		printf("\nSelect SDR50 mode !!! ");
	}
	else
	{
		printf("\nSelect %d mode", func_num);

	}
	timeout = 4;
	while (timeout--) {
		err = sd_switch(mmc, SD_SWITCH_CHECK, 0, func_num,
				(u8 *)switch_status);

		if (err)
			return err;

		/* The function is busy if bit is set. Try again */
		if (!(__be32_to_cpu(switch_status[7]) & (1 << (16 + func_num))))
			break;
	}

	err = sd_switch(mmc, SD_SWITCH_SWITCH, 0, func_num,
			(u8 *)switch_status);

	if (err)
		return err;

	if (((__be32_to_cpu(switch_status[4]) & 0x0f000000) >> 24) ==
			func_num) {
		mmc->card_uhs_mode = func_num;

		if (mmc->card_uhs_mode == SD_UHSI_FUNC_DDR50)
			mmc->card_caps |= EMMC_MODE_4BIT_DDR;
	}

	//printf("\nsd_uhsi_mode_select() return success!");
	return 0;

}

int sd_send_tuning_cmd(struct mmc *mmc)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	char buff[64]; /* 64 byte tuning block */

	/* Switch the frequency */
	cmd.cmdidx = SD_CMD_TUNING;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = 0;
	cmd.flags = 0;

	data.dest = buff;
	data.blocksize = 64;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;

	return mmc_send_cmd(mmc, &cmd, &data);
}

void sd_uhsi_tuning(struct mmc *mmc)
{
	//int min, max, avg;
	u32 timeout= 100;

	/* Tuning only required for SDR50 and SDR104 modes */
	if (mmc->card_uhs_mode != SD_UHSI_FUNC_SDR50 &&
			mmc->card_uhs_mode != SD_UHSI_FUNC_SDR104)
		return;


	printf("\nStart tuning...");
	iproc_set_tuning();

	while ( timeout != 0 )
	{

		sd_send_tuning_cmd(mmc);
		if ( iproc_return_tuning_status() == 1 )
		{
			break;
		}
		timeout --;
	}
	printf("\nTuning done\n");


}

/* frequency bases */
/* divided by 10 to be nice to platforms without floating point */
static const int fbase[] = {
		10000,
		100000,
		1000000,
		10000000,
};

/* Multiplier values for TRAN_SPEED. Multiplied by 10 to be nice
 * to platforms without floating point.
 */
static const int multipliers[] = {
		0,	/* reserved */
		10,
		12,
		13,
		15,
		20,
		25,
		30,
		35,
		40,
		45,
		50,
		55,
		60,
		70,
		80,
};

void mmc_set_ios(struct mmc *mmc, uint flag)
{
	mmc->set_ios(mmc, flag);
}

void mmc_set_clock(struct mmc *mmc, uint clock)
{
	if (clock > mmc->f_max)
		clock = mmc->f_max;

	if (clock < mmc->f_min)
		clock = mmc->f_min;

	mmc->clock = clock;
    //1 for clock
	mmc_set_ios(mmc,1);
}

void mmc_set_bus_width(struct mmc *mmc, uint width)
{
	mmc->bus_width = width;

	//0 for width
	mmc_set_ios(mmc,0);
}

int mmc_startup(struct mmc *mmc)
{
	int err;
	uint mult, freq;
	u64 cmult, csize, capacity;
	struct mmc_cmd cmd;
	char ext_csd[512];
	int timeout = 1000;

#ifdef CONFIG_MMC_SPI_CRC_ON
	if (mmc_host_is_spi(mmc)) { /* enable CRC check for spi */
		cmd.cmdidx = MMC_CMD_SPI_CRC_ON_OFF;
		cmd.resp_type = MMC_RSP_R1;
		cmd.cmdarg = 1;
		cmd.flags = 0;
		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;
	}
#endif
	/* If this is a UHS-I compliant SD card, switch to 1.8V for I/O now */
	if (mmc->uhs18v) {
		err = sd_send_switch_uhs18v(mmc);
		if (err)
			return err;
	}
	/* Put the Card in Identify Mode */
	cmd.cmdidx = mmc_host_is_spi(mmc) ? MMC_CMD_SEND_CID :
			MMC_CMD_ALL_SEND_CID; /* cmd not supported in spi */
	cmd.resp_type = MMC_RSP_R2;
	cmd.cmdarg = 0;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	memcpy(mmc->cid, cmd.response, 16);

	/*
	 * For MMC cards, set the Relative Address.
	 * For SD cards, get the Relatvie Address.
	 * This also puts the cards into Standby State
	 */
	if (!mmc_host_is_spi(mmc)) { /* cmd not supported in spi */
		cmd.cmdidx = SD_CMD_SEND_RELATIVE_ADDR;
		cmd.cmdarg = mmc->rca << 16;
		cmd.resp_type = MMC_RSP_R6;
		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;

		if (IS_SD(mmc))
			mmc->rca = (cmd.response[0] >> 16) & 0xffff;
	}

	/* Get the Card-Specific Data */
	cmd.cmdidx = MMC_CMD_SEND_CSD;
	cmd.resp_type = MMC_RSP_R2;
	cmd.cmdarg = mmc->rca << 16;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	/* Waiting for the ready status */
	mmc_send_status(mmc, timeout);

	if (err)
		return err;

	mmc->csd[0] = cmd.response[0];
	mmc->csd[1] = cmd.response[1];
	mmc->csd[2] = cmd.response[2];
	mmc->csd[3] = cmd.response[3];

	if (mmc->version == MMC_VERSION_UNKNOWN) {
		int version = (cmd.response[0] >> 26) & 0xf;

		switch (version) {
		case 0:
			mmc->version = MMC_VERSION_1_2;
			break;
		case 1:
			mmc->version = MMC_VERSION_1_4;
			break;
		case 2:
			mmc->version = MMC_VERSION_2_2;
			break;
		case 3:
			mmc->version = MMC_VERSION_3;
			break;
		case 4:
			mmc->version = MMC_VERSION_4;
			break;
		default:
			mmc->version = MMC_VERSION_1_2;
			break;
		}
	}

	/* divide frequency by 10, since the mults are 10x bigger */
	freq = fbase[(cmd.response[0] & 0x7)];
	mult = multipliers[((cmd.response[0] >> 3) & 0xf)];

	mmc->tran_speed = freq * mult;

	mmc->read_bl_len = 1 << ((cmd.response[1] >> 16) & 0xf);

	if (IS_SD(mmc))
		mmc->write_bl_len = mmc->read_bl_len;
	else
		mmc->write_bl_len = 1 << ((cmd.response[3] >> 22) & 0xf);

	if (mmc->high_capacity) {
		csize = (mmc->csd[1] & 0x3f) << 16
				| (mmc->csd[2] & 0xffff0000) >> 16;
		cmult = 8;
	} else {
		csize = (mmc->csd[1] & 0x3ff) << 2
				| (mmc->csd[2] & 0xc0000000) >> 30;
		cmult = (mmc->csd[2] & 0x00038000) >> 15;
	}

	mmc->capacity = (csize + 1) << (cmult + 2);
	mmc->capacity *= mmc->read_bl_len;

	if (mmc->read_bl_len > 512)
		mmc->read_bl_len = 512;

	if (mmc->write_bl_len > 512)
		mmc->write_bl_len = 512;

	/* Select the card, and put it into Transfer Mode */
	if (!mmc_host_is_spi(mmc)) { /* cmd not supported in spi */
		cmd.cmdidx = MMC_CMD_SELECT_CARD;
		cmd.resp_type = MMC_RSP_R1;
		cmd.cmdarg = mmc->rca << 16;
		cmd.flags = 0;
		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;
	}

	/*
	 * For SD, its erase group is always one sector
	 */
	mmc->erase_grp_size = 1;
	mmc->boot_part_num = 0;
	mmc->part_config = MMCPART_NOAVAILABLE;
	if (!IS_SD(mmc) && (mmc->version >= MMC_VERSION_4)) {
		/* check ext_csd version and capacity */
		err = mmc_send_ext_csd(mmc, ext_csd);
		if (!err & (ext_csd[192] >= 2)) {
			/*
			 * According to the JEDEC Standard, the value of
			 * ext_csd's capacity is valid if the value is more
			 * than 2GB
			 */
			capacity = ext_csd[212] << 0 | ext_csd[213] << 8 |
					ext_csd[214] << 16 | ext_csd[215] << 24;
			capacity *= 512;
			if ((capacity >> 20) > 2 * 1024)
				mmc->capacity = capacity;
		}

		/*
		 * Check whether GROUP_DEF is set, if yes, read out
		 * group size from ext_csd directly, or calculate
		 * the group size from the csd value.
		 */
		if (ext_csd[175])
			mmc->erase_grp_size = ext_csd[224] * 512 * 1024;
		else {
			int erase_gsz, erase_gmul;
			erase_gsz = (mmc->csd[2] & 0x00007c00) >> 10;
			erase_gmul = (mmc->csd[2] & 0x000003e0) >> 5;
			mmc->erase_grp_size = (erase_gsz + 1)
		* (erase_gmul + 1);
		}

		/* store the partition info of emmc */
		if (ext_csd[160] & PART_SUPPORT) {
			mmc->part_config = ext_csd[179];
			mmc->boot_part_num = (ext_csd[179] >> 3) & 0x7;
		}
	}
	if (IS_SD(mmc))
		err = sd_change_freq(mmc);
	else
		err = mmc_change_freq(mmc);

	if (err)
		return err;
	/* Restrict card's capabilities by what the host can do */
	mmc->card_caps &= mmc->host_caps;

	if (IS_SD(mmc)) {
		if (mmc->card_caps & MMC_MODE_4BIT) {
			cmd.cmdidx = MMC_CMD_APP_CMD;
			cmd.resp_type = MMC_RSP_R1;
			cmd.cmdarg = mmc->rca << 16;
			cmd.flags = 0;

			err = mmc_send_cmd(mmc, &cmd, NULL);
			if (err)
				return err;

			cmd.cmdidx = SD_CMD_APP_SET_BUS_WIDTH;
			cmd.resp_type = MMC_RSP_R1;
			cmd.cmdarg = 2;
			cmd.flags = 0;
			err = mmc_send_cmd(mmc, &cmd, NULL);
			if (err)
				return err;

			mmc_set_bus_width(mmc, 4);
		}
		/* Switch the card and host to UHS-I modes, if available */
		if (mmc->uhs18v) {
			err = sd_uhsi_mode_select(mmc);
			if (err)
				return err;

			switch (mmc->card_uhs_mode) {
			case SD_UHSI_FUNC_SDR104:
				mmc_set_clock(mmc, 208000000);
				break;
			case SD_UHSI_FUNC_SDR50:
				//printf("\nSWITCH CLOCK !!!\n");
				//mmc_set_clock(mmc, 50000000);
				//iproc_enable_preset();
				mdelay(1000);
				break;
			case SD_UHSI_FUNC_SDR25:
			case SD_UHSI_FUNC_DDR50:
				mmc_set_clock(mmc, 50000000);
				break;
			case SD_UHSI_FUNC_SDR12:
			default:
				mmc_set_clock(mmc, 25000000);
				break;
			}

			sd_uhsi_tuning(mmc);


		} else {
			if (mmc->card_caps & MMC_MODE_HS)
				mmc_set_clock(mmc, 50000000);
			else
				mmc_set_clock(mmc, 25000000);
		}
	} else {

		if (mmc->card_caps & MMC_MODE_4BIT) {
			if (mmc->card_caps & EMMC_MODE_4BIT_DDR) {
				/* Set the card to use 4 bit DDR mode */
				err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
						EXT_CSD_BUS_WIDTH,
						EXT_CSD_BUS_WIDTH_4_DDR);
			} else {
				/* Set the card to use 4 bit*/
				err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
						EXT_CSD_BUS_WIDTH,
						EXT_CSD_BUS_WIDTH_4);
			}

			if (err)
				return err;

			mmc_set_bus_width(mmc, 4);
		} else if (mmc->card_caps & MMC_MODE_8BIT) {
			if (mmc->card_caps & EMMC_MODE_8BIT_DDR) {
				/* Set the card to use 8 bit DDR mode */
				err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
						EXT_CSD_BUS_WIDTH,
						EXT_CSD_BUS_WIDTH_8_DDR);
			} else {
				/* Set the card to use 8 bit*/
				err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
						EXT_CSD_BUS_WIDTH,
						EXT_CSD_BUS_WIDTH_8);
			}

			if (err)
				return err;

			mmc_set_bus_width(mmc, 8);
		}

		if (mmc->card_caps & MMC_MODE_HS) {
			if (mmc->card_caps & MMC_MODE_HS_52MHz)
				mmc_set_clock(mmc, 52000000);
			else
				mmc_set_clock(mmc, 26000000);
		} else
			mmc_set_clock(mmc, 20000000);
	}

	/* fill in device description */
	mmc->block_dev.lun = 0;
	mmc->block_dev.type = 0;
	mmc->block_dev.blksz = mmc->read_bl_len;
	mmc->block_dev.lba = lldiv(mmc->capacity, mmc->read_bl_len);
	sprintf(mmc->block_dev.vendor, "Man %06x Snr %08x", mmc->cid[0] >> 8,
			(mmc->cid[2] << 8) | (mmc->cid[3] >> 24));
	sprintf(mmc->block_dev.product, "%c%c%c%c%c", mmc->cid[0] & 0xff,
			(mmc->cid[1] >> 24), (mmc->cid[1] >> 16) & 0xff,
			(mmc->cid[1] >> 8) & 0xff, mmc->cid[1] & 0xff);
	sprintf(mmc->block_dev.revision, "%d.%d", mmc->cid[2] >> 28,
			(mmc->cid[2] >> 24) & 0xf);
	init_part(&mmc->block_dev);

	return 0;
}

int mmc_send_if_cond(struct mmc *mmc)
{
	struct mmc_cmd cmd;
	int err;

	cmd.cmdidx = SD_CMD_SEND_IF_COND;
	/* We set the bit if the host supports voltages between 2.7 and 3.6 V */
	cmd.cmdarg = ((mmc->voltages & 0xff8000) != 0) << 8 | 0xaa;
	cmd.resp_type = MMC_RSP_R7;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	//printf("\nmmc_send_if_cond : err 0x%x", err);

	if (err)
		return err;

	if ((cmd.response[0] & 0xff) != 0xaa)
	{
		//printf("\nmmc_send_if_cond : UNUSABLE_ERR");
		return UNUSABLE_ERR;
	}
	else
	{
		//printf("\nmmc_send_if_cond: set version to 2 ");
		mmc->version = SD_VERSION_2;
	}

	return 0;
}

int mmc_register(struct mmc *mmc)
{
	/* Setup the universal parts of the block interface just once */
	mmc->block_dev.if_type = IF_TYPE_MMC;
	mmc->block_dev.dev = cur_dev_num++;
	mmc->block_dev.removable = 1;
	mmc->block_dev.block_read = mmc_bread;
	mmc->block_dev.block_write = mmc_bwrite;
	mmc->block_dev.block_erase = mmc_berase;
	if (!mmc->b_max)
		mmc->b_max = CONFIG_SYS_MMC_MAX_BLK_COUNT;

	INIT_LIST_HEAD (&mmc->link);

	list_add_tail (&mmc->link, &mmc_devices);

	return 0;
}

#ifdef CONFIG_PARTITIONS
block_dev_desc_t *mmc_get_dev(int dev)
{
	struct mmc *mmc = find_mmc_device(dev);

	return mmc ? &mmc->block_dev : NULL;
}
#endif

int mmc_init(struct mmc *mmc)
{
	int err;

	if (mmc->has_init)
		return 0;

	mmc->uhs18v = 0;

	err = mmc->init(mmc);

	if (err)
		return err;

	mmc_set_bus_width(mmc, 1);
	mmc_set_clock(mmc, 40000000);//400000);swang

	/* Reset the Card */
	err = mmc_go_idle(mmc);

	if (err)
		return err;

	/* The internal partition reset to user partition(0) at every CMD0*/
	mmc->part_num = 0;

	/* Test for SD version 2 */
	err = mmc_send_if_cond(mmc);

	/* Now try to get the SD card's operating condition */
	err = sd_send_op_cond(mmc);

	/* If the command timed out, we check for an MMC card */
	if (err == TIMEOUT) {
		err = mmc_send_op_cond(mmc);

		if (err) {
			printf("Card did not respond to voltage select!\n");
			return UNUSABLE_ERR;
		}
	}

	err = mmc_startup(mmc);
	if (err)
		mmc->has_init = 0;
	else
		mmc->has_init = 1;
	return err;
}

/*
 * CPU and board-specific MMC initializations. Aliased function
 * signals caller to move on
 */
static int __def_mmc_init(bd_t *bis)
{
	return -1;
}

int cpu_mmc_init(bd_t *bis) __attribute__((weak, alias("__def_mmc_init")));
int board_mmc_init(bd_t *bis) __attribute__((weak, alias("__def_mmc_init")));

void print_mmc_devices(char separator)
{
	struct mmc *m;
	struct list_head *entry;

	list_for_each(entry, &mmc_devices) {
		m = list_entry(entry, struct mmc, link);

		printf("%s: %d", m->name, m->block_dev.dev);

		if (entry->next != &mmc_devices)
			printf("%c", separator);
	}

	printf("\n");
}

int get_mmc_num(void)
{
	return cur_dev_num;
}

int mmc_initialize(bd_t *bis)
{
	INIT_LIST_HEAD (&mmc_devices);
	cur_dev_num = 0;

	if (board_mmc_init(bis) < 0)
		cpu_mmc_init(bis);

	print_mmc_devices(',');

	return 0;
}
