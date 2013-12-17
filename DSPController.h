/*! \mainpage
 *
 * # DSP Controller HOST firmware
 * \author  Tibor Simon <tiborsimon@tibor-simon.com>
 * \version 1.0
 * 
 */

#ifndef _DSPCONTROLLER_H_
#define _DSPCONTROLLER_H_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <SRU21364.h>


#define _ADD_ASSEMBLER
#define _ADD_FLOAT_TO_STRING_CONVERTER



//========================================================================
//------------------------------------------------------------------------
//            D S P   C O N T R O L L E R   C O R E   A P I
//------------------------------------------------------------------------
//========================================================================

//========================================================================
//  T Y P E   D E F I N I T I O N S
//========================================================================

typedef unsigned char 	Event;
typedef unsigned char	DIP;
typedef signed short	Encoder;
typedef unsigned char   Led;


//========================================================================
//  S E T T I N G S   S Y M B O L S
//========================================================================

#define FS_48KHZ									0x01
#define FS_96KHZ									0x00

#define ENCODER_VELOCITY_ON							0x02
#define ENCODER_VELOCITY_OFF						0x00

//========================================================================
//  V A R I A B L E   P A R A M E T E R S
//========================================================================
#define DSPC_TICK_THRESHOLD_48                  26   /* @48kHz */
#define DSPC_TICK_THRESHOLD_96                  52   /* @96kHz */

#define DSPC_LED_COUNTER_MAX_48                 4
#define DSPC_LCD_COUNTER_MAX_48                 100
#define DSPC_DIP_COUNTER_MAX_48                 20

#define DSPC_LED_COUNTER_MAX_96                 10
#define DSPC_LCD_COUNTER_MAX_96                 200
#define DSPC_DIP_COUNTER_MAX_96                 50


#define DSPC_EVENT_BUFFER_SIZE                  32

#define DSPC_CONV_TYPE                          float


#define DSPC_ENCODER_VELOCITY_THRESHOLD			300
#define DSPC_ENCODER_VELOCITY_MULTIPLIER		3

#define DSPC_LCD_REFRESH_RATE					200


//========================================================================
//------------------------------------------------------------------------
//                              E V E N T S
//------------------------------------------------------------------------
//========================================================================

#define DSPC_EVENT_NOTHING		0               /**<  */

//========================================================================
//  N U M E R I C   E V E N T S
//========================================================================
#define DSPC_EVENT_N1_DOWN		1
#define DSPC_EVENT_N1_UP		2
#define DSPC_EVENT_N1_SHORT		3
#define DSPC_EVENT_N1_LONG		4

#define DSPC_EVENT_N2_DOWN		5
#define DSPC_EVENT_N2_UP		6
#define DSPC_EVENT_N2_SHORT		7
#define DSPC_EVENT_N2_LONG		8

#define DSPC_EVENT_N3_DOWN		9
#define DSPC_EVENT_N3_UP		10
#define DSPC_EVENT_N3_SHORT		11
#define DSPC_EVENT_N3_LONG		12

#define DSPC_EVENT_N4_DOWN		13
#define DSPC_EVENT_N4_UP		14
#define DSPC_EVENT_N4_SHORT		15
#define DSPC_EVENT_N4_LONG		16

#define DSPC_EVENT_N5_DOWN		17
#define DSPC_EVENT_N5_UP		18
#define DSPC_EVENT_N5_SHORT		19
#define DSPC_EVENT_N5_LONG		20

#define DSPC_EVENT_N6_DOWN		21
#define DSPC_EVENT_N6_UP		22
#define DSPC_EVENT_N6_SHORT		23
#define DSPC_EVENT_N6_LONG		24

#define DSPC_EVENT_N7_DOWN		25
#define DSPC_EVENT_N7_UP		26
#define DSPC_EVENT_N7_SHORT		27
#define DSPC_EVENT_N7_LONG		28

#define DSPC_EVENT_N8_DOWN		29
#define DSPC_EVENT_N8_UP		30
#define DSPC_EVENT_N8_SHORT		31
#define DSPC_EVENT_N8_LONG		32

#define DSPC_EVENT_N9_DOWN		33
#define DSPC_EVENT_N9_UP		34
#define DSPC_EVENT_N9_SHORT		35
#define DSPC_EVENT_N9_LONG		36

#define DSPC_EVENT_N10_DOWN		37
#define DSPC_EVENT_N10_UP		38
#define DSPC_EVENT_N10_SHORT	39
#define DSPC_EVENT_N10_LONG		40

#define DSPC_EVENT_N11_DOWN		41
#define DSPC_EVENT_N11_UP		42
#define DSPC_EVENT_N11_SHORT	43
#define DSPC_EVENT_N11_LONG		44

#define DSPC_EVENT_N12_DOWN		45
#define DSPC_EVENT_N12_UP		46
#define DSPC_EVENT_N12_SHORT	47
#define DSPC_EVENT_N12_LONG		48


//========================================================================
//  F U N C T I O N   E V E N T S
//========================================================================
#define DSPC_EVENT_F1_DOWN		49
#define DSPC_EVENT_F1_UP		50
#define DSPC_EVENT_F1_SHORT		51
#define DSPC_EVENT_F1_LONG		52

