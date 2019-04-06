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

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "mechbotShield.h"



/*****Delay Functions*****/
void delay_ms (uint16_t ms) {
	uint16_t i;
	for (i = 0; i < ms; i++)
		_delay_ms(1);
		
}
void delay_us (uint16_t us) {
	uint16_t i;
	
	for (i = 0; i < us; i++)
		_delay_us(1);
		
}
/**************************/
/**END of Delay Functions**/


/*****Hardware UART Functions*****/
/*
  Initialize settings for uart functions.
  Must be done once at the beginning of the program.
*/ 

void initUART(unsigned int baud) {
	unsigned int ubrr = F_CPU/8/baud-1; 	//Double Speed UBRR formula 
   
    //unsigned int ubrr = F_CPU/16/baud-1; 	//Normal Speed UBRR formula 
	
	UBRR0H = (unsigned char) (ubrr >> 8); 	// shift MSB and store in UBRR0H 

	UBRR0L = (unsigned char) ubrr; 	// store LSB in UBRR0L 

	UCSR0A = (1 << U2X0); //0b00000010; 	//Set Double Speed Operation (0 for Normal mode)	
	
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); 	// Enable transmitter/receiver 

	//UCSR0C = (1 <<UCSZ00) | (1 <<UCSZ01);  	//8-Bit Characters, 0 Stop bits, No parity
	
	UCSR0C =  (0 << USBS0)| (1 <<UCSZ00) | (1 <<UCSZ01);  	//8-Bit Characters, 0 Stop bits, No parity
}

/*
   Write byte to UART
*/ 

void uartPutChar (unsigned char data) {
	while ( !(UCSR0A & (1 << UDRE0)) );
	// Wait for empty transmit buffer 

	UDR0 = data;
	// Start transmission by writing to UDR0 register
} 

/*
   Read byte from UART
*/ 

unsigned char uartGetChar ( void ) {
	while ( !(UCSR0A &  (1 << RXC0)) );     
	// Wait for incoming byte 

	return UDR0;
	// Return the byte
}

/*
	Convert a 4 digit decimal and output the string of it to serial
*/
void printDec( int16_t num) {
	uint16_t a, b, c, d;

	if (num < 0) {
		uartPutChar('-');
		num = -num;
	}
	else
		uartPutChar(' ');

	if (num <= 9999)
	{
		a=num/1000;
		uartPutChar((a+0x30));

		b=num%1000;
		uartPutChar(((b/100)+0x30));

		c=b%100;
		uartPutChar(((c/10)+0x30));

		d=c%10;
		uartPutChar((d+0x30));
	}
	else
	{
		uartPutChar('e');
		uartPutChar('r');
		uartPutChar('r');
		uartPutChar('o');
		uartPutChar('r');
	}

}

void printByte(uint8_t num) {
	uint8_t i;
	
	for (i=0; i<8; i++) {
		if (num & (0x80 >> i)) {
			uartPutChar('1');
		}
		else {
			uartPutChar('0');
		}
	}
}
/**********************************/
/**END of Hardware UART Functions**/



/*****LCD/Software UART Functions*****/
void initSoftSerial(void) {
  SOFT_TX_PORT |= (1<<SOFT_TX_BIT);
  SOFT_TX_DDR |= (1<<SOFT_TX_BIT);
}

void softPutchar (char c) {
  uint8_t  bit_mask;

  // start bit
  SOFT_TX_PORT &= ~(1<<SOFT_TX_BIT);
  _delay_us(MICROSECONDS_PER_BIT);

  // data bits
  for (bit_mask=0x01; bit_mask; bit_mask<<=1) {
    if (c & bit_mask) {
      SOFT_TX_PORT |= (1<<SOFT_TX_BIT);
    }
    else {
      SOFT_TX_PORT &= ~(1<<SOFT_TX_BIT);
    }
    _delay_us(MICROSECONDS_PER_BIT);
  }

  // stop bit(s)
  SOFT_TX_PORT |= (1<<SOFT_TX_BIT);
  _delay_us(MICROSECONDS_PER_BIT * STOP_BITS);

}

void lcdPrint(char *msg){
	uint8_t i;
	i = 0;
	while(msg[i] != '\0')
		softPutchar(msg[i++]);

}

/* Clears LCD Screen */
void clrLCD(void) {
	softPutchar(0xFE); // move cursor to beginning of first line
	softPutchar(0x01);
}

/* Moves the cursor to the 32 (16x2) positions on the LCD */
void moveLCDCursor(uint8_t position) {
	softPutchar(0xFE); // move cursor to beginning of first line
	softPutchar(0x80);
	softPutchar(position);
}

/* Sets the brightness value of the LCD (0-255)*/
void setLCDBackLight(uint8_t brightness) {
	softPutchar(0x80); //Change back light
	softPutchar(brightness);
	//delay_ms(10);
}

