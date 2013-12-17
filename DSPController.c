////////////////////////////////////////////////////////////////////////////
// DSPController.c
/////////////////////////////////////////////////////////////////////////////

#include "DSPController.h"
#include <SRU.h>




//========================================================================
//------------------------------------------------------------------------
//                          V A R I A B L E S
//------------------------------------------------------------------------
//========================================================================

//========================================================================
//  C O R E   V A R I A B L E S
//========================================================================
// settings variables
volatile int 				dspcontroller_tick_threshold = DSPC_TICK_THRESHOLD_48;

volatile int 				dspcontroller_led_counter_max = DSPC_LED_COUNTER_MAX_48;
volatile int 				dspcontroller_lcd_counter_max = DSPC_LCD_COUNTER_MAX_48;
volatile int 				dspcontroller_dip_counter_max = DSPC_DIP_COUNTER_MAX_48;

volatile char 				dspcontroller_encoder_velocity_enabled = 1;

// state machine variables
volatile short          	dspcontroller_tick_counter;
volatile unsigned char	    dspcontroller_spi_state;
volatile unsigned char	    dspcontroller_receive_counter;

// event handling variables
volatile unsigned char 		dspcontroller_event_buffer[DSPC_EVENT_BUFFER_SIZE];
volatile unsigned char      dspcontroller_event_pointer;

// encoder variables
volatile int         		dspcontroller_encoder_1;   
volatile int         		dspcontroller_encoder_2; 
volatile int         		dspcontroller_encoder_3;

volatile int 				dspcontroller_encoder_velocity_1;
volatile int 				dspcontroller_encoder_velocity_2;
volatile int 				dspcontroller_encoder_velocity_3;

// led handling variables
volatile Led      	    dspcontroller_led_l;
volatile Led    	    dspcontroller_led_r;
volatile Led                dspcontroller_led_l_out;
volatile Led                dspcontroller_led_r_out;
volatile unsigned char      dspcontroller_led_counter;
volatile unsigned char      dspcontroller_led_cycle_counter;
volatile unsigned char      dspcontroller_leds_are_waiting;

// lcd handling variables
volatile unsigned char	    dspcontroller_lcd_top_is_waiting;
volatile unsigned char      dspcontroller_lcd_top_counter;
volatile unsigned char      dspcontroller_lcd_top_cycle_counter;


volatile unsigned char	    dspcontroller_lcd_bottom_is_waiting;
volatile unsigned char      dspcontroller_lcd_bottom_counter;
volatile unsigned char      dspcontroller_lcd_bottom_cycle_counter;




char						dspcontroller_lcd_top[16];
char						dspcontroller_lcd_bottom[16];
char      					dspcontroller_lcd_top_out[16];
char      					dspcontroller_lcd_bottom_out[16];

char						dspcontroller_lcd_top_check[16];
char						dspcontroller_lcd_bottom_check[16];

// dip handling variables
volatile unsigned char      dspcontroller_dip;
volatile unsigned char      dspcontroller_dip_counter;





//========================================================================
//------------------------------------------------------------------------
//            D S P   C O N T R O L L E R   C O R E   A P I
//------------------------------------------------------------------------
//========================================================================


//========================================================================
//  P L A T F O R M   D E P E N D E N T   F U N C T I O N S 
//========================================================================

void dspcontroller_spi_init() {
    
    // init SPI MASTER TX
	*(volatile int *)DSPC_SHARC_SPICTL = 0;
	*(volatile int *)DSPC_SHARC_SPIFLG = 0;

	// set SPI BAUD
	*(volatile int *)DSPC_SHARC_SPIBAUD = 20;

	// set up DAG registers
	*(volatile int *)DSPC_SHARC_SPICTL = DSPC_SHARC_TIMOD|
         								 DSPC_SHARC_MSBF|
         								 DSPC_SHARC_SPIMS|
         								 DSPC_SHARC_SPIEN|
         								 DSPC_SHARC_GM;
         								 
    // init DAI pin for SS
    // *(volatile int *)(0x247D) = 0;
    SRU(HIGH,PBEN15_I);
    SRU(HIGH,DAI_PB15_I);

}


//========================================================================
//------------------------------------------------------------------------
//                  C O R E   A P I   F U N C T I O N S 
//------------------------------------------------------------------------
//========================================================================


void DSPController_init_default() {
    
    dspcontroller_tick_threshold = DSPC_TICK_THRESHOLD_48;
    
    dspcontroller_led_counter_max = DSPC_LED_COUNTER_MAX_48;
	dspcontroller_lcd_counter_max = DSPC_LCD_COUNTER_MAX_48;
	dspcontroller_dip_counter_max = DSPC_DIP_COUNTER_MAX_48;

    dspcontroller_encoder_velocity_enabled = 1;
    
    dspcontroller_init();
}

