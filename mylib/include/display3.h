/* 
 * File:   display3.h
 * Author: Mocan Marin Dorin
 *
 * NOTE: be sure to add the "Display3.c" file into source files (in MPLAB X IDE)
 * this header provides an " interface "
 * for your use in displaying data on a 3x  7 Segment LED Display
 * 
 * NOTE 2: the definition of these function is in Display.c 
 * Created on 15 iunie 2019, 12:28
 */

#ifndef DISPLAY3_H
#define	DISPLAY3_H

#ifdef	__cplusplus
extern "C" {
#endif
    //DEMO FUNCTION
    void numarator(void);           //just call it in your main. WIll not return
    void test_segments(unsigned char); //test each LED separately
    
    
    // GENERAL FUNCTIONS YOU MIGHT USE: 
    void display3(const char*); //Display MAX 3 chars on 7 Segment LED Display
                                //ex: display3("123");   
                                //    display3("oo1"); -> will display "1"
    
    void send_digit(const unsigned char );
                                //This function will send data to your
                                // shift register through "SER" pin
    
    void setupPortDisplay(void);    //will setup your IO ports on uC
    
    
    // Function you might NOT need in your main.c project
    void setup_TMR0(void);           //will setup the timer 0. used by my_ISR
    unsigned char character_table(unsigned char);
    void __interrupt(high_priority) my_ISR(void);
    extern void (*ptr_ISR)(void);   //a function pointer that will be called
                                        //by my_ISR(void)
                                //the Interuption Subroutine Routine



#endif	/* DISPLAY3_H */

