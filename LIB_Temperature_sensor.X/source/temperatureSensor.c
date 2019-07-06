/* 
 * File:   temperatureSensor.c
 * Author: Mocan Marin Dorin
 * 
 * PROJECT:TEMPERATURE SENSOR LIB
 * Created on 15 iunie 2019, 12:55
 */

#include <xc.h>
#include <stdio.h>
#include <math.h>   //pow(x,y))


#include "display3.h"
#include "temperatureSensor.h"



#define ADC_StartConversion()   ADCON0bits.GO=1
#define ADC_StartModule()       ADCON0bits.ADON=1
#define ADC_StopModule()       ADCON0bits.ADON=0


#define _XTAL_FREQ 8000000



//extern char* buffer; //the buffer from the display3.c
unsigned count_sec_OLD=0;


typedef struct{
    union {
        unsigned int all;
        struct{
            unsigned char LOW       :8;
            unsigned char HIGH      :8;
        };
    };
    char buffer[4];
    unsigned char isUpdated;    //reflect if the buffer was updated
} ADC_results_t;
ADC_results_t ADC_results;

ThermistorData_t thermistor;         //the thermistor parameters for 



void ADC_setup(){
    ADCON0bits.CHS=0b0000;  //select AN0 as Analog in
    ADCON0bits.GODONE=0;    //no convertion for now
    ADCON0bits.ADON=0;       //ADC is off
    
    ADCON1bits.VCFG1=0;      //reference is vss
    ADCON1bits.VCFG0=0;     //V+ ref is VDD
    ADCON1bits.PCFG=0x0E;   //select AN0 as analog in, others are digital
    
    ADCON2bits.ADFM=1;      //1= RIGHT Justified; 0 =left 
    ADCON2bits.ACQT=0x04;    //     0,7 <= TAD < 2,5 us. for For FOSC=8Mhz :
    ADCON2bits.ADCS=0x01;    //     Fosc/8= 8Tosc= 1TAD =1us;
    
    //define TAD
    ADCON2bits.ADCS=0x01;   //Fosc/8 or 8 Tosc
    //define Tacq
    ADCON2bits.ACQT=0x02;   //4 TAD
    __delay_us(3);          //the acq Time. just to be sure
    
    TRISAbits.RA0=1;        //AN0 is set to be an INPUT.
    
    /////////////////////////////
    // interruption registers and stuff
    PIR1bits.ADIF=0;        //clear the AD interr. flag
    PIE1bits.ADIE=1 ;       //activate the AD interrupts (0= no AD interrupt) 
    INTCONbits.GIE=1;       //global interr.    
}



const char Ohm2C(unsigned const x){
//using Steinhart-Hart equation 

    float logR=log(x);    
    return 1.0f/(
            thermistor.A + 
            thermistor.B*logR + 
            thermistor.C*logR*logR*logR)- KELVIN
            ;
}


char* update_ADC_results(){
    double Vout=(double)thermistor.Vin/1024 * ADC_results.all;
    unsigned Rt=thermistor.R1*Vout/(thermistor.Vin-Vout);             //Rt->the Termistor in ohms

    snprintf(ADC_results.buffer,4,"%d",Ohm2C(Rt));

    return ADC_results.buffer;
    
    
}

inline void ADC_Read(){    //will be used in ISR
    ADC_results.LOW=ADRESL;
    ADC_results.HIGH=ADRESH;
    ADC_results.isUpdated=0;    //new RAW data is avaible for update_ADC_results())
}

char* temperature(){
    if(ADC_results.isUpdated){
        return ADC_results.buffer;
    }
    ADC_results.isUpdated=1;
    return update_ADC_results();
}

void sensorRead_ISR(){
    extern void counting_ISR(); //display.c
    extern unsigned count_sec;  //display.c. This var holds the seconds

    
    counting_ISR();             //call the external function
    
    // * if one sec has passed and ADC finished
    // and the AD conversion flag is set (conversion finished), then:
    if( count_sec_OLD!=count_sec && ADIF==1){
        count_sec_OLD=count_sec; ADIF=0;    //reset flags
        ADC_Read();                         //read into ADC_results
        ADC_StartConversion();              //start another conversion 
    }
}
    
    
void setupTemperatureSensor(){
    ptr_ISR=sensorRead_ISR; //the interuption will call this function
    
    setup_TMR0();  //Interupts for refresh rate
               
    ADC_setup();  
            //setup the AD converter and start the fisrt conversion
            // * NOTE: both TMR0 and ADC interrupts are active from
            // * setup_TMR0() and ADC_setup()

    
    ADC_StartModule();  __delay_us(3);              
    ADC_StartConversion();
}

