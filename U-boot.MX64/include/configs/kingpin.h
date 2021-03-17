#ifndef __KINGPIN_H
#define __KINGPIN_H

#include <asm/sizes.h>

#define CONFIG_BOARD_EARLY_INIT_F (1)
#define CONFIG_BOARD_LATE_INIT
#define CONFIG_PHYS_SDRAM_1_SIZE			0x02000000 /* 32 MB */
#define IPROC_ETH_MALLOC_BASE 0xD00000

#define STDK_BUILD (1)

/* Architecture, CPU, etc */
#define CONFIG_ARMV7
#define CONFIG_IPROC (1)
#define CONFIG_NS_PLUS (1)

/*
Fix me

When DEBUG is enabled, need to disable both CACHE to make u-boot running
#define CONFIG_SYS_NO_ICACHE
#define CONFIG_SYS_NO_DCACHE
#define DEBUG

*/
#define CONFIG_IPROC_MMU	(1)
#define CONFIG_L2_OFF				/* Disable L2 cache */
#define CONFIG_SYS_ARM_CACHE_WRITETHROUGH (1)

#define CONFIG_MISC_INIT_R			/* Call board's misc_init_r function */

/* Interrupt configuration */
#define CONFIG_USE_IRQ          1	/* we need IRQ stuff for timer	*/

/* Memory Info */
#undef CONFIG_SYS_MALLOC_LEN
#define CONFIG_SYS_MALLOC_LEN 			SZ_2M  	/* see armv7/start.S. */
#define CONFIG_SYS_MEM_TOP_HIDE			SZ_64K
#define CONFIG_STACKSIZE_IRQ			(4096)
#define CONFIG_STACKSIZE_FIQ			(4096)
#ifdef CONFIG_L2C_AS_RAM
//#define CONFIG_PHYS_SDRAM_0				0xc0000000
#define CONFIG_PHYS_SDRAM_0				0x50000000  /* SRAM */
#define CONFIG_L2_CACHE_SIZE			0x40000  
#define CONFIG_PHYS_SDRAM_1				0x60000000
#define CONFIG_LOADADDR					0x90000000 /* default destination location for tftp file (tftpboot cmd) */
#define CONFIG_PHYS_SDRAM_RSVD_SIZE		0x0 /* bytes reserved from CONFIG_PHYS_SDRAM_1 for custom use */
#else
#define CONFIG_PHYS_SDRAM_0				0x00000000
#define CONFIG_PHYS_SDRAM_1				0x00000000
#define CONFIG_LOADADDR					0x90000000 /* default destination location for tftp file (tftpboot cmd) */
#define CONFIG_PHYS_SDRAM_RSVD_SIZE		0x00100000 /* bytes reserved from CONFIG_PHYS_SDRAM_1 for custom use */
#endif /* CONFIG_L2C_AS_RAM */

/* Where kernel is loaded to in memory */
#define CONFIG_SYS_LOAD_ADDR				0x70000000
#define LINUX_BOOT_PARAM_ADDR				0x60200000 /* default mapped location to store the atags pointer */

#define CONFIG_SYS_MEMTEST_START			CONFIG_PHYS_SDRAM_1
#define CONFIG_SYS_MEMTEST_END			(CONFIG_PHYS_SDRAM_1+CONFIG_PHYS_SDRAM_1_SIZE)
#define CONFIG_NR_DRAM_BANKS				1

#define CONFIG_SYS_SDRAM_BASE		(CONFIG_PHYS_SDRAM_1 + CONFIG_PHYS_SDRAM_RSVD_SIZE)
/* CONFIG_SYS_TEXT_BASE is where u-boot is loaded by boot1 */
#ifdef CONFIG_L2C_AS_RAM
#define CONFIG_SYS_STACK_SIZE		(0x0001f000)
#else
#define CONFIG_SYS_STACK_SIZE		(0x00010000) /* 64K */       
#endif /* CONFIG_L2C_AS_RAM */

#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_PHYS_SDRAM_0 + CONFIG_SYS_STACK_SIZE - 16)
//#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_PHYS_SDRAM_1 + CONFIG_SYS_STACK_SIZE - 16)

/* Clocks */
#define CONFIG_SYS_REF_CLK			(25000000) /*Reference clock = 25MHz */
#define CONFIG_SYS_REF2_CLK			(200000000) /*Reference clock = 25MHz */
#define IPROC_ARM_CLK		(1000000000) /* 1GHz */
#define IPROC_AXI_CLK		(500000000)  /* 500 MHz */
#define IPROC_APB_CLK		(125000000)  /* 125 MHz */

