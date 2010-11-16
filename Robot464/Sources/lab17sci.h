// filename **********Lab17SCI.H***********
// Serial I/O

//  This example accompanies the books
//   "Embedded Microcomputer Systems: Real Time Interfacing", 
//     Thomsen, copyright (c) 2006,
//   "Introduction to Embedded Microcomputer Systems: 
//    Motorola 6811 and 6812 Simulation", Brooks-Cole, copyright (c) 2002,
//    Jonathan W. Valvano 3/17/07

// You may use, edit, run or distribute this file 
//    as long as the above copyright notices remain

 
#define CR 13
#define LF 10
#define BS 8
#define SP 32
#define BR19200 13  // baud rate of  19,200 bps
#define BR250000 1  // baud rate of 250,000 bps

// ******** SCI_Open ************
// capture serial device for this thread to use
// this thread will spin/block if device being used by another thread
// inputs:  none
// outputs: none
void SCI_Open(void);

// ******** SCI_Close ************
// release serial device for other threads to use
// inputs:  none
// outputs: none
void SCI_Close(void);

// ******** SCI_Init ************
// Initialize Serial port SC0
// inputs:  bitTime determine baud rate = 500,000/bitTime
// outputs: none
void SCI_Init(unsigned char bitTime);
    
// ******** SCI_InChar ************
// Input one character from SCI terminal
// will spin/block if no data ready
// Inputs:  none    
// Outputs: ASCII character 
unsigned char SCI_InChar(void);

// ******** SCI_OutChar ************
// Output one character to SCI terminal
// will spin/block if TxFifo is full
// Inputs:  ASCII character
// Outputs: none 
void SCI_OutChar(unsigned char data);

//*****************SCI_OutCRLF***************
// Output a CR,LF to go to a new line
void SCI_OutCRLF(void);

// ******** SCI_OutString ************
// Output String (NULL termination)
// Inputs:  call by reference    
// Outputs: none 
void SCI_OutString(char *pt); 
    

//-----------------------SCI_OutUDec--------------------------------------
// Output a 16 bit number in unsigned decimal format
// Variable format 1 to 5 digits with no space before or after
// This function uses recursion to convert a decimal number of 
//   unspecified length as an ASCII string
// Inputs:  16-bit unsigned number    
// Outputs: none 
void SCI_OutUDec(unsigned short number);


