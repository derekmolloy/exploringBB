/****************************************************************************/
/*  AM3517.cmd                                                              */
/*  Copyright (c) 2012  Texas Instruments Incorporated                      */
/*  Author: Cody Addison                                                    */
/*                                                                          */
/*    Description: This file is a sample linker command file that can be    */
/*                 used for linking programs built with the C compiler and  */
/*                 running the resulting .out file on an AM3517.            */
/*                 Use it as a guideline.  You will want to                 */
/*                 change the memory layout to match your specific          */
/*                 target system.  You may want to change the allocation    */
/*                 scheme according to the size of your program.            */
/*                                                                          */
/****************************************************************************/

MEMORY
{
    SRAM:       o = 0x40200000  l = 0x00010000  /* 64kB Internal SRAM */
    SDRAM:      o = 0x80000000  l = 0x20000000  /* 512MB of external DDR */

    PRU0DMEM   o = 0x4A300000  l = 0x00002000  /* PRU0 Data */
    PRU0IMEM   o = 0x4A334000  l = 0x00002000  /* PRU0 Instruction */
}

SECTIONS
{
   /*
       Place the PRU0 data. Make sure the data in external RAM is where
       it was placed on the PRU0. 
   */
    
    PRU0_IMEM      >  PRU0IMEM
    PRU0_DMEM      >  PRU0DMEM
    PRU0_EXT       > 0x80000000

    .text          >  SRAM
    .stack         >  SRAM
    .bss           >  SRAM
    .cio           >  SRAM
    .const         >  SRAM
    .data          >  SRAM
    .switch        >  SRAM
    .sysmem        >  SRAM
    .far           >  SRAM
    .args          >  SRAM
    .ppinfo        >  SRAM
    .ppdata        >  SRAM

    /* TI-ABI sections */
    .pinit         >  SRAM
    .cinit         >  SRAM
  
    /* EABI sections */
    .binit         >  SRAM
    .init_array    >  SRAM
    .neardata      >  SRAM
    .fardata       >  SRAM
    .rodata        >  SRAM
    .c6xabi.exidx  >  SRAM
    .c6xabi.extab  >  SRAM
}