#define CONFIG_ENV_OVERWRITE	/* Allow serial# and ethernet mac address to be overwritten in nv storage */
#ifdef CONFIG_L2C_AS_RAM
#define CONFIG_ENV_SIZE			0x1000 /* 4K */
#else
#define CONFIG_ENV_SIZE			0x10000 /* 64K */
#endif /* CONFIG_L2C_AS_RAM */
/* NO flash */
#define CONFIG_SYS_NO_FLASH		/* Not using NAND/NOR unmanaged flash */

/* Ethernet configuration */
#ifndef CONFIG_NO_CODE_RELOC
#define CONFIG_BCMIPROC_ETH
#endif /* CONFIG_NO_CODE_RELOC */
//#define CONFIG_BCMHANA_ETH
//#define CONFIG_NET_MULTI
#define CONFIG_CMD_LOADB

/* DMA configuration */
//#define CONFIG_BCM5301X_DMA

/* General U-Boot configuration */
#define CONFIG_SYS_CBSIZE			1024	/* Console buffer size */
#define CONFIG_SYS_PBSIZE			(CONFIG_SYS_CBSIZE +	sizeof(CONFIG_SYS_PROMPT) + 16) /* Printbuffer size */
#define CONFIG_SYS_MAXARGS			64
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE

//#define CONFIG_VERSION_VARIABLE	/* Enabled UBOOT build date/time id string */
#define CONFIG_AUTO_COMPLETE
//#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_CMDLINE_EDITING
#define CONFIG_SYS_LONGHELP

#define CONFIG_CRC32_VERIFY		/* Add crc32 to memory verify commands */
#define CONFIG_MX_CYCLIC			/* Memory display cyclic */

#define CONFIG_CMDLINE_TAG				/* ATAG_CMDLINE setup */
#define CONFIG_SETUP_MEMORY_TAGS		/* ATAG_MEM setup */

#define CONFIG_SYS_PROMPT					"u-boot> "  

//#include <config_cmd_default.h>
#define CONFIG_CMD_CONSOLE
//#define CONFIG_CMD_NFS
//#undef CONFIG_GENERIC_MMC
//#define CONFIG_CMD_PING
#define CONFIG_CMD_MEMORY
#define CONFIG_CMD_MISC
//#define CONFIG_CMD_LICENSE

#undef CONFIG_ETHADDR
#define CONFIG_ETHADDR		00:18:0a:00:01:02
//#define CONFIG_NETMASK		255.255.255.0
#define CONFIG_IPADDR		192.168.0.1
//#define CONFIG_GATEWAYIP	192.168.0.100
//#define CONFIG_SERVERIP		192.168.0.10
#undef CONFIG_SERVERIP

#define CONFIG_BOOTP_MAY_FAIL

#ifdef UBOOT_MDK
#define CONFIG_GMAC_NUM		2 
#else
#define CONFIG_GMAC_NUM		2
#endif /* UBOOT_MDK */

/* Environment variables */
#undef CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_IS_IN_NAND                   1
#define CONFIG_ENV_OFFSET                       0x180000
#define CONFIG_ENV_RANGE                        0x080000

/* Environment variables for NAND flash */
#define CONFIG_CMD_NAND 
#define CONFIG_IPROC_NAND 
#define CONFIG_SYS_MAX_NAND_DEVICE			1
#define CONFIG_SYS_NAND_BASE		        0xdeadbeef
#define CONFIG_SYS_NAND_ONFI_DETECTION

//#define CONFIG_CMD_UBI
//#define CONFIG_CMD_UBIFS
//#define CONFIG_RBTREE
//#define CONFIG_MTD_DEVICE               /* needed for mtdparts commands */
//#define CONFIG_MTD_PARTITIONS
//#define CONFIG_CMD_MTDPARTS

#define CONFIG_CMD_CACHE
//#define CONFIG_CMD_ELF
#undef CONFIG_CMD_IMI


#undef CONFIG_CMD_DIAG
//#define CONFIG_SWANG_DEBUG_BUILD
#ifdef CONFIG_SWANG_DEBUG_BUILD
#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_SYS_INIT_SP_ADDR         (0x10000- 16)
#endif
/*
#define CONFIG_DIAGS_MEMTEST_START                (CONFIG_PHYS_SDRAM_1 + 0x1000000 )//offset by 16 MB if not running from L2
#define CONFIG_DIAGS_MEMTEST_END                  0x8000000
*/

