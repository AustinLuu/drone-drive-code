
/*
 * 
 * Library for Mechbot Shield functions for 
 * Ryerson University, Department of Mechanical and Industrial Engineering.
 * 
 * Mechbot Shield is compatible with Arduino Uno or equivalent boards
 * 
 * Written by: Jimmy Tran
 * Last Edited: August 24, 2015
 * 
 * Files: mechbotShield.c, mechbotShield.h 
 * 
 * 
 */


#ifndef _mechbotShield_included_h_
#define _mechbotShiled_included_h_

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

void delay_ms (uint16_t ms);
void delay_us (uint16_t us) ;

/****LCD/Software UART Definitions****/
#define BAUD 9600
#define STOP_BITS 1

#define SOFT_TX_BIT PD2
#define SOFT_TX_DDR DDRD
#define SOFT_TX_PORT PORTD

#define MICROSECONDS_OVERHEAD_ADJUST 0

#define MICROSECONDS_PER_BIT ((1000000ul / BAUD) - MICROSECONDS_OVERHEAD_ADJUST)
/**END of LCD/Software UART Definitions**/

/*****LCD/Software UART Functions*****/
void initUART(unsigned int baud);
void uartPutChar (unsigned char c);
unsigned char uartGetChar ( void );
void printDec(int num);
void printByte(uint8_t num);
void print(char *msg);
/**************************************/
/**END of LCD/Software UART Functions**/


/*****LCD/Software UART Functions*****/
void initSoftSerial(void);
void softPutchar (char c);
void lcdPrint(char *msg);
void clrLCD(void);
void moveLCDCursor(uint8_t position);
void setLCDBackLight(uint8_t brightness);
void lcdPrintByte(uint8_t num);
void lcdPrintDec( int16_t num);
/**************************************/
/**END of LCD/Software UART Functions**/


/****Analog to Digital Functions****/
void initADC(void);
uint16_t analog(uint8_t channel);
/**************************************/
/**END of Analog to Digital Functions**/


/*****Motor Functions*****/
void initMotor (void);
void disableMotor (void);
void motor (int16_t leftMotor, int16_t rightMotor);
/**************************/
/**End of Motor Functions**/



 
/****Buzzer Notes Definitions****/
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
/***********************************/
/**End of Buzzer Notes Definitions**/


/*****Buzzer Song Functions*****/
void mute(void) ;
void beep (uint16_t noteFrequency, uint32_t noteDuration);
void playShaveAndHaircut (void);
void playSuperMario (void);
void playSuperMarioUnderworld (void);
/********************************/
/**End of Buzzer Song Functions**/

		




#endif
