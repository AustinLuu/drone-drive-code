#include <mechbotShield.h>
#include <avr/io.h>
#include "MEC733_I_O.h"
#include <USART.h>

uint16_t Dis_left, Dis_center, Dis_right;
uint16_t threshold_table = 650,  threshold_blackTape = 850, threshold_wall = 300, counter = 0;
int left_en_total = 0, right_en_total = 0, base_speed = 550, left_speed, right_speed, Dis_right_avg, Dis_left_avg;
int left_en_cur, right_en_cur, left_en_pre, right_en_pre;
int c, r = 0, p = 10;

/////////////////////////
/////////////////////////
/////////////////////////
/////////////////////////

void straight (int dist){
DDRC = DDRC & (~((1 << DDRC2) | (1 << DDRC3))); //can also be DDRC&=~~((1<<DDRC2)|(1<<DDRC3)), Week 5 note Page 30
PORTC = PORTC | ((1 << PORTC2) | (1 << PORTC3));// Step 1.5:  internal resistor pull up

left_en_total = 0;
right_en_total = 0;

left_en_pre = PINC & (1 << PINC2);
right_en_pre = PINC & (1 << PINC3);

left_speed=550;
right_speed=550;

motor(left_speed, right_speed);

for (i = 0; i < 50; i++){//initial feedback code 
  Dis_left+=analog(4); 
  Dis_right+=analog(6);
}
Dis_left_avg=Dis_left/50;
Dis_right_avg=Dis_right/50;

while (1)//drive code
{ 
  for (i = 0; i < 50; i++){//initial feedback code 
    Dis_left+=analog(4); 
    Dis_right+=analog(6);
  }
  Dis_left=Dis_left/50;
  Dis_right=Dis_right/50;

  left_en_cur = PINC & (1 << PINC2);
  right_en_cur = PINC & (1 << PINC3);
  c = left_en_total - right_en_total;

  if (left_en_cur != left_en_pre){
    left_en_total++;
    left_en_pre = left_en_cur;
  }

  if (right_en_cur != right_en_pre){ 
    right_en_total++;
    right_en_pre = right_en_cur;
  }

/*  if ((100>(Dis_left-Dis_left_avg) && (100>(Dis_right_avg-Dis_right))) //Moving to left too much: big Change 
  {
    left_speed+=20;
    right_speed-=20;
  }

  else if((50>(Dis_left-Dis_left_avg)) && (50>(Dis_right_avg-Dis_right)))//Small Change
  {
    left_speed+=10;
    right_speed-=10;
  }

  if ((100>(Dis_right-Dis_right_avg) && (100>(Dis_left_avg-Dis_left))) //Moving to right too much: big Change 
  {
    left_speed-=20;
    right_speed+=20;
  }

  else if((50>(Dis_right-Dis_right_avg)) && (50>(Dis_left_avg-Dis_left)))//Small Change
  {
    left_speed-=10;
    right_speed+=10;
  }*/

  Dis_left_avg=Dis_left;
  Dis_right_avg=Dis_right;

  if ((left_en_total >= dist) && (right_en_total >= dist)){ 
    motor(0, 0); 
    break;
  }
  
  if (e != 0){
    left_speed-=(c*p);
    right_speed+=(c*p);
    motor(left_speed, right_speed);
    delay_ms(30);
  }

}

/*  while(1){
    
    motor(left_speed, right_speed);
    
    for (i = 0; i < 30; i++){//initial feedback code 
      Dis_center+=analog(5); 
    }
    
    Dis_center=Dis_center/50;
    
    if (Dis_center > threshold_wall){//close enough
      break;
    }
  }*/
}

/////////////////////////
/////////////////////////
/////////////////////////
/////////////////////////

void turn (int dir)//left = 0 right = 1
{
DDRC = DDRC & (~((1 << DDRC2) | (1 << DDRC3))); //can also be DDRC&=~~((1<<DDRC2)|(1<<DDRC3))
PORTC = PORTC | ((1 << PORTC2) | (1 << PORTC3)); // Step 1.5:  internal resistor pull up

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

while (1){ 
  left_en_cur = PINC & (1 << PINC2);
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
    { 
      motor(0, 0); 
      break;
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

/////////////////////////
/////////////////////////
/////////////////////////
/////////////////////////

int main(void)
{

  initSoftSerial();
  setLCDBackLight(255);
  clrLCD();
  moveLCDCursor(0);
  initADC();
  lcdPrint( "press bump to start");

  initMotor();
  motor(0, 0);

  int bump1, bump2, bump3, bump4;//using bump switch to start program, i.e., wait till the bump switch is pressed to start the line following program
  
  while(1){
    bump1 = PIND & (1 << PIND3);
    bump2 = PIND & (1 << PIND4);
    bump3 = PIND & (1 << PIND5);
    bump4 = PINC & (1 << PINC1);
    if ((bump1 == 0) | (bump2 == 0) | (bump3 == 0) | (bump4 == 0))
    {
      delay_ms(200);
      break;
    }
  }

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
  lcdPrint( "YEEEEETTTTT");
  delay_ms(1000);
  return 0;

}