#if (defined(CONFIG_NS_PLUS))
#define CONFIG_DIAGS_MEMTEST_START                0x61000000//offset by 16 MB if not running from L2
#define CONFIG_DIAGS_MEMTEST_END                  0xA0000000
#else
#define CONFIG_DIAGS_MEMTEST_START                0x81000000//offset by 16 MB if not running from L2
#define CONFIG_DIAGS_MEMTEST_END                  0xC0000000
#endif

//Chip Common A UART 0
//#define CONFIG_SYS_NS16550_COM1                       (0x18000300)
//#ifndef CONFIG_NO_COM2
////Chip Common A UART 1
//#define CONFIG_SYS_NS16550_COM2                 (0x18000400)
//#endif
//#ifndef CONFIG_NO_COM3
////Chip Common B UART 0
//#define CONFIG_SYS_NS16550_COM3                 (0x18037000)
//#endif
#define CONFIG_NO_COM2
#define CONFIG_NO_COM3

#define  CONFIG_UART0_INDEX              1
#define  CONFIG_UART1_INDEX              2
#define  CONFIG_UART2_INDEX              3

/* USB */
#define CONFIG_CMD_USB
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_IPROC
#define CONFIG_USB_STORAGE
#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION
//#define CONFIG_LEGACY_USB_INIT_SEQ

/* PCIE */
//#define CONFIG_CMD_PCI
//#define CONFIG_CMD_PCI_ENUM
//#define CONFIG_PCI
//#define CONFIG_PCI_SCAN_SHOW
//#define CONFIG_IPROC_PCIE

/* SDIO */
//#define CONFIG_CMD_MMC
//#define CONFIG_GENERIC_MMC
//#define CONFIG_IPROC_MMC

// #define CONFIG_AUTOBOOT_KEYED
// #define CONFIG_AUTOBOOT_STOP_STR "xyzzy"

//#define CONFIG_BOOT_RETRY_TIME 15
//#define CONFIG_RESET_TO_RETRY

/* Sound */
//#define CONFIG_IPROC_I2S
//#define CONFIG_IPROC_PCM

/* USB3.0 */
//#define CONFIG_USB_XHCI_IPROC

/* SATA */
//#define CONFIG_IPROC_AHCI

//#define CONFIG_NS_DMA

#include "iproc_common_configs.h"

#undef MTDPARTS_DEFAULT
#define MTDPARTS_DEFAULT    "mtdparts=nand_iproc.0:2048k(U-boot),2048k(U-boot-env),2048k(U-boot-backup),2048K(U-boot-env-backup),0x3F700000(ubi)"

#undef CONFIG_BOOTARGS
#define CONFIG_BOOTARGS     				"console=ttyS0,115200n8 earlyprintk"
#define CONFIG_BAUDRATE 115200
//#define CONFIG_BOOTARGS     	"console=ttyS0,115200n8 maxcpus=2 mem=512M"

#define CONFIG_BOOTDELAY		5	/* User can hit a key to abort kernel boot and stay in uboot cmdline */
//#define CONFIG_BOOTCOMMAND 		"dhcp; run nfsargs; bootm;"	/* UBoot command issued on power up */

// machid=bb8 is from the Broadcom iProc Linux Development Kit Guide Version 0.21
//#define MERAKI_AUTOBOOT_CMD   "sleep 10; nand read 0x60008000 0x500000 0x200000; bootbk 0x60008000 bootkernel2 ; nand read 0x60008000 0x100000 0x200000; bootbk 0x60008000 bootkernel1"

#define MERAKI_AUTOBOOT_CMD 	"sleep 3; mx64init; usbload; nand read 0x60008000 0x100000 0x300000; bootbk 0x60008000 bootkernel2"

#define CONFIG_IDENT_STRING   "Meraki MX64 Boot Kernel Loader"