#define DSPC_EVENT_F2_DOWN		53
#define DSPC_EVENT_F2_UP		54
#define DSPC_EVENT_F2_SHORT		55
#define DSPC_EVENT_F2_LONG		56

#define DSPC_EVENT_F3_DOWN		57
#define DSPC_EVENT_F3_UP		58
#define DSPC_EVENT_F3_SHORT		59
#define DSPC_EVENT_F3_LONG		60

#define DSPC_EVENT_F4_DOWN		61
#define DSPC_EVENT_F4_UP		62
#define DSPC_EVENT_F4_SHORT		63
#define DSPC_EVENT_F4_LONG		64


//========================================================================
//  A R R O W   E V E N T S
//========================================================================
#define DSPC_EVENT_A1_DOWN		65
#define DSPC_EVENT_A1_UP		66
#define DSPC_EVENT_A1_SHORT		67
#define DSPC_EVENT_A1_LONG		68

#define DSPC_EVENT_A2_DOWN		69
#define DSPC_EVENT_A2_UP		70
#define DSPC_EVENT_A2_SHORT		71
#define DSPC_EVENT_A2_LONG		72

#define DSPC_EVENT_A3_DOWN		73
#define DSPC_EVENT_A3_UP		74
#define DSPC_EVENT_A3_SHORT		75
#define DSPC_EVENT_A3_LONG		76

#define DSPC_EVENT_A4_DOWN		77
#define DSPC_EVENT_A4_UP		78
#define DSPC_EVENT_A4_SHORT		79
#define DSPC_EVENT_A4_LONG		80

#define DSPC_EVENT_A5_DOWN		81
#define DSPC_EVENT_A5_UP		82
#define DSPC_EVENT_A5_SHORT		83
#define DSPC_EVENT_A5_LONG		84


//========================================================================
//  E N C O D E R   E V E N T S
//========================================================================
#define DSPC_EVENT_E1_DOWN		85
#define DSPC_EVENT_E1_UP		86
#define DSPC_EVENT_E1_SHORT		87
#define DSPC_EVENT_E1_LONG		88

#define DSPC_EVENT_E2_DOWN		89
#define DSPC_EVENT_E2_UP		90
#define DSPC_EVENT_E2_SHORT		91
#define DSPC_EVENT_E2_LONG		92

#define DSPC_EVENT_E3_DOWN		93
#define DSPC_EVENT_E3_UP		94
#define DSPC_EVENT_E3_SHORT		95
#define DSPC_EVENT_E3_LONG		96


//========================================================================
//  S P I   S T A T E   M A C H I N E   S T A T E S
//========================================================================
#define DSPC_STATE_SPI_BOOT                     1
#define DSPC_STATE_SPI_BOOT_SENT                2

#define DSPC_STATE_SPI_IDLE                     3

#define DSPC_STATE_SPI_SIMPLE_INIT_SENT         4
#define DSPC_STATE_SPI_LED_INIT_SENT            5
#define DSPC_STATE_SPI_LCD_TOP_INIT_SENT        6
#define DSPC_STATE_SPI_LCD_BOTTOM_INIT_SENT     7
#define DSPC_STATE_SPI_DIP_INIT_SENT            8

#define DSPC_STATE_SPI_SIMPLE_SENT              9
#define DSPC_STATE_SPI_LED_SENT                 10
#define DSPC_STATE_SPI_LCD_TOP_SENT             11
#define DSPC_STATE_SPI_LCD_BOTTOM_SENT          12
#define DSPC_STATE_SPI_DIP_SENT                 13


//========================================================================
//  M E S S A G E S
//========================================================================
#define DSCP_MESSAGE_GET_SIMPLE             	0x10
#define DSCP_MESSAGE_GET_WITH_LED           	0x11
#define DSCP_MESSAGE_GET_WITH_LCD_TOP       	0x12
#define DSCP_MESSAGE_GET_WITH_LCD_BOTTOM    	0x13
#define DSCP_MESSAGE_GET_DIP_STATUS         	0x14


#define DSCP_MESSAGE_TYPE_NUMPAD            	0
#define DSCP_MESSAGE_TYPE_F_AND_A           	1
#define DSCP_MESSAGE_TYPE_ENCODER           	2

#define DSCP_MESSAGE_STATUS_DOWN            	0
#define DSCP_MESSAGE_STATUS_UP              	1
#define DSCP_MESSAGE_STATUS_SHORT           	2
#define DSCP_MESSAGE_STATUS_LONG            	3



//========================================================================
//  D S P   D E P E N D E N T   D E F I N E S  (ADSP-21364) 
//========================================================================
#define DSPC_SHARC_SPICTL  (0x1000)
#define DSPC_SHARC_SPIFLG  (0x1001)
#define DSPC_SHARC_SPISTAT (0x1002)
#define DSPC_SHARC_SPIBAUD (0x1005)
#define DSPC_SHARC_TXSPI   (0x1003)
#define DSPC_SHARC_RXSPI   (0x1004)

