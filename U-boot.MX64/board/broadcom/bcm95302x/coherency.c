/*
 * Copyright (C) 2013, Broadcom Corporation. All Rights Reserved.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 *  NorthStar+ B0 and later coherency handling
 */
#include <config.h>
#include <common.h>
#include <asm/arch/iproc_regs.h>
#include "asm/arch/socregs.h"
//#include "asm/arch-iproc/socregs.h"
#include "asm/arch/reg_utils.h"

/* these are the hardware registers not included in socregs_nsp_open.h */
#define ICFG_CHIP_ID_REG 0x18000000
#define AXIIC_REMAP 0x1a000000
#define DDR_S1_IDM_IO_CONTROL_DIRECT 0x18109408
#define DDR_S2_IDM_IO_CONTROL_DIRECT 0x1810a408
#define PAXB_0_PCIE_RC_AXI_CONFIG 0x18012100
#define PAXB_0_PCIE_EP_AXI_CONFIG 0x18012104
#define PAXB_1_PCIE_RC_AXI_CONFIG 0x18013100
#define PAXB_1_PCIE_EP_AXI_CONFIG 0x18013104
#define PAXB_2_PCIE_RC_AXI_CONFIG 0x18014100
#define PAXB_2_PCIE_EP_AXI_CONFIG 0x18014104
#define I2S_M0_IDM_IO_CONTROL_DIRECT 0x18118408
#define A9JTAG_M0_IDM_IO_CONTROL_DIRECT 0x18119408
#define APBW_IDM_IDM_IO_CONTROL_DIRECT 0x18131408
#define AMAC_IDM0_IO_CONTROL_DIRECT 0x18110408
#define AMAC_IDM1_IO_CONTROL_DIRECT 0x18111408
#define FA_AMAC_IDM0_IO_CONTROL_DIRECT 0x18112408
#define FA_AMAC_IDM1_IO_CONTROL_DIRECT 0x18113408
#define PAE_M0_IDM_IO_CONTROL_DIRECT 0x1811f408
#define DMAC_M0_IDM_IO_CONTROL_DIRECT 0x18114408
#define USBH_usb2h_axi_m_AxUSER_cfg 0x1802a600
#define USB3_IDM_IDM_IO_CONTROL_DIRECT 0x18104408

#define IHOST_L2C_AUX_CONTROL 0x19022104
#define IHOST_L2C_TAG_RAM_CTRL 0x19022108
#define IHOST_L2C_DATA_RAM_CTRL 0x1902210c
#define IHOST_L2C_EVENT_CTR_CTRL 0x19022200
#define IHOST_L2C_EVENT_CTR_CTRL__Counter_reset_R 1
#define IHOST_L2C_EVENT_CTR_CTRL__Counter_reset_WIDTH 2
#define IHOST_L2C_EVENT_CTR_CTRL__Counter_en 0
#define IHOST_L2C_EVENT_CTR_CTRL__Counter_en_R IHOST_L2C_EVENT_CTR_CTRL__Counter_en
#define IHOST_L2C_EVENT_CTR_CTRL__Counter_en_WIDTH 1
#define IHOST_L2C_EVENT_CTR_CFG0 0x19022204
#define IHOST_L2C_EVENT_CTR_CFG0__Counter_event_source0_R 2
#define IHOST_L2C_EVENT_CTR_CFG0__Counter_event_source0_WIDTH 4
#define IHOST_L2C_EVENT_CTR_CFG0__Counter_interrupt_gen0_R 0
#define IHOST_L2C_EVENT_CTR_CFG0__Counter_interrupt_gen0_WIDTH 2
#define IHOST_L2C_EVENT_CTR_CFG1 0x19022208
#define IHOST_L2C_EVENT_CTR_CFG1__Counter_event_source1_R 2
#define IHOST_L2C_EVENT_CTR_CFG1__Counter_event_source1_WIDTH 4
#define IHOST_L2C_EVENT_CTR_CFG1__Counter_interrupt_gen1_R 0
#define IHOST_L2C_EVENT_CTR_CFG1__Counter_interrupt_gen1_WIDTH 2
#define IHOST_L2C_EVENT_CTR0 0x1902220c
#define IHOST_L2C_EVENT_CTR1 0x19022210
#define IHOST_L2C_DEBUG_CTRL 0x19022f40

static int
readl_relaxed(u32 address)
{
    u32 *ptr = (u32*)address;
    return *ptr;
}

static void
writel_relaxed(u32 value, u32 address)
{
    u32 *ptr = (u32*)address;
    *ptr = value;
}

#define MMU_TABLE_SIZE 4096
extern u32* mmu_table_addr;
static int mmu_table_copied = 0;
static u32 mmu_table_backup[MMU_TABLE_SIZE];