//#define CONFIG_EXTRA_ENV_SETTINGS \
//    "brcmtag=1\0"                 \
//    "console=ttyS0\0"             \
//    "loglevel=7\0"                \
//    "tftpblocksize=512\0"         \
//    "hostname=northstar\0"        \
//	"machid=bb8\0"                \
//    "vlan1ports=0 1 2 3 8u\0"     \
//    "netloadmethod=dhcp\0"        \
//    "ubootfile=u-boot-kingpin\0"  \
//    "ubootsize=0x00200000\0"      \
//    "loaduboot=${netloadmethod} ${meraki_loadaddr} ${serverpath}${ubootfile}\0" \
//    "eraseboot=nand erase 0x0 ${ubootsize}\0" \
//    "writeboot=nand write ${meraki_loadaddr} 0x0 ${filesize}\0" \
//    "updateuboot=run loaduboot eraseboot writeboot\0" \
//    "meraki_loadaddr=0x64000000\0" \
//    "mtdparts=" MTDPARTS_DEFAULT "\0" \
//    "meraki_bootfile=wired-arm-nsp.itb\0" \
//    "meraki_bootlinux=bootm ${meraki_loadaddr}\0" \
//	"meraki_boot=run meraki_ubi meraki_load_script meraki_run_script; " \
//        "run meraki_load_part_safe meraki_bootlinux; " \
//        "run meraki_load_part_new meraki_bootlinux; " \
//        "run meraki_load_part_old meraki_bootlinux; " \
//        "run meraki_load_part1 meraki_bootlinux; " \
//        "run meraki_load_part2 meraki_bootlinux; " \
//        "run meraki_netboot;" \
//        "run meraki_netboot_static\0" \
//    "meraki_ubi=ubi part ubi\0" \
//    "meraki_load_part_safe=ubi read ${meraki_loadaddr} part.safe\0" \
//    "meraki_load_part_new=ubi read ${meraki_loadaddr} part.new\0" \
//    "meraki_load_part_old=ubi read ${meraki_loadaddr} part.old\0" \
//    "meraki_load_part1=ubi read ${meraki_loadaddr} part1\0" \
//    "meraki_load_part2=ubi read ${meraki_loadaddr} part2\0" \
//    "meraki_load_net=${netloadmethod} ${meraki_loadaddr} ${serverpath}${meraki_bootfile}\0" \
//    "meraki_load_script=ubi read ${meraki_loadaddr} bootscr\0" \
//    "meraki_netboot=run meraki_load_net meraki_bootlinux\0" \
//    "meraki_netboot_static=run meraki_set_static_ip meraki_tftp_load_net meraki_bootlinux\0" \
//    "meraki_run_script=source ${meraki_loadaddr}\0" \
//    "meraki_set_static_ip=setenv ipaddr 10.128.128.128; " \
//    "setenv netmask 255.0.0.0\0" \
//    "meraki_tftp_load_net=tftpboot ${meraki_loadaddr} ${serverpath}${meraki_bootfile}\0" \
//    "factory_boot=run meraki_ubi factory_load meraki_bootlinux\0" \
//    "factory_load=ubi read ${meraki_loadaddr} mr26-wnc-mfg\0" \
//    "bootcmd=run meraki_boot\0"

#ifdef CONFIG_RUN_DDR_SHMOO2
/* Shmoo reuse: skip Shmoo process by reusing values saved in flash. */
#define CONFIG_SHMOO_REUSE
/* Define it for 32bit DDR */
#define CONFIG_SHMOO_REUSE_DDR_32BIT            0
/* Offset of spi flash to save Shmoo values */
//#ifdef CONFIG_NSPLUS_SV_STDK_BUILD
//#define CONFIG_SHMOO_REUSE_QSPI_OFFSET          0x00F00000
//#else
//#define CONFIG_SHMOO_REUSE_QSPI_OFFSET          0x000E0000
//#endif
#undef CONFIG_SHMOO_REUSE_QSPI_OFFSET
/* Offset of NAND flash to save Shmoo values */
#define CONFIG_SHMOO_REUSE_NAND_OFFSET          0x00080000
/* Range for the partition to support NAND bad blocks replacement */
#define CONFIG_SHMOO_REUSE_NAND_RANGE           0x00080000
/* Delay to wait for the magic character to force Shmoo; 0 to disable delay */
#define CONFIG_SHMOO_REUSE_DELAY_MSECS          50
/* Length of memory test after restored; 0 to disable memory test */
#define CONFIG_SHMOO_REUSE_MEMTEST_LENGTH       (0x080000)
/* Starting address of memory test after restored */
#define CONFIG_SHMOO_REUSE_MEMTEST_START        IPROC_DDR_MEM_BASE2
#endif /* CONFIG_RUN_DDR_SHMOO2 */

//#undef CONFIG_GZIP
#define CONFIG_GZIP_COMPRESSED
#define CONFIG_LZMA
#undef CONFIG_CMD_LICENSE
#define CONFIG_AUTO_COMPLETE
//#undef CONFIG_CMD_IMPORTENV
//#undef CONFIG_CMD_EXPORTENV
//#undef CONFIG_CMD_MEMORY
#undef CONFIG_SYS_VSNPRINTF
#undef CONFIG_CMD_UBI
#undef CONFIG_CMD_NET
#undef CONFIG_CMD_PING
#undef CONFIG_CMD_MTDPARTS
#undef CONFIG_MTD_DEVICE
//#define CONFIG_SYS_NO_BOOTM
#undef CONFIG_BOOTM_NETBSD
#undef CONFIG_HAS_POST
#undef CONFIG_POST_STD_LIST
#undef CONFIG_POST

