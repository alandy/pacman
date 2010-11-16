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
 

#define RXFIFOSIZE 4
/* Number of characters in the Fifo
     the FIFO is full when it has RXFIFOSIZE-1 characters */
 
// ******** RxFifo_Init ************
// Initialize RxFifo to be empty
// Inputs: none
// Outputs: none 
void RxFifo_Init(void);

// ******** RxFifo_Put ************
// Enter one character into the RxFifo
// Called from the background, so no wait
// Inputs:  8-bit data
// Outputs: true if data is properly saved,
//          false if data not saved, because it was full
// Since this is called by interrupt handlers no sei,cli*/
short RxFifo_Put(unsigned char data);  

// ******** RxFifo_Get ************
// Remove one character from the RxFifo
// Called in foreground, will wait if empty
// Inputs:  pointer to place to save 8-bit data
// Outputs: none 
void RxFifo_Get(unsigned char *datapt);

// ******** RxFifo_Status ************
//  Check the status of the RxFifo
// Inputs: none
// Outputs: true if there is any data in the RxFifo
//          true if a call to RxFifo_Get will return right away
//          false if the RxFifo is empty 
//          false if a call to RxFifo_Get will spin (or block)
short RxFifo_Status(void);