void DSPController_init(int code) {
    
    if((code & 0x01) != 0) {
        dspcontroller_tick_threshold = DSPC_TICK_THRESHOLD_48;
        
        dspcontroller_led_counter_max = DSPC_LED_COUNTER_MAX_48;
		dspcontroller_lcd_counter_max = DSPC_LCD_COUNTER_MAX_48;
		dspcontroller_dip_counter_max = DSPC_DIP_COUNTER_MAX_48;
		
    } else {
        dspcontroller_tick_threshold = DSPC_TICK_THRESHOLD_96;
        
        dspcontroller_led_counter_max = DSPC_LED_COUNTER_MAX_96;
		dspcontroller_lcd_counter_max = DSPC_LCD_COUNTER_MAX_96;
		dspcontroller_dip_counter_max = DSPC_DIP_COUNTER_MAX_96;
    }
    
    if((code & 0x02) != 0) {
        dspcontroller_encoder_velocity_enabled = 1;
    } else {
        dspcontroller_encoder_velocity_enabled = 0;
    }
    
    dspcontroller_init();
}


void dspcontroller_init() {

    // state machine variables
    dspcontroller_tick_counter = 0;
    dspcontroller_spi_state = DSPC_STATE_SPI_IDLE;
    dspcontroller_receive_counter = 0;

    // event handling variables
    dspcontroller_event_pointer = 0;

    // encoder variables
    dspcontroller_encoder_1 = 0;
    dspcontroller_encoder_2 = 0;
    dspcontroller_encoder_3 = 0;
    
    dspcontroller_encoder_velocity_1 = 0;
    dspcontroller_encoder_velocity_2 = 0;
    dspcontroller_encoder_velocity_3 = 0;

    // led handling variables
    dspcontroller_leds_are_waiting = 0;
    dspcontroller_led_counter = 0;
    dspcontroller_led_l = 0;
    dspcontroller_led_r = 0;

    // lcd handling variables
    dspcontroller_lcd_top_is_waiting = 0;
    dspcontroller_lcd_top_is_waiting = 0;
    //dspcontroller_lcd_top_sum = 0;

    dspcontroller_lcd_bottom_counter = 0;
    dspcontroller_lcd_bottom_counter = 0;
    //dspcontroller_lcd_bottom_sum = 0;

    
    // dip handling variables
    dspcontroller_dip = 0;
    dspcontroller_dip_counter = 0;

    // platform dependent low level init
    dspcontroller_spi_init();

    // init applications
    #ifdef _ADD_ASSEMBLER
    dspcontroller_assembler_init();
    #endif // _ADD_ASSEMBLER
}


//========================================================================
//  M A I N   S T A T E   M A C H I N E
//========================================================================

