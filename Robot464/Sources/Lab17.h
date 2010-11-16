// filename **********Lab17.h***********
// global constants

//  This example accompanies the books
//   "Embedded Microcomputer Systems: Real Time Interfacing", 
//     Thomsen, copyright (c) 2006,
//   "Introduction to Embedded Microcomputer Systems: 
//    Motorola 6811 and 6812 Simulation", Brooks-Cole, copyright (c) 2002,
//    Jonathan W. Valvano 3/17/07

// You may use, edit, run or distribute this file 
//    as long as the above copyright notices remain

#define TIMESLICE 4000  // thread switch time in 250ns cycles
#define NUM  20         // number of squareroots calculated in Math
#define PERIOD 2000     // sampling period in 250ns cycles
#define LENGTH 10       // find min/max for this many samples
#define BR BR250000     // BR=1 for 250000 bps
#define DEBUG 1         // 1/0 means insert/skip debugging instruments
#define RUNLENGTH 200   // display results and quit when Time==RUNLENGTH
// bandwidth check, make sure average producer rate < maximum consumer rate
// producer creates 4(cycles/usec)/PERIOD(samples/cycle)=2000 samples/sec 
// maximum consumer rate = BaudRate(bits/s)/10(bits/frame) *1(byte/frame) 
//  * LENGTH(samples/output)/15(bytes/output) 
//  = 125000 *1  *10  /15 /10 = 8333 samples/sec

