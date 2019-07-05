/*
 * File:   main.c
 * Author: <YOUR NAME>
 *
 * Created on : <DATE>
 */


#include <xc.h>

#include "config.h"
#include "display3.h"
#include "temperatureSensor.h"


void init_thermistor(){   
    //*using the Steinhart-Hart equation 
    thermistor.A=0.0012910588653905842;
    thermistor.B=0.00020449429894872976;
    thermistor.C=2.4786418838291797e-7;
    
    thermistor.R1=2500;
    thermistor.Vin=5;
    
       //*these values are used to convert the Rezistance of the thersmistor to
    //*Celsius degree
    //*R1 is the series rezistor
    //Vout is connected to AN0 pin
/*        
     ____Vin 
       | 
      _|_  
     |   |
     |   | R1
     |___|
       |
       |
       +-----O  (Vout)   
       |    
       | /\
      _|_/  
     |  /|
     | / | *C THERMISTOR
     |/__|         
     / |      
       |      
       |      
     __|__
      ___
       _
  */  
  //* I got A,B,C from: https://rusefi.com/Steinhart-Hart.html

}

int main(int argc, char** argv) {
    setupPortDisplay();
    setupTemperatureSensor(); 
    init_thermistor();

    
    while(1) {
        display3(temperature());    //display the values in Celsius Degrees
        //
        //do other stuff
        //
    }
    return (EXIT_SUCCESS);
}