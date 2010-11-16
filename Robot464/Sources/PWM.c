// filename ******** PWM.C ************** 
// Implementation of Pulse Width Modulated outputs 
// two 8-bit Pulse Width Modulated outputs 
// Technological Arts 9S12DP512 board  
// PP3 PP1 are PWM output, 93.75Hz wave
// PWMDTY3, PWMDTY1 vary from 0 to 250 

//        ___________  250-PWMDTY3     ___________
// PP3 __/  PWMDTY3	 \________________/ 	PWMDTY3	\___

//        ___________  250-PWMDTY1     ___________
// PP1 __/  PWMDTY1	 \________________/ 	PWMDTY1	\___

// Jonathan W. Valvano 9/1/09

//  This example accompanies the books
//   "Embedded Microcomputer Systems: Real Time Interfacing",
//        Thompson, copyright (c) 2006,
//   "Introduction to Embedded Systems: Interfacing to the Freescale 9S12",
//        Cengage Publishing 2009, ISBN-10: 049541137X | ISBN-13: 9780495411376

// Copyright 2009 by Jonathan W. Valvano, valvano@mail.utexas.edu 
// You may use, edit, run or distribute this file 
//    as long as the above copyright notices remain


#include <hidef.h>         /* common defines and macros */
#include <mc9s12dp512.h>     /* derivative information */

