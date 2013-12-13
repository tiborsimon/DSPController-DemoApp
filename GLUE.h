#ifndef _GLUE_H_
#define _GLUE_H_

#include "tt.h"
#include "DSPController.h"

//========================================================================//
//        G L U E   V A R I A B L E   D E C L A R A T I O N S             //
//========================================================================//
//                                                                        //
//  The GLUE variables connect the interface code to the DSP code. They   //
//  can be used to control the DSP algorithm and to transfer parameters.  //
//                                                                        //
//  You should declare GLUE variables in this file as an extern variable  //
//  and define/initialize them in the INTERFACE.c file, than you can use  //
//  them in arbitrari files.                                              //
//                                                                        //
//========================================================================//
//                                                                        //
//  Typical workflow:                                                     //
//                                                                        //
//      1. Declare the variable in this file (GLUE.h)                     //
//      2. Define the variable in the INTERFACE.c file                    //
//      3. Update the variable in the INTERFACE.c file                    //
//      4. Use the variable in the DSP.c file                             //
//                                                                        //
//========================================================================//
//                                                                        //
//  Example:                                                              //
//                                                                        //
//     extern float GLUE_volume;                                          //
//      |            |                                                    //
//      |            *--------- use the GLUE_.. notation                  //
//      |                                                                 //
//      *---------------------- declare the variable as an extern         //
//                              variable                                  //
//                                                                        //
//========================================================================//

#define EFFECT_STATE_MUTED			1
#define EFFECT_STATE_OSCILLATOR		2
#define EFFECT_STATE_LOW_PASS		3

#define OSCILLATOR_TYPE_NONE     	0
#define OSCILLATOR_TYPE_SIN     	1
#define OSCILLATOR_TYPE_SQR     	2

// Default values
#define DEFAULT_GLUE_effect_state   EFFECT_STATE_MUTED
#define DEFAULT_GLUE_output_mux     0xf

#define DEFAULT_GLUE_1_freq         440.0f
#define DEFAULT_GLUE_1_amp          0.004f
#define DEFAULT_GLUE_1_type         OSCILLATOR_TYPE_SIN

#define DEFAULT_GLUE_2_fc           1000.0f
#define DEFAULT_GLUE_2_q            1.0f
#define DEFAULT_GLUE_2_mod_type     OSCILLATOR_TYPE_SIN
#define DEFAULT_GLUE_2_mod_span     800.0f
#define DEFAULT_GLUE_2_mod_freq     0.1f


// Global effect parameters
extern float    GLUE_effect_state;
extern int      GLUE_output_mux;

// Oscillator parameters
extern float    GLUE_1_freq;
extern float    GLUE_1_amp;
extern int      GLUE_1_type;

// LowPass paramters
extern float    GLUE_2_fc;
extern float    GLUE_2_q;
extern int	    GLUE_2_mod_type;
extern float    GLUE_2_mod_span;
extern float    GLUE_2_mod_freq;




#endif
