#include <hidef.h>      /* common defines and macros */
#include <mc9s12dp512.h>     /* derivative information */
#pragma LINK_INFO DERIVATIVE "mc9s12dp512"



#include "oc.c"
#include "PLL.h"
#include "Timer.h"
#include "LCD.h"
#include <stdio.h>
#include "xbee.h"


#define ROW_INIT 0xAA

unsigned short static volatile Count3;
unsigned short static volatile row0;
unsigned short static volatile row1 = ROW_INIT;
unsigned short static volatile row2 = ROW_INIT;
unsigned short static volatile row3 = ROW_INIT;
unsigned short static volatile row4 = ROW_INIT;
unsigned short static volatile Count0;
interrupt 8 void TOC0handler(void){ // executes at 100 Hz 
  TFLG1 = 0x01;         // acknowledge OC0
  Count0++;
  TC0 = TC0+1000;   // 1 ms
  PTT = row0 & 0xfF;
  PTP = 0x08;
 // row1 ^= row1;
  //row2 ^= row2;
  //row3 ^= row3;
  //PTT ^= 0x01;          // debugging monitor
  
}
interrupt 9 void TOC1handler(void){ // executes at 100 Hz 
  TFLG1 = 0x02;         // acknowledge OC0
  Count0++;
  TC1 = TC1+1000;  // 1 ms
  PTT = row1 & 0xfF;
  PTP = 0x02;  
  //PTT ^= 0x01;          // debugging monitor
}
interrupt 10 void TOC2handler(void){ // executes at 100 Hz 
  TFLG1 = 0x04;         // acknowledge OC0
  Count0++;
  TC2 = TC2+1000;    // 1 ms
  PTT = row2 & 0xfF;
  PTP = 0x04;
  //PTT ^= 0x01;          // debugging monitor
}
interrupt 11 void TOC3handler(void){ // executes at 1000 Hz 
  TFLG1 = 0x08;         // acknowledge OC3
  Count3++;
  TC3 = TC3+1000;       // 1 ms
  PTT = row3 & 0xfF;
  PTP = 0x08;
  //PTT ^= 0x08;          // debugging monitor
}

interrupt 12 void TOC4handler(void){ // executes at 100 Hz 
  TFLG1 = 0x10;         // acknowledge OC0
  Count0++;
  TC4 = TC4+1000;   // 1 ms
  PTT = row4 & 0xfF;
  PTP = 0x10;
 // row1 ^= row1;
  //row2 ^= row2;
  //row3 ^= row3;
  //PTT ^= 0x01;          // debugging monitor
       PTP ^= 0x80; // flash LED
}
//---------------------OC_Init0---------------------
// arm output compare 0 for 100Hz periodic interrupt
// Input: none
// Output: none 
void OC_Init0(){
//  Count0 = 0;     // debugging monitor
  //DDRT |= 0x01;   // debugging monitor
  TIOS |= 0x01;   // activate TC0 as output compare
  TIE  |= 0x01;   // arm OC0
  //TSCR1 = 0x80;   // Enable TCNT, 24MHz boot mode, 8MHz in run mode
  //TSCR2 = 0x03;   // divide by 8 TCNT prescale, TOI disarm
  PACTL = 0;      // timer prescale used for TCNT
/* Bottom three bits of TSCR2 (PR2,PR1,PR0) set TCNT period
    divide  FastMode(24MHz)    Slow Mode (8MHz)
000   1     42ns  TOF  2.73ms  125ns TOF  8.192ms
001   2     84ns  TOF  5.46ms  250ns TOF 16.384ms 
010   4    167ns  TOF  10.9ms  500ns TOF 32.768ms     
011   8    333ns  TOF  21.8ms    1us TOF 65.536ms
100  16    667ns  TOF  43.7ms    2us TOF 131.072ms
101  32   1.33us  TOF  87.4ms    4us TOF 262.144ns
110  64   2.67us  TOF 174.8ms    8us TOF 524.288ms
111 128   5.33us  TOF 349.5ms   16us TOF 1.048576s */
  TC0    = TCNT+50; // first interrupt right away

}
 
//---------------------OC_Init1---------------------
// arm output compare 3 for 1000Hz periodic interrupt
// Input: none    assumes another ritual enabled the TCNT
// Output: none 
void OC_Init1(){
  //Count3 = 0;      // debugging monitor      
  //DDRT |= 0x08;    // debugging monitor
  TIOS |= 0x02;    // activate TC3 as output compare
  TIE  |= 0x02;    // arm OC3
  TC1   = TCNT+200; // first interrupt right away
  
}
//---------------------OC_Init2---------------------
// arm output compare 3 for 1000Hz periodic interrupt
// Input: none    assumes another ritual enabled the TCNT
// Output: none 
void OC_Init2(){
  //Count3 = 0;      // debugging monitor      
  //DDRT |= 0x08;    // debugging monitor
  TIOS |= 0x04;    // activate TC3 as output compare
  TIE  |= 0x04;    // arm OC3
  TC2   = TCNT+400; // first interrupt right away
  
}
//---------------------OC_Init3---------------------
// arm output compare 3 for 1000Hz periodic interrupt
// Input: none    assumes another ritual enabled the TCNT
// Output: none 
void OC_Init3(){
  //Count3 = 0;      // debugging monitor      
  //DDRT |= 0x08;    // debugging monitor
  TIOS |= 0x08;    // activate TC3 as output compare
  TIE  |= 0x08;    // arm OC3
  TC3   = TCNT+600; // first interrupt right away
}
//---------------------OC_Init4---------------------
// arm output compare 3 for 1000Hz periodic interrupt
// Input: none    assumes another ritual enabled the TCNT
// Output: none 
void OC_Init4(){
  //Count3 = 0;      // debugging monitor      
  //DDRT |= 0x08;    // debugging monitor
  TIOS |= 0x10;    // activate TC3 as output compare
  TIE  |= 0x10;    // arm OC3
  TC4   = TCNT+800; // first interrupt right away
  
}


#define TOKEN_MAZE_LED_PATTERN 0xC3

void main(void) {
  char str[20];
  int checkStatus;
  xbPacketRX16 * pt;
  
/*  row0=0x00;
  row1 = 0x00;
  row2 = 0x00;
  row3 = 0x00;*/
  DDRP |= 0xff;    // PP2-0 output to LED
  DDRT |= 0xfF; // 
  //DDRP |= 0x80;  // LED
  PLL_Init();
  Timer_Init();
  TOF_Init();
  LCD_Open();
  OC_Init0();
  OC_Init1();
  OC_Init2();
  OC_Init3();
  OC_Init4();
  
  asm cli       //Enable interrupts
  
  
  checkStatus = xb_Init("11", 0);
	if ( checkStatus < 0) {
	 	//printf("Error in xb_Init: %d", checkStatus);
	   PTP_PTP7 = 1;
	   while(1);
	};
  
  LCD_Clear();
  sprintf(str, "%x%x%x%x", row1, row2, row3, row4);
  LCD_OutString(str);
  
  for(;;) {
      
    while( (pt = xb_ReceivePacket()) != NULL);
      
    if(pt->data[0] == TOKEN_MAZE_LED_PATTERN) {
      
      row1 = pt->data[1];
      row2 = pt->data[2];
      row3 = pt->data[3];
      row4 = pt->data[4];

      sprintf(str, "%x%x%x%x", row1, row2, row3, row4);
      LCD_Clear();
      LCD_OutString(str);
    
    } else {
      free(pt);
    }
  } 
}

