///////////////////////////////////////////////////////////////////////////////////////
//NAME:     SPORTisr.c (Block-based Talkthrough)
//DATE:     7/15/04
//PURPOSE:  Talkthrough framework for sending and receiving samples to the AD1835.
//
//USAGE:    This file contains SPORT0 Interrupt Service Routine. Three buffers are used
//          for this example. One is filled by the ADC, another is sent to the DAC, and
//          the final buffer is processed. Each buffer rotates between these functions
//          upon each SP0 interrupt received.
///////////////////////////////////////////////////////////////////////////////////////
/*
   Here is the mapping between the SPORTS and the DACS
   ADC -> DSP  : SPORT0A : IIS
   DSP -> DAC1 : SPORT1A : IIS
   DSP -> DAC2 : SPORT1B : IIS
   DSP -> DAC3 : SPORT2A : IIS
   DSP -> DAC4 : SPORT2B : IIS
*/

#include "tt.h"
#include <stdio.h>
#include "DSPController.h"

#define RIGHT  0
#define LEFT   1




float inRight  , inLeft;
float out1Right, out1Left;
float out2Right, out2Left;
float out3Right, out3Left;
float out4Right, out4Left;

extern float inRight  , inLeft;
extern float out1Right, out1Left;
extern float out2Right, out2Left;
extern float out3Right, out3Left;
extern float out4Right, out4Left;



float rightInD  , leftInD;
float rightOut1D, leftOut1D;
float rightOut2D, leftOut2D;
float rightOut3D, leftOut3D;
float rightOut4D, leftOut4D;


float rightInNext   , inLeft;
float rightOut1D    , leftOut1DNext;
float rightOut2D    , leftOut2DNext;
float rightOut3D    , leftOut3DNext;
float rightOut4D    , leftOut4DNext;

//#define DELAY_LEFT_OUT 
//#define DELAY_LEFT_IN

int rightIn_i  , leftIn_i;
int rightOut1_i, leftOut1_i;
int rightOut2_i, leftOut2_i;
int rightOut3_i, leftOut3_i;
int rightOut4_i, leftOut4_i;


int mode1RegSPisr, mode1RegSPisr2;

int isProcessing=0;

//If the processing takes too long, the program will be stuck in this infinite loop.
void ProcessingTooLong()
{
    while(1);
}

bool channelCheck;

void receive(int sig_int)
{
    //    if(isProcessing){  ProcessingTooLong();}


    channelCheck = !!((*pDAI_PIN_STAT)&DAI_PB14);
    //adó reg.-be írás
    if ( channelCheck   ){  //így kompatibilis a 4.0-s VisualDSP-vel
        #ifdef DAC1
            *pTXSP1A = rightOut1_i;
            *pTXSP1A = leftOut1_i;
        #endif
        
        #ifdef DAC2
            *pTXSP1B = rightOut2_i;
            *pTXSP1B = leftOut2_i;
        #endif
        
        #ifdef DAC3
            *pTXSP2A = rightOut3_i;
            *pTXSP2A = leftOut3_i;
        #endif
            
        #ifdef DAC4
            *pTXSP2B = rightOut4_i;
            *pTXSP2B = leftOut4_i;
        #endif    
    } else {
        #ifdef DAC1
            *pTXSP1A = leftOut1_i;
            *pTXSP1A = rightOut1_i;
        #endif
        
        #ifdef DAC2
           *pTXSP1B = leftOut2_i;
            *pTXSP1B = rightOut2_i;
        #endif
            
        #ifdef DAC3
            *pTXSP2A = leftOut3_i;
            *pTXSP2A = rightOut3_i;
        #endif
            
        #ifdef DAC4
            *pTXSP2B = leftOut4_i;
            *pTXSP2B = rightOut4_i;
        #endif    
    }   


    if (1==isProcessing ){
            asm volatile(
                    "%0 = mode1;\n\t"
                    : "=d" (mode1RegSPisr): :);//PEYEN
                        }

    if (0==isProcessing ){
            asm volatile(
                    "%0 = mode1;\n\t"
                    : "=d" (mode1RegSPisr2): :);//PEYEN
                        }
        


            
    if ( channelCheck   ){   //így kompatibilis a 4.0-s VisualDSP-vel
        rightIn_i = (int) Block_A[RIGHT];  
        leftIn_i  = (int) Block_A[LEFT];  
    } else{
        leftIn_i  = (int) Block_A[RIGHT];
        rightIn_i = (int) Block_A[LEFT];   
    }    
    
        
    #ifdef  DELAY_LEFT_IN   
        inRight = rightInNext;  
        rightInNext = (float)rightIn_i / (float)SCALE;        
        inLeft  = (float)leftIn_i  / (float)SCALE;
    #else
        inRight = (float)rightIn_i / (float)SCALE;        
        inLeft  = (float)leftIn_i  / (float)SCALE;
    #endif

                    
        
             

                     
    #ifdef DELAY_LEFT_OUT
        rightOut1D = out1Right;                        
        leftOut1D  = leftOut1DNext;                        
        leftOut1DNext = out1Left;                        
        rightOut2D = out2Right;                        
        leftOut2D  = leftOut2DNext;                        
        leftOut2DNext = out2Left;
        rightOut3D = out3Right;                        
        leftOut3D  = leftOut3DNext;                        
        leftOut3DNext = out3Left;
        rightOut4D = out4Right;                        
        leftOut4D  = leftOut4DNext;                        
        leftOut4DNext = out4Left;
    #else
        rightOut1D = out1Right;                        
        leftOut1D  = out1Left;                        
        rightOut2D = out2Right;                        
        leftOut2D  = out2Left;                        
        rightOut3D = out3Right;                        
        leftOut3D  = out3Left;                        
        rightOut4D = out4Right;                        
        leftOut4D  = out4Left;                        
    #endif


    //típuskonverzió és skálázás
    rightOut1_i = (unsigned int)(rightOut1D * SCALE);
    leftOut1_i  = (unsigned int)( leftOut1D * SCALE);

    rightOut2_i = (unsigned int)(rightOut2D * SCALE);
    leftOut2_i  = (unsigned int)( leftOut2D * SCALE);

    rightOut3_i = (unsigned int)(rightOut3D * SCALE);
    leftOut3_i  = (unsigned int)( leftOut3D * SCALE);

    rightOut4_i = (unsigned int)(rightOut4D * SCALE);
    leftOut4_i  = (unsigned int)( leftOut4D * SCALE);

    #define SATURATE(in,sat) if ((in)>(sat)) in = sat; if ((in)<(-sat)) in = -sat;              

    SATURATE(rightOut1_i,0x007FFFFF);
    SATURATE( leftOut1_i,0x007FFFFF);

    DSPController_tick();
    // processBlock(0,0);
    process();

}