void DSPController_tick() {
    // update state machine
    if(dspcontroller_tick_counter++ == dspcontroller_tick_threshold) {
        dspcontroller_tick_counter = 0;

        // counter logic
        if (dspcontroller_led_counter != 0) 
            dspcontroller_led_counter--;

        if (dspcontroller_lcd_top_counter != 0) 
            dspcontroller_lcd_top_counter--;

        if (dspcontroller_lcd_bottom_counter != 0) 
            dspcontroller_lcd_bottom_counter--;

        if (dspcontroller_dip_counter != 0)
            dspcontroller_dip_counter--;
            
            
        // encoder velocity logic
        if (dspcontroller_encoder_velocity_1 > 0) dspcontroller_encoder_velocity_1--; 
        if (dspcontroller_encoder_velocity_2 > 0) dspcontroller_encoder_velocity_2--;
        if (dspcontroller_encoder_velocity_3 > 0) dspcontroller_encoder_velocity_3--;


        //================================================================
        // IDLE
        //================================================================
        if (dspcontroller_spi_state == DSPC_STATE_SPI_IDLE) {

            dspcontroller_select_device();

            // decide which getter protocol to use
            if (dspcontroller_leds_are_waiting) {
                if (dspcontroller_led_counter == 0) {
                    dspcontroller_leds_are_waiting = 0;
                    dspcontroller_led_counter = dspcontroller_led_counter_max;
                    dspcontroller_spi_send_data(DSCP_MESSAGE_GET_WITH_LED);
                    dspcontroller_spi_state = DSPC_STATE_SPI_LED_INIT_SENT;
                }

            } else if (dspcontroller_lcd_top_is_waiting) {
                if (dspcontroller_lcd_top_counter == 0) {
                    dspcontroller_lcd_top_is_waiting = 0;
                    dspcontroller_lcd_top_counter = dspcontroller_lcd_counter_max;
                    dspcontroller_spi_send_data(DSCP_MESSAGE_GET_WITH_LCD_TOP);
                    dspcontroller_spi_state = DSPC_STATE_SPI_LCD_TOP_INIT_SENT;
                }

            } else if (dspcontroller_lcd_bottom_is_waiting) {
                if (dspcontroller_lcd_bottom_counter == 0) {
                    dspcontroller_lcd_bottom_is_waiting = 0;
                    dspcontroller_lcd_bottom_counter = dspcontroller_lcd_counter_max;
                    dspcontroller_spi_send_data(DSCP_MESSAGE_GET_WITH_LCD_BOTTOM);
                    dspcontroller_spi_state = DSPC_STATE_SPI_LCD_BOTTOM_INIT_SENT;
                }

            } else if (dspcontroller_dip_counter == 0) {
                dspcontroller_dip_counter = dspcontroller_dip_counter_max;
                dspcontroller_spi_send_data(DSCP_MESSAGE_GET_DIP_STATUS);
                dspcontroller_spi_state = DSPC_STATE_SPI_DIP_INIT_SENT;

            } else {
                dspcontroller_spi_send_data(DSCP_MESSAGE_GET_SIMPLE);
                dspcontroller_spi_state = DSPC_STATE_SPI_SIMPLE_INIT_SENT;
            }

        //================================================================
        // SIMPLE INIT SENT
        //================================================================
        } else if (dspcontroller_spi_state == DSPC_STATE_SPI_SIMPLE_INIT_SENT) {

            if (dspcontroller_spi_check()) {
                unsigned char received = dspcontroller_spi_read_data();
                if (received == 0 || received > 25) {
                    dspcontroller_spi_state = DSPC_STATE_SPI_IDLE;
                    dspcontroller_deselect_device();
                } else {
                    dspcontroller_receive_counter = received-1;
                    dspcontroller_spi_state = DSPC_STATE_SPI_SIMPLE_SENT;
                    dspcontroller_spi_send_data(0);
                }
            }

        //================================================================
        // LED INIT SENT
        //================================================================
        } else if (dspcontroller_spi_state == DSPC_STATE_SPI_LED_INIT_SENT) {

            if (dspcontroller_spi_check()) {
                dspcontroller_led_cycle_counter = 0;
                dspcontroller_spi_state = DSPC_STATE_SPI_LED_SENT;

                // prepare data
                dspcontroller_led_l_out = dspcontroller_led_l;
                dspcontroller_led_r_out = dspcontroller_led_r;

                dspcontroller_spi_send_data(dspcontroller_led_l_out);
            }

        //================================================================
        // LCD TOP INIT SENT
        //================================================================
        } else if (dspcontroller_spi_state == DSPC_STATE_SPI_LCD_TOP_INIT_SENT) {

            if (dspcontroller_spi_check()) {
                unsigned char i;
                dspcontroller_lcd_top_cycle_counter = 15;
                dspcontroller_spi_state = DSPC_STATE_SPI_LCD_TOP_SENT;

                // prepare data
                memcpy(dspcontroller_lcd_top_out,dspcontroller_lcd_top,16);

                dspcontroller_spi_send_data(dspcontroller_lcd_top_out[dspcontroller_lcd_top_cycle_counter]);
            }

        //================================================================
        // LCD BOTTOM INIT SENT
        //================================================================
        } else if (dspcontroller_spi_state == DSPC_STATE_SPI_LCD_BOTTOM_INIT_SENT) {

            if (dspcontroller_spi_check()) {
                unsigned char i;
                dspcontroller_lcd_bottom_cycle_counter = 15;
                dspcontroller_spi_state = DSPC_STATE_SPI_LCD_BOTTOM_SENT;

                // prepare data
                memcpy(dspcontroller_lcd_bottom_out,dspcontroller_lcd_bottom,16);

                dspcontroller_spi_send_data(dspcontroller_lcd_bottom_out[dspcontroller_lcd_bottom_cycle_counter]);
            }

        //================================================================
        // DIP INIT SENT
        //================================================================
        } else if (dspcontroller_spi_state == DSPC_STATE_SPI_DIP_INIT_SENT) {

            if (dspcontroller_spi_check()) {
                dspcontroller_spi_send_data(0);
                dspcontroller_spi_state = DSPC_STATE_SPI_DIP_SENT;
            }

        //================================================================
        // SIMPLE SENT
        //================================================================
        } else if (dspcontroller_spi_state == DSPC_STATE_SPI_SIMPLE_SENT) {

            if (dspcontroller_spi_check()) {
                unsigned char data = dspcontroller_spi_read_data();

                dspcontroller_process_event(data);

                if (dspcontroller_receive_counter != 0) {
                    dspcontroller_receive_counter--;
                    dspcontroller_spi_send_data(0);
                } else {
                    dspcontroller_spi_state = DSPC_STATE_SPI_IDLE;
                    dspcontroller_deselect_device();
                }
            }

        //================================================================
        // LED SENT
        //================================================================
        } else if (dspcontroller_spi_state == DSPC_STATE_SPI_LED_SENT) {

            if (dspcontroller_spi_check()) {
                unsigned char data = dspcontroller_spi_read_data();
                if (data != 0) {
                    dspcontroller_process_event(data);
                }

                if (dspcontroller_led_cycle_counter) {
                    dspcontroller_spi_state = DSPC_STATE_SPI_IDLE;
                    dspcontroller_deselect_device();
                } else {
                    dspcontroller_spi_send_data(dspcontroller_led_r_out);
                    dspcontroller_led_cycle_counter = 1;
                }

            }

        //================================================================
        // LCD TOP SENT
        //================================================================
        } else if (dspcontroller_spi_state == DSPC_STATE_SPI_LCD_TOP_SENT) {

            if (dspcontroller_spi_check()) {
                unsigned char data = dspcontroller_spi_read_data();
                if (data != 0) {
                    dspcontroller_process_event(data);
                }

                if (dspcontroller_lcd_top_cycle_counter == 0) {
                    dspcontroller_spi_state = DSPC_STATE_SPI_IDLE;
                    dspcontroller_deselect_device();
                } else {
                    dspcontroller_lcd_top_cycle_counter--;
                    dspcontroller_spi_send_data(dspcontroller_lcd_top_out[dspcontroller_lcd_top_cycle_counter]);
                }
            }

        //================================================================
        // LCD BOTTOM SENT
        //================================================================
        } else if (dspcontroller_spi_state == DSPC_STATE_SPI_LCD_BOTTOM_SENT) {

            if (dspcontroller_spi_check()) {
                unsigned char data = dspcontroller_spi_read_data();
                if (data != 0) {
                    dspcontroller_process_event(data);
                }

                if (dspcontroller_lcd_bottom_cycle_counter == 0) {
                    dspcontroller_spi_state = DSPC_STATE_SPI_IDLE;
                    dspcontroller_deselect_device();
                } else {
                    dspcontroller_lcd_bottom_cycle_counter--;
                    dspcontroller_spi_send_data(dspcontroller_lcd_bottom_out[dspcontroller_lcd_bottom_cycle_counter]);
                }
            }

        //================================================================
        // DIP SENT
        //================================================================
        } else if (dspcontroller_spi_state == DSPC_STATE_SPI_DIP_SENT) {

            if (dspcontroller_spi_check()) {
                
                dspcontroller_dip = dspcontroller_spi_read_data();
                dspcontroller_spi_state = DSPC_STATE_SPI_IDLE;
                dspcontroller_deselect_device();
            }

        //================================================================
        // BOOT
        //================================================================
        } else if (dspcontroller_spi_state == DSPC_STATE_SPI_BOOT) {

            dspcontroller_select_device();
            dspcontroller_spi_send_data(0);
            dspcontroller_spi_state = DSPC_STATE_SPI_BOOT_SENT;

        //================================================================
        // BOOT SENT
        //================================================================
        } else if (dspcontroller_spi_state == DSPC_STATE_SPI_BOOT_SENT) {

            if (dspcontroller_spi_check()) {
                
                unsigned char data = dspcontroller_spi_read_data();
                if (data == 0x80) {
                    dspcontroller_spi_state = DSPC_STATE_SPI_IDLE;
                } else {
                    dspcontroller_spi_state = DSPC_STATE_SPI_BOOT;
                }
                dspcontroller_deselect_device();
            }
        }
    }
}

