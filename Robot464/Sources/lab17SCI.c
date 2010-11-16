// filename **********Lab17SCI.C***********
// Serial I/O

//  This example accompanies the books
//   "Embedded Microcomputer Systems: Real Time Interfacing", 
//     Thomsen, copyright (c) 2006,
//   "Introduction to Embedded Microcomputer Systems: 
//    Motorola 6811 and 6812 Simulation", Brooks-Cole, copyright (c) 2002,
//    Jonathan W. Valvano 3/17/07

// You may use, edit, run or distribute this file 
//    as long as the above copyright notices remain

#include <mc9s12c32.h>     /* derivative information */
#include "Lab17.h"   // global constants
#include "debug.h"   // debug MACROs
#include "Lab17RxF.H"
#include "Lab17TxF.H"
#include "Lab17SCI.H"
#include "Lab17OS.h"  // needs access to semaphores
#define RDRF 0x20
#define TDRE 0x80 
unsigned short RxLost; // number of receive characters lost
								  
// semaphore associated with SCI output
Sema4Type DisplayFree;  // SCI output free

// ******** SCI_Init ************
// Initialize Serial port SCI
// inputs:  bitTime specifics rate, baud rate = 250000/bitTime
// outputs: none
void SCI_Init(unsigned char bitTime){
  RxFifo_Init();
  TxFifo_Init();
  OS_InitSemaphore(&DisplayFree, 1);   // free to use 
  RxLost = 0;        // no receive characters lost yet
  SCIBDH = 0;        // 250000 BR=1 (MCLK=4MHz) clock 
  SCIBDL = bitTime;  // bitTime=MCLK/(16*BaudRate)
  SCICR1 = 0;
/* bit value meaning
    7   0    LOOPS, no looping, normal
    6   0    WOMS, normal high/low outputs
    5   0    RSRC, not appliable with LOOPS=0
    4   0    M, 1 start, 8 data, 1 stop
    3   0    WAKE, wake by idle (not applicable)
    2   0    ILT, short idle time (not applicable)
    1   0    PE, no parity
    0   0    PT, parity type (not applicable with PE=0) */ 
    SCICR2 = 0x2C; 
/* bit value meaning
    7   0    TIE, no transmit interrupts on TDRE
    6   0    TCIE, no transmit interrupts on TC
    5   1    RIE, arm receive interrupts on RDRF
    4   0    ILIE, no interrupts on idle
    3   1    TE, enable transmitter
    2   1    RE, enable receiver
    1   0    RWU, no receiver wakeup
    0   0    SBK, no send break */ 
asm cli      /* enable interrupts */
}

// ******** SCI_Open ************
// capture serial device for this thread to use
// this thread will spin/block if device being used by another thread
// inputs:  none
// outputs: none
void SCI_Open(void){
  SETBIT(PTT,2);        // for profiling
  OS_Wait(&DisplayFree);  // mutual exclusive access
  CLRBIT(PTT,2);        // for profiling
}
    
// ******** SCI_InChar ************
// Input one character from SCI terminal
// will spin/block if no data ready
// Inputs:  none    
// Outputs: ASCII character 
unsigned char SCI_InChar(void){ unsigned char letter;
  SETBIT(PTT,5);       // for profiling
  RxFifo_Get(&letter);
  CLRBIT(PTT,5);       // for profiling
  return(letter);
}

// ******** SCI_OutChar ************
// Output one character to SCI terminal
// will spin/block if TxFifo is full
// Inputs:  ASCII character
// Outputs: none 
void SCI_OutChar(unsigned char data){
  SETBIT(PTT,6);       // for profiling
  TxFifo_Put(data);
  SCICR2 = 0xAC;       // arm TDRE 
  CLRBIT(PTT,6);       // for profiling
}

//*****************SCI_OutCRLF***************
// Output a CR,LF to go to a new line
void SCI_OutCRLF(void){
  SCI_OutChar(CR);
  SCI_OutChar(LF);
}

// ******** SCI_OutString ************
// Output String (NULL termination)
// Inputs:  call by reference    
// Outputs: none 
void SCI_OutString(char *pt){ 
unsigned char letter;
  while (letter = *pt++){
    SCI_OutChar(letter);
  }
}


// ******** SCIHandler ************
// SCI interrupts on TDRE or RDRF
interrupt 20 void SCIHandler(void){ unsigned char letter;
  if(SCISR1 & RDRF) {
    letter = SCIDRL;          // acknowledge, clears RDRF
    if(!RxFifo_Put(letter)){
      RxLost++;  // number of characters lost
    }
  }
  if((SCICR2&0x80)&&(SCISR1 & TDRE)){
    if(TxFifo_Get(&letter)){
      SCIDRL = letter;  // acknowledge, clears TDRE
    }
    else{
      SCICR2 = 0x2c;   // disarm TDRE
    }
  }
}


//-----------------------SCI_OutUDec--------------------------------------
// Output a 16 bit number in unsigned decimal format
// Variable format 1 to 5 digits with no space before or after
// This function uses recursion to convert a decimal number of 
//   unspecified length as an ASCII string
// Inputs:  16-bit unsigned number    
// Outputs: none 
void SCI_OutUDec(unsigned short number){
  if(number >= 10){
    SCI_OutUDec(number/10);
    SCI_OutUDec(number%10);
  }
  else{
    SCI_OutChar(number+'0');
  }
}

// ******** SCI_Close ************
// release serial device for other threads to use
// inputs:  none
// outputs: none
void SCI_Close(){
  OS_Signal(&DisplayFree);
}


