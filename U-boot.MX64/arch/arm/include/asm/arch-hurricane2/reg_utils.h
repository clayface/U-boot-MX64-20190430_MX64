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

#ifndef REG_UTILS
#define REG_UTILS

/* ---- Include Files ---------------------------------------------------- */

#include <linux/types.h>

/* ---- Public Constants and Types --------------------------------------- */

#define __REG32(x)      (*((volatile uint32_t *)(x)))
#define __REG16(x)      (*((volatile uint16_t *)(x)))
#define __REG8(x)       (*((volatile uint8_t *) (x)))

/* ---- Public Variable Externs ------------------------------------------ */
/* ---- Public Function Prototypes --------------------------------------- */

/****************************************************************************/
/*
 *   32-bit register access functions
 */
/****************************************************************************/

extern uint32_t reg_debug;
#define	REG_DEBUG(val) (reg_debug = val)

//#define DEBUG_REG

static inline void 
reg32_clear_bits(volatile uint32_t *reg, uint32_t value)
{
#ifdef DEBUG_REG
	if (reg_debug)
		printf("%s reg (0x%x): 0x%x 0x%x\n", __FUNCTION__, (uint32_t)reg, *reg, (*reg & ~(value)));
#endif
    *reg &= ~(value);
}

static inline void 
reg32_set_bits(volatile uint32_t *reg, uint32_t value)
{
#ifdef DEBUG_REG
	if (reg_debug)
		printf("%s reg (0x%x): 0x%x 0x%x\n", __FUNCTION__, (uint32_t)reg, *reg, (*reg | value));
#endif
    *reg |= value;
}

static inline void 
reg32_toggle_bits(volatile uint32_t *reg, uint32_t value)
{
#ifdef DEBUG_REG
	if (reg_debug)
		printf("%s reg (0x%x): 0x%x 0x%x\n", __FUNCTION__, (uint32_t)reg, *reg, (*reg ^ value));
#endif
    *reg ^= value;
}

static inline void 
reg32_write_masked(volatile uint32_t *reg, uint32_t mask, 
                                      uint32_t value)
{
#ifdef DEBUG_REG
	if (reg_debug)
		printf("%s reg (0x%x): 0x%x 0x%x\n", __FUNCTION__, (uint32_t)reg, *reg, (*reg & (~mask)) | (value & mask));
#endif
    *reg = (*reg & (~mask)) | (value & mask);
}

static inline void 
reg32_write(volatile uint32_t *reg, uint32_t value)
{
#ifdef DEBUG_REG
	if (reg_debug)
		printf("%s reg (0x%x, 0x%x)\n", __FUNCTION__, (uint32_t)reg, value);
#endif
    *reg = value;
}

static inline uint32_t 
reg32_read(volatile uint32_t *reg)
{
#ifdef DEBUG_REG
	if (reg_debug)
		printf("%s reg (0x%x): 0x%x\n", __FUNCTION__, (uint32_t)reg, *reg);
#endif
    return *reg;
}

/****************************************************************************/
/*
 *   16-bit register access functions
 */
/****************************************************************************/

static inline void 
reg16_clear_bits(volatile uint16_t *reg, uint16_t value)
{
    *reg &= ~(value);
}

static inline void 
reg16_set_bits(volatile uint16_t *reg, uint16_t value)
{
    *reg |= value;
}

static inline void 
reg16_toggle_bits(volatile uint16_t *reg, uint16_t value)
{
    *reg ^= value;
}

static inline void 
reg16_write_masked(volatile uint16_t *reg, uint16_t mask, uint16_t value)
{
    *reg = (*reg & (~mask)) | (value & mask);
}

static inline void 
reg16_write(volatile uint16_t *reg, uint16_t value)
{
    *reg = value;
}

static inline uint16_t 
reg16_read(volatile uint16_t *reg)
{
    return *reg;
}

/****************************************************************************/
/*
 *   8-bit register access functions
 */
/****************************************************************************/

static inline void 
reg8_clear_bits(volatile uint8_t *reg, uint8_t value)
{
    *reg &= ~(value);
}

static inline void 
reg8_set_bits(volatile uint8_t *reg, uint8_t value)
{
    *reg |= value;
}

static inline void 
reg8_toggle_bits(volatile uint8_t *reg, uint8_t value)
{
    *reg ^= value;
}

static inline void 
reg8_write_masked(volatile uint8_t *reg, uint8_t mask, uint8_t value)
{
    *reg = (*reg & (~mask)) | (value & mask);
}

static inline void 
reg8_write(volatile uint8_t *reg, uint8_t value)
{
    *reg = value;
}

static inline uint8_t 
reg8_read(volatile uint8_t *reg)
{
    return *reg;
}
#endif /* REG_UTILS */
