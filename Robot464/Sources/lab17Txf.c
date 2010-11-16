// filename **********Lab17TxF.C***********
// Transmit FIFO

//  This example accompanies the books
//   "Embedded Microcomputer Systems: Real Time Interfacing", 
//     Thomsen, copyright (c) 2006,
//   "Introduction to Embedded Microcomputer Systems: 
//    Motorola 6811 and 6812 Simulation", Brooks-Cole, copyright (c) 2002,
//    Jonathan W. Valvano 3/17/07

// You may use, edit, run or distribute this file 
//    as long as the above copyright notices remain



#include "Lab17OS.h"  // needs access to semaphores
#include "Lab17TxF.h"
// semaphore associated TxFifo
Sema4Type TxRoomLeft;  /* Semaphore counting empty spaces in TxFifo */


unsigned char static *TxPutPt;  // Pointer of where to put next 
unsigned char static *TxGetPt;  // Pointer of where to get next 
                  // FIFO is empty if PutPt=GetPt 
                  // FIFO is full  if PutPt+1=GetPt 
unsigned char static TxFifo[TXFIFOSIZE];  // statically allocated fifo 

// ******** TxFifo_Init ************
// Initialize TxFifo to be empty
// Inputs:  none
// Outputs: none 
void TxFifo_Init(void) {
  TxPutPt = TxGetPt = &TxFifo[0];  // Empty when TxPutPt==TxGetPt
  OS_InitSemaphore(&TxRoomLeft, TXFIFOSIZE-1);  
  // TXFIFOSIZE-1 is the Maximum storage capability 
}

// ******** TxFifo_Put ************
// Enter one character into the TxFifo
// Called from the foreground, will wait if full
// Inputs:  8-bit data
// Outputs: none 
void TxFifo_Put (unsigned char data) { 
  OS_Wait(&TxRoomLeft);
  *TxPutPt = data;    // try to Put data into fifo 
  TxPutPt++;          // place to put next 
  if(TxPutPt==&TxFifo[TXFIFOSIZE]){ // need to wrap?
    TxPutPt = &TxFifo[0];
  }
}

// ******** TxFifo_Get ************
// Remove one character from the TxFifo
// Called from the background, so no Wait, no disable interrupt
// Inputs:  pointer to place to save 8-bit data
// Outputs: true if data is valid 
//          false if TxFifo is empty at the time of the call
short TxFifo_Get(unsigned char *datapt){
  if(TxPutPt == TxGetPt){
    return(0);     // Empty if TxPutPt=TxGetPt 
  }
  else {
    *datapt = *TxGetPt;
    TxGetPt++;             // place to get next
    if(TxGetPt==&TxFifo[TXFIFOSIZE]){ // need to wrap? 
      TxGetPt = &TxFifo[0];
    }
    OS_Signal(&TxRoomLeft); // increment if data removed
    return(1); 
  }
}

// ******** TxFifo_Status ************
//  Check the status of the TxFifo
//  Inputs: none
//  Outputs: true if there is room in the TxFifo 
//           true if a call to TxFifo_Put will return right away
//           false if the TxFifo is full 
//           false if a call to TxFifo_Put will spin (or block)
short TxFifo_Status (void) { 
  return (TxRoomLeft.Counter>0);
}

