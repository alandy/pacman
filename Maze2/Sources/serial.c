/*    
	rev 1.1.0914.1248.A
 
	Serial.c
	Implements serial port interface 
 
	Created: September 5, 2010, 18:54 - AML
	Modified: September 14, 2010 12:48 - AML
 
	Created for EE 464 Project Pac-Man
 
 #EOC*/
#include "serial.h"
#include <mc9s12dp512.h>     /* derivative information */


/****************Serial_Open****************
 * Open serial port specified by device file 
 * Inputs:	path - path to device file characterizing port
 *			baud - baud rate to set port to
 * Returns the file descriptor on success or -1 on error.
 */

SerialPort Serial_Open(char * path, int baud, SerialOptions options) {

  SCI1_Init(baud);

	
	return 1;
}



/****************Serial_SetSpeed****************
 */
int Serial_SetSpeed(int port, int baud) {
  short br;
  SCI1BDH = 0;   
  br=(1500000+baud/2)/baud; // MCLK/(16*baudRate) 
  SCI1BDL= (unsigned char) br; 
	
	return baud;
	
}


/****************Serial_SetBlocking****************
 */

void Serial_SetBlocking(SerialPort port, int bool) {

  // No meaning until FIFOs set up
  return;

}


/****************Serial_Write****************
 * Write bytes to serial port specified by file descriptor returned
 * by previous call to Serial_Open().
 * buf[0] -> buf[bytes - 1] will be written
 * Inputs:	buf - pointer to bytes to be written
 *			bytes - number of bytes to write
 * Returns the number of bytes successfully written.
 */
int Serial_Write(SerialPort port, unsigned char * buf, int bytes) {
	int i;

	for (i = 0; i < bytes; i++) {
      SCI1_OutChar(buf[i]);
	}

	return bytes;                                                                       	                                
}


/****************Serial_Readn****************
 * Read up to specified number of bytes from serial port specified by file 
 * descriptor returned by previous call to Serial_Open().
 * Inputs:	buf - pointer to preallocated buffer to store bytes
 *			bytes - number of bytes to read
 * Returns the number of bytes read.
 */
int Serial_Readn(SerialPort port, unsigned char * buf, int bytes) {
	int bytes_read = 0;

	int i;

		for (i = 0; i < bytes; i++) {
			buf[i] = SCI1_InChar();
			bytes_read++;
		}
	
	return bytes_read;
}


/****************Serial_Read****************
 * Read all available bytes up to max definied by MAX_READ_SIZE from serial 
 * port specified by file descriptor returned by previous call to Serial_Open().
 * Inputs:	buf - pointer to preallocated buffer to store bytes
 * Returns the number of bytes read.
 */
 
 
 // WARNING: THIS FUNCTION WILL WORK BUT *****DO NOT**** CALL IT 
 // UNLESS YOU KNOW WHAT YOURE DOING
 
int Serial_Read(SerialPort port, unsigned char * buf) {
	
	return Serial_Readn(port, buf, MAX_READ_SIZE);
	
}


/****************Serial_ReadnX****************
 * Read exactly specified number of bytes from serial
 * port specified by file descriptor returned by previous call to Serial_Open().
 * Inputs:	buf - pointer to preallocated buffer to store bytes
 *			bytes - number of bytes to read
 * Returns the number of bytes read.
 */
int Serial_ReadnX(SerialPort port, unsigned char * buf, int bytes) {
	int bytes_read = 0;
	
		int i;

		for (i = 0; i < bytes; i++) {
			buf[i] = SCI1_InChar();
			bytes_read++;
		}
	
	
	return bytes_read;
}