//========================================================================
//  E V E N T   H A N D L I N G
//========================================================================

void dspcontroller_process_event(unsigned char event) {

    unsigned char address = event & 0x0f;

    if ( address > 12 ) {
        // encoder event => get increment
        int value = event >> 4;
        if ((value & 0x08) != 0) {
            value |= ~0x0f; 
        }
        
        if (address == 13) {
            
            if (dspcontroller_encoder_velocity_enabled != 0 && dspcontroller_encoder_velocity_1 > 0) {
                value *= DSPC_ENCODER_VELOCITY_MULTIPLIER;
            }
            
            dspcontroller_encoder_1 += value;
            dspcontroller_encoder_velocity_1 = DSPC_ENCODER_VELOCITY_THRESHOLD;
        }
        
        if (address == 14) {
            
            if (dspcontroller_encoder_velocity_enabled != 0 && dspcontroller_encoder_velocity_2 > 0) {
                value *= DSPC_ENCODER_VELOCITY_MULTIPLIER;
            }
            
            dspcontroller_encoder_2 += value;
            dspcontroller_encoder_velocity_2 = DSPC_ENCODER_VELOCITY_THRESHOLD;
        }
        
        if (address == 15) {
            
            if (dspcontroller_encoder_velocity_enabled != 0 && dspcontroller_encoder_velocity_3 > 0) {
                value *= DSPC_ENCODER_VELOCITY_MULTIPLIER;
            }
            
            dspcontroller_encoder_3 += value;
            dspcontroller_encoder_velocity_3 = DSPC_ENCODER_VELOCITY_THRESHOLD;
        }
        
    } else {
        dspcontroller_add_event_to_buffer(event);

        #ifdef _ADD_ASSEMBLER
        dspcontroller_assembler_process_event(event);
        #endif // _ADD_ASSEMBLER
    }
}

void dspcontroller_add_event_to_buffer(unsigned char event) {
    // dspcontroller_atomic_block_BEGIN();
    if(dspcontroller_event_pointer == DSPC_EVENT_BUFFER_SIZE-1) {
        return;
    }
    dspcontroller_event_buffer[dspcontroller_event_pointer++] = event;
    // dspcontroller_atomic_block_END();
}

void DSPController_flush(void) {
    // event handling variables
    dspcontroller_event_pointer = 0;

    // encoder variables
    dspcontroller_encoder_1 = 0;
    dspcontroller_encoder_2 = 0;
    dspcontroller_encoder_3 = 0;
    
    dspcontroller_encoder_velocity_1 = 0;
    dspcontroller_encoder_velocity_2 = 0;
    dspcontroller_encoder_velocity_3 = 0;
}

