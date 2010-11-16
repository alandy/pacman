// File **********Timer.h***********
// Timer wait routine, 9S12DP512
// assumes PLL is active and E clock is 24 MHz
// TCNT will become 1.5MHz
// Jonathan W. Valvano 3/1/07

//  This example accompanies the books
//   "Embedded Microcomputer Systems: Real Time Interfacing",
//         Thomson Engineering, copyright (c) 2006,
//   "Introduction to Embedded Microcomputer Systems: 
//    Motorola 6811 and 6812 Simulation", Thomson, copyright (c) 2002

// Copyright 2007 by Jonathan W. Valvano, valvano@mail.utexas.edu 
//    You may use, edit, run or distribute this file 
//    as long as the above copyright notice remains 

 

//---------------------Timer_Init---------------------
// activate TCNT at 1.5 MHz
// inputs: none
// outputs: none
void Timer_Init(void);


//---------Timer_Wait------------------
// fixed time delay
// inputs: time to wait in 667ns cycles
// outputs: none
void Timer_Wait(unsigned short delay);

//---------Timer_Wait1ms------------------
// fixed time delay
// inputs: time to wait in ms
// outputs: none
// 1500 cycles equals 1ms
void Timer_Wait1ms(unsigned short delay);

//---------Timer_Wait10ms------------------
// fixed time delay
// inputs: time to wait in 10ms
// outputs: none
// 15000 cycles equals 10ms
void Timer_Wait10ms(unsigned short delay);



//=======================================
//---------------------mwait---------------------
// wait specified number of msec
// Input: number of msec to wait
// Output: none
// assumes TCNT timer is running at 667ns
void mwait(unsigned short msec);


//---------------------check---------------------
// if LCD broken toggle LED1 at 2Hz
// Input: last LCD status, 0 means bad
// Output: none
// Error: if status is zero, this function will not return
void check(short status);

//---------------------swait---------------------
// wait specified 2 sec, then clear LCD
// Input: none
// Output: none
// uses mswait and TCNT timer
void swait(void);

//---------------------blank---------------------
// move cursor to second half of LCD display
// 32 spaces from address 08 to 40
// Input: none
// Output: none
void blank(void);