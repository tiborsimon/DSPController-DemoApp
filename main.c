//========================================================================//
//                            D E M O   A P P                             //
//========================================================================//
//                                                                        //
//  This project demonstrates a simple audio application that uses the    //
//  DSPController API to achieve the control over the DSP algotithm.      //
//                                                                        //
//------------------------------------------------------------------------//
//                                                                        //
//  Created by Tibor Simon <tiborsimon90@gmail.com>                       //
//  2013.12.10.                                                           //
//                                                                        //
//  BME VIK MIT DSP Laboratory                                            //
//                                                                        //
//========================================================================//

#include "tt.h"

void SetupIRQ12(void) ;

void main(void)
{
    
	INIT_SHARC;
    
    initInterface();
    
    while(1) {
        updateInterface();   
    }
    
}
