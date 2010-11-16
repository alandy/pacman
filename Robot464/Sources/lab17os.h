// filename **********Lab17OS.H***********
// Operating System

//  This example accompanies the books
//   "Embedded Microcomputer Systems: Real Time Interfacing", 
//     Thomsen, copyright (c) 2006,
//   "Introduction to Embedded Microcomputer Systems: 
//    Motorola 6811 and 6812 Simulation", Brooks-Cole, copyright (c) 2002,
//    Jonathan W. Valvano 3/17/07

// You may use, edit, run or distribute this file 
//    as long as the above copyright notices remain
 
#ifndef __LAB17OS_H
#define __LAB17OS_H	1

#define MAX_THREADS 3  // up to three foreground threads can be created
#define DATAFIFOSIZE 4 // size of data channel
// data channel is full when it has DATAFIFOSIZE-1 characters 

// ******** OS_RunId ************
// determine which foregound thread is running 
// input:  none
// output: the Id of the Running thread
unsigned char OS_RunId(void);

struct	Sema4{
  short Counter;   // >0 means free, otherwise means busy        
// add other components here
};
typedef struct Sema4 Sema4Type;

// ******** OS_InitSemaphore ************
// initialize semaphore 
// input:  pointer to a semaphore
// output: 1 if successful, 0 if not successful
short OS_InitSemaphore(Sema4Type *semaPt, short value); 

// ******** OS_Wait ************
// decrement semaphore and spin if less than zero
// input:  pointer to a semaphore
// output: none
void OS_Wait(Sema4Type *semaPt); 

// ******** OS_Signal ************
// increment semaphore 
// input:  pointer to a semaphore
// output: none
void OS_Signal(Sema4Type *semaPt); 

//******** OS_OpenDataChannel *************** 
// Initialize data transfer channel
// inputs:  none
// outputs: none
void OS_OpenDataChannel(void);

//******** OS_SendData *************** 
// enter 16-bit data into the DataFifo 
// called from background, so does not wait if full
// inputs:  16-bit data
// outputs: 0 if data is lost, 1 if successful
short OS_SendData(unsigned short data); 

//******** OS_ReceiveData *************** 
// Get a 16-bit data from the DataFifo
// called from the foreground
// Will wait if the DataFifo is empty
// Inputs: none
// Outputs: data
unsigned short OS_ReceiveData(void);  

//******** OS_AddThread *************** 
// add a foregound thread to the scheduler
// Inputs: pointer to a void/void foreground function
// Outputs: 1 if successful, 0 if this thread can not be added
short OS_AddThread(void(*fp)(void));

//******** OS_Launch *************** 
// start the scheduler
// Inputs: number of E clock cycles for each time slice
// Outputs: none (does not return)
void OS_Launch(unsigned short theTimeSlice);
#endif
