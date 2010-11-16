// filename **********Lab17RxF.C***********
// Receiver FIFO

//  This example accompanies the books
//   "Embedded Microcomputer Systems: Real Time Interfacing", 
//     Thomsen, copyright (c) 2006,
//   "Introduction to Embedded Microcomputer Systems: 
//    Motorola 6811 and 6812 Simulation", Brooks-Cole, copyright (c) 2002,
//    Jonathan W. Valvano 3/17/07

// You may use, edit, run or distribute this file 
//    as long as the above copyright notices remain

 
#include "Lab17OS.h"  // needs access to semaphores
#include "Lab17RxF.h"

// semaphore associated with RxFifo
Sema4Type RxAvailable;  // Semaphore counting data in RxFifo
unsigned char static *RxPutPt; // Pointer of where to put next 
unsigned char static *RxGetPt; // Pointer of where to get next
                  // FIFO is empty if RxPutPt=RxGetPt 
                  // FIFO is full  if RxPutPt+1=RxGetPt 
unsigned char static RxFifo[RXFIFOSIZE];     //  statically allocated fifo
 
// ******** RxFifo_Init ************
// Initialize RxFifo to be empty
// Inputs: none
// Outputs: none 
void RxFifo_Init(void){
  RxPutPt = RxGetPt = &RxFifo[0];     // Empty when RxPutPt=RxGetPt 
  OS_InitSemaphore(&RxAvailable, 0);  // Initially empty 
}

// ******** RxFifo_Put ************
// Enter one character into the RxFifo
// Called from the background, so no wait
// Inputs:  8 bit data
// Outputs: true if data is properly saved,
//          false if data not saved, because it was full
// Since this is called by interrupt handlers no sei,cli
short RxFifo_Put(unsigned char data){
unsigned char *tempPt;
  tempPt  = RxPutPt;
  *tempPt = data;    // try to Put data into fifo 
  tempPt++;          // place to Put next 
  if (tempPt==&RxFifo[RXFIFOSIZE]){ // need to wrap?
    tempPt = &RxFifo[0];
  }
  if (tempPt == RxGetPt ){
    return(0);    // Failed, fifo was full 
  }     
  else {
    RxPutPt = tempPt;        // Success, so update pointer
    OS_Signal(&RxAvailable); // increment only if data actually stored
    return(1);
  }
}

// ******** RxFifo_Get ************
// Remove one character from the RxFifo
// Called in foreground, will wait if empty
// Inputs:  pointer to place to save 8 bit data
// Outputs: none 
void RxFifo_Get(unsigned char *datapt){ 
  OS_Wait(&RxAvailable);
  *datapt = *RxGetPt;    // remove data
  RxGetPt++;             // place to get next
  if (RxGetPt==&RxFifo[RXFIFOSIZE]){
    RxGetPt = &RxFifo[0];  // wrap
  }
}

// ******** RxFifo_Status ************
//  Check the status of the RxFifo
// Inputs: none
// Outputs: true if there is any data in the RxFifo
//          true if a call to RxFifo_Get will return right away
//          false if the RxFifo is empty 
//          false if a call to RxFifo_Get will spin (or block)
short RxFifo_Status(void){ 
  return (RxAvailable.Counter>0);
}
