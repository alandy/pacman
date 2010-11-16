/*    
	rev 0.2.09018.1645.A

	xbee.h
	Headers for xbee interface

	Created: September 7, 2010, 15:00 - AML
	Modified: September 18, 2010 16:45 - AML

	Created for EE 464 Project Pac-Man

#EOC*/

#include "serial.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "time.h"

#define MAX_QUEUED_PACKETS 20
#define MAX_WAITLIST_SIZE 20
#define PLATFORM LINUX
#define TIMEOUT_TIME 0xf000000
#define AT_PACKET_MAX_PARAM_LEN 10
#define TX_OPT_DISABLE_ACK 0x01
#define TX_OPT_BROADCAST 0x04
#define ADDRESS_SIZE 2	//Number of bytes in address
#define SERIAL_OPTIONS (INLOG | OUTLOG | BLOCKING_READ | DELIM_INLOG | DELIM_OUTLOG)

enum platform {
	LINUX,
	DP512
};

typedef enum status_resp {
	OK,
	NO_DATA,
	ERROR,
	MALLOC_FAIL,
	PACKET_DESYNC,
	UNKNOWN_PACKETID
} err_t;

typedef struct {
	SerialPort port;
	int address	:16;
	unsigned char nodeType;
	struct {
		int messagesSent;
		int dataBytesSent;
		int totalAckTime;
		int messagesReceived;
		int dataBytesReceived;
	} statistics;
} xbModule;



/********************** XBee API Packet Structure Types ***********************/

typedef struct {
	unsigned int delim		:8;
	unsigned int length		:16;
	unsigned int APIIdent	:8;
	unsigned int frameID	:8;
	unsigned int command	:16;
	unsigned char params[AT_PACKET_MAX_PARAM_LEN];
	unsigned int checksum	:8;
} xbPacketAT;

typedef struct {
	unsigned int delim		:8;
	unsigned int length		:16;
	unsigned int APIIdent	:8;
	unsigned int frameID	:8;
	unsigned int status		:8;
	unsigned int checksum	:8;
} xbPacketTXstat;

/*
 	Need to be extremely careful with variable length packet structures, member 
 	data[] is merely a placeholder to serve as a pointer to the data starting in 
 	that location. However, packet payload data will extend far beyond 
	sizeof(xbPacket). To create a new packet, malloc should always be used and
 	called as 

		xbPacketNAME * pt = malloc(packet_length + 4);
 
 	where packet length is the length field of the TX/RX packet
 
 */

typedef struct {
	unsigned int delim		:8;
	unsigned int length		:16;
	unsigned int APIIdent	:8;
	unsigned int source		:16;
	unsigned int RSSI		:8;
	unsigned int options	:8;
	unsigned char data[2];		// placeholder - use this array to access the extra space malloced to hold data
	// checksum is last byte of data[]
} xbPacketRX16;

typedef struct {
	unsigned int delim		:8;
	unsigned int length		:16;
	unsigned int APIIdent	:8;
	unsigned int frameID	:8;
	unsigned int destination:16;
	unsigned int options	:8;
	unsigned char data[2];		// placeholder - use this array to access the extra space malloced to hold data
	// checksum is last byte of data[]
} xbPacketTX16;


/********************** XBee API Internal Structure Types *********************/

typedef struct {
	unsigned int delim		:8;
	unsigned int length		:16;
	unsigned int APIIdent	:8;
	unsigned char data[2];
} xbPacketGeneric;

typedef struct {
	int frame_id;
	time_t qtime;
	xbPacketTX16 * packet;
} xbAckQEntry;

/*************** XBee Library Functions ***************/

int xb_Init(char * address, char * devpath);

xbPacketRX16 * xb_ReceivePacket(void);

err_t xb_ProcessPackets(int maxPackets);

int xb_SendPacket(short address, char frame_id, char options, unsigned char * data, char data_len);

int xb_RXStat(void);

int xb_TXStat(void);

int xb_PrintPacket(xbPacketGeneric * pt);