// ---------PWM_Init---------
// enable 8-bit PWM channels 3,1
// Port P PP3 and PP1 are output, 93.75Hz wave
// inputs: none
// outputs: none
void PWM_Init(void){
  DDRP |= 0x0F;      
  PWMPOL = 0x0F;   //PWM channels high then low
  // bits 7-0, PPOL7-0, Pulse Width Channel 7-0 Polarity
  //  1 = high at beginning of period, then low when duty count reached.
  //  0 = low at beginning of period, then high when duty count reached.
  PWMCLK = 0x0F;   // clock SB for 3, clock SA for 1, 
  // bit 7, PCLK7, Pulse Width Channel 7 Clock Select
  //   1 = Clock SB is the clock source for PWM channel 7
  //   0 = Clock B is the clock source for PWM channel 7
  // bit 6, PCLK6, Pulse Width Channel 6 Clock Select
  //   1 = Clock SB is the clock source for PWM channel 6
  //   0 = Clock B is the clock source for PWM channel 6
  // bit 5, PCLK5, Pulse Width Channel 5 Clock Select
  //   1 = Clock SB is the clock source for PWM channel 5
  //   0 = Clock B is the clock source for PWM channel 5
  // bit 4, PCLK4, Pulse Width Channel 4 Clock Select
  //   1 = Clock SA is the clock source for PWM channel 4
  //   0 = Clock A is the clock source for PWM channel 4
  // bit 3, PCLK3, Pulse Width Channel 3 Clock Select
  //   1 = Clock SB is the clock source for PWM channel 3  
  //   0 = Clock B is the clock source for PWM channel 3
  // bit 2, PCLK2, Pulse Width Channel 2 Clock Select
  //   1 = Clock SB is the clock source for PWM channel 2
  //   0 = Clock B is the clock source for PWM channel 2
  // bit 1, PCLK1, Pulse Width Channel 1 Clock Select
  //   1 = Clock SA is the clock source for PWM channel 1
  //   0 = Clock A is the clock source for PWM channel 1
  // bit 0, PCLK0, Pulse Width Channel 0 Clock Select
  //   1 = Clock SA is the clock source for PWM channel 0
  //   0 = Clock A is the clock source for PWM channel 0
  PWMPRCLK = 0x77; // B is bus clock/128, A is 24MHz/128 = 187.5 kHz
  // bits 6-4, PCKB2-PCKB0, n is 0 to 7, Prescaler Select for Clock B
  //           clock B is bus clock/(2**n)
  // bits 2-0, PCKA2-PCKA0, m is 0 to 7, Prescaler Select for Clock A
  //           clock A is bus clock/(2**m)
  PWMCAE = 0;      // left aligned mode
  // bits 7-0, CAE7-0, Center Aligned Output Mode on Channel 7-0
  //   1 = operates in Center Aligned Output Mode.
  //   0 = operates in Left Aligned Output Mode.
  PWMCTL = 0x00;   // Concatenate no channels, runs in wait/freeze mode 
  // bit 7, CON67, Concatenate channels 6 and 7
  //   1 = Channels 6 and 5 are concatenated to create one 16-bit channel 
  //       Channel 6 becomes high byte and channel 7 becomes low byte 
  //       Channel 7 output pin is used as output for 16-bit PWM
  //       Channel 7 clock select control-bit determines clock source 
  //       Channel 7 polarity bit determines polarity
  //       Channel 7 enable	bit enables  output and 
  //       Channel 7 center aligned enable bit determines output mode.
  //   0 = Channels 4 and 5 are separate 8-bit PWMs.
  // bit 6, CON45, Concatenate channels 4 and 5
  //   1 = Channels 4 and 5 are concatenated to create one 16-bit channel 
  //       Channel 4 becomes high byte and channel 5 becomes low byte 
  //       Channel 5 output pin is used as output for 16-bit PWM
  //       Channel 5 clock select control-bit determines clock source 
  //       Channel 5 polarity bit determines polarity
  //       Channel 5 enable	bit enables  output and 
  //       Channel 5 center aligned enable bit determines output mode.
  //   0 = Channels 4 and 5 are separate 8-bit PWMs.
  // bit 5, CON23, Concatenate channels 2 and 3
  //   1 = Channels 2 and 3 are concatenated to create one 16-bit channel 
  //       Channel 2 becomes high byte and channel 3 becomes low byte 
  //       Channel 3 output pin is used as output for 16-bit PWM
  //       Channel 3 clock select control-bit determines clock source 
  //       Channel 3 polarity bit determines polarity 
  //       Channel 3 enable	bit enables  output and 
  //       Channel 3 center aligned enable bit determines output mode
  //   0 = Channels 2 and 3 are separate 8-bit PWMs.
  // bit 4, CON01, Concatenate channels 0 and 1
  //   1 = Channels 0 and 1 are concatenated to create one 16-bit channel 
  //       Channel 0 becomes high byte and channel 1 becomes low byte 
  //       Channel 1 output pin is used as output for 16-bit PWM
  //       Channel 1 clock select control-bit determines clock source 
  //       Channel 1 polarity bit determines polarity 
  //       Channel 1 enable	bit enables  output and 
  //       Channel 1 center aligned enable bit determines output mode
  //   0 = Channels 0 and 1 are separate 8-bit PWMs.
  // bit 3, PSWAI, PWM Stops in Wait Mode
  // bit 2, PFRZ, PWM Counters Stop in Freeze Mode
  PWMSCLA = 4;     // SA prescaled A by 4  = 24MHz/128/8 = 23437.5Hz
  // Clock SA = Clock A / (2 * PWMSCLA)
  PWMSCLB = 4;     // SB prescaled B by 4	 = 24MHz/128/8 = 23437.5Hz
  // Clock SB = Clock B / (2 * PWMSCLB)
  PWMPER3 = 250;   // period3, 23437.5Hz/250=93.75Hz wave
  PWMPER2 = 250;   // period2, 23437.5Hz/250=93.75Hz wave
  PWMPER1 = 250;   // period1, 23437.5Hz/250=93.75Hz wave
  PWMPER0 = 250;   // period0, 23437.5Hz/250=93.75Hz wave
  PWMDTY3 = 0;     // duty cycle3, off
  PWMDTY2 = 0;     // duty cycle1, off
  PWMDTY1 = 0;     // duty cycle3, off
  PWMDTY0 = 0;     // duty cycle1, off
  PWME = 0x0F;	   // enable channels 3~1
  // bits 7-0, PWME7-0, Pulse Width Channel 7-0 Enable
}

// ---------PWM_Duty3---------
// set duty cycle, power to PP3
// inputs: power from 0 to 250
// outputs: none
void PWM_Duty3(unsigned char power){
    PWMDTY3 = power;      // PP3 goes 1 for power/250 
}

// ---------PWM_Duty2---------
// set duty cycle, power to PP1
// inputs: power from 0 to 250
// outputs: none
void PWM_Duty2(unsigned char power){
    PWMDTY2 = power;      // PP1 goes 1 for power/250 
}
   

// ---------PWM_Duty1---------
// set duty cycle, power to PP3
// inputs: power from 0 to 250
// outputs: none
void PWM_Duty1(unsigned char power){
    PWMDTY1 = power;      // PP3 goes 1 for power/250 
}

// ---------PWM_Duty0---------
// set duty cycle, power to PP1
// inputs: power from 0 to 250
// outputs: none
void PWM_Duty0(unsigned char power){
    PWMDTY0 = power;      // PP1 goes 1 for power/250 
}
     