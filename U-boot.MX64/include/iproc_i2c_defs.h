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

#ifndef __IPROC_SMBUS_DEFS_H__
#define __IPROC_SMBUS_DEFS_H__

/* Transaction error codes defined in Master command register (0x30) */
#define MSTR_STS_XACT_SUCCESS          0
#define MSTR_STS_LOST_ARB              1
#define MSTR_STS_NACK_FIRST_BYTE       2
#define MSTR_STS_NACK_NON_FIRST_BYTE   3 /* NACK on a byte other than 
                                            the first byte */
#define MSTR_STS_TTIMEOUT_EXCEEDED     4
#define MSTR_STS_TX_TLOW_MEXT_EXCEEDED 5
#define MSTR_STS_RX_TLOW_MEXT_EXCEEDED 6

/* SMBUS protocol values defined in register 0x30 */
#define SMBUS_PROT_QUICK_CMD               0 
#define SMBUS_PROT_SEND_BYTE               1 
#define SMBUS_PROT_RECV_BYTE               2 
#define SMBUS_PROT_WR_BYTE                 3 
#define SMBUS_PROT_RD_BYTE                 4 
#define SMBUS_PROT_WR_WORD                 5 
#define SMBUS_PROT_RD_WORD                 6 
#define SMBUS_PROT_BLK_WR                  7 
#define SMBUS_PROT_BLK_RD                  8 
#define SMBUS_PROT_PROC_CALL               9 
#define SMBUS_PROT_BLK_WR_BLK_RD_PROC_CALL 10 

#define BUS_BUSY_COUNT                 100000 /* Number can be changed later */

#endif /* __IPROC_SMBUS_DEFS_H__ */
