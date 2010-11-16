// filename **********Lab17OS.C***********
// Operating System

//  This example accompanies the books
//   "Embedded Microcomputer Systems: Real Time Interfacing", 
//     Thomsen, copyright (c) 2006,
//   "Introduction to Embedded Microcomputer Systems: 
//    Motorola 6811 and 6812 Simulation", Brooks-Cole, copyright (c) 2002,
//    Jonathan W. Valvano 3/17/07

// You may use, edit, run or distribute this file 
//    as long as the above copyright notices remain

#include <mc9s12dp512.h>  /* derivative information */
#include "Lab17.h"   // global constants
#include "Lab17OS.h"
#include "debug.h"
/****************warning*********************
 *** when changing this structure,        ***
 *** make sure CCR,RegB,RegA,RegX,RegY,PC *** 
 *** fields are continguous!!             ***
 *******************************************/
struct	TCB{
  struct TCB *Next;             // Link to Next TCB
  unsigned char *StackPt;       // Stack Pointer
  unsigned char Id;             // 1, 2, 4, ...    
  unsigned char MoreStack[100]; // additional stack 
  unsigned char InitialCCR; 
  unsigned char InitialRegB;
  unsigned char InitialRegA;          
  unsigned short InitialRegX;           
  unsigned short InitialRegY;     
  void (*InitialPC)(void); 
};
typedef struct TCB TCBType;
typedef TCBType * TCBPtr;
TCBType SystemTCB[MAX_THREADS];
unsigned short static NumThread=0; // number of threads
 
TCBPtr RunPt;  // Pointer to thread currently running 
unsigned short static TimeSlice;
// ******** OS_RunId ************
// determine which foregound thread is running 
// input:  none
// output: the Id of the Running thread
unsigned char OS_RunId(void){
  return(RunPt->Id);
}

// ******** OS_InitSemaphore ************
// initialize semaphore 
// input:  pointer to a semaphore
// output: 1 if successful, 0 if not successful
short OS_InitSemaphore(Sema4Type *semaPt, short value){ 
  semaPt->Counter = value ; 
  return 1;
}  

// ******** OS_Wait ************
// decrement semaphore and spin if less than zero
// input:  pointer to a semaphore
// output: none
void OS_Wait(Sema4Type *semaPt){ 
  SETBIT(PTT,3);  // for profiling
  asm sei      // Test and set is atomic
  while(semaPt->Counter <= 0){  // disabled 
    asm cli             // disabled 
    asm nop             // enabled  
    asm sei             // enabled
// make an OS function and call it from here
//    TC3=TCNT+15;  // add for cooperative multitasking
  }
  (semaPt->Counter)--;     // disabled 
  asm cli                  // disabled 
  CLRBIT(PTT,3);           // for profiling
}                          // enabled 

// ******** OS_Signal ************
// increment semaphore 
// input:  pointer to a semaphore
// output: none
void OS_Signal(Sema4Type *semaPt){ 
unsigned char SaveCCR;
  SETBIT(PTT,4);     // for profiling
  asm tpa            // previous interrupt enable 
  asm staa SaveCCR   // save previous 
  asm sei            // make atomic 
  (semaPt->Counter)++;
  asm ldaa SaveCCR   // recall previous 
  asm tap            // end critical section 
  CLRBIT(PTT,4);     // for profiling
}

Sema4Type DataAvailable;    // Semaphore counting data in DataFifo
unsigned short static *DataPutPt;  // Pointer of where to put next 
unsigned short static *DataGetPt;  // Pointer of where to get next 
// FIFO is empty when DataAvailable is less than or equal to zero 
// FIFO is full  when DataRoomLeft is less than or equal to zero 
unsigned short static DataFifo[DATAFIFOSIZE];   // statically allocated fifo 

//******** OS_OpenDataChannel *************** 
// Initialize data transfer channel
// inputs:  none
// outputs: none
void OS_OpenDataChannel(void){
  OS_InitSemaphore(&DataAvailable, 0);  // Initially empty
  DataPutPt = DataGetPt = &DataFifo[0];
}

