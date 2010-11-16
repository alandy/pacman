// filename **********Lab17.C***********
// User programs

//  This example accompanies the books
//   "Embedded Microcomputer Systems: Real Time Interfacing", 
//     Thomsen, copyright (c) 2006,
//   "Introduction to Embedded Microcomputer Systems: 
//    Motorola 6811 and 6812 Simulation", Brooks-Cole, copyright (c) 2002,
//    Jonathan W. Valvano 3/17/07

// You may use, edit, run or distribute this file 
//    as long as the above copyright notices remain
#include <hidef.h>      /* common defines and macros */
#include <mc9s12dp512.h>     /* derivative informati on */
#pragma LINK_INFO DERIVATIVE "mc9s12dp512"

//#include "Lab17.h"   // global constants
//#include "Lab17OS.h"   // needs access to OS routines
//#include "Lab17SCI.h"  // needs access to SCI routines
//#include "debug.h"

#include "LCD.H"
#include "PLL.H"
#include "Timer.H"
#include "PWM.h"
#include <stdio.h>
#include "i2c_master.h"
#include "xbee.h"
#include "message.h"

//=========Global Variables======================
int data0;
int data1;
int data2;
int data3;
int range0;
int range1;
int range2;
int range3;
int range4;
char stop_flag;

//===============================

unsigned short static volatile Count0;
interrupt 8 void TOC0handler(void){ // executes at 100 Hz 
  TFLG1 = 0x01;         // acknowledge OC0
  Count0++;
  TC0 = TC0+10000;      // 10 ms
  PTT ^= 0x01;          // debugging monitor
}

