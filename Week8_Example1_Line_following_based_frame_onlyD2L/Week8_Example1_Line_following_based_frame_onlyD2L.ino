#include <mechbotShield.h>
#include <avr/io.h>
#include "MEC733_I_O.h"
#include <USART.h>

void straight (int dist){
  DDRC = DDRC & (~((1 << DDRC2) | (1 << DDRC3))); //can also be DDRC&=~~((1<<DDRC2)|(1<<DDRC3)), Week 5 note Page 30

  // Step 1.5:  internal resistor pull up

  PORTC = PORTC | ((1 << PORTC2) | (1 << PORTC3));
  int i = 1, bump1, bump2, bump3, bump4;

  while (i)
  {
    bump1 = PIND & (1 << PIND3);
    bump2 = PIND & (1 << PIND4);
    bump3 = PIND & (1 << PIND5);
    bump4 = PINC & (1 << PINC1);
    if ((bump1 == 0) | (bump2 == 0) | (bump3 == 0) | (bump4 == 0))
    {
      i = 0;
    }
  }

  int left_en_total = 0, right_en_total = 0, base_speed = 600, left_speed, right_speed;
  int left_en_cur, right_en_cur, left_en_pre, right_en_pre;
  int error;

  left_en_total = 0;
  right_en_total = 0;

  left_en_pre = PINC & (1 << PINC2);
  right_en_pre = PINC & (1 << PINC3);
  
  motor(base_speed, base_speed);

  i = 1;

  while (i)
  { left_en_cur = PINC & (1 << PINC2);
    right_en_cur = PINC & (1 << PINC3);

    if (left_en_cur != left_en_pre)
    { left_en_total++;
      left_en_pre = left_en_cur;
    }

    if (right_en_cur != right_en_pre)
    { right_en_total++;
      right_en_pre = right_en_cur;
    }

    if ((left_en_total >= dist) && (right_en_total >= dist))
    { 
     motor(0, 0); 
     i = 0;
    }

    if ((left_en_total >= dist) && (right_en_total < dist))
    {
      motor(0, base_speed);
    }

    if ((left_en_total < dist) && (right_en_total >= dist))
    {
      motor(base_speed,0);
    }
    
  }

}

void turn (int dir)//left = 0 right = 1
{
  DDRC = DDRC & (~((1 << DDRC2) | (1 << DDRC3))); //can also be DDRC&=~~((1<<DDRC2)|(1<<DDRC3)), Week 5 note Page 30

  // Step 1.5:  internal resistor pull up

  PORTC = PORTC | ((1 << PORTC2) | (1 << PORTC3));
  int i = 1, bump1, bump2, bump3, bump4;

  while (i)
  {
    bump1 = PIND & (1 << PIND3);
    bump2 = PIND & (1 << PIND4);
    bump3 = PIND & (1 << PIND5);
    bump4 = PINC & (1 << PINC1);
    if ((bump1 == 0) | (bump2 == 0) | (bump3 == 0) | (bump4 == 0))
    {
      i = 0;
    }
  }

  int left_en_total = 0, right_en_total = 0, base_speed = 600, left_speed, right_speed;
  int left_en_cur, right_en_cur, left_en_pre, right_en_pre;
  int error;

  left_en_total = 0;
  right_en_total = 0;

  left_en_pre = PINC & (1 << PINC2);
  right_en_pre = PINC & (1 << PINC3);

  if (dir == 0) {
    motor(-base_speed, base_speed);
  }
  else {
    motor(base_speed, -base_speed);
  }
  i = 1;

  while (i)
  { left_en_cur = PINC & (1 << PINC2);
    right_en_cur = PINC & (1 << PINC3);

    if (left_en_cur != left_en_pre)
    { left_en_total++;
      left_en_pre = left_en_cur;
    }

    if (right_en_cur != right_en_pre)
    { right_en_total++;
      right_en_pre = right_en_cur;
    }

    if ((left_en_total >= 16) && (right_en_total >= 16))
    { motor(0, 0); i = 0;
    }
        if ((left_en_total >= 16) && (right_en_total < 16))
    {
      motor(0, base_speed);
    }

    if ((left_en_total < 16) && (right_en_total >= 16))
    {
      motor(base_speed,0);
    }
  }

}

int main(void)
{
  uint16_t Line1, Line2, Line3, Line4, Dis_left, Dis_center, Dis_right;
  uint16_t t = 1000, threshold_table = 650,  threshold_blackTape = 850, counter = 0;

  initSoftSerial();
  setLCDBackLight(255);
  clrLCD();
  moveLCDCursor(0);
  initADC();

  initMotor();
  motor(0, 0);
  initSoftSerial(); //dwdsds

  //using bump switch to start program, i.e., wait till the bump switch is pressed to start the line following program
  int i = 1, bump1, bump2, bump3;
  clrLCD();
  lcdPrint( "press bump to start");

  delay_ms(200);

  straight(138);
  turn(0);
  straight(32);
  turn(1);
  straight(28);
  turn(0);
  straight(59);
  turn(0);
  straight(32);
  turn(0);
  straight(32);
  turn(1);
  straight(58);
  turn(1);
  straight(126);
  return 0;

}