/* Displays the binary representations of a byte on LCD */
void lcdPrintByte(uint8_t num) {
	uint8_t i;
	
	for (i=0; i<8; i++) {
		if (num & (0x80 >> i)) {
			softPutchar('1');
		}
		else {
			softPutchar('0');
		}
	}
}

/* Converts a signed 4 digit decimal (-9999 to 9999) and displays the string on LCD */
void lcdPrintDec( int16_t num) {
	uint16_t a, b, c, d;

	if (num < 0) {
		softPutchar('-');
		num = -num;
	}
	else
		softPutchar(' ');

	if (num <= 9999)
	{
		a=num/1000;
		softPutchar((a+0x30));

		b=num%1000;
		softPutchar(((b/100)+0x30));

		c=b%100;
		softPutchar(((c/10)+0x30));

		d=c%10;
		softPutchar((d+0x30));
	}
	else
	{
		softPutchar('e');
		softPutchar('r');
		softPutchar('r');
		softPutchar('o');
		softPutchar('r');
	}

}
/**************************************/
/**END of LCD/Software UART Functions**/



/****Analog to Digital Functions****/
/* Initializes ADC parameters */
void initADC(void) {
	ADCSRA |= (1 << ADEN); //enable ADC
	ADMUX |= (1 << REFS0); // Vref AVcc page 255
   //ADMUX |= (1 << ADLAR); // Left justified output for 8bit mode  
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1); //Set prescale to 8 (8000000/64 = 125Khz)
	ADCSRA |= (1 << ADSC); // start conversion 1st time will take 25 cycles
	DDRB |= (1<<PB3) | (1<<PB4) | (1<<PB5);
}

/*	Analog to digital function 10-bit resolution */
uint16_t analog( uint8_t channel )
{
	ADMUX  &=  0xF0; //Clearing the last 4 bits of ADMUX

   ADCSRA |=  ( 1 << ADSC  );
   //Select channel
	PORTB &= 0b11000111; //Clear PB3-PB5
	PORTB |= (channel << 3); //Set PB3-PB5 with channel value
    //wait for complete conversion and return the result 
   while ( ADCSRA & ( 1 << ADSC ) );

   return ADC;
}
/**************************************/
/**END of Analog to Digital Functions**/



/*****Motor Functions*****/
/* Initialize Timer1 for driving motors */
void initMotor (void) {
	// set up counter 1 (16 bit) to act as a dual channel PWM generator
	// we want OC1A and B to be set on reaching BOTTOM, clear on reaching
	// compare match, use ICR1 as TOP and have a prescale of 8.
	TCCR1A = (1 << COM1A1) // set OC1A/B at TOP
		   | (1 << COM1B1) // clear OC1A/B when match
		   | (1 << WGM11); // mode 14 (fast PWM, clear TCNT1 on match ICR1)
	TCCR1B = (1 << WGM13)
		   | (1 << WGM12)
		   | (1 << CS11); // timer uses main system clock with 1/8 prescale
	
	ICR1 = 1200; // used for TOP, makes for 1.67KHz
	
	OCR1A = 0; // Motor init to 0
	OCR1B = 0; // Motor init to 0
	
	// Set up pins as outputs
	DDRD |= (1 << PD7);
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);
	

}

/* Disable motor function and return normal port operation */
void disableMotor (void) {
	TCCR1A &= ~((1 << COM1A1) | (1 << COM1B1));
}

/* Drives left and right motors with speed of -1000 to 1000 */
void motor (int16_t leftMotor, int16_t rightMotor) {
	//Left Motor: Enable=PB0; Direction=PD7
	if(leftMotor < 0) {
		
		if (leftMotor < -1000)
			leftMotor = -1000;
		PORTD |= (1 << PD7);
		OCR1A = (uint16_t)(-leftMotor);
	}
	else if (leftMotor > 0) {
		if (leftMotor > 1000)
			leftMotor = 1000;

		PORTD &= ~(1 << PD7);
		OCR1A = (uint16_t)leftMotor;
	}
	else {
		OCR1A = 0;
	}	
	
	//Right Motor: Enable=PB2; Direction=PB0
	if(rightMotor < 0) {
		if (rightMotor < -1000)
			rightMotor = -1000;
			
		PORTB |= (1 << PB0);
		OCR1B = (uint16_t)(-rightMotor);
	}
	else if (rightMotor > 0) {
		if (rightMotor > 1000)
			rightMotor = 1000;
			
		PORTB &= ~(1 << PB0);
		OCR1B = (uint16_t)rightMotor;
	}
	else {
		OCR1B = 0;
	}
}
/**************************/
/**End of Motor Functions**/



/*****Buzzer Song Functions*****/
/* Mute buzzer */
void mute(void) {
   DDRB |= (1<<PB3);
   PORTB &= ~(1<<PB3);
}

