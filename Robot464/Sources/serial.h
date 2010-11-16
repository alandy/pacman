/*    
	rev 1.1.0914.1248.A
 
	Serial.h
	Serial port interface header file
 
	Created: September 5, 2010, 18:54 - AML
	Modified: September 7, 2010 13:40 - AML
 
	Created for EE 464 Project Pac-Mam 
 
	#EOC*/

//#include <stdio.h>
#include <string.h>
#include "SCI1.h"



#define MAX_READ_SIZE 255
#define INLOG_DELIMITER '\n'
#define OUTLOG_DELIMITER '\n'
#define OUTLOG_PATH "serial_out.log"
#define INLOG_PATH "serial_in.log"

typedef enum {
	INLOG = 0x1,
	OUTLOG = 0x2,
	BLOCKING_READ = 0x4,
	DELIM_INLOG = 0x8,
	DELIM_OUTLOG = 0x10
} SerialOptions;


typedef int SerialPort;	// Used for serial port file descriptor


/****************Serial_Open****************
 * Open serial port specified by device file 
 * Inputs:	path - path to device file characterizing port
 *			baud - baud rate to set port to
<<<<<<< HEAD
 *			options - bitfield options
 *								0: store all read data to serial_in.log
 *								1: store all write data to serial_out.log 
 * Returns the file descriptor on success or -1 on error.
 */
SerialPort Serial_Open(char * path, int baud, SerialOptions options);

int Serial_Open_TestFork( int baud, SerialPort * port);

int Serial_SetSpeed(int port, int baud);

void Serial_SetBlocking(SerialPort port, int bool);


/****************Serial_Write****************
 * Write bytes to serial port specified by file descriptor returned
 * by previous call to Serial_Open().
 * buf[0] -> buf[bytes - 1] will be written
 * Inputs:	buf - pointer to bytes to be written
 *			bytes - number of bytes to write
 * Returns the number of bytes successfully written.
 */
int Serial_Write(int fd, unsigned char *buf, int bytes);


/****************Serial_Readn****************
 * Read up to specified number of bytes from serial port specified by file 
 * descriptor returned by previous call to Serial_Open().
 * Inputs:	buf - pointer to preallocated buffer to store bytes
 *			bytes - number of bytes to read
 * Returns the number of bytes read.
 */
int Serial_Readn(SerialPort port, unsigned char * buf, int bytes);


/****************Serial_Read****************
 * Read all available bytes up to max definied by MAX_READ_SIZE from serial 
 * port specified by file descriptor returned by previous call to Serial_Open().
 * Inputs:	buf - pointer to preallocated buffer to store bytes
 * Returns the number of bytes read.
 */
int Serial_Read(int fd, unsigned char *buf);


/****************Serial_ReadnX****************
 * Read exactly specified number of bytes from serial
 * port specified by file descriptor returned by previous call to Serial_Open().
 * Inputs:	buf - pointer to preallocated buffer to store bytes
 *			bytes - number of bytes to read
 * Returns the number of bytes read.
 */
int Serial_ReadnX(SerialPort port, unsigned char * buf, int bytes);

