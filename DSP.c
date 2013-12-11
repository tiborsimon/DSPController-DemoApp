#include "GLUE.h"

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

void process(void) {

    //====================================================================
    // PREPARE THE WORKING VARIABLES
    //====================================================================
    float x  = (inLeft+inRight)*0.5f;
    float y = 0;


    //====================================================================
    // DSP ALGORITHM
    //====================================================================
    


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