#undef CONFIG_HAS_PWM
#undef CONFIG_IPROC_GSIOSPI


/* USB */
//#undef CONFIG_CMD_USB

/* PCIE */
#undef CONFIG_CMD_PCI
#undef CONFIG_CMD_PCI_ENUM
#undef CONFIG_PCI
#undef CONFIG_PCI_SCAN_SHOW
#undef CONFIG_IPROC_PCIE

/* SDIO */
#undef CONFIG_CMD_MMC
#undef CONFIG_GENERIC_MMC
#undef CONFIG_IPROC_MMC

/* USB3.0 */
//#undef CONFIG_USB_XHCI_IPROC

/* QSPI */
#undef CONFIG_CMD_SPI
#undef CONFIG_IPROC_QSPI

/* MDIO */
#undef CONFIG_IPROC_MDIO

#undef CONFIG_CMD_UBIFS
#undef CONFIG_RBTREE
#undef CONFIG_MTD_DEVICE               /* needed for mtdparts commands */
#undef CONFIG_MTD_PARTITIONS
#undef CONFIG_CMD_MTDPARTS
#define CONFIG_LZO
#undef CONFIG_CMD_ELF
#define CONFIG_FIT
#define CONFIG_OF_LIBFDT
#define CONFIG_CMD_BOOTZ
//#define CONFIG_CMD_BOOTELF

/* libnet */
//#undef CONFIG_BCMIPROC_ETH
//#undef CONFIG_NET_MULTI
//#undef CONFIG_CMDLINE_EDITING
//#undef CONFIG_SYS_LONGHELP
#undef CONFIG_CMD_NFS

/* lib fs */
#undef CONFIG_CMD_CRAMFS
#define CONFIG_CMD_EXT4
#undef CONFIG_CMD_EXT2
#define CONFIG_CMD_FAT
#undef CONFIG_CMD_FDOS
#undef CONFIG_CMD_JFFS2
#undef CONFIG_CMD_REISER
#undef CONFIG_YAFFS2
#undef CONFIG_CMD_UBIFS
#undef CONFIG_CMD_ZFS

/* libdisk */
//#undef CONFIG_PARTITIONS
#undef CONFIG_MAC_PARTITION
//#undef CONFIG_DOS_PARTITION
#undef CONFIG_ISO_PARTITION
#undef CONFIG_AMIGA_PARTITION
#undef CONFIG_EFI_PARTITION

/* libblock */
#undef CONFIG_SCSI_AHCI
#undef CONFIG_ATA_PIIX
#undef CONFIG_DWC_AHSATA
#undef CONFIG_FSL_SATA
#undef CONFIG_IDE_FTIDE020
#undef CONFIG_LIBATA
#undef CONFIG_MVSATA_IDE
#undef CONFIG_MX51_PATA
#undef CONFIG_PATA_BFIN
#undef CONFIG_SATA_DWC
#undef CONFIG_SATA_SIL3114
#undef CONFIG_SATA_SIL
#undef CONFIG_IDE_SIL680
#undef CONFIG_SCSI_SYM53C8XX
#undef CONFIG_SYSTEMACE
#undef CONFIG_IPROC_AHCI

/* libfpga */
#undef CONFIG_FPGA

/* libgpio */
//#define CONFIG_IPROC_GPIO

/* libi2c */
#undef CONFIG_CMD_I2C
#undef CONFIG_IPROC_I2C

/* I2C */
//#define CONFIG_SYS_I2C_SPEED    0    	/* Default on 100KHz */
//#define CONFIG_SYS_I2C_SLAVE	0xff	/* No slave address */

/* libsound */
#undef CONFIG_IPROC_I2S
#undef CONFIG_IPROC_PCM
#undef CONFIG_NS_DMA

#undef CONFIG_CMD_TFTPSRV
#undef CONFIG_CMD_WNC

#undef CONFIG_AUTOBOOT_KEYED
#undef CONFIG_AUTOBOOT_STOP_STR

#undef CONFIG_ETHADDR
#undef CONFIG_CMD_ITEST

#undef CONFIG_SYS_HUSH_PARSER
//#define CONFIG_ZLIB
//#undef CONFIG_CMD_BOOTM
#undef CONFIG_EXTRA_ENV_SETTINGS
#undef MTDPARTS_DEFAULT
#undef CONFIG_USB_TTY

#endif /* __KINGPIN_H */
