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
 *
 * These routines provide access to the serdes
 *
 */

#ifndef _bcm_iproc_robo_serdes_h_
#define _bcm_iproc_robo_serdes_h_


/* ---- Include Files ---------------------------------------------------- */

#define	PHY_REG_BLK_ADDR	0x001f		/* GLOBAL BLOCK ADDRESS REGISTER */

/*
 * MII Link Advertisment (Clause 37) 
 */
#define MII_ANA_C37_NP          (1 << 15)  /* Next Page */
#define MII_ANA_C37_RF_OK       (0 << 12)  /* No error, link OK */
#define MII_ANA_C37_RF_LINK_FAIL (1 << 12)  /* Offline */
#define MII_ANA_C37_RF_OFFLINE  (2 << 12)  /* Link failure */
#define MII_ANA_C37_RF_AN_ERR   (3 << 12)  /* Auto-Negotiation Error */
#define MII_ANA_C37_PAUSE       (1 << 7)   /* Symmetric Pause */
#define MII_ANA_C37_ASYM_PAUSE  (1 << 8)   /* Asymmetric Pause */
#define MII_ANA_C37_HD          (1 << 6)   /* Half duplex */
#define MII_ANA_C37_FD          (1 << 5)   /* Full duplex */ 

/* MII Control Register: bit definitions */

#define MII_CTRL_FS_2500        (1 << 5) /* Force speed to 2500 Mbps */
#define MII_CTRL_SS_MSB         (1 << 6) /* Speed select, MSb */
#define MII_CTRL_CST            (1 << 7) /* Collision Signal test */
#define MII_CTRL_FD             (1 << 8) /* Full Duplex */
#define MII_CTRL_RAN            (1 << 9) /* Restart Autonegotiation */
#define MII_CTRL_IP             (1 << 10) /* Isolate Phy */
#define MII_CTRL_PD             (1 << 11) /* Power Down */
#define MII_CTRL_AE             (1 << 12) /* Autonegotiation enable */
#define MII_CTRL_SS_LSB         (1 << 13) /* Speed select, LSb */
#define MII_CTRL_LE             (1 << 14) /* Loopback enable */
#define MII_CTRL_RESET          (1 << 15) /* PHY reset */

#define MII_CTRL_SS(_x)         ((_x) & (MII_CTRL_SS_LSB|MII_CTRL_SS_MSB))
#define MII_CTRL_SS_10          0
#define MII_CTRL_SS_100         (MII_CTRL_SS_LSB)
#define MII_CTRL_SS_1000        (MII_CTRL_SS_MSB)
#define MII_CTRL_SS_INVALID     (MII_CTRL_SS_LSB | MII_CTRL_SS_MSB)
#define MII_CTRL_SS_MASK        (MII_CTRL_SS_LSB | MII_CTRL_SS_MSB)

#define GPIO_SFP0_TXDIS		26
#define GPIO_SFP1_TXDIS		27

/* ---- External Function Prototypes ------------------------------------- */

extern uint16 robo_serdes_get_id(robo_info_t *robo, uint page, uint off);
extern void robo_serdes_reset(robo_info_t *robo, uint page);
extern int robo_serdes_reset_core(robo_info_t *robo, uint page);
extern int robo_serdes_start_pll(robo_info_t *robo, uint page);
extern int robo_serdes_init(robo_info_t *robo, uint page);
extern int robo_serdes_enable_set(robo_info_t *robo, uint page, int enable);
extern int robo_serdes_speed_set(robo_info_t *robo, uint page, int speed);
extern int robo_serdes_speed_get(robo_info_t *robo, uint page, int *speed);

#endif	/* _bcm_iproc_serdes_h_ */
