/*
 * RoboSwitch setup functions
 *
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
 *
 * $Id: bcmrobo.h 309568 2012-01-20 01:40:56Z $
 */

#ifndef _bcm_robo_h_
#define _bcm_robo_h_

#include <asm/arch/bcmutils.h>

#define	DEVID5325	0x25	/* 5325 (Not really but we fake it) */
#define	DEVID5395	0x95	/* 5395 */
#define	DEVID5397	0x97	/* 5397 */
#define	DEVID5398	0x98	/* 5398 */
#define	DEVID53115	0x3115	/* 53115 */
#define	DEVID53125	0x3125	/* 53125 */

/*
 * MODELID:
 * 0x53010: BCM53010, Select Low SKU device if SKU ID[1:0] = 01.
 * 0x53011: BCM53011, Select Middle SKU device if SKU ID[1:0] = 10.
 * 0x53012: BCM53012, Select High SKU device if SKU ID[1:0] = 00.
 * Note: The SKU ID[1:0] is loaded from OTP configuration data.
 */
#define DEVID53010	0x53010	/* 53010 */
#define DEVID53011	0x53011	/* 53011 */
#define DEVID53012	0x53012	/* 53012 */
#define ROBO_IS_BCM5301X(id) ((id) == DEVID53010 || (id) == DEVID53011 || (id) == DEVID53012)

/* Power save duty cycle times */
#define MAX_NO_PHYS		5
#define PWRSAVE_SLEEP_TIME	12
#define PWRSAVE_WAKE_TIME	3

/* Power save modes for the switch */
#define ROBO_PWRSAVE_NORMAL 		0
#define ROBO_PWRSAVE_AUTO		1
#define ROBO_PWRSAVE_MANUAL		2
#define ROBO_PWRSAVE_AUTO_MANUAL 	3

/* Forward declaration */
typedef struct robo_info_s robo_info_t;

/* Device access/config oprands */
typedef struct {
	/* low level routines */
	void (*enable_mgmtif)(robo_info_t *robo);	/* enable mgmt i/f, optional */
	void (*disable_mgmtif)(robo_info_t *robo);	/* disable mgmt i/f, optional */
	int (*write_reg)(robo_info_t *robo, uint8 page, uint8 reg, void *val, int len);
	int (*read_reg)(robo_info_t *robo, uint8 page, uint8 reg, void *val, int len);
	/* description */
	char *desc;
} dev_ops_t;


typedef	uint16 (*miird_f)(void *h, int add, int off);
typedef	void (*miiwr_f)(void *h, int add, int off, uint16 val);

/* Private state per RoboSwitch */
struct robo_info_s {
	si_t	*sih;			/* SiliconBackplane handle */
	char	*vars;			/* nvram variables handle */
	void	*h;			/* dev handle */
	uint16	devid;			/* Device id for the switch */
	uint32	devid32;		/* Device id for the switch (32bits) */
	uint32	corerev;		/* Core rev of internal switch */

	dev_ops_t *ops;			/* device ops */
	uint8	page;			/* current page */

	/* SPI */
	uint32	ss, sck, mosi, miso;	/* GPIO mapping */

	/* MII */
	miird_f	miird;
	miiwr_f	miiwr;

	uint16	prev_status;		/* link status of switch ports */
	uint32	pwrsave_mode_manual; 	/* bitmap of ports in manual power save */
	uint32	pwrsave_mode_auto; 	/* bitmap of ports in auto power save mode */
	uint8	pwrsave_phys; 		/* Phys that can be put into power save mode */
	uint8	pwrsave_mode_phys[MAX_NO_PHYS];         /* Power save mode on the switch */
};

extern robo_info_t *bcm_robo_attach(si_t *sih, void *h, char *vars, miird_f miird, miiwr_f miiwr);
extern void bcm_robo_detach(robo_info_t *robo);
extern int bcm_robo_enable_device(robo_info_t *robo);
extern int bcm_robo_config_vlan(robo_info_t *robo, uint8 *mac_addr);
extern int bcm_robo_enable_switch(robo_info_t *robo);

#ifdef BCMDBG
extern void robo_dump_regs(robo_info_t *robo);
#endif /* BCMDBG */

#endif /* _bcm_robo_h_ */