unsigned char DSPController_get_event() {
    if (dspcontroller_event_pointer == 0) {
        return DSPC_EVENT_NOTHING;
    } else {
        dspcontroller_event_pointer--;
        unsigned char raw_event = dspcontroller_event_buffer[dspcontroller_event_pointer];
        
        //========================================================================
        //  BREAKING DOWN THE EVENTS
        //========================================================================
        unsigned char type = (raw_event & 0x30) >> 4;
        unsigned char status = raw_event >> 6;
        unsigned char address = raw_event & 0x0f;

        switch(type) {

            //================================================================
            // NUMPAD
            //================================================================
            case DSCP_MESSAGE_TYPE_NUMPAD:
                switch(status) {

                    //========================================================
                    // DOWN
                    //========================================================
                    case DSCP_MESSAGE_STATUS_DOWN:
                        switch(address) {
                            case 1: return DSPC_EVENT_N1_DOWN;
                            case 2: return DSPC_EVENT_N2_DOWN;
                            case 3: return DSPC_EVENT_N3_DOWN;
                            case 4: return DSPC_EVENT_N4_DOWN;
                            case 5: return DSPC_EVENT_N5_DOWN;
                            case 6: return DSPC_EVENT_N6_DOWN;
                            case 7: return DSPC_EVENT_N7_DOWN;
                            case 8: return DSPC_EVENT_N8_DOWN;
                            case 9: return DSPC_EVENT_N9_DOWN;
                            case 10: return DSPC_EVENT_N10_DOWN;
                            case 11: return DSPC_EVENT_N11_DOWN;
                            case 12: return DSPC_EVENT_N12_DOWN;
                        }
                        break;

                    //========================================================
                    // UP
                    //========================================================
                    case DSCP_MESSAGE_STATUS_UP:
                        switch(address) {
                            case 1: return DSPC_EVENT_N1_UP;
                            case 2: return DSPC_EVENT_N2_UP;
                            case 3: return DSPC_EVENT_N3_UP;
                            case 4: return DSPC_EVENT_N4_UP;
                            case 5: return DSPC_EVENT_N5_UP;
                            case 6: return DSPC_EVENT_N6_UP;
                            case 7: return DSPC_EVENT_N7_UP;
                            case 8: return DSPC_EVENT_N8_UP;
                            case 9: return DSPC_EVENT_N9_UP;
                            case 10: return DSPC_EVENT_N10_UP;
                            case 11: return DSPC_EVENT_N11_UP;
                            case 12: return DSPC_EVENT_N12_UP;
                        }
                        break;

                    //========================================================
                    // SHORT
                    //========================================================
                    case DSCP_MESSAGE_STATUS_SHORT:
                        switch(address) {
                            case 1: return DSPC_EVENT_N1_SHORT;
                            case 2: return DSPC_EVENT_N2_SHORT;
                            case 3: return DSPC_EVENT_N3_SHORT;
                            case 4: return DSPC_EVENT_N4_SHORT;
                            case 5: return DSPC_EVENT_N5_SHORT;
                            case 6: return DSPC_EVENT_N6_SHORT;
                            case 7: return DSPC_EVENT_N7_SHORT;
                            case 8: return DSPC_EVENT_N8_SHORT;
                            case 9: return DSPC_EVENT_N9_SHORT;
                            case 10: return DSPC_EVENT_N10_SHORT;
                            case 11: return DSPC_EVENT_N11_SHORT;
                            case 12: return DSPC_EVENT_N12_SHORT;
                        }
                        break;

                    //========================================================
                    // LONG
                    //========================================================
                    case DSCP_MESSAGE_STATUS_LONG:
                        switch(address) {
                            case 1: return DSPC_EVENT_N1_LONG;
                            case 2: return DSPC_EVENT_N2_LONG;
                            case 3: return DSPC_EVENT_N3_LONG;
                            case 4: return DSPC_EVENT_N4_LONG;
                            case 5: return DSPC_EVENT_N5_LONG;
                            case 6: return DSPC_EVENT_N6_LONG;
                            case 7: return DSPC_EVENT_N7_LONG;
                            case 8: return DSPC_EVENT_N8_LONG;
                            case 9: return DSPC_EVENT_N9_LONG;
                            case 10: return DSPC_EVENT_N10_LONG;
                            case 11: return DSPC_EVENT_N11_LONG;
                            case 12: return DSPC_EVENT_N12_LONG;
                        }
                        break;

                    default:
                        return DSPC_EVENT_NOTHING;
                }
                break;

            //================================================================
            // FUNCTION & ARROW
            //================================================================
            case DSCP_MESSAGE_TYPE_F_AND_A:
                switch(status) {

                    //========================================================
                    // DOWN
                    //========================================================
                    case DSCP_MESSAGE_STATUS_DOWN:
                        switch(address) {
                            case 1: return DSPC_EVENT_F1_DOWN;
                            case 2: return DSPC_EVENT_F2_DOWN;
                            case 3: return DSPC_EVENT_F3_DOWN;
                            case 4: return DSPC_EVENT_F4_DOWN;
                            case 5: return DSPC_EVENT_A1_DOWN;
                            case 6: return DSPC_EVENT_A2_DOWN;
                            case 7: return DSPC_EVENT_A3_DOWN;
                            case 8: return DSPC_EVENT_A4_DOWN;
                            case 9: return DSPC_EVENT_A5_DOWN;
                        }
                        break;

                    //========================================================
                    // UP
                    //========================================================
                    case DSCP_MESSAGE_STATUS_UP:
                        switch(address) {
                            case 1: return DSPC_EVENT_F1_UP;
                            case 2: return DSPC_EVENT_F2_UP;
                            case 3: return DSPC_EVENT_F3_UP;
                            case 4: return DSPC_EVENT_F4_UP;
                            case 5: return DSPC_EVENT_A1_UP;
                            case 6: return DSPC_EVENT_A2_UP;
                            case 7: return DSPC_EVENT_A3_UP;
                            case 8: return DSPC_EVENT_A4_UP;
                            case 9: return DSPC_EVENT_A5_UP;
                        }
                        break;

                    //========================================================
                    // SHORT
                    //========================================================
                    case DSCP_MESSAGE_STATUS_SHORT:
                        switch(address) {
                            case 1: return DSPC_EVENT_F1_SHORT;
                            case 2: return DSPC_EVENT_F2_SHORT;
                            case 3: return DSPC_EVENT_F3_SHORT;
                            case 4: return DSPC_EVENT_F4_SHORT;
                            case 5: return DSPC_EVENT_A1_SHORT;
                            case 6: return DSPC_EVENT_A2_SHORT;
                            case 7: return DSPC_EVENT_A3_SHORT;
                            case 8: return DSPC_EVENT_A4_SHORT;
                            case 9: return DSPC_EVENT_A5_SHORT;
                        }
                        break;

                    //========================================================
                    // LONG
                    //========================================================
                    case DSCP_MESSAGE_STATUS_LONG:
                        switch(address) {
                            case 1: return DSPC_EVENT_F1_LONG;
                            case 2: return DSPC_EVENT_F2_LONG;
                            case 3: return DSPC_EVENT_F3_LONG;
                            case 4: return DSPC_EVENT_F4_LONG;
                            case 5: return DSPC_EVENT_A1_LONG;
                            case 6: return DSPC_EVENT_A2_LONG;
                            case 7: return DSPC_EVENT_A3_LONG;
                            case 8: return DSPC_EVENT_A4_LONG;
                            case 9: return DSPC_EVENT_A5_LONG;
                        }
                        break;

                    default:
                        return DSPC_EVENT_NOTHING;
                }
                break;

            //================================================================
            // ENCODER
            //================================================================
            case DSCP_MESSAGE_TYPE_ENCODER:
                switch(status) {

                    //========================================================
                    // DOWN
                    //========================================================
                    case DSCP_MESSAGE_STATUS_DOWN:
                        switch(address) {
                            case 1: return DSPC_EVENT_E1_DOWN;
                            case 2: return DSPC_EVENT_E2_DOWN;
                            case 3: return DSPC_EVENT_E3_DOWN;
                        }
                        break;

                    //========================================================
                    // UP
                    //========================================================
                    case DSCP_MESSAGE_STATUS_UP:
                        switch(address) {
                            case 1: return DSPC_EVENT_E1_UP;
                            case 2: return DSPC_EVENT_E2_UP;
                            case 3: return DSPC_EVENT_E3_UP;
                        }
                        break;

                    //========================================================
                    // SHORT
                    //========================================================
                    case DSCP_MESSAGE_STATUS_SHORT:
                        switch(address) {
                            case 1: return DSPC_EVENT_E1_SHORT;
                            case 2: return DSPC_EVENT_E2_SHORT;
                            case 3: return DSPC_EVENT_E3_SHORT;
                        }
                        break;

                    //========================================================
                    // LONG
                    //========================================================
                    case DSCP_MESSAGE_STATUS_LONG:
                        switch(address) {
                            case 1: return DSPC_EVENT_E1_LONG;
                            case 2: return DSPC_EVENT_E2_LONG;
                            case 3: return DSPC_EVENT_E3_LONG;
                        }
                        break;

                    default:
                        return DSPC_EVENT_NOTHING;
                }
                break;
            default:
                return DSPC_EVENT_NOTHING;
        }
    }

    return DSPC_EVENT_NOTHING;
}


