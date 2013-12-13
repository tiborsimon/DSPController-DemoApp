/********************************************************************************************
**
**  File:   Init_PLL.c
**  Date:   5-25-05
**  Author: BM
**  Use:    Initialize the DSP PLL for the required CCLK rate
**  Note:
**      CLKIN will be 24.576MHz from an external oscillator.  The PLL is programmed
**      to generate a core clock (CCLK) of 331.7MHz - PLL multiplier = 0x001B and
**      divider = 0x02.
**
**      Updated code to not set Bypass and enable the divisor in the same cycle.
**
**
*********************************************************************************************/
#include    <tt.h>
int i;

void InitPLL(){

/********************************************************************************************/
//24.576 MHz CLKIN
//(24.576MHz * 27) /2 = 331.776 MHz

int i, pmctlsetting;

pmctlsetting = *pPMCTL;
pmctlsetting &= ~(0xff);    //Clear old divisor and multiplier setting.
                            //and start clkout
pmctlsetting |= PLLM27 | PLLD2 | DIVEN ;

*pPMCTL = pmctlsetting;

pmctlsetting |= PLLBP ;

*pPMCTL = pmctlsetting;

for (i=0;i<4096;i++)
          asm("nop;");

pmctlsetting = *pPMCTL;
pmctlsetting ^= PLLBP; //Clear Bypass Mode
*pPMCTL = pmctlsetting;

}