//---------------------OC_Init0---------------------
// arm output compare 0 for 100Hz periodic interrupt
// Input: none
// Output: none 
void OC_Init0(){
  Count0 = 0;     // debugging monitor
  DDRT |= 0x01;   // debugging monitor
  TIOS |= 0x01;   // activate TC0 as output compare
  TIE  |= 0x01;   // arm OC0
  TSCR1 = 0x80;   // Enable TCNT, 24MHz boot mode, 8MHz in run mode
  TSCR2 = 0x03;   // divide by 8 TCNT prescale, TOI disarm
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

unsigned short static volatile Count3;
interrupt 11 void TOC3handler(void){ // executes at 1000 Hz 
  TFLG1 = 0x08;         // acknowledge OC3
  Count3++;
  TC3 = TC3+1000;       // 1 ms
  PTT ^= 0x08;          // debugging monitor
}

//---------------------OC_Init3---------------------
// arm output compare 3 for 1000Hz periodic interrupt
// Input: none    assumes another ritual enabled the TCNT
// Output: none 
void OC_Init3(){
  DDRT |= 0x08;    // debugging monitor
  TIOS |= 0x08;    // activate TC3 as output compare
  TIE  |= 0x08;    // arm OC3
  TC3   = TCNT+50; // first interrupt right away
}



//---------------------OC_Init7---------------------
// arm output compare 3 for 1000Hz periodic interrupt
// Input: none    assumes another ritual enabled the TCNT
// Output: none 
void OC_Init3(){
  Count3 = 0;      // debugging monitor      
  DDRT |= 0x80;    // debugging monitor
  TIOS |= 0x80;    // activate TC3 as output compare
  TIE  |= 0x80;    // arm OC3
  TC3   = TCNT+50; // first interrupt right away
}

unsigned short DesiredHeading;
unsigned short DesiredSpeed;
unsigned short XBFrame = 0;

#define MOTOR_DIFF_1_DEGREE_PER_SECOND 10

interrupt 15 void TOC7handler(void){ // executes at 100 Hz 
  TFLG1 = 0x80;         // acknowledge OC0
  TC0 = TC0+10000;      // 10 ms
  PTT ^= 0x80;          // debugging monitor
  
  
  
  
}

//#######################################################

//---------------------Move--------------------------
void Move_update(void){
  
  PWM_Duty3(data3);
  PWM_Duty2(data2);
  PWM_Duty1(data1);
  PWM_Duty0(data0);

}

//---------------------Sensor_Read-------------------
void Sensor_Read(void){
  range0 = ADC_In(0x80);	 // right justified channel 0
  range1 = ADC_In(0x81);	 // right justified channel 1
  range2 = ADC_In(0x82);	 // right justified channel 2
  range3 = ADC_In(0x83);	 // right justified channel 3
  range4 = ADC_In(0x84);	 // right justified channel 4
}

//---------------------Turn_Left--------------------
void Turn_Left(void){

      data3=25;
      data1=75;
      data2=75;
      data0=25;
      
  Move_update();
  
  swait();
      data3=25;
      data1=25;
      data2=25;
      data0=25;
      
  //Move_update();
  
}

//---------------------Turn_Right--------------------
void Turn_Right(void){

      data3=75;
      data1=25;
      data2=25;
      data0=75;
      
  Move_update();
  
  swait();
      data3=25;
      data1=25;
      data2=25;
      data0=25;
      
  Move_update();
  
}
unsigned short FullMotorPower = 95;

#define FULL_SPEED_POWER  FullMotorPower
#define THREE_QUARTER     (FULL_SPEED_POWER * 3) / 4)
#define HALF_POWER        (FULL_SPEED_POWER / 2)
#define QUARTER_POWER     (HALF_POWER / 2)
#define ZERO_SPEED_POWER  25
void straight(void) {
  
    data3=FULL_SPEED_POWER;
    data1=FULL_SPEED_POWER;
    data2=ZERO_SPEED_POWER;
    data0=ZERO_SPEED_POWER;
    Move_update();
  
}

//#######################################################


unsigned short run = 0;
unsigned short MazeRefHeading=0;


void message(xbPacketRX16 * pt) {
  
  unsigned char msg[15] = {0};
  
  if (pt->data[0] == PACKET_PREFIX) {
    
    switch(pt->data[1]) {   //Switch on OPCODE
    
      case OPCODE_NET_REASSIGN_ADDRESS:
        //STUB
        break;
        
      case OPCODE_START:
        run = 1;
        break;
        
      case OPCODE_STOP:
      case OPCODE_PAUSE:
        run = 0;
        break;
        
      
      case OPCODE_DISABLE_MOTORS:
        //Disable PWM[3:0]
        PWME &= ~0x0f;
        break;
        
       case OPCODE_DISABLE_MOTORS: 
        //Enable PWM[3:0]
        PWME |= 0x0f;
        break;
    
    
      case OPCODE_MOVE:
        //STUB
        break;
        
      case OPCODE_SET_MOTOR_POWER:
        if (pt->data[2] == 4) {
          data0 = pt->data[3];
          data1 = pt->data[4];
          data2 = pt->data[5];
          data3 = pt->data[6];
          Move_update();  
        }
        break;
        
      case OPCODE_GET_FULL_MOTOR_POWER:
        msg[0] = PACKET_PREFIX;
        msg[1] = OPCODE_GET_FULL_MOTOR_POWER;
        msg[2] = 1;
        msg[3] = FullMotorPower;
        msg[4] = 0;
        xb_SendPacket(pt->source, FrameID++, 0, msg, 5);
        
        break;
        
      case OPCODE_DATA_STATUS:
        //STUB
        
        msg[0] = PACKET_PREFIX;
        msg[1] = OPCODE_DATA_STATUS;
        msg[2] = 1;
        msg[3] = 'O';
        msg[4] = 'K';
        msg[5] = 0;
        xb_SendPacket(pt->source, FrameID++, 0, msg, 4);
        
        break;
        
      case OPCODE_DATA_STATUS_LONG:
        //STUB
        
        msg[0] = PACKET_PREFIX;
        msg[1] = OPCODE_DATA_STATUS_LONG;
        msg[2] = 1;
        msg[3] = 'O';
        msg[4] = 'K';
        msg[5] = 0;
        xb_SendPacket(pt->source, FrameID++, 0, msg, 4);
        
        break;
        
      case OPCODE_DATA_SENSOR_REQUEST_IR:
        msg[0] = 0xPACKET_PREFIX;
        msg[1] = OPCODE_DATA_SENSOR_REQUEST_IR;
        msg[2] = 5;
        
        Sensor_Read();
        msg[3] = range0;
        msg[4] = range1;
        msg[5] = range2;
        msg[6] = range3;
        msg[7] = range4;
        msg[8] = 0;
        
        xb_SendPacket(pt->source, XBFrame++, 0, msg, 9);
        break;
        
      case OPCODE_DATA_SENSOR_REQUEST_HEADING:
        msg[0] = 0xPACKET_PREFIX;
        msg[1] = OPCODE_DATA_SENSOR_REQUEST_IR;
        msg[2] = 1;
        msg[3] = I2C_Recv16(0xC0, 0x02);
        msg[4] = 0;
        
        xb_SendPacket( pt->source, XBFrame++, 0, msg, 5);
        break;
        
      case OPCODE_DATA_SET_MAZE_REF_HEADING:
        MazeRefHeading = data[3];
        break;
        
      default:
        break;
        
  } 
 
  free(pt); 
}

//=========Main======================

unsigned short startHeading = 0;



unsigned short xbPacketRX16 * pt;

void main(void){ 

  int checkStatus;
  short heading;
  unsigned char msg[30];


  char msg[10];
  OC_Init0();
  OC_Init3();
  DDRP |= 0x80;  // LED

  PLL_Init();       // set E clock to 24 MHz
  Timer_Init();      // enable timer
  DDRP |= 0x80;     // PortP bit 7 is output to LED, used for debugging
  DDRM |= 0x03;

  PWM_Init();
  ADC_Init();     // Activate ADC 
  I2C_Init();
  asm cli

#define MY_ADDRESS NET_ID_GHOST
#define SERVER_ADDRESS NET_ID_SERVER

  checkStatus = xb_Init(MY_ADDRESS, 0);
	if ( checkStatus < 0) {
	 	//printf("Error in xb_Init: %d", checkStatus);
	   PTP_PTP7 = 1;
	   while(1);
	};
	
	msg[0] = PACKET_PREFIX;
	msg[1] = OPCODE_NET_JOIN;                             
	msg[3] = NET_ID_GHOST1;
	
	xb_SendPacket(SERVER_ADDRESS, XBFrame++, 0, msg, 3);

  check(LCD_Open());
  check(LCD_Clear());
  check(LCD_OutString(" Robot  ")); blank();
  check(LCD_OutString("Pacman ")); swait();  
    
    PTM |= 0x03;
    while(run) {
      if (PTP_PTP6) run = 1;
      
      if ( (pt = xb_ReceivePacket()) != NULL) message(pt);
          
      mwait(100);    
      LCD_Clear();
      sprintf(msg, "%d", startHeading = I2C_Recv16(0xC0, 0x02));
      LCD_OutString(msg);
      
    }
    
    desiredHeading = startHeading;
    
    PTM &= ~0x03;
    
  straight();    
         
  while(1) {
    
    PTP ^= 0x80;           // toggle LED
    
    if ( (pt = xb_ReceivePacket()) != NULL) message(pt);
    
    if (run) {
        
      mwait(500);
      
      LCD_Clear();
      //sprintf(msg, "%d", ADC_In(0x87));
      heading = I2C_Recv16(0xC0, 0x02);
      sprintf(msg, "%d", heading);
      LCD_OutString(msg);
      
      #define DELTA 5
      #define THRESH1 15
      //HEADING: Left is bigger, right is smaller (compass mounted upside down)
      // data1 is rightForward, data3 is leftForward, data2 is leftBack, data0 is rightBack
      
      heading -= desiredHeading;
      
      if (heading > THRESH1) {
        //drifting to the left, reduce power to right wheel
        data1 -= DELTA;
        
        PTM_PTM0 = 1;
        PTM_PTM1 = 0;
        Move_update();
        
      } else if (heading < (-THRESH1)) {
        //drifting to the right, reduce power to left wheel
        data3 -= DELTA;
        
        Move_update();
        PTM_PTM1 = 1;
        PTM_PTM0 = 0;
      } else {
        straight();
        PTM &= ~0x03;
      }
    
    } else {
      data3=0;
      data1=0;
      data2=0;
      data0=0;
      
  }
}




















void main1(void){ 

// Lee's original main:


  EnableInterrupts;
  // running at 4 MHz
  
  OC_Init0();
  OC_Init3();
  DDRP |= 0x80;  // LED
  asm cli

  PLL_Init();       // set E clock to 24 MHz
  Timer_Init();      // enable timer
  ADC_Init();     // Activate ADC 
  PWM_Init();
  DDRP |= 0x80;     // PortP bit 7 is output to LED, used for debugging
  check(LCD_Open());
  check(LCD_Clear());
  check(LCD_OutString(" Robot  ")); blank();
  check(LCD_OutString("Pacman ")); swait();
  asm cli   // allows debugger to run
  
    
  //PWM_Duty3(25);  // PP3 10% 93.75Hz wave
  //PWM_Duty1(225); // PP1 90% 93.75Hz wave
  

 
  for(;;) {
    
    PTP ^= 0x80;           // toggle LED
    Sensor_Read();
    //check(LCD_GoTo(2,3));
    stop_flag = 0;
    
    data3=95;
    data1=95;
    data2=25;
    data0=25;
    Move_update();
     
    //sensor detection
    if( (range0 > 500) ){     
      data3=25;
      data1=25;
      data2=25;
      data0=25;
      Move_update();
      stop_flag = 1;
      
      //Turn_Left();    
      
      
    }else{
      stop_flag = 0;
      data3=95;
      data1=95;
      data2=25;
      data0=25;
         
         if(range0 > 500){      //left side sensor
           data1 -= 5;    
         } else if(range1 > 500){      //right side sensor
           data3 -= 5;    
         } 
    }   
    /*
    if(stop_flag == 1){
      Turn_Left();    
      swait();
      range2 = ADC_In(0x82);
      if(range2 > 500){
        Turn_Right();
        Turn_Right();
      
      }
      
    }
    */
     
              
     
    
    /*
    //This section is used to display the IR reading on the LCD
    check(LCD_OutString("Range0")); blank();
    check(LCD_OutNum(range0)); swait();
    check(LCD_OutString("Range1")); blank();    
    check(LCD_OutNum(range1)); swait();
    check(LCD_OutString("Range2")); blank();    
    check(LCD_OutNum(range2)); swait();
                                                  
    /*                                              
    check(LCD_OutString("Range2")); blank();    
    check(LCD_OutNum(range2)); swait();
    
    check(LCD_OutString("Stop")); blank();    
    check(LCD_OutNum(stop_flag)); swait();
    */
  } 
    

  //OS_AddThread(&Interpreter); 
  //OS_AddThread(&Consumer); 
  //OS_AddThread(&Math); 
 
  //SCI_Init(BR);
  //OS_Launch(TIMESLICE); // doesn't return
}
 	
 