//========================================================================
//  E N C O D E R   H A N D L I N G
//========================================================================

int DSPController_get_encoder(char encoder) {
    int ret = 0;
    if (encoder == 1) {
        ret = dspcontroller_encoder_1;
        dspcontroller_encoder_1 = 0;
    } else if (encoder == 2) {
        ret = dspcontroller_encoder_2;
        dspcontroller_encoder_2 = 0;
    } else if (encoder == 3) {
        ret = dspcontroller_encoder_3;
        dspcontroller_encoder_3 = 0;
    }
    return ret;
}


//========================================================================
//  L E D   H A N D L I N G
//========================================================================

void DSPController_led(Led led_l, Led led_r) {

    if (dspcontroller_spi_state < DSPC_STATE_SPI_IDLE) {
        return;
    }

    if (led_l != dspcontroller_led_l || led_r != dspcontroller_led_r) {
        dspcontroller_led_l = led_l;
        dspcontroller_led_r = led_r;
        dspcontroller_leds_are_waiting = 1;
    }
}


//========================================================================
//  L C D   H A N D L I N G
//========================================================================

void dspcontroller_lcd_handler(unsigned char line, const char* format, va_list args) {

    char input_buff[32];
    char limit_buff[16];
    unsigned char inputLength;
    unsigned char limitLength;
    int i;

    if (dspcontroller_spi_state < DSPC_STATE_SPI_IDLE) {
        return;
    }
    
    vsprintf (input_buff,format, args);
    
    
    size_t l = strlen(input_buff); 
    if (l < 16) limitLength = l;
    else limitLength = 16;
    
    
    for(i=0;i<16;i++) {
        limit_buff[i] = i<limitLength ? input_buff[i] : ' ';
    }
	
    
    // check if its new or not
    if (line == 0) {
        char equal = 1;
        for(i=0;i<16;i++) {
            if (limit_buff[i] != dspcontroller_lcd_top_check[i]) {
                equal = 0;
            }
            dspcontroller_lcd_top_check[i] = limit_buff[i];
    	}
    	if(equal == 1) {
    		return;   
    	}
    } else {
        char equal = 1;
        for(i=0;i<16;i++) {
            if (limit_buff[i] != dspcontroller_lcd_bottom_check[i]) {
                equal = 0;
            }
            dspcontroller_lcd_bottom_check[i] = limit_buff[i];
    	}
    	if(equal == 1) {
    		return;   
    	}
    }
    
    
    
    if (line == 0) {
        memcpy(dspcontroller_lcd_top,limit_buff,16);
        dspcontroller_lcd_top_is_waiting = 1;
    } else {
        memcpy(dspcontroller_lcd_bottom,limit_buff,16);
        dspcontroller_lcd_bottom_is_waiting = 1;
    }
    
}