// SPICTL bits
#define DSPC_SHARC_TIMOD   (0x0001)  /* Initiate transfer by writing to the tx register. */
#define DSPC_SHARC_MSBF    (0x0200)  /* MSB first */
#define DSPC_SHARC_SPIMS   (0x1000)  /* SPI Master if 1, Slave if 0  */
#define DSPC_SHARC_SPIEN   (0x4000)  /* SPI enable */
#define DSPC_SHARC_GM	   (0x0008)  /* GM - overwrite wit hnew data if rx is full */


//========================================================================
//------------------------------------------------------------------------
//                          F U N C T I O N S
//------------------------------------------------------------------------
//========================================================================

//========================================================================
//  P L A T F O R M   D E P E N D E N T   F U N C T I O N S 
//========================================================================
void dspcontroller_spi_init(void);

#define dspcontroller_spi_send_data(data)   {*(volatile int *)DSPC_SHARC_TXSPI = data;}
#define dspcontroller_spi_read_data()       (*(volatile int *)DSPC_SHARC_RXSPI)
#define dspcontroller_spi_check()           (*(volatile int *)DSPC_SHARC_SPISTAT & 1)
#define dspcontroller_select_device()       SRU(LOW,DAI_PB15_I)
#define dspcontroller_deselect_device()     SRU(HIGH,DAI_PB15_I)

//========================================================================
//  L O W   L E V E L   A P I   F U N C T I O N S 
//========================================================================
void dspcontroller_init(void);
void dspcontroller_process_event(unsigned char event);
void dspcontroller_add_event_to_buffer(unsigned char event);
void dspcontroller_lcd_handler(unsigned char line, const char* format, va_list args);

//========================================================================
//  C O R E   A P I   F U N C T I O N S 
//========================================================================
void DSPController_init_default(void);
void DSPController_init(int code);
void DSPController_tick(void);
void DSPController_flush(void);
void DSPController_led(Led leftLed, Led rightLed);
void DSPController_lcd_top(const char* format, ...);
void DSPController_lcd_bottom(const char* format, ...);
void DSPController_lcd(char line, const char* format, ...);
DIP DSPController_get_dip(void);
Encoder DSPController_get_encoder(char encoder);
Event DSPController_get_event(void);


//========================================================================
//------------------------------------------------------------------------
//                 E N D   O F   T H E   C O R E   A P I  
//------------------------------------------------------------------------
//========================================================================











//========================================================================
//------------------------------------------------------------------------
//                         A P P L I C A T I O N S
//------------------------------------------------------------------------
//========================================================================



#ifdef _ADD_ASSEMBLER
//========================================================================
//
//                          A S S E M B L E R
//
//========================================================================

    #define DSPC_ASSEMBLER_STATE_OFF        0
    #define DSPC_ASSEMBLER_STATE_IDLE       1
    #define DSPC_ASSEMBLER_STATE_DECIMAL    2
    #define DSPC_ASSEMBLER_STATE_FLOAT      3

//========================================================================
//  A S S E M B L E R   V A R I A B L E S
//========================================================================
    unsigned char 		dspcontroller_assembler_wp;
    unsigned char 		dspcontroller_assembler_dp;
    unsigned char 		dspcontroller_assembler_ep;
    unsigned char 		dspcontroller_assembler_state;
    unsigned char 		dspcontroller_assembler_buffer[32];
    signed char 		dspcontroller_assembler_sign;
    unsigned char 		dspcontroller_assembler_new_number;

    DSPC_CONV_TYPE dspcontroller_last_conversion;


//========================================================================
//  A S S E M B L E R   F U N C T I O N S 
//========================================================================
    // API functions
    void DSPController_assembler_reset(void);
    void DSPController_assembler_engage(void);
    DSPC_CONV_TYPE DSPController_assembler_finish_number(void);
    DSPC_CONV_TYPE DSPController_assembler_get_last(void);
    DSPC_CONV_TYPE DSPController_assembler_get_current(void);
    unsigned char DSPController_assembler_is_new_number(void);

    // core functions
    DSPC_CONV_TYPE dspcontroller_assembler_convert(void);
    void dspcontroller_assembler_init(void);
    void dspcontroller_assembler_add_number(unsigned char n);
    void dspcontroller_assembler_add_decimal_point(void);
    void dspcontroller_assembler_add_sign(void);
    void dspcontroller_assembler_process_event(unsigned char event);
#endif // _ADD_ASSEMBLER



#ifdef _ADD_FLOAT_TO_STRING_CONVERTER
//========================================================================
//
//                      F L O A T I N G   P O I N T   
//                 T O   S T R I N G   C O N V E R T E R
//
//========================================================================

//========================================================================
//  C O N V E R T E R   V A R I A B L E S
//========================================================================
    char 		dspcontroller_float_to_char_buffer[6][17];
    unsigned char 		dspcontroller_float_to_char_index;

//========================================================================
//  C O N V E R T E R   F U N C T I O N S 
//========================================================================
    char* DSPC_FTS(DSPC_CONV_TYPE f, unsigned char precision);
#endif // _ADD_FLOAT_TO_STRING_CONVERTER


#endif // _DSPCONTROLLER_H_

