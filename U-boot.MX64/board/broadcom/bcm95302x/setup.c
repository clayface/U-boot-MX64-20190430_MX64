#include <common.h>
#include <asm/io.h>

#define ICFG_CHIP_ID_REG		(0x18000000)
#define ChipcommonA_GPIOInput		(0x18000060)
#define ChipcommonA_GPIOOut		(0x18000064)
#define ChipcommonA_GPIOOutEn		(0x18000068)
#define CRU_GPIO_CONTROL0               (0x1803F1C0)
#define IPROC_IDM_DMAC_RESET_CONTROL	(0x18114800)
#define USB2_IDM_IDM_IO_CONTROL_DIRECT	(0x18115408)
#define IPROC_IDM_USB2_RESET_CONTROL	(0x18115800)
#define IPROC_STRAP_SKU_VECTOR		(0x1811A500)

extern int do_ccsetup(void);

int ehci_init(void)
{
	int usb2_clk_enable, usb2_reset_state;

	__raw_readl(IPROC_STRAP_SKU_VECTOR);
	usb2_clk_enable = __raw_readl(USB2_IDM_IDM_IO_CONTROL_DIRECT);
//	printf("Before reset, USB clk enable register is: %08x\n", usb2_clk_enable);
	usb2_clk_enable |= 0x1;
	__raw_writel( usb2_clk_enable, USB2_IDM_IDM_IO_CONTROL_DIRECT);
	usb2_clk_enable = __raw_readl(USB2_IDM_IDM_IO_CONTROL_DIRECT);
//	printf("Before reset, USB clk enable register is: %08x\n", usb2_clk_enable);
	udelay(1000);
	usb2_reset_state = __raw_readl(IPROC_IDM_USB2_RESET_CONTROL);
	if ((usb2_reset_state & 1) == 1)
	{
//		printf("\nBring usb2h_out of reset.......\n");
		__raw_writel(0x0, IPROC_IDM_USB2_RESET_CONTROL);
		udelay(1000);
		usb2_reset_state = __raw_readl(IPROC_IDM_USB2_RESET_CONTROL);
//		printf("usb2_reset_state is set and now it is: %08x\n", usb2_reset_state);
	};
	return 0;
};
/*
U_BOOT_CMD(
	ehciinit, 1, 0, ehci_init, "", ""
);
*/

int periph_enable(void)
{
	int tmp;

	/* enable orange pins, USB port, reset button & white led CRU PINs */
	tmp = __raw_readl(CRU_GPIO_CONTROL0);
	tmp |= ( 1 << 0 ); tmp |= ( 1 << 1 ); tmp |= ( 1 << 6 ); tmp |= ( 1 << 31 );
	__raw_writel(tmp, CRU_GPIO_CONTROL0);

	/* enable USB power */
	tmp = __raw_readl(ChipcommonA_GPIOOutEn);
	tmp |= ( 1 << 1 );
	__raw_writel(tmp, ChipcommonA_GPIOOutEn);

	/* Bring DMA out of reset */
	__raw_writel(0x0, IPROC_IDM_DMAC_RESET_CONTROL);

	return 0;
};
/*
int dma_init(void)
{
	return (__raw_writel(0x0, IPROC_IDM_DMAC_RESET_CONTROL));
};
*/
int led_flash(int time)
{
	uint32_t out;
	uint32_t outen;

	out = __raw_readl(ChipcommonA_GPIOOut);
	outen = __raw_readl(ChipcommonA_GPIOOutEn);
	out ^= ( 1 << 31 ); outen ^= ( 1 << 0 );
	__raw_writel(out, ChipcommonA_GPIOOut);
	__raw_writel(outen, ChipcommonA_GPIOOutEn);

	int i=0;
	for(i;i<6;i++) {
	
		out ^= ( 1 << 0 ); outen ^= ( 1 << 31 );
		__raw_writel(out, ChipcommonA_GPIOOut);
		__raw_writel(outen, ChipcommonA_GPIOOutEn);
		mdelay(time);
	};
};

int get_socrev(void)
{
	uint32_t regData = __raw_readl(ICFG_CHIP_ID_REG);
	uint32_t tmp = ((regData & 0x000F0000) >> 16);
	if (4 <= tmp)
		return 1;
	else
		return 0;
};

int usb_boot(void)
{
//	uint32_t regData;
//	uint32_t tmp;
//	tmp = __raw_readl(ChipcommonA_GPIOInput);
//	tmp = tmp & (1 << 6);
//	if (tmp == 0) {
//		printf("\nRESET pressed: USB boot enabled\n");
		led_flash(250);

		run_command("usb start", 0);

		if (get_socrev() == 1) {
			run_command("fatload usb 0:1 ${loadaddr} \
			${initramfs_filename}", 0);
		}
		else {
			run_command("fatload usb 0:1 ${loadaddr} \
			${initramfs_filename_a0}", 0);
		}
		run_command("bootm ${loadaddr}#${config_dts}", 0);
//	};

	return 0;
};

int rst_check(void)
{
	uint32_t regData;
	uint32_t tmp;
	tmp =  __raw_readl(ChipcommonA_GPIOInput);
	tmp = tmp & (1 << 6);
	return tmp;
}

int dev_init(void)
{
	do_ccsetup();
	periph_enable();
	ehci_init();

	if (rst_check() == 0) {
		printf("\nRESET pressed: USB boot enabled\n");
		if (usb_boot() == 1)
			if (rst_check() == 0) {
				led_flash(100);
				run_command_list("env default -a -f && saveenv", -1, 0);
			}
	}
	return 0;
}

U_BOOT_CMD(
	mx64init, 1, 0, dev_init, "", ""
);

U_BOOT_CMD(
	usbload, 1, 0, usb_boot, "",""
);

U_BOOT_CMD(
        getsocrev, 1, 0, get_socrev, "",""
);