void DSPController_lcd(char line, const char* format, ...) {
    va_list args;
    va_start (args, format);
    dspcontroller_lcd_handler(line,format,args);
    va_end (args);
}

void DSPController_lcd_top(const char* format, ...) {
    va_list args;
    va_start (args, format);
    dspcontroller_lcd_handler(0,format,args);
    va_end (args);
}

void DSPController_lcd_bottom(const char* format, ...) {
    va_list args;
    va_start (args, format);
    dspcontroller_lcd_handler(1,format,args);
    va_end (args);
}


//========================================================================
//  D I P   H A N D L I N G
//========================================================================

unsigned char DSPController_get_dip() {
    return dspcontroller_dip;
}

//========================================================================
//       ***      E N D   O F   T H E   C O R E   A P I       ***
//========================================================================















//========================================================================
//------------------------------------------------------------------------
//                         A P P L I C A T I O N S
//------------------------------------------------------------------------
//========================================================================


#ifdef _ADD_ASSEMBLER
//========================================================================
//                     N U M B E R   A S S E M B L E R
//========================================================================

//========================================================================
//  C O R E   F U N C T I O N S
//========================================================================
void dspcontroller_assembler_init() {
    dspcontroller_float_to_char_index = 0;
    DSPController_assembler_reset();
    dspcontroller_assembler_state = DSPC_ASSEMBLER_STATE_OFF;
    dspcontroller_last_conversion = 0.0f;

    dspcontroller_assembler_new_number = 0;
}

void DSPController_assembler_reset() {
    
    dspcontroller_assembler_state = DSPC_ASSEMBLER_STATE_IDLE;
    
    dspcontroller_assembler_wp = 0;
    dspcontroller_assembler_dp = 0;
    dspcontroller_assembler_ep = 0;
    
    dspcontroller_assembler_sign = 1;
}

void dspcontroller_assembler_process_event(unsigned char event) {
    unsigned char type;
    unsigned char status;
    unsigned char address;

    if (dspcontroller_assembler_state == DSPC_ASSEMBLER_STATE_OFF) {
        return;
    }

    type = (event & 0x30) >> 4;
    status = event >> 6;
    address = event & 0x0f;

    if (status == DSCP_MESSAGE_STATUS_DOWN) {
        if (type == DSCP_MESSAGE_TYPE_NUMPAD) {
            switch(address) {
                case 1: dspcontroller_assembler_add_number(7); break;
                case 2: dspcontroller_assembler_add_number(8); break;
                case 3: dspcontroller_assembler_add_number(9); break;
                case 4: dspcontroller_assembler_add_number(4); break;
                case 5: dspcontroller_assembler_add_number(5); break;
                case 6: dspcontroller_assembler_add_number(6); break;
                case 7: dspcontroller_assembler_add_number(1); break;
                case 8: dspcontroller_assembler_add_number(2); break;
                case 9: dspcontroller_assembler_add_number(3); break;
                case 10: dspcontroller_assembler_add_sign(); break;
                case 11: dspcontroller_assembler_add_number(0); break;
                case 12: dspcontroller_assembler_add_decimal_point(); break;
            }
        } else {
            DSPController_assembler_finish_number();
        }
    }
}

DSPC_CONV_TYPE dspcontroller_assembler_convert() {
    unsigned char i;
    DSPC_CONV_TYPE buff = 0.0f;
    if (dspcontroller_assembler_state == DSPC_ASSEMBLER_STATE_IDLE ||
        dspcontroller_assembler_state == DSPC_ASSEMBLER_STATE_OFF) {
        return buff;
    } else {
        if (dspcontroller_assembler_dp>0) {
            DSPC_CONV_TYPE mult = 1.0f;
            i = dspcontroller_assembler_dp-1;
            do {
                buff += dspcontroller_assembler_buffer[i]*mult;
                mult *= 10.0f;
            } while(i--);
        }
        
    }
    if (dspcontroller_assembler_state == DSPC_ASSEMBLER_STATE_FLOAT) {
        DSPC_CONV_TYPE mult = 0.1f;
        for(i=dspcontroller_assembler_dp+1;i<dspcontroller_assembler_ep;i++) {
            buff += dspcontroller_assembler_buffer[i]*mult;
            mult *= 0.1f;
        }
    }
    return dspcontroller_assembler_sign*buff;
}

