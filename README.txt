-------------------------------------------------------------------------------
Example: C Block-based talkthrough
Target: ADSP-21364
Hardware: ADSP-21364 EZ-KIT LITE
-------------------------------------------------------------------------------
This project contains a talkthrough example using the onboard AD1835 to acquire
and output an audio stream. The digital audio data is available for processing
in the file SPORTisr.c. The block size is 1024 samples.

The AD1835 is set up with a 48 Khz Sampling rate.  The ADC is connected to
SPORT 0A. DAC1 is connected to SPORT 1A, DAC2 to SPORT 1B, DAC3 to SPORT2A, and
DAC4 (Headphone output) to SPORT2B. All channels of the codec are accessed in
I2S mode. See initDAI.c for the details of which DAI pins are used to access the
codec. Use #defines to select the output channel. DAC4 is the only channel
enabled by default.
-------------------------------------------------------------------------------
Instructions: Apply an input signal to J4 on the ADSP-21364 EZ-KIT LITE, and
              attach an output device to the selected channel of J5.
-------------------------------------------------------------------------------
Source Files contained in this directory:
Block Based Talkthrough.dpj      VisualDSP project file
init1835viaSPI.c            ADSP-21364 source - SPI Subroutines
initDAI.c           Set up the DAI to connect to the AD1835
main.c              Main section to call setup routines
initSPORT.c         Initialize the SPORT DMA to communicate with the AD1835
IRQprocess.c        Set up and process IRQ1 and IRQ2 (Pushbutton) interrupts
SPORTisr.c               Process SPORT 0 interrupts
blockProcess.c      Process the audio data in the current block
ad1835.h            Macro Definitions for AD1835 registers
tt.h                Includes and external declarations used for all files
-------------------------------------------------------------------------------
Dependencies contained in VisualDSP++ default include path:
def21364.h          Header file with generic definitions for ADSP-21364
SRU.h               Header file with SRU definitions and Macros
*******************************************************************************
Analog Devices, Inc.
DSP Division
One Technology Way
Norwood, MA 02062

(c) 2004 Analog Devices, Inc.  All rights reserved.
*******************************************************************************
