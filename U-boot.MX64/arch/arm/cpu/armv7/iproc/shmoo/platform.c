/*
 * Copyright (C) 1999-2013, Broadcom Corporation 
 *  
 *      Unless you and Broadcom execute a separate written software license 
 * agreement governing use of this software, this software is licensed to you 
 * under the terms of the GNU General Public License version 2 (the "GPL"), 
 * available at http://www.broadcom.com/licenses/GPLv2.php, with the 
 * following added to such license: 
 *  
 *      As a special exception, the copyright holders of this software give you 
 * permission to link this software with independent modules, and to copy and 
 * distribute the resulting executable under terms of your choice, provided that 
 * you also meet, for each linked independent module, the terms and conditions of 
 * the license of that module.  An independent module is a module which is not 
 * derived from this software.  The special exception does not apply to any 
 * modifications of the software. 
 *  
 *      Notwithstanding the above, under no circumstances may you combine this 
 * software in any way with any other Broadcom software provided under a license 
 * other than the GPL, without Broadcom's express prior written consent. 
 */
/*
** Copyright 2000, 2001  Broadcom Corporation
** All Rights Reserved
**
** No portions of this material may be reproduced in any form 
** without the written permission of:
**
**   Broadcom Corporation
**   5300 California Avenue
**   Irvine, California 92617
**
** All information contained in this document is Broadcom 
** Corporation company private proprietary, and trade secret.
**
** ----------------------------------------------------------
**
** 
**
** $Id::                                                          $:
** $Rev::file =  : Global SVN Revision = 1780                     $:
**
*/

// platform.c
//
// Use those header files for the northstar build
#include <common.h>
#include <asm/iproc/platform.h>
#include <stdarg.h>

#define CACHE_LINE_SIZE   (32)

void shmoo_flush_dcache_range( unsigned long start , unsigned long stop )
{
    unsigned long i;

    for( i = start ; i < stop ; i += CACHE_LINE_SIZE ) {
        asm volatile( "MCR  p15, 0, %0, c7, c10, 1"::"r"(i) );
    }

    asm volatile( "MCR  p15, 0, r0, c7, c10, 4"::: ); /*DSB*/

    return;
}

void shmoo_invalidate_dcache_range( unsigned long start , unsigned long stop )
{
    unsigned long i;

    for ( i = start ; i < stop ; i += CACHE_LINE_SIZE ) {
        asm volatile( "MCR  p15, 0, %0, c7, c6, 1"::"r"(i) );
    }

    asm volatile( "MCR  p15, 0, r0, c7, c10, 4"::: ); /*DSB*/

    return;
} 

void SHMOO_FLUSH_DATA_TO_DRAM( ddr40_addr_t Address , unsigned int bytes ) 
{
    unsigned long const EndAddress = Address + bytes;

    // printf("Flushing %08X+%d bytes\n", Address, bytes);
    shmoo_flush_dcache_range( Address , EndAddress );

    return;
}

void SHMOO_INVALIDATE_DATA_FROM_DRAM( ddr40_addr_t Address , unsigned int bytes )
{
    unsigned long const EndAddress = Address + bytes;
  
    // printf("Invalidating %08X+%d bytes\n", Address, bytes);
	shmoo_invalidate_dcache_range( Address , EndAddress );

    return;
}

void UART_OUT( unsigned int val )
{
    printf( "%c" , val );

    return;
}

void plot_dec_number( unsigned int val )
{

    printf( "%08d" , val );

    return;
}

void plot_hex_number( unsigned int val )
{

    printf( "%08X" , val );

    return;
}

unsigned int tb_r( ddr40_addr_t Address )
{
    unsigned int const Data = *(unsigned int *)Address;

    return Data;
}

void tb_w( ddr40_addr_t Address , unsigned int Data )
{
    unsigned int tmp;
    *(volatile unsigned int *)Address = Data;
	/* Do a dummy read, required on Northstar */
	tmp = *(volatile unsigned int *)Address;

    return;
}

unsigned int SHMOO_DRAM_READ_32( ddr40_addr_t Address )
{

    return( *(unsigned int *)Address );
}

void SHMOO_DRAM_WRITE_32( ddr40_addr_t Address , unsigned int Data )
{

    *(unsigned int *)Address = Data;

    return;
}

int ReWriteModeRegisters( void );

int rewrite_dram_mode_regs( void )
{

    return ReWriteModeRegisters();
} 


//
// Functions for the PHY init
//
void PrintfLog( char * const ptFormatStr , ... )
{
    char __tmp_buf[ 128 ] = { 'L' , 'o' , 'g' , ':' , ' ' };
    va_list arg;

    va_start( arg , ptFormatStr );
    vsprintf( &__tmp_buf[ 5 ] , ptFormatStr , arg ); 
    va_end( arg );

    printf( __tmp_buf );

    return;
}

void PrintfErr( char * const ptFormatStr , ... )
{
    char __tmp_buf[ 128 ] = { 'E' , 'r' , 'r' , 'o' , 'r' , ':' , ' ' };
    va_list arg;

    va_start( arg , ptFormatStr );
    vsprintf( &__tmp_buf[ 7 ] , ptFormatStr , arg ); 
    va_end( arg );

    printf( __tmp_buf );

    return;
}

void PrintfFatal( char * const ptFormatStr , ... )
{
    char __tmp_buf[ 128 ] = { 'F' , 'a' , 't' , 'a' , 'l' , ':' , ' ' };
    va_list arg;

    va_start( arg , ptFormatStr );
    vsprintf( &__tmp_buf[ 7 ] , ptFormatStr , arg ); 
    va_end( arg );

    printf( __tmp_buf );

    return;
}
