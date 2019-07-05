/*
 * 1)  You will need to initialise the thermistor
 * before using the temperature() func.
 *  You will need the so called A, B, C parameters for the 
 * Steinhart-Hart equation from datasheet, or calculate them. EX:
 * from: https://rusefi.com/Steinhart-Hart.html  
 * 
 * 2)  Call `setupTemperatureSensor()" in your main function
 * 
 * 3)  Call `temperature()" to get a temperature reading (char[4] null terminated) 
 * 
 * 
 */

#ifndef TEMPERATURESENSOR_H
#define	TEMPERATURESENSOR_H
#define KELVIN 273.15

typedef struct{
    double A,B,C;
    unsigned char Vin;
    unsigned int R1;    
}ThermistorData_t;
extern ThermistorData_t thermistor;



//*this function will read the temperature on AN0 and return a char[4] 
//*using the Steinhart-Hart equation 
char* temperature(void);


/*
 * this function will setup your Analog to Digital Converter.
 * AN0 is an analog input. It uses TMR0 interupts.
*/
void setupTemperatureSensor(void);



#endif	/* TEMPERATURESENSOR_H */

