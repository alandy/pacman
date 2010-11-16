// filename ******** PWM.h ************** 
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

// ---------PWM_Init---------
// enable 8-bit PWM channels 3,1
// Port P PP3 and PP1 are output, 93.75Hz wave
// inputs: none
// outputs: none
void PWM_Init(void);

// ---------PWM_Duty3---------
// set duty cycle, power to PP3
// inputs: power from 0 to 250
// outputs: none
void PWM_Duty3(unsigned char power);
 
// ---------PWM_Duty1---------
// set duty cycle, power to PP1
// inputs: power from 0 to 250
// outputs: none
void PWM_Duty2(unsigned char power);
      
// ---------PWM_Duty1---------
// set duty cycle, power to PP1
// inputs: power from 0 to 250
// outputs: none
void PWM_Duty1(unsigned char power);

// ---------PWM_Duty1---------
// set duty cycle, power to PP1
// inputs: power from 0 to 250
// outputs: none
void PWM_Duty0(unsigned char power);
            