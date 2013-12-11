#include "GLUE.h"

//========================================================================//
//                    I N T E R F A C E   L O G I C                       //
//========================================================================//
//                                                                        //
//  This  file  contains  all of the  interface functionalities. You can  //
//  implement  your custom  interface  application  by writing code with  //
//  the DSPController API inside the two function defined below.          //
//                                                                        //
//========================================================================//


//========================================================================//
//  STEP 1 - Define the variables and symbols                             //
//========================================================================//
//                                                                        //
//  Define local and GLUE variables and give them initial values          //
//                                                                        //
//========================================================================//

// Defined symbols
#define STATE_WELCOME       1
#define STATE_MAIN          2
#define STATE_DEMO          3
#define STATE_EFFECT_MAIN   4
#define STATE_EFFECT_OSC	5
#define STATE_EFFECT_LPF1	6
#define STATE_EFFECT_LPF2	7

// Local variables
// Global interface variables
int i = 0;
int interface_state = STATE_WELCOME;
int menu_pointer = 0;

// Demo interface variables
Encoder e1 = 0;
Encoder e2 = 0;
Encoder e3 = 0;

Encoder t1;
Encoder t2;
Encoder t3;

DIP dip;

int led_l = 1;
int led_r = 1;

float temp_float = 0.0f;
int float_precision;

// Audio interface variables
int temp_effect_state = 1;
int temp_mux = 0xf;





// GLUE variables
// Global effect parameters
float    GLUE_effect_state  = DEFAULT_GLUE_effect_state;
int      GLUE_output_mux    = DEFAULT_GLUE_output_mux;

// Oscillator parameters
float    GLUE_1_freq        = DEFAULT_GLUE_1_freq;
float    GLUE_1_amp         = DEFAULT_GLUE_1_amp;
int      GLUE_1_type        = DEFAULT_GLUE_1_type;

// LowPass paramters
float    GLUE_2_fc          = DEFAULT_GLUE_2_fc;
float    GLUE_2_q           = DEFAULT_GLUE_2_q;
float    GLUE_2_mod_type    = DEFAULT_GLUE_2_mod_type;
float    GLUE_2_mod_span    = DEFAULT_GLUE_2_mod_span;
float    GLUE_2_mod_freq    = DEFAULT_GLUE_2_mod_freq;


//========================================================================//
//  STEP 2 - Initialize the interface and the data structures             //
//========================================================================//
//                                                                        //
//  Initialize the  DSPController  with  API functions  and set up  your  //
//  data structures. This function will be called once before the signal  //
//  processing starts.                                                    //
//                                                                        //
//========================================================================//

void initInterface(void) {
    
    DSPController_init( FS_48KHZ | ENCODER_VELOCITY_OFF );
    DSPController_assembler_engage();
    
}


//========================================================================//
//  STEP 3 - Write your interface logic                                   //
//========================================================================//
//                                                                        //
//  With the DSPConroller API  create an event based  state machine that  //
//  interacts the user by processing the input events, and by displaying  //
//  data on the outputs (LCD/LED bars).                                   //
//                                                                        //
//  You can update or read  the GLUE variables here,  to control the DSP  //
//  code or update the displays according to the audio input.             //
//                                                                        //
//  This function will be  called  periodically  from the main loop as a  //
//  low priority function, therefore you are free to put some processing  //
//  heavy code here (i.e. sprintf(), LCD handling API functions).         //
//                                                                        //
//========================================================================//

int top = 0;
int bottom = 1;

const char *byte_to_binary(int x)
{
    static char b[5];
    b[0] = '\0';

    int z;
    for (z = 8; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

void updateInterface(void) {

    Event e = DSPController_get_event();
    
    switch (interface_state) {

        //=================================================
        // WELCOME STATE
        //=================================================
        case STATE_WELCOME:

            DSPController_lcd(0,"    Demo App    ");
            DSPController_lcd(1,"Press any button");

            if (e != DSPC_EVENT_NOTHING) {
                interface_state = STATE_MAIN;
                menu_pointer = 0;
            }

            break;

        //=================================================
        // MAIN STATE
        //=================================================
        case STATE_MAIN:
			
        	// Render top LCD
	        if (menu_pointer == 0) {
	            DSPController_lcd(0,"[Effect]  Demo  ");
	            DSPController_lcd(1,"  Audio effect  ");
	        } else if (menu_pointer == 1) {
	            DSPController_lcd(0," Effect  [Demo] ");
	            DSPController_lcd(1," Interface demo ");
	        }
        	
            // Step left
            if (e == DSPC_EVENT_A2_SHORT) {
                menu_pointer = 0;
            }

            // Step right
            if (e == DSPC_EVENT_A4_SHORT) {
                menu_pointer = 1;
            }

            // Enter
            if (e == DSPC_EVENT_A3_SHORT) {
                if (menu_pointer == 0) {
                    interface_state = STATE_EFFECT_MAIN;
                    DSPController_flush();
                } else {
                    interface_state = STATE_DEMO;
                    e1 = 0;
                    e2 = 0;
                    e3 = 0;
                    led_l = 1;
                    led_r = 1;
                    DSPController_flush();
                }
                menu_pointer = 0;
            }

            break;

        //=================================================
        // DEMO STATE
        //=================================================
        case STATE_DEMO:

        	// read encoders in a temporary variable
            t1 = DSPController_get_encoder(1);
            t2 = DSPController_get_encoder(2);
            t3 = DSPController_get_encoder(3);

            // LED rendering based on the encoder readings
            if (t1 > 0 || t2 > 0) {
                led_l = led_l << 1;
            } else if (t1 < 0 || t2 < 0) {
                led_l = led_l >> 1;
            }

            if (t3 > 0 || t2 > 0) {
                led_r = led_r << 1;
            } else if (t3 < 0 || t2 < 0) {
                led_r = led_r >> 1;
            }

            if (led_l == 0) {
                led_l = 1;
            }
            if (led_r == 0) {
                led_r = 1;
            }

            if (led_l > 0xff) {
                led_l = 0xff;
            }
            if (led_r > 0xff) {
                led_r = 0xff;
            }

            led_l |= 1;
            led_r |= 1;
			
            DSPController_led(led_l,led_r);

            // update encoder variables with the freshly readed values
			e1 += t1;
			e2 += t2;
			e3 += t3;
			
			
			
            
            // Top LCD handling
            // get dip value
            dip = DSPController_get_dip();
            
            // calculate floating point display precision based on the dip value
            float_precision = 1;
            for(i=1;i<256;i=i<<1) {
                if((dip&i)==0) {
                	break;   
                }
                float_precision++;
            }
            
            // render top LCD
            temp_float = DSPController_assembler_get_current();
    		DSPController_lcd_top("Num: %s",DSPC_FTS(temp_float,float_precision));
    		
    		

    		

    		// Bottom LCD  handling. Get menu pointer
            if (e == DSPC_EVENT_E1_SHORT) {
                menu_pointer = 0;
            }
                
            if (e == DSPC_EVENT_E2_SHORT) {
                menu_pointer = 1;
            }
                
            if (e == DSPC_EVENT_E3_SHORT) {
                menu_pointer = 2;
            }
            
            // Render bottom lcd
            if (menu_pointer == 0) {
                DSPController_lcd_bottom("[%03d] %03d  %03d  ",e1,e2,e3);
            } else if (menu_pointer == 1) {
                DSPController_lcd_bottom(" %03d [%03d] %03d  ",e1,e2,e3);
            } else if (menu_pointer == 2) {
                DSPController_lcd_bottom(" %03d  %03d [%03d] ",e1,e2,e3);
            }
            


            // Menu handling: BACK
            if (e == DSPC_EVENT_A1_SHORT) {
                interface_state = STATE_MAIN;
                menu_pointer = 0;
            }

            break;
            
        
        //=================================================
        // MAIN EFFECT STATE
        //=================================================
        case STATE_EFFECT_MAIN:

            
         
            
            
            t1 = DSPController_get_encoder(1);
            
            temp_effect_state += t1;
            
            if(temp_effect_state < 1) temp_effect_state = 1;
            if(temp_effect_state > 3) temp_effect_state = 3;
            
            
            switch (temp_effect_state) {
                case 1:
                	GLUE_effect_state = EFFECT_STATE_MUTED; break;
              	case 2:
                	GLUE_effect_state = EFFECT_STATE_OSCILLATOR; break;
                case 3:
                	GLUE_effect_state = EFFECT_STATE_LOW_PASS; break;
                default: break;
            }
            
            t3 = DSPController_get_encoder(3);
            temp_mux += t3;
            if(temp_mux < 0) temp_mux = 0;
            if(temp_mux > 0xf) temp_mux = 0xf;
            GLUE_output_mux = temp_mux;
            
            // render LCD
	        if (GLUE_effect_state == EFFECT_STATE_MUTED) {
	            DSPController_lcd(0," Effect   MUX  ");
	            DSPController_lcd(1,"  1 Mute  %s",byte_to_binary(temp_mux));
	        } else if (GLUE_effect_state == EFFECT_STATE_OSCILLATOR) {
	            DSPController_lcd(0,"[Effect]  MUX  ");
	            DSPController_lcd(1,"  2 Osc   %s",byte_to_binary(temp_mux));
	        } else if (GLUE_effect_state == EFFECT_STATE_LOW_PASS) {
	            DSPController_lcd(0,"[Effect]  MUX  ");
	            DSPController_lcd(1,"  3 LPF   %s",byte_to_binary(temp_mux));
	        }
            
            
	        // Menu handling: ENTER
            if (e == DSPC_EVENT_A3_SHORT) {
                
                switch (temp_effect_state) {
	                case 1: 
	                	break;
	              	case 2:
	                	interface_state = STATE_EFFECT_OSC; 
	                	break;
	                case 3:
	                	interface_state = STATE_EFFECT_LPF1; 
	                	break;
	                default: break;
	            }
	            
	            DSPController_flush();
                menu_pointer = 0;
            }
            
            // Menu handling: BACK
            if (e == DSPC_EVENT_A1_SHORT) {
                interface_state = STATE_MAIN;
                menu_pointer = 0;
            }

            break;
            
            
        //=================================================
        // EFFECT OSCILLATOR STATE
        //=================================================
        case STATE_EFFECT_OSC:

            DSPController_lcd(0,"OSC");
            DSPController_lcd(1," ");
            
            // Menu handling: BACK
            if (e == DSPC_EVENT_A1_SHORT) {
                interface_state = STATE_EFFECT_MAIN;
                menu_pointer = 0;
            }

            break;
            
            
        //=================================================
        // EFFECT LPF 1 STATE
        //=================================================
        case STATE_EFFECT_LPF1:

            DSPController_lcd(0,"LPF1");
            DSPController_lcd(1," ");
            
            // Menu handling: BACK
            if (e == DSPC_EVENT_A1_SHORT) {
                interface_state = STATE_EFFECT_MAIN;
                menu_pointer = 0;
            }

            break;
            
            
            
        //=================================================
        // EFFECT LPF 2 STATE
        //=================================================
        case STATE_EFFECT_LPF2:

            DSPController_lcd(0,"LPF2");
            DSPController_lcd(1," ");
            
            // Menu handling: BACK
            if (e == DSPC_EVENT_A1_SHORT) {
                interface_state = STATE_EFFECT_MAIN;
                menu_pointer = 0;
            }

            break;
		

        default:
            break;
    }
    
    

}
