// filename **********Lab17TxF.H***********
// Transmit FIFO

//  This example accompanies the books
//   "Embedded Microcomputer Systems: Real Time Interfacing", 
//     Thomsen, copyright (c) 2006,
//   "Introduction to Embedded Microcomputer Systems: 
//    Motorola 6811 and 6812 Simulation", Brooks-Cole, copyright (c) 2002,
//    Jonathan W. Valvano 3/17/07

// You may use, edit, run or distribute this file 
//    as long as the above copyright notices remain


#define TXFIFOSIZE 4

// ******** TxFifo_Init ************
// Initialize TxFifo to be empty
// Inputs:  none
// Outputs: none 
void TxFifo_Init(void);

// ******** TxFifo_Put ************
// Enter one character into the TxFifo
// Called from the foreground, will wait if full
// Inputs:  8-bit data
// Outputs: none 
void TxFifo_Put (unsigned char data);

// ******** TxFifo_Get ************
// Remove one character from the TxFifo
// Called from the background, so no Wait, no disable interrupt
// Inputs:  pointer to place to save 8-bit data
// Outputs: true if data is valid 
//          false if TxFifo is empty at the time of the call
short TxFifo_Get(unsigned char *datapt);

// ******** TxFifo_Status ************
//  Check the status of the TxFifo
//  Inputs: none
//  Outputs: true if there is room in the TxFifo 
//           true if a call to TxFifo_Put will return right away
//           false if the TxFifo is full 
//           false if a call to TxFifo_Put will spin (or block)
short TxFifo_Status (void);

