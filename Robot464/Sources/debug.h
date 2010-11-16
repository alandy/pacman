// filename **********debug.h***********
// debugging macros

//  This example accompanies the books
//   "Embedded Microcomputer Systems: Real Time Interfacing", 
//     Thomsen, copyright (c) 2006,
//   "Introduction to Embedded Microcomputer Systems: 
//    Motorola 6811 and 6812 Simulation", Brooks-Cole, copyright (c) 2002,
//    Jonathan W. Valvano 3/17/07

// You may use, edit, run or distribute this file 
//    as long as the above copyright notices remain

#ifndef DEBUG_H
#define DEBUG_H	1
#include "Lab17.h"   // global constants

#if DEBUG
#ifndef MASKBIT
#define MASKBIT(bit)	(1 << (bit))
#endif
#define SETBIT(port,bit)  (port|=MASKBIT(bit))
#define CLRBIT(port,bit)  (port&=~MASKBIT(bit))
#define FLIPBIT(port,bit) (port^=MASKBIT(bit))
#else
#define SETBIT(port,bit)
#define CLRBIT(port,bit)
#define FLIPBIT(port,bit)
#endif

#endif
