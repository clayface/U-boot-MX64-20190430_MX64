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
/*  *********************************************************************
    *********************************************************************
    *  Broadcom Common Diagnostics Environment (CDE)
    *********************************************************************
    *  Filename: mde_local_cmd.c
    *
    *  Function:
    *
    *  Author:  Chung Kwong (C.K.) Der (ckder@broadcom.com)
    *
    * 
    * $Id::                                                       $:
    * $Rev:: : Global SVN Revision = 1950                         $:
    * 
 */

#include <mde_common_headers.h>
#include <mde_local_headers.h>

extern unsigned  mde_gtlb ( sMde_t *mde );
extern unsigned auto_time_read_eye ( sMde_t *mde );
extern unsigned auto_time_write_eye ( void );

unsigned  mde_ns_amt ( sMde_t *mde )
{
  // default auto-run conditions

#define AMT_7425_MODE       0
#define AMT_7425_PASS       0
#define AMT_7425_RUN_TIME   0
#define AMT_7425_SEED       0

  unsigned mode     = AMT_7425_MODE;
  unsigned pass     = AMT_7425_PASS;
  unsigned run_time = AMT_7425_RUN_TIME;
  unsigned seed     = AMT_7425_SEED;


  mtShowConfig ( mde );

  printf ("\nAUTO-RUN memory test: hit 'q' to stop\n\n");

  sprintf ( mde->user_line , "mtrt 0 0 0 0" );

  mtRunTest ( mde );

  return DIAG_RET_OK;
}


MDE_CMD mdeLocalCmds [] = {
  //// auto-memory-test
  {"local-mt-auto-run",        "amt",  "Run memory stress test with default arguments", 
   "   Invokes the memory test with default arguments\n\n"
   "\tMemory region from 0x02000000 for 96MB\n"
   , mde_ns_amt },

  {"rdi-auto-time-ri",         "atri",   "Run auto-read-eye with default arguments",
   "   MIPS-read-eye auto time elapsed read-eye\n\n"
   ,   auto_time_read_eye  },
//DDR-EYE-SHMOO  
   {"wri-auto-time-wi",         "atwi",   "Run auto-write-eye with default arguments",
   "   Write eye\n\n"
   ,   auto_time_write_eye  },
};

#define LOCAL_CMD_COUNT (sizeof(mdeLocalCmds)/sizeof(MDE_CMD))

MDE_CMD *host_setup_local_cmds ( unsigned *uCommandCount )
{
  printf ("host_setup_local_cmds for NS valid cmd count %d\n", LOCAL_CMD_COUNT );

  *uCommandCount = LOCAL_CMD_COUNT;

  return mdeLocalCmds;
}

/*
  For NS auto read-eye 
  120912: Current version requires the code be running from FLASH
  The memory region should have L1 (WB) and L2 disallowed

  mrdi with plot once
  time loop
    if interval is up, allow print
	if time is up, exit

  actual command sequence:
                                        P C V
  mrdi 1 0 0 0 08000000 00100000 16 0 0 1 1 1 0 0
  mrdi 1 0 0 0 08000000 00100000 16 0 0 0 0 1 0 0

 */
unsigned auto_time_read_eye ( sMde_t *mde )
{
   char name [DIAG_SIZE_32];
   char cmd1  [DIAG_SIZE_128];
   char cmd2p [DIAG_SIZE_128];
   char cmd2n [DIAG_SIZE_128];

   int inx, plot_flag = 1;
   unsigned interval = ATRI_DEFAULT_INTERVAL;
   unsigned loop     = ATRI_DEFAULT_LOOP;

   unsigned time_start   = 0; 
   unsigned time_elapsed = 0;

   unsigned print_allowed;
   unsigned plotted;

   // support MODE 1 - dual RDI only
   unsigned mode = 1;
   unsigned sta_vref = VREF_MID_POINT;
   OverrideInfo_t sOverride;
   // No support for OVERRIDE   
   sOverride.uDqOverride    = 0xFFFFFFFF;
   sOverride.uDqOverrideVal = 0;
   
  
#if AUTO_READ_EYE_LOOP_COUNT
   loop     = AUTO_READ_EYE_LOOP_COUNT;
#endif

#if AUTO_READ_EYE_INTERVAL
   interval = AUTO_READ_EYE_INTERVAL;
#endif

#ifdef STA_VREF
#warning: STA_VREF_DEFINED
   sta_vref = MV_TO_VREF(STA_VREF);
	host_printf ("STA_VREF %dmV index %d\n", STA_VREF, sta_vref);
#endif

#if CFG_DISABLE_RD_EYE_PLOT
   plot_flag = 0;
   host_printf ("Readeye plot disabled\n");
#endif

//DDR-EYE-SHMOO: change from 0x08000000 to 0x01000000, so that no MMU table change is needed
#define NS_RDI_MT_TOP    0x01000000
#define NS_RDI_MT_BSIZE  0x00100000

   host_printf ("auto-read-eye mode %d interval %d seconds, loop %d\n",
               mode, interval, loop);
   // set up the first command - clear buffer, print allowed - NO DRY RUN needed
   // mrdi 1 0 0 0 08000000 00100000 16 0 0 1 1 1 0 0
   sprintf (cmd1,
			"mrdi 1 0 0 0 %x %x 16 0 0 1 1 1 0 0",
			NS_RDI_MT_TOP, NS_RDI_MT_BSIZE);


   // set up the command with print enabled
   // mrdi 1 0 0 0 08000000 00100000 16 0 0 0 0 1 0 0
   sprintf (cmd2p,
			//                         P C V
			"mrdi 1 0 0 0 %x %x 16 0 0 1 0 1 0 0", 
			NS_RDI_MT_TOP, NS_RDI_MT_BSIZE);

   // set up the command with print disabled - reset the clear_result flag

   sprintf (cmd2n,
			//                         P C V
			"mrdi 1 0 0 0 %x %x 16 0 0 0 0 1 0 0", 
			NS_RDI_MT_TOP, NS_RDI_MT_BSIZE);

   // do the first RDI, plot allowed, do_vref set
   mips_rd_eye_plot (cmd1);
   /// we have done plotting once  
   print_allowed = 0;
   for (inx = 1,   time_start = 0; inx < loop;  )
   {
      if (print_allowed)
      {
         host_printf ("\nPlot enabled for the next run\n");    
         print_allowed = 0;
         mips_rd_eye_plot (cmd2p);
         inx ++;
         time_start = current_time_in_seconds ();
      }
      else
      {
         host_printf ("\nWaiting to plot #%d (of %d). Interval %d - time elapsed %d", inx+1, loop, interval, time_elapsed);
         host_printf ("\n\tRunning read-eye on MEMC0 ");
         mips_rd_eye_plot (cmd2n);
      }
      time_elapsed = current_time_in_seconds () - time_start;
      if (time_elapsed >= interval)
      {
         print_allowed = 1;
		 time_start = current_time_in_seconds ();
      }
   }
   host_printf ("WARNING: SDRAM may not be stable after read-eye\n");
}


//DDR-EYE-SHMOO: added for write-eye shmoo
extern unsigned auto_wr_eye(char*);
unsigned auto_time_write_eye()
{
   return auto_wr_eye("");
}