//******** OS_SendData *************** 
// enter 16-bit data into the DataFifo 
// called from background, so does not wait if full
// inputs:  16-bit data
// outputs: 0 if data is lost, 1 if successful
short OS_SendData(unsigned short data){ 
// must be atomic if more than one producer
// but since there is only one producer, there is no critical section
unsigned short *tempPt;  
  SETBIT(PTT,0);       // for profiling
  tempPt = DataPutPt;
  *tempPt = data;        // Try to Put data into fifo
  tempPt++;              // place to Put next
  if(tempPt == &DataFifo[DATAFIFOSIZE]){ // need to wrap?
    tempPt = &DataFifo[0];
  }
  if(tempPt == DataGetPt){
    CLRBIT(PTT,0);       // for profiling
    return(0);             // Failed, fifo was full 
  }     
  DataPutPt = tempPt;        // Success, so update pointer
  OS_Signal(&DataAvailable); // increment only if data actually stored
  CLRBIT(PTT,0);           // for profiling
  return(1);
}

//******** OS_ReceiveData *************** 
// Get a 16-bit data from the DataFifo
// called from the foreground
// Will wait if the DataFifo is empty
// Inputs: none
// Outputs: data
unsigned short OS_ReceiveData(void){ unsigned short data;  
  SETBIT(PTT,1);       // for profiling
  OS_Wait(&DataAvailable);
// must be atomic if more than one consumer
  data = (*DataGetPt);  // remove data 
  DataGetPt++;          // Place to Put next
  if (DataGetPt == &DataFifo[DATAFIFOSIZE]){
    DataGetPt = &DataFifo[0];
  }
  CLRBIT(PTT,1);       // for profiling
  return(data);
}

//******** threadSwitch *************** 
// decides which thread to run next
// add blocking, killing, sleeping here
// do NOT add input/output parameters
// can add local variables to this function
// Inputs: none
// Outputs: none
void threadSwitch(void){ 
// do most of the work here
  RunPt = RunPt->Next;
#if DEBUG
  PTM = RunPt->Id;     // PTM shows which thread is running
#endif
}
/****************warning************************
 *** when changing ThreadSwitch, make sure   ***
 *** the compiler doesn't allocate anything  *** 
 *** on the stack. Look at LST file!!        ***
 *** don't add any local variables           ***
 **********************************************/
interrupt 11 void threadSwitchISR(void){
asm ldx RunPt			 
asm sts 2,x
  threadSwitch();
  TC3 = TCNT+TimeSlice;  // Thread runs for a unit of time 
  TFLG1 = 0x08;          // acknowledge by clearing TC3F 
asm ldx RunPt
asm lds 2,x
}


//******** OS_AddThread *************** 
// add a foregound thread to the scheduler
// Inputs: pointer to a void/void foreground function
// Outputs: 1 if successful, 0 if this thread can not be added
short OS_AddThread(void(*fp)(void)){
  if(NumThread >= MAX_THREADS){ 
    return 0;  // structure is full
  }
  if(NumThread){
    SystemTCB[NumThread-1].Next=&SystemTCB[NumThread]; // Pointer to Next  
  }
  SystemTCB[NumThread].StackPt = &SystemTCB[NumThread].InitialCCR; // set SP
  SystemTCB[NumThread].Id = 1<<NumThread;         // Id 
  SystemTCB[NumThread].InitialCCR = 0x40;         // CCR I bit clear 
  SystemTCB[NumThread].InitialPC = fp;            // Initial PC 
  SystemTCB[NumThread].Next = &SystemTCB[0];      // Pointer to first 
  NumThread++;
  return 1;
}

//******** OS_Launch *************** 
// start the scheduler
// Inputs: number of E clock cycles for each time slice
// Outputs: none (does not return)
void OS_Launch(unsigned short theTimeSlice){
#if DEBUG
  DDRM = 0x3F;  // for debugging
  DDRT = 0xFF;
#endif
  TimeSlice = theTimeSlice;   // user specifies time slice
  RunPt = &SystemTCB[0];      // Specify first thread to run 
asm  sei
  TIOS |= 0x08;    // activate OC3
  TSCR1 = 0x80;    // enable TCNT
  TIE = 0x08;      // Arm TC3 
  TC3 = TCNT+TIMESLICE; 
  TFLG1 = 0x08;    // Clear C3F 
#if DEBUG
  PTM = RunPt->Id;
#endif
asm ldx RunPt
asm lds 2,x
asm rti     // Launch First Thread 
}
