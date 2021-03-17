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
#include <common.h>
#include <config.h>
#include <asm/arch/iproc.h>
#include <asm/system.h>

void scu_disable(void) {
	scu_reg_struct_ptr scu_ptr = (scu_reg_struct_ptr)IPROC_PERIPH_SCU_REG_BASE;

	scu_ptr->control &= ~IPROC_SCU_CTRL_SCU_EN;
}

void scu_enable(void) {
	scu_reg_struct_ptr scu_ptr = (scu_reg_struct_ptr)IPROC_PERIPH_SCU_REG_BASE;

	scu_ptr->invalidate = 0xFF;
	
	/* Turn on the SCU */
	scu_ptr->control |= IPROC_SCU_CTRL_SCU_EN;

}

void l2cc_init(void) 
{
	uint32_t regval;
	l2cc_reg_struct_ptr l2cc_ptr = (l2cc_reg_struct_ptr)IPROC_L2CC_REG_BASE;
	regval = l2cc_ptr->aux_control;
	regval &= ~(0x000F0000); // Clear the Way-size and associativity (8 way)
	regval |= 0x0A130000; /* Non-secure interrupt access, Way-size 16KB, 16 way
						    and event monitoring */
	l2cc_ptr->aux_control = regval;
	l2cc_ptr->tag_ram_control = 0; /* Tag ram latency */
	l2cc_ptr->data_ram_control = 0; /* Data ram latency */

}

void l2cc_invalidate(void) 
{
	l2cc_reg_struct_ptr l2cc_ptr = (l2cc_reg_struct_ptr)IPROC_L2CC_REG_BASE;
	
	/* Invalidate the entire L2 cache */
	l2cc_ptr->inv_way = 0x0000FFFF;
}

void l2cc_unlock(void) 
{
	l2cc_reg_struct_ptr l2cc_ptr = (l2cc_reg_struct_ptr)IPROC_L2CC_REG_BASE;
	
	/* unlock the entire L2 cache */
	l2cc_ptr->unlock_way = 0x0000FFFF;
}


int l2cc_enable(void) 
{
	int i;
	l2cc_reg_struct_ptr l2cc_ptr = (l2cc_reg_struct_ptr)IPROC_L2CC_REG_BASE;
	//printf("l2cc_init\n");
	l2cc_init();
	//printf("l2cc_invalidate\n");
	l2cc_invalidate();

	i = 1000;
	while(l2cc_ptr->inv_way && i)
	{
		--i;
	};
	//printf("l2cc_invalidate - Done, i = %d, invway = %x\n", i, l2cc_ptr->inv_way);
	if(i == 0) return (-1);
	/* Clear any pending interrupts from this controller */
	l2cc_ptr->int_clear = 0x1FF;

	/* Enable the L2 */
	l2cc_ptr->control = 0x01;

	/* mem barrier to sync up things */
	i = 0;
	/* mem barrier to sync up things */
	asm("mcr p15, 0, %0, c7, c10, 4": :"r"(i));
/* These instructions work with ARMV7 arch only 	
	//asm("isb sy"); 
	//asm("dsb sy");
*/

	return 0;
}

int l2cc_disable(void)
{
	int i;
	l2cc_reg_struct_ptr l2cc_ptr = (l2cc_reg_struct_ptr)IPROC_L2CC_REG_BASE;
	l2cc_invalidate();

	i = 1000;
	while(l2cc_ptr->inv_way && i)
	{
		--i;
	};
	printf("l2cc_invalidate - Done, i = %d, invway = %x\n", i, l2cc_ptr->inv_way);
	if(i == 0) {
		printf("l2cc_invalidate failed\n");
		return (-1);
	}
	l2cc_ptr->control = 0;

	return 0;
}

#if 0
void cpu_cache_initialization(void)
{
	v7_outer_cache_enable();
}
#endif