/* Generate tone on buzzer at 'noteFrequency' for 'noteDuration' ( in micro seconds)*/
void beep (uint16_t noteFrequency, uint32_t noteDuration){
   uint16_t pauseBetweenNotes;
   uint32_t i;
   uint32_t uSecondsPerWave,uSecondsPerHalfWave;
   uint32_t repeatWave;
   DDRB |= (1<<PB3);

   // Convert the frequency to microseconds
   uSecondsPerWave = 1000000/noteFrequency;
   uSecondsPerHalfWave = uSecondsPerWave/2;

   repeatWave = noteDuration/uSecondsPerWave;
   for (i=0;i<repeatWave;i++) {
      PORTB |= (1<<PB3);
      delay_us(uSecondsPerHalfWave);
      PORTB &= ~(1<<PB3);
      delay_us(uSecondsPerHalfWave);
   }

   // to distinguish the notes, set a minimum time between them.
   // the note's duration + 30% seems to work well:
   mute();
   pauseBetweenNotes = (noteDuration/1000)+(noteDuration/1000) * 2/10;
   delay_ms(pauseBetweenNotes);
}

void playShaveAndHaircut (void) {
   uint8_t i=0;
   uint16_t shaveHaircut[] = { NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};
   uint8_t shaveHaircutDuration[] = {8, 16, 16, 8,8,8,8,8 };

   uint32_t noteDuration;

   // iterate over the notes of the melody "Shave and a Haircut":
   for (i = 0; i < 8; i++) {
      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000000 / 4, eighth note = 1000000/8, etc.
      noteDuration = 1000000/shaveHaircutDuration[i];
      beep(shaveHaircut[i], noteDuration);

   }
   mute();
}

void playSuperMario (void) {
   uint8_t i=0;
   uint16_t superMario[] =  {
        NOTE_E7, NOTE_E7, 0, NOTE_E7,
        0, NOTE_C7, NOTE_E7, 0,
        NOTE_G7, 0, 0,  0,
        NOTE_G6, 0, 0, 0,

        NOTE_C7, 0, 0, NOTE_G6,
        0, 0, NOTE_E6, 0,
        0, NOTE_A6, 0, NOTE_B6,
        0, NOTE_AS6, NOTE_A6, 0,

        NOTE_G6, NOTE_E7, NOTE_G7,
        NOTE_A7, 0, NOTE_F7, NOTE_G7,
        0, NOTE_E7, 0,NOTE_C7,
        NOTE_D7, NOTE_B6, 0, 0,

        NOTE_C7, 0, 0, NOTE_G6,
        0, 0, NOTE_E6, 0,
        0, NOTE_A6, 0, NOTE_B6,
        0, NOTE_AS6, NOTE_A6, 0,

        NOTE_G6, NOTE_E7, NOTE_G7,
        NOTE_A7, 0, NOTE_F7, NOTE_G7,
        0, NOTE_E7, 0,NOTE_C7,
        NOTE_D7, NOTE_B6, 0, 0
   };

   uint8_t superMarioDuration[] = {
        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,

        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,

        9, 9, 9,
        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,

        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,

        9, 9, 9,
        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,
   };

   uint32_t noteDuration;

   // iterate over the notes of the melody "Shave and a Haircut":
   for (i = 0; i < 78; i++) {
      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000000 / 4, eighth note = 1000000/8, etc.
      noteDuration = 1000000/superMarioDuration[i];
      beep(superMario[i], noteDuration);

   }
   mute();
}

void playSuperMarioUnderworld (void) {
   uint8_t i=0;
   uint16_t superMarioUnderworld[] =  {
      NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
      NOTE_AS3, NOTE_AS4, 0,
      0,
      NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
      NOTE_AS3, NOTE_AS4, 0,
      0,
      NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
      NOTE_DS3, NOTE_DS4, 0,
      0,
      NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
      NOTE_DS3, NOTE_DS4, 0,
      0, NOTE_DS4, NOTE_CS4, NOTE_D4,
      NOTE_CS4, NOTE_DS4,
      NOTE_DS4, NOTE_GS3,
      NOTE_G3, NOTE_CS4,
      NOTE_C4, NOTE_FS4,NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
      NOTE_GS4, NOTE_DS4, NOTE_B3,
      NOTE_AS3, NOTE_A3, NOTE_GS3,
      0, 0, 0
   };

   //Underwolrd tempo
   uint8_t superMarioUnderworldDuration[] = {
		12, 12, 12, 12,
		12, 12, 6,
		3,
		12, 12, 12, 12,
		12, 12, 6,
		3,
		12, 12, 12, 12,
		12, 12, 6,
		3,
		12, 12, 12, 12,
		12, 12, 6,
		6, 18, 18, 18,
		6, 6,
		6, 6,
		6, 6,
		18, 18, 18,18, 18, 18,
		10, 10, 10,
		10, 10, 10,
		3, 3, 3
   };

   uint32_t noteDuration;

   // iterate over the notes of the melody "Shave and a Haircut":
   for (i = 0; i < 56; i++) {
      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000000 / 4, eighth note = 1000000/8, etc.
      noteDuration = 1000000/superMarioUnderworldDuration[i];
      beep(superMarioUnderworld[i], noteDuration);

   }
   mute();
}
/********************************/
/**End of Buzzer Song Functions**/
