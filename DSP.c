#include "GLUE.h"
#include <math.h>

//========================================================================//
//          D I G I T A L   S I G N A L   P R O C E S S I N G             //
//========================================================================//
//                                                                        //
//  In this file you can  implement your  DSP algorithm  by writing your  //
//  code iside the process() function. You will get  access to 10 global  //
//  variables  that represent  the  actual input and output samples. The  //
//  input variables contain the new left and right samples come from the  //
//  AD converter  before the  process() function is  called.  The output  //
//  variables  will  be  sent to  the  DA concerter  after the process()  //
//  function returned.                                                    //
//                                                                        //
//  After every ADC interrupt  the process() function  will be called as  //
//  one of  the highest  priority processing  function.  Your task is to  //
//  fill up the output  variables  with  new  data  calculated according  //
//  to  the  input  samples  and/or you algorithm,  otherwise the  input  //
//  samples will be mirrored to te outputs.                               //
//                                                                        //
//========================================================================//
//                                                                        //
//  Global sample variables you can use:                                  //
//                                                                        //
//         INPUTS              OUTPUTS                                    //
//             inLeft              out1Left    out3Left                   //
//             inRight             out1Right   out3Right                  //
//                                 out2Left    out4Left                   //
//                                 out2Right   out4Right                  //
//                                                                        //
//========================================================================//

#define M_PI 		3.14159265359f
#define Fs			48000.0f
// DSP base variables
float lfo_theta = 0.0f;
float lfo = 0.0f;

float c_r_old = 0.0f;
float c_i_old = 0.0f;

void process(void) {

    //====================================================================
    // PREPARE THE WORKING VARIABLES
    //====================================================================
    float x  = (inLeft+inRight)*0.5f;
    float y = 0;


    //====================================================================
    // DSP ALGORITHM
    //====================================================================
    if (GLUE_effect_state == EFFECT_STATE_MUTED) {
        
    } else if (GLUE_effect_state == EFFECT_STATE_OSCILLATOR) {
        
        lfo = sin(lfo_theta);
        lfo_theta += 2.0f*M_PI*GLUE_1_freq/Fs;
        
        if(lfo_theta > 2.0f*M_PI){
         	lfo_theta -= 2.0f*M_PI;
        }
        
        switch(GLUE_1_type) {
          	case OSCILLATOR_TYPE_SIN:
          		// nothing to do
             	break;
            case OSCILLATOR_TYPE_SQR:
            	lfo = lfo<0?1.0f:-1.0f;
             	break;
             default:
             	break;   
        }
        
        y = GLUE_1_amp*lfo;
        
    } else if (GLUE_effect_state == EFFECT_STATE_LOW_PASS) {
        
        lfo = sin(lfo_theta);
        lfo_theta += 2.0f*M_PI*GLUE_2_mod_freq/Fs;
        
        if(lfo_theta > 2.0f*M_PI){
         	lfo_theta -= 2.0f*M_PI;
        }
        
        switch(GLUE_2_mod_type) {
            case OSCILLATOR_TYPE_NONE:
            	lfo = 0.0f;
            	break;
          	case OSCILLATOR_TYPE_SIN:
          		// nothing to do
             	break;
            case OSCILLATOR_TYPE_SQR:
            	lfo = lfo<0?1.0f:-1.0f;
             	break;
             default:
             	break;   
        }
        
        // komplex szuroparameterek szamolasa 
        float fc = GLUE_2_fc + GLUE_2_mod_span * lfo;
		float wc = 2*M_PI*fc;
		float sigma = wc/(2*GLUE_2_q);
		float theta = wc/Fs; 
		float r = exp(-sigma/Fs); 
		float p_r = r*cos(theta); 
		float p_i = r*sin(theta);
		// komplex szuro
		float c_r = p_r * c_r_old - p_i * c_i_old + x; 
		float c_i = p_i * c_r_old + p_r * c_i_old;

		c_r_old = c_r; 
		c_i_old = c_i;

		y = c_i;

		float norm_factor = ((82*exp(-(GLUE_2_fc+GLUE_2_mod_span*lfo)/500)+1)*(1-3.13*sqrt(exp(-GLUE_2_q/0.56-2.23))));
		y = y/norm_factor;
		
    }
    


    //====================================================================
    // FILL OUTPUT VARIABLES
    //====================================================================

    // OUTPUT MUX
    if ((GLUE_output_mux & 0x01) != 0) {
        out1Left  = y;
        out1Right = y;
    } else {
        out1Left  = 0;
        out1Right = 0;
    }

    if ((GLUE_output_mux & 0x02) != 0) {
        out2Left  = y;
        out2Right = y;
    } else {
        out2Left  = 0;
        out2Right = 0;
    }

    if ((GLUE_output_mux & 0x04) != 0) {
        out3Left  = y;
        out3Right = y;
    } else {
        out3Left  = 0;
        out3Right = 0;
    }

    if ((GLUE_output_mux & 0x08) != 0) {
        out4Left  = y;
        out4Right = y;
    } else {
        out4Left  = 0;
        out4Right = 0;
    }

}