static int
_nsp_coherent_mode_set(int enable,
                       int cacheMode,
                       int dramMode,
                       int aruser,
                       int awuser,
                       int arcache,
                       int awcache,
                       int pae_axcache_override)
{
    int result = 0;
    uint32_t chipId;
    uint32_t chipRev;
    uint32_t regData;
    uint32_t dramModeOr;
    uint32_t virtAddr;
    unsigned int index;
    u32 *mmuEntry;

    /* get chip Id and revision */
    chipId = readl_relaxed(ICFG_CHIP_ID_REG);
    chipRev = ((chipId & 0x000F0000) >> 16);
    chipId &= 0x0000FFFF;
    if (0xCF1E == chipId) {
        /* Device is Northstar Plus */
        if (4 > chipRev) {
		enable=0;
//		dramMode=0;
//		aruser=0;
//		awuser=0;
		arcache=3;
		awcache=3;
	}
            /*
             *  CF1E == NorthStar Plus
             *  Revision 4 or greater means B0 or later.
             *
             *  We assume all revisions of NS+ B0 and later will have the
             *  coherency features, so enable them for any such revision.
             */
/*            printf("NS+ B0 or later: DRAM %scache, %s mode, %u, %u, %u, %u, %s\n",
                   cacheMode?"":"non-",
                   enable?"coherent":"legacy",
                   aruser,
                   awuser,
                   arcache,
                   awcache,
                   pae_axcache_override?"TRUE":"FALSE");
*/ 
           /*
             *  Configure devices to operate in coherent mode
             *
             *  ARUSER (5b):
             *    HW group suggested value = 0x03
             *  AWUSER (5b):
             *    HW group suggested value = 0x03
             *  ARCACHE (4b):
             *    HW group suggested value = 0xB
             *  AWCACHE (4b):
             *    HW group suggested value = 0x7
             *
             *  AWCACHE/ARCACHE is bitmap
             *    0 = Bufferable
             *    1 = Cacheable
             *    2 = Read allocate
             *    3 = Write allocate
             *
             *  AWCACHE/ARCACHE valid settings:
             *    0000 - noncacheable, nonbufferable
             *           strongly ordered
             *    0001 - bufferable only
             *           shared and non-shared device
             *    0010 - cacheable but do not allocate
             *           outer noncacheable
             *    0011 - cacheable and bufferable but do not allocate
             *           (no ARM description)
             *    0110 - cacheable write-through, allocate on read
             *           outer write-through, no allocate on write
             *    0111 - cacheable write-back, allocate on read
             *           outer write-back, no allocate on write
             *    1010 - cacheable write-through, allocate on write
             *           (no ARM description)
             *    1011 - cacheable write-back, allocate on write
             *           (no ARM description)
             *    1110 - cacheable write-through, allocate on read & write
             *           (no ARM description)
             *    1111 - cacheable write-back, allocate on read & write
             *           outer write-back, allocate on write
             *
             *  It looks like AWUSER/ARUSER are similarly a bitmap, and the
             *  upper four bits follow the meanings given above for the
             *  AWCACHE/ARCACHE 'valid settings' above.  Bit zero indicates
             *  whether the request is intended to be coherent (set) or not
             *  coherent (clear).
             *
             *  Some hints in the ARM documentation make me suspect that
             *  the AWCACHE/ARCACHE setting applies to the L2 ('outer')
             *  cache, while the AWUSER/ARUSER setting applies to the
             *  core's ('inner') cache.
             *
             *  Placements and presence in each register will vary.
             */
            /* IHOST_M0_IO_CONTROL_DIRECT */
            /* Various overflow registers */
            regData = readl_relaxed(IHOST_M0_IO_CONTROL_DIRECT);
            regData &= (~((0x1F << 21) /* DMAC_ARUSER */ |
                          (0x1F << 16) /* SDIO_ARUSER */ |
                          (0xF << 10)  /* SDIO_ARCACHE */));
            regData |= ((aruser << 21)  /* DMAC_ARUSER */ |
                        (aruser << 16)  /* SDIO_ARUSER */ |
                        (arcache << 10) /* SDIO_ARCACHE */);
            writel_relaxed(regData, IHOST_M0_IO_CONTROL_DIRECT);
            /* DRAM controller */
            regData = readl_relaxed(DDR_S1_IDM_IO_CONTROL_DIRECT);
            regData &= (~((1 << 31) /* axi_s1_use_master */ |
                          (1 << 30) /* axi_s1_awcobuf */ |
                          (1 << 29) /* axi_s1_awcache_0 */));
            regData |= ((0 << 31) /* axi_s1_use_master */ |
                        (1 << 30) /* axi_s1_awcobuf */ |
                        (1 << 29) /* axi_s1_awcache_0 */);
            writel_relaxed(regData, DDR_S1_IDM_IO_CONTROL_DIRECT);
            regData = readl_relaxed(DDR_S2_IDM_IO_CONTROL_DIRECT);
            regData &= (~((1 << 31) /* axi_s2_use_master */ |
                          (1 << 30) /* axi_s2_awcobuf */ |
                          (1 << 29) /* axi_s2_awcache_0 */ |
                          (1 << 28) /* axi_s0_use_master */ |
                          (1 << 27) /* axi_s0_awcobuf */ |
                          (1 << 26) /* axi_s0_awcache_0 */));
            regData |= ((0 << 31) /* axi_s2_use_master */ |
                        (1 << 30) /* axi_s2_awcobuf */ |
                        (1 << 29) /* axi_s2_awcache_0 */ |
                        (1 << 28) /* axi_s0_use_master */ |
                        (1 << 27) /* axi_s0_awcobuf */ |
                        (1 << 26) /* axi_s0_awcache_0 */);
            writel_relaxed(regData, DDR_S2_IDM_IO_CONTROL_DIRECT);
            /* PCIe 0 root complex mode */
            regData = readl_relaxed(PAXB_0_PCIE_RC_AXI_CONFIG);
            regData &= (~((0x1F << 0) /* ARUSER */ |
                          (0x1F << 9) /* AWUSER */ |
                          (0xF << 5)  /* ARCACHE */ |
                          (0xF << 14) /* AWCACHE */));
            regData |= ((aruser << 0)   /* ARUSER */ |
                        (awuser << 9)   /* AWUSER */ |
                        (arcache << 5)  /* ARCACHE */ |
                        (awcache << 14) /* AWCACHE */);
            writel_relaxed(regData, PAXB_0_PCIE_RC_AXI_CONFIG);
            /* PCIe 0 endpoint mode */
            regData = readl_relaxed(PAXB_0_PCIE_EP_AXI_CONFIG);
            regData &= (~((0x1F << 0) /* ARUSER */ |
                          (0x1F << 9) /* AWUSER */ |
                          (0xF << 5)  /* ARCACHE */ |
                          (0xF << 14) /* AWCACHE */));
            regData |= ((aruser << 0)   /* ARUSER */ |
                        (awuser << 9)   /* AWUSER */ |
                        (arcache << 5)  /* ARCACHE */ |
                        (awcache << 14) /* AWCACHE */);
            writel_relaxed(regData, PAXB_0_PCIE_EP_AXI_CONFIG);
            /* PCIe 1 root complex mode */
            regData = readl_relaxed(PAXB_1_PCIE_RC_AXI_CONFIG);
            regData &= (~((0x1F << 0) /* ARUSER */ |
                          (0x1F << 9) /* AWUSER */ |
                          (0xF << 5)  /* ARCACHE */ |
                          (0xF << 14) /* AWCACHE */));
            regData |= ((aruser << 0)   /* ARUSER */ |
                        (awuser << 9)   /* AWUSER */ |
                        (arcache << 5)  /* ARCACHE */ |
                        (awcache << 14) /* AWCACHE */);
            writel_relaxed(regData, PAXB_1_PCIE_RC_AXI_CONFIG);
            /* PCIe 1 endpoint mode */
            regData = readl_relaxed(PAXB_1_PCIE_EP_AXI_CONFIG);
            regData &= (~((0x1F << 0) /* ARUSER */ |
                          (0x1F << 9) /* AWUSER */ |
                          (0xF << 5)  /* ARCACHE */ |
                          (0xF << 14) /* AWCACHE */));
            regData |= ((aruser << 0)   /* ARUSER */ |
                        (awuser << 9)   /* AWUSER */ |
                        (arcache << 5)  /* ARCACHE */ |
                        (awcache << 14) /* AWCACHE */);
            writel_relaxed(regData, PAXB_1_PCIE_EP_AXI_CONFIG);
            /* PCIe 2 root complex mode */
            regData = readl_relaxed(PAXB_2_PCIE_RC_AXI_CONFIG);
            regData &= (~((0x1F << 0) /* ARUSER */ |
                          (0x1F << 9) /* AWUSER */ |
                          (0xF << 5)  /* ARCACHE */ |
                          (0xF << 14) /* AWCACHE */));
            regData |= ((aruser << 0)   /* ARUSER */ |
                        (awuser << 9)   /* AWUSER */ |
                        (arcache << 5)  /* ARCACHE */ |
                        (awcache << 14) /* AWCACHE */);
            writel_relaxed(regData, PAXB_2_PCIE_RC_AXI_CONFIG);
            /* PCIe 2 endpoint mode */
            regData = readl_relaxed(PAXB_2_PCIE_EP_AXI_CONFIG);
            regData &= (~((0x1F << 0) /* ARUSER */ |
                          (0x1F << 9) /* AWUSER */ |
                          (0xF << 5)  /* ARCACHE */ |
                          (0xF << 14) /* AWCACHE */));
            regData |= ((aruser << 0)   /* ARUSER */ |
                        (awuser << 9)   /* AWUSER */ |
                        (arcache << 5)  /* ARCACHE */ |
                        (awcache << 14) /* AWCACHE */);
            writel_relaxed(regData, PAXB_2_PCIE_EP_AXI_CONFIG);
            /* I2S device */
            regData = readl_relaxed(I2S_M0_IDM_IO_CONTROL_DIRECT);
            regData &= (~((0x1F << 7)  /* ARUSER */ |
                          (0x1F << 24) /* AWUSER */ |
                          (0xF << 16)  /* ARCACHE */ |
                          (0xF << 20)  /* AWCACHE */));
            regData |= ((aruser << 7)   /* ARUSER */ |
                        (awuser << 24)  /* AWUSER */ |
                        (arcache << 16) /* ARCACHE */ |
                        (awcache << 20) /* AWCACHE */);
            writel_relaxed(regData, I2S_M0_IDM_IO_CONTROL_DIRECT);
            /* JTAG connections */
            regData = readl_relaxed(A9JTAG_M0_IDM_IO_CONTROL_DIRECT);
            regData &= (~((0x1F << 20) /* ARUSER */ |
                          (0x1F << 6)  /* AWUSER */ |
                          (0xF << 16)  /* ARCACHE */ |
                          (0xF << 2)   /* AWCACHE */));
            regData |= ((aruser << 20)  /* ARUSER */ |
                        (awuser << 6)   /* AWUSER */ |
                        (arcache << 16) /* ARCACHE */ |
                        (awcache << 2)  /* AWCACHE */);
            writel_relaxed(regData, A9JTAG_M0_IDM_IO_CONTROL_DIRECT);
            /* APBW (?) */
            regData = readl_relaxed(APBW_IDM_IDM_IO_CONTROL_DIRECT);
            regData &= (~((0xF << 24) /* ARCACHE_JTAG */ |
                          (0xF << 20) /* AWCACHE_JTAG */));
            regData |= ((arcache << 24) /* ARCACHE_JTAG */ |
                        (awcache << 20) /* AWCACHE_JTAG */);
            writel_relaxed(regData, APBW_IDM_IDM_IO_CONTROL_DIRECT);
            /* SDIO device */
            /* ARUSER & ARCACHE set in ARMCore_M0_IO_CONTROL_DIRECT_VA */
            regData = readl_relaxed(SDIO_IDM_IO_CONTROL_DIRECT);
            regData &= (~((0x1F << 27) /* AWUSER */ |
                          (0xF << 23)  /* AWCACHE */));
            regData |= ((awuser << 27)  /* AWUSER */ |
                        (awcache << 23) /* AWCACHE */);
            writel_relaxed(regData, SDIO_IDM_IO_CONTROL_DIRECT);
            /* AMAC device */
            regData = readl_relaxed(AMAC_IDM0_IO_CONTROL_DIRECT);
            regData &= (~((0x1F << 25) /* ARUSER */ |
                          (0x1F << 20) /* AWUSER */ |
                          (0xF << 16)  /* ARCACHE */ |
                          (0xF << 7)   /* AWCACHE */));
            regData |= ((aruser << 25)  /* ARUSER */ |
                        (awuser << 20)  /* AWUSER */ |
                        (arcache << 16) /* ARCACHE */ |
                        (awcache << 7)  /* AWCACHE */);
            writel_relaxed(regData, AMAC_IDM0_IO_CONTROL_DIRECT);
            regData = readl_relaxed(AMAC_IDM1_IO_CONTROL_DIRECT);
            regData &= (~((0x1F << 25) /* ARUSER */ |
                          (0x1F << 20) /* AWUSER */ |
                          (0xF << 16)  /* ARCACHE */ |
                          (0xF << 7)   /* AWCACHE */));
            regData |= ((aruser << 25)  /* ARUSER */ |
                        (awuser << 20)  /* AWUSER */ |
                        (arcache << 16) /* ARCACHE */ |
                        (awcache << 7)  /* AWCACHE */);
            writel_relaxed(regData, AMAC_IDM1_IO_CONTROL_DIRECT);
            /* AMAC_FA_M0_IDM_IO_CONTROL_DIRECT */
            regData = readl_relaxed(FA_AMAC_IDM0_IO_CONTROL_DIRECT);
            regData &= (~((0x1F << 25) /* ARUSER */ |
                          (0x1F << 20) /* AWUSER */ |
                          (0xF << 16)  /* ARCACHE */ |
                          (0xF << 7)   /* AWCACHE */));
            regData |= ((aruser << 25)  /* ARUSER */ |
                        (awuser << 20)  /* AWUSER */ |
                        (arcache << 16) /* ARCACHE */ |
                        (awcache << 7)  /* AWCACHE */);
            writel_relaxed(regData, FA_AMAC_IDM0_IO_CONTROL_DIRECT);
            /* AMAC_FA_M1_IDM_IO_CONTROL_DIRECT */
            regData = readl_relaxed(FA_AMAC_IDM1_IO_CONTROL_DIRECT);
            regData &= (~((0x1F << 25) /* ARUSER */ |
                          (0x1F << 20) /* AWUSER */ |
                          (0xF << 16)  /* ARCACHE */ |
                          (0xF << 7)   /* AWCACHE */));
            regData |= ((aruser << 25)  /* ARUSER */ |
                        (awuser << 20)  /* AWUSER */ |
                        (arcache << 16) /* ARCACHE */ |
                        (awcache << 7)  /* AWCACHE */);
            writel_relaxed(regData, FA_AMAC_IDM1_IO_CONTROL_DIRECT);
            /* PAE device */
            regData = readl_relaxed(PAE_M0_IDM_IO_CONTROL_DIRECT);
            regData &= (~((0x1 << 25) /* AWCACHE_SEL */ |
                          (0x1 << 24) /* ARCACHE_SEL */ |
                          (0x1F << 2) /* ARUSER */ |
                          (0x1F << 7) /* AWUSER */ |
                          (0xF << 16) /* ARCACHE */ |
                          (0xF << 20) /* AWCACHE */));
            regData |= ((pae_axcache_override << 25) /* AWCACHE_SEL */ |
                        (pae_axcache_override << 24) /* ARCACHE_SEL */ |
                        (aruser << 2)   /* ARUSER */ |
                        (awuser << 7)   /* AWUSER */ |
                        (arcache << 16) /* ARCACHE */ |
                        (awcache << 20) /* AWCACHE */);
            writel_relaxed(regData, PAE_M0_IDM_IO_CONTROL_DIRECT);
            /* DMA controller */
            /* the AWUSER field is fragmented in this one */
            /* ARUSER set in ARMCore_M0_IO_CONTROL_DIRECT_VA */
            regData = readl_relaxed(DMAC_M0_IDM_IO_CONTROL_DIRECT);
            regData &= (~((0x1 << 30) |
                          (0x3 << 16) |
                          (0x3 << 12))); /* all this is AWUSER */
            regData |= (((awuser & 0x10) << (30 - 4)) |
                        ((awuser & 0x0C) << (16 - 2)) |
                        ((awuser & 0x03) << 12));
            writel_relaxed(regData, DMAC_M0_IDM_IO_CONTROL_DIRECT);
            /* XHCI device */
            regData = readl_relaxed(USB3_IDM_IDM_IO_CONTROL_DIRECT);
            regData &= (~((0x1F << 18) /* ARUSER */ |
                          (0x1F << 12) /* AWUSER */ |
                          (0xF << 6)   /* ARCACHE */ |
                          (0xF << 2)   /* AWCACHE */));
            regData |= ((aruser << 18) /* ARUSER */ |
                        (awuser << 12) /* AWUSER */ |
                        (arcache << 6) /* ARCACHE */ |
                        (awcache << 2) /* AWCACHE */);
            writel_relaxed(regData, USB3_IDM_IDM_IO_CONTROL_DIRECT);
            /* EHCI device */
            regData = readl_relaxed(USB2_IDM_IDM_IO_CONTROL_DIRECT);
            regData &= (~((0xF << 6) /* ARCACHE */ |
                          (0xF << 2) /* AWCACHE */));
            regData |= ((arcache << 6) /* ARCACHE */ |
                        (awcache << 2) /* AWCACHE */);
            writel_relaxed(regData, USB2_IDM_IDM_IO_CONTROL_DIRECT);
            regData = readl_relaxed(USBH_usb2h_axi_m_AxUSER_cfg);
            regData &= (~((0xF << 6) /* ARUSER */ |
                          (0xF << 0) /* AWUSER */));
            regData |= ((aruser << 6) /* ARUSER */ |
                        (awuser << 0) /* AWUSER */);
            writel_relaxed(regData, USBH_usb2h_axi_m_AxUSER_cfg);
            /* AHCI device */
            regData = readl_relaxed(SATA_M0_IDM_IO_CONTROL_DIRECT);
            regData &= (~((0x1F << 2) /* ARUSER */ |
                          (0x1F << 7) /* AWUSER */ |
                          (0xF << 16) /* ARCACHE */ |
                          (0xF << 20) /* AWCACHE */));
            regData |= ((aruser << 2)   /* ARUSER */ |
                        (awuser << 7)   /* AWUSER */ |
                        (arcache << 16) /* ARCACHE */ |
                        (awcache << 20) /* AWCACHE */);
            writel_relaxed(regData, SATA_M0_IDM_IO_CONTROL_DIRECT);
            /*
             *  Configure AXI remap mode
             *
             *  AIXIC_REMAP.REMAP (bits 0..3)
             *    0 = Legacy mode (A0 compatibiliry)
             *    1 = Coherent mode
             *    2 = Non-coherent mode
             *    3..F are reserved
             *
             *  Apparently the AXIIC registers all backed by WOM.
             *  No point in reading first, or verifying it afterward,
             *  so just write blind.
             */
            if (enable) {
                regData = (0x1 /* << 0 */);
            } else {
                regData = (0x0 /* << 0 */);
            }
            writel_relaxed(regData, AXIIC_REMAP);
            regData = readl_relaxed(IHOST_L2C_AUX_CONTROL);
//            printf("IHOST_L2C_AUX_CONTROL = %08X\n", regData);
            regData &= (~((1 << 0)  /* Full_line_zero_en */ |
                          (1 << 25) /* cache_replace_policy */ |
                          (1 << 28) /* Data_prefetch_en */ |
                          (1 << 29) /* Instr_prefetch_en */ |
                          (1 << 30) /* Early_BRESP_en */));
            regData |= ((1 << 11) /* device store buffer limit enable */ |
                        (1 << 25) /* Cache replacement policy - round robin */ |
                        (1 << 28) /* Data prefetch enable */ |
                        (1 << 29) /* Instruction prefetch enable */ |
                        (1 << 30) /* Early BRESP enable */);
            writel_relaxed(regData, IHOST_L2C_AUX_CONTROL);
//            printf("IHOST_L2C_AUX_CONTROL = %08X\n", regData);
            regData = readl_relaxed(IHOST_L2C_TAG_RAM_CTRL);
            regData = 0x121;
            writel_relaxed(regData, IHOST_L2C_TAG_RAM_CTRL);
//            printf("IHOST_L2C_TAG_RAM_CTRL = %08X\n", regData);
            regData = readl_relaxed(IHOST_L2C_DATA_RAM_CTRL);
//            printf("IHOST_L2C_DATA_RAM_CTRL = %08X\n", regData);

            if (!mmu_table_copied) {
                for (index = 0, mmuEntry = mmu_table_addr;
                     index < MMU_TABLE_SIZE;
                     index++) {
                    mmu_table_backup[index] = mmuEntry[index];
                }
                mmu_table_copied = 1;
            }

            if (cacheMode) {
                /* make DRAM cacheable */
                dramModeOr = 0;
                dramModeOr |= (((dramMode & 0x20) << (16 - 5)) |
                               ((dramMode & 0x1C) << (12 - 2)) |
                               ((dramMode & 0x03) << (2 - 0)));
//                printf("adjusting DRAM sections: %08X\n", dramModeOr);
                for (index = 0, mmuEntry = mmu_table_addr;
                     index < MMU_TABLE_SIZE;
                     index++) {
                    regData = mmuEntry[index];
                    virtAddr = regData & 0xFFF00000;
                    if ((0x60000000 <= virtAddr) &&
                        (0xE0000000 > virtAddr) &&
                        (0x00040002 == (regData & 0x00040003))) {
                        /* this section is in DRAM space */
                        regData &= 0xFFFE8FF3;
                        regData |= dramModeOr;
                        mmuEntry[index] = regData;
                    }
                }
            } else {
                /* revert MMU table to original */
//                printf("setting DRAM to default cache values\n");
                for (index = 0, mmuEntry = mmu_table_addr;
                     index < MMU_TABLE_SIZE;
                     index++) {
                    mmuEntry[index] = mmu_table_backup[index];
                }
            }
            /* ensure updates committed to memory */
            flush_dcache_range((unsigned long)mmuEntry,
                               (unsigned long)&(mmuEntry[MMU_TABLE_SIZE]));
            /* invalidate TLB */
            asm volatile ("mov r1, #0;\
                           MCR p15,0,r1,c8, c5, 0");
/*            printf("NS+ B0 or later: DRAM %scache, %s mode, %u, %u, %u, %u, %s\n",
                   cacheMode?"":"non-",
                   enable?"coherent":"legacy",
                   aruser,
                   awuser,
                   arcache,
                   awcache,
                   pae_axcache_override?"TRUE":"FALSE");
*/
//        } else { /* if (revision is B0 or higher) */
//            printf("NorthStar+ < B0 in legacy mode.\n");
//            result = -1;
//        } /* if (revision is B0 or higher) */
    } else {
//        printf("unable to set NS+ coherent mode on non-NS+ device\n");
        result = -1;
    }
    return result;
}
/*
extern int
strtouint(const char *buffer, const char **next, unsigned int base, unsigned int *value);

int nsp_coherent_mode_enable()
{
    static u8 cacheMode = 0;
    static u8 arUser = 3;
    static u8 awUser = 3;
    static u8 arCache = 3;
    static u8 awCache = 3;
    static u8 dramMode = 0x35;
    static u8 pae_axcache_override = 1;
    unsigned int temp;
    char readbuffer[CONFIG_SYS_CBSIZE];
    int result;

    snprintf(readbuffer, CONFIG_SYS_CBSIZE - 1, "%u", cacheMode);
    if (readline_into_buffer("DRAM cacheable by core? (1=yes, 0=no) :",
                             readbuffer,
                             0)) {
        result = strtouint(readbuffer, NULL, 10, &temp);
        if (0 == result) {
            if ((0 <= temp) && (1 >= temp)) {
                cacheMode = temp;
            } else {
                result = -1;
            }
        } else {
            printf("error %d parsing response\n", result);
        }
        if (0 != result) {
            printf("using prior value %u\n", cacheMode);
        }
    }
    snprintf(readbuffer, CONFIG_SYS_CBSIZE - 1, "%u", dramMode);
    if (readline_into_buffer("DRAM S,TEX[2:0],C,B? (packed bits) :",
                             readbuffer,
                             0)) {
        result = strtouint(readbuffer, NULL, 10, &temp);
        if (0 == result) {
            if ((0 <= temp) && (0x3F >= temp)) {
                dramMode = temp;
            } else {
                result = -1;
            }
        } else {
            printf("error %d parsing response\n", result);
        }
        if (0 != result) {
            printf("using prior value %u\n", dramMode);
        }
    }
    snprintf(readbuffer, CONFIG_SYS_CBSIZE - 1, "%u", pae_axcache_override);
    if (readline_into_buffer("Override PAE AxUSER,AxCACHE? (1=yes, 0=no) :",
                             readbuffer,
                             0)) {
        result = strtouint(readbuffer, NULL, 10, &temp);
        if (0 == result) {
            if ((0 <= temp) && (1 >= temp)) {
                pae_axcache_override = temp;
            } else {
                result = -1;
            }
        } else {
            printf("error %d parsing response\n", result);
        }
        if (0 != result) {
            printf("using prior value %u\n", pae_axcache_override);
        }
    }
    snprintf(readbuffer, CONFIG_SYS_CBSIZE - 1, "%u", arUser);
    if (readline_into_buffer("ARUSER value? (0..31) :",
                             readbuffer,
                             0)) {
        result = strtouint(readbuffer, NULL, 10, &temp);
        if (0 == result) {
            if ((0 <= temp) && (31 >= temp)) {
                arUser = temp;
            } else {
                result = -1;
            }
        } else {
            printf("error %d parsing response\n", result);
        }
        if (0 != result) {
            printf("using prior value %u\n", arUser);
        }
    }
    snprintf(readbuffer, CONFIG_SYS_CBSIZE - 1, "%u", awUser);
    if (readline_into_buffer("AWUSER value? (0..31) :",
                             readbuffer,
                             0)) {
        result = strtouint(readbuffer, NULL, 10, &temp);
        if (0 == result) {
            if ((0 <= temp) && (31 >= temp)) {
                awUser = temp;
            } else {
                result = -1;
            }
        } else {
            printf("error %d parsing response\n", result);
        }
        if (0 != result) {
            printf("using prior value %u\n", awUser);
        }
    }
    snprintf(readbuffer, CONFIG_SYS_CBSIZE - 1, "%u", arCache);
    if (readline_into_buffer("ARCACHE value? (0..15) :",
                             readbuffer,
                             0)) {
        result = strtouint(readbuffer, NULL, 10, &temp);
        if (0 == result) {
            if ((0 <= temp) && (15 >= temp)) {
                arCache = temp;
            } else {
                result = -1;
            }
        } else {
            printf("error %d parsing response\n", result);
        }
        if (0 != result) {
            printf("using prior value %u\n", arCache);
        }
    }
    snprintf(readbuffer, CONFIG_SYS_CBSIZE - 1, "%u", awCache);
    if (readline_into_buffer("AWCACHE value? (0..15) :",
                             readbuffer,
                             0)) {
        result = strtouint(readbuffer, NULL, 10, &temp);
        if (0 == result) {
            if ((0 <= temp) && (15 >= temp)) {
                awCache = temp;
            } else {
                result = -1;
            }
        } else {
            printf("error %d parsing response\n", result);
        }
        if (0 != result) {
            printf("using prior value %u\n", awCache);
        }
    }
    return _nsp_coherent_mode_set(1 ,
                                  cacheMode,
                                  dramMode,
                                  arUser,
                                  awUser,
                                  arCache,
                                  awCache,
                                  pae_axcache_override);
}
*/
/*
int nsp_legacy_mode_enable()
{
    return _nsp_coherent_mode_set(0, 0, 0x10, 0x1F, 0x1F, 0xF, 0xF, 0);
}
*/