void dspcontroller_assembler_add_number(unsigned char n) {
    if (dspcontroller_assembler_state == DSPC_ASSEMBLER_STATE_OFF) return;
    if (dspcontroller_assembler_ep == 31) return;
    
    if (dspcontroller_assembler_ep == 0 && n == 0) {
        return;
    }
    
    if (dspcontroller_assembler_state == DSPC_ASSEMBLER_STATE_DECIMAL
    	&& dspcontroller_assembler_wp >= 7) return;
    
    if (dspcontroller_assembler_state == DSPC_ASSEMBLER_STATE_IDLE) {
        dspcontroller_assembler_state = DSPC_ASSEMBLER_STATE_DECIMAL;
    }
    dspcontroller_assembler_buffer[dspcontroller_assembler_wp++] = n;
    if (dspcontroller_assembler_state == DSPC_ASSEMBLER_STATE_DECIMAL) {
        dspcontroller_assembler_dp++;
    }
    dspcontroller_assembler_ep++;
}

void dspcontroller_assembler_add_decimal_point() {
    if (dspcontroller_assembler_state == DSPC_ASSEMBLER_STATE_OFF) return;
    if (dspcontroller_assembler_state == DSPC_ASSEMBLER_STATE_FLOAT) return;
    if (dspcontroller_assembler_ep == 31) return;
    
    dspcontroller_assembler_state = DSPC_ASSEMBLER_STATE_FLOAT;
    dspcontroller_assembler_wp++;
    dspcontroller_assembler_ep = dspcontroller_assembler_wp;
}

void dspcontroller_assembler_add_sign() {
    if (dspcontroller_assembler_state == DSPC_ASSEMBLER_STATE_OFF) return;
    dspcontroller_assembler_sign = -dspcontroller_assembler_sign;
}

//========================================================================
//  A P I   F U N C T I O N S
//========================================================================
void DSPController_assembler_engage() {
    dspcontroller_assembler_state = DSPC_ASSEMBLER_STATE_IDLE;
}

DSPC_CONV_TYPE DSPController_assembler_get_last() {
    return dspcontroller_last_conversion;
}

DSPC_CONV_TYPE DSPController_assembler_get_current() {
    return dspcontroller_assembler_convert();
}

DSPC_CONV_TYPE DSPController_assembler_finish_number() {
    dspcontroller_last_conversion = dspcontroller_assembler_convert();
    DSPController_assembler_reset();
    dspcontroller_assembler_new_number = 1;
    return dspcontroller_last_conversion;
}

unsigned char DSPController_assembler_is_new_number() {
    unsigned char temp = dspcontroller_assembler_new_number;
    dspcontroller_assembler_new_number = 0;
    return temp;
}

#endif // _ADD_ASSEMBLER


#ifdef _ADD_FLOAT_TO_STRING_CONVERTER
//========================================================================
//          F L O A T   T O   S T R I N G   C O N V E R T E R 
//========================================================================
char* DSPC_FTS(DSPC_CONV_TYPE f, unsigned char precision) {
    unsigned char local_index = 0;
    int integer_part;
    int i,integer_limit;
    int div_factor;
    
    // roll buffer index
    if (++dspcontroller_float_to_char_index > 5) {
        dspcontroller_float_to_char_index = 0;
    }
    
    // get integer part
    integer_part = (int)f;
    
    // handle negative sign
    if (f < 0) {
        dspcontroller_float_to_char_buffer[dspcontroller_float_to_char_index][local_index++] = '-';
        integer_part = -integer_part;
        f = -f;
    }
    
    // prepare fraction part
    f = f-integer_part;
    
    
    
    // set up the max div factor to save cpu clock
    if (integer_part >= 1000000) {
        div_factor = 1000000;
        integer_limit = 7;
    } else  if (integer_part >= 100000) {
        div_factor = 100000;
        integer_limit = 6;
    } else  if (integer_part >= 10000) {
        div_factor = 10000;
        integer_limit = 5;
    } else  if (integer_part >= 1000) {
        div_factor = 1000;
        integer_limit = 4;
    } else  if (integer_part >= 100) {
        div_factor = 100;
        integer_limit = 3;
    } else  if (integer_part >= 10) {
        div_factor = 10;
        integer_limit = 2;
    } else if (integer_part > 0) {
        dspcontroller_float_to_char_buffer[dspcontroller_float_to_char_index][local_index++] = integer_part + 48;
        div_factor = 0;
        integer_limit = 0;
    } else {
        dspcontroller_float_to_char_buffer[dspcontroller_float_to_char_index][local_index++] = 0 + 48;
        div_factor = 0;
        integer_limit = 0;
    }
    
    // convert integer part if needed
    for (i=0; i<integer_limit; i++) {
        int t = integer_part / div_factor;
        
        dspcontroller_float_to_char_buffer[dspcontroller_float_to_char_index][local_index++] = t + 48;
        integer_part -= t*div_factor;
            
        div_factor *= 0.1;
    }
    
    // convert fraction part with the given precision
    dspcontroller_float_to_char_buffer[dspcontroller_float_to_char_index][local_index++] = '.';
    for (i=0; i<precision; i++) {
        int t;
        f *= 10;
        t = (int)f;
        f -= t;
        dspcontroller_float_to_char_buffer[dspcontroller_float_to_char_index][local_index++] = t + 48;
        if (local_index == 15) break;
    }
    
    // add termination zero and retrun the buffer
    dspcontroller_float_to_char_buffer[dspcontroller_float_to_char_index][local_index++] = 0;
    return dspcontroller_float_to_char_buffer[dspcontroller_float_to_char_index];
}
#endif // _ADD_FLOAT_TO_STRING_CONVERTER


