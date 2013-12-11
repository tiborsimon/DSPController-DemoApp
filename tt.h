///////////////////////////////////////////////////////////////////////////////////////
//NAME:     tt.h
//DATE:     7/15/04
//PURPOSE:  Header file with definitions use in the C-based talkthrough examples
//
////////////////////////////////////////////////////////////////////////////////////////
#ifndef _TT_H_
#define _TT_H_

#include <cdef21364.h>
#include <def21364.h>
#include <signal.h>

#define NUM_SAMPLES 2
#define SCALE   (16777216)

#define L	1
#define R	0

#define DAC1
#define DAC2
#define DAC3
#define DAC4  //eredetileg csak ez.




// Function prototypes for this talkthrough code

extern void InitPLL(void);
extern void processBlock(float *input, float *output);

extern void InitDAI(void);
extern void Init1835viaSPI(void);

extern void InitSPORT(void);
extern void receive(int);
extern void ClearSPORT(void);

extern void SetupSPI1835 () ;
extern void DisableSPI1835 () ;
extern void Configure1835Register (int i) ;
extern unsigned int Get1835Register (int i) ;

extern void SetupIRQ12 () ;
extern void Irq1ISR (int i) ;
extern void Irq2ISR (int i) ;

extern void Delay (int i) ;

extern int isProcessing;
extern int blockReady;
extern unsigned int *src_pointer[3];
extern int int_cntr;

extern float input[2];
extern float output[2];


extern unsigned int Block_A[NUM_SAMPLES] ;
extern unsigned int Block_B[NUM_SAMPLES] ;
extern unsigned int Block_C[NUM_SAMPLES] ;


extern float inRight  , inLeft;
extern float out1Right, out1Left;
extern float out2Right, out2Left;
extern float out3Right, out3Left;
extern float out4Right, out4Left;

extern int rightIn_i  , leftIn_i;
extern int rightOut1_i, leftOut1_i;
extern int rightOut2_i, leftOut2_i;
extern int rightOut3_i, leftOut3_i;
extern int rightOut4_i, leftOut4_i;

#define INIT_SHARC  {								\
    					InitPLL ();					\
						SetupIRQ12 ();				\
    					InitDAI();					\
						Init1835viaSPI();			\
    					InitSPORT();				\
						interrupt(SIG_SP0,receive);	\
					}

// INTERFACE functions
void initInterface(void);
void updateInterface(void);

void process(void);


#endif