int do_ccsetup(void)
{
//int res;
//if (atoi(argv[1]) == 1)
return(_nsp_coherent_mode_set(1, 1, 0x35, 0x3, 0x3, 0xB, 0x7, 1));
//else
//res=_nsp_coherent_mode_set(0, 0, 0x10, 0x1F, 0x1F, 0xF, 0xF, 0);

//return(res);
}
/*
U_BOOT_CMD(
	ccsetup,      1,     2,      do_ccsetup,
	"ccsetup	- Enable coherency settings \n",
	"ccsetup\n"
);
*/
/*
const char *_l2c_evShort[] = {
    "disabled",
    "CO",
    "DRHIT",
    "DRREQ",
    "DWHIT",
    "DWREQ",
    "DWTREQ",
    "IRHIT",
    "IRREQ",
    "WA",
    "IPFALLOC",
    "EPFHIT",
    "EPFALLOC",
    "SRRCVD",
    "SRCONF",
    "EPFRCVD"
};
const char *_l2c_evIntr[] = {
    "disabled",
    "on increment",
    "on overflow",
    "disabled"
};
const char *_l2c_evLong[] = {
    "disabled",
    "Cast Out (line eviction)",
    "Data read hit",
    "Data read request",
    "Data write hit",
    "Data write request",
    "Data write-through request",
    "Instruction read hit",
    "Instruction read request",
    "Write allocate",
    "Allocation of prefetch generated by L2C",
    "Prefetch hint hit",
    "Prefetch hint allocation",
    "Speculative read received",
    "Speculative read confirmed",
    "Prefetch hint received"
};

char _toupper(const char c)
{
    if (('a' <= c) && ('z' >= c)) {
        return (c - ('a' - 'A'));
    } else {
        return c;
    }
}

int _strncasecmp(const char *a, const char *b, unsigned int l)
{
    char ac;
    char bc;
    unsigned int i;
    int r = 0;
    if (a && b) {
        for (i = 0; i < l; i++) {
            ac = a[i];
            bc = b[i];
            r = (_toupper(ac) - _toupper(bc));
            if (r || (0 == ac) || (0 == bc)) {
                break;
            }
        }
    } else if (a) {
        r = 1;
    } else if (b) {
        r = -1;
    }
    return r;
}

int nsp_l2c_evctr_ctrl()
{
    char readbuffer[CONFIG_SYS_CBSIZE];
    char *nextChar;
    volatile u32 *ctrCtrl = (u32*)IHOST_L2C_EVENT_CTR_CTRL;
    volatile u32 *ctr0Cfg = (u32*)IHOST_L2C_EVENT_CTR_CFG0;
    volatile u32 *ctr1Cfg = (u32*)IHOST_L2C_EVENT_CTR_CFG1;
    u32 regVal;
    unsigned int temp;
    unsigned int setting;
    unsigned int evSrc0;
    unsigned int evSrc1;
    int result;

    regVal = *ctrCtrl;
    if (regVal & (1 << IHOST_L2C_EVENT_CTR_CTRL__Counter_en_R)) {
        setting = 1;
    } else {
        setting = 0;
    }
    snprintf(readbuffer, CONFIG_SYS_CBSIZE - 1, "%u", setting);
    if (readline_into_buffer("Enable L2C event counters? (0=no; 1=yes) :",
                             readbuffer,
                             0)) {
        result = strtouint(readbuffer, NULL, 10, &temp);
        if (0 == result) {
            if ((0 <= temp) && (1 >= temp)) {
                setting = temp;
            } else {
                result = -1;
            }
        } else {
            printf("error %d parsing response\n", result);
        }
        if (0 != result) {
            printf("using prior value %u\n", setting);
        }
    }
    regVal &= (~(1 << IHOST_L2C_EVENT_CTR_CTRL__Counter_en_R));
    *ctrCtrl = regVal;

    if (setting) {
        printf("Supported sources:\n");
        for (temp = 0; temp < 16; temp++) {
            printf("  %2u: %10s  --  %s\n",
                   temp,
                   _l2c_evShort[temp],
                   _l2c_evLong[temp]);
        }
        printf("\n");

        evSrc0 = (((*ctr0Cfg) >> IHOST_L2C_EVENT_CTR_CFG0__Counter_event_source0_R) &
                  ((1 << IHOST_L2C_EVENT_CTR_CFG0__Counter_event_source0_WIDTH) - 1));
        snprintf(readbuffer, CONFIG_SYS_CBSIZE - 1, "%s", _l2c_evShort[evSrc0]);
        if (readline_into_buffer("Event counter 0 source :",
                                 readbuffer,
                                 0)) {
            for (temp = 0; temp < 16; temp++) {
                if (!_strncasecmp(readbuffer,
                                  _l2c_evShort[temp],
                                  strlen(readbuffer))) {
                    break;
                }
            }
            if (temp < 16) {
                result = 0;
                evSrc0 = temp;
            } else {
                result = strtouint(readbuffer, &nextChar, 10, &temp);
                if ((0 == result) && (nextChar != readbuffer)) {
                    if ((0 <= temp) && (15 >= temp)) {
                        evSrc0 = temp;
                    } else {
                        result = -1;
                    }
                } else {
                    result = -1;
                    printf("error %d parsing response\n", result);
                }
            }
            if (0 != result) {
                printf("using prior value %u: %s -- %s\n",
                       evSrc0,
                       _l2c_evShort[evSrc0],
                       _l2c_evLong[evSrc0]);
            }
        }
        evSrc1 = (((*ctr1Cfg) >> IHOST_L2C_EVENT_CTR_CFG1__Counter_event_source1_R) &
                  ((1 << IHOST_L2C_EVENT_CTR_CFG1__Counter_event_source1_WIDTH) - 1));
        snprintf(readbuffer, CONFIG_SYS_CBSIZE - 1, "%s", _l2c_evShort[evSrc1]);
        if (readline_into_buffer("Event counter 1 source :",
                                 readbuffer,
                                 0)) {
            for (temp = 0; temp < 16; temp++) {
                if (!_strncasecmp(readbuffer,
                                  _l2c_evShort[temp],
                                  strlen(readbuffer))) {
                    break;
                }
            }
            if (temp < 16) {
                result = 0;
                evSrc1 = temp;
            } else {
                result = strtouint(readbuffer, NULL, 10, &temp);
                if ((0 == result) && (nextChar != readbuffer)) {
                    if ((0 <= temp) && (15 >= temp)) {
                        evSrc1 = temp;
                    } else {
                        result = -1;
                    }
                } else {
                    result = -1;
                    printf("error %d parsing response\n", result);
                }
            }
            if (0 != result) {
                printf("using prior value %u: %s -- %s\n",
                       evSrc1,
                       _l2c_evShort[evSrc1],
                       _l2c_evLong[evSrc1]);
            }
        }
        regVal = (evSrc0 << IHOST_L2C_EVENT_CTR_CFG0__Counter_event_source0_R);
        *ctr0Cfg = regVal;
        regVal = (evSrc1 << IHOST_L2C_EVENT_CTR_CFG0__Counter_event_source0_R);
        *ctr1Cfg = regVal;
        regVal = *ctrCtrl;
        regVal |= ((1 << IHOST_L2C_EVENT_CTR_CTRL__Counter_en_R) ||
                   (((1 << IHOST_L2C_EVENT_CTR_CTRL__Counter_reset_WIDTH) - 1) <<
                    IHOST_L2C_EVENT_CTR_CTRL__Counter_reset_R));
        *ctrCtrl = regVal;
    }

    return 0;
}

int nsp_l2c_evctr_read()
{
    volatile u32 *ctrCtrl = (u32*)IHOST_L2C_EVENT_CTR_CTRL;
    volatile u32 *ctr0Cfg = (u32*)IHOST_L2C_EVENT_CTR_CFG0;
    volatile u32 *ctr1Cfg = (u32*)IHOST_L2C_EVENT_CTR_CFG1;
    volatile u32 *ctr0Val = (u32*)IHOST_L2C_EVENT_CTR0;
    volatile u32 *ctr1Val = (u32*)IHOST_L2C_EVENT_CTR1;
    u32 regVal;
    u32 temp0;
    u32 temp1;
    u32 val0;
    u32 val1;

    regVal = *ctrCtrl;
    if (regVal & (1 << IHOST_L2C_EVENT_CTR_CTRL__Counter_en)) {
        printf("L2C event counters %sabled\n", "en");
        regVal = *ctr0Cfg;
        temp0 = ((regVal >> IHOST_L2C_EVENT_CTR_CFG0__Counter_event_source0_R) &
                 ((1 << IHOST_L2C_EVENT_CTR_CFG0__Counter_event_source0_WIDTH) - 1));
        temp1 = ((regVal >> IHOST_L2C_EVENT_CTR_CFG0__Counter_interrupt_gen0_R) &
                 ((1 << IHOST_L2C_EVENT_CTR_CFG0__Counter_interrupt_gen0_WIDTH) - 1));
        printf("Counter 0:\n");
        printf("  Event %2u: %10s -- %s\n",
               temp0,
               _l2c_evShort[temp0],
               _l2c_evLong[temp0]);
        printf("  Interrupt %s\n", _l2c_evIntr[temp1]);
        regVal = *ctr1Cfg;
        temp0 = ((regVal >> IHOST_L2C_EVENT_CTR_CFG0__Counter_event_source0_R) &
                 ((1 << IHOST_L2C_EVENT_CTR_CFG0__Counter_event_source0_WIDTH) - 1));
        temp1 = ((regVal >> IHOST_L2C_EVENT_CTR_CFG0__Counter_interrupt_gen0_R) &
                 ((1 << IHOST_L2C_EVENT_CTR_CFG0__Counter_interrupt_gen0_WIDTH) - 1));
        val0 = *ctr0Val;
        val1 = *ctr1Val;
        regVal = *ctrCtrl;
        *ctrCtrl = (regVal |
                    (((1 << IHOST_L2C_EVENT_CTR_CTRL__Counter_reset_WIDTH) - 1) <<
                     IHOST_L2C_EVENT_CTR_CTRL__Counter_reset_R));
        printf("  Value %08X (%u)\n", val0, val0);
        printf("Counter 1:\n");
        printf("  Event %2u: %10s -- %s\n",
               temp0,
               _l2c_evShort[temp0],
               _l2c_evLong[temp0]);
        printf("  Interrupt %s\n", _l2c_evIntr[temp1]);
        printf("  Value %08X (%u)\n", val1, val1);
    } else {
        printf("L2C event counters %sabled\n", "dis");
    }
    return 0;
}
*/
