/*    
	rev 0.4.1112.1830.A
 
 	xb_9s12.c
 	Implements xbee interface as background thread for 9s12

	Created: November 12, 2010, 14:00 - AML
	Modified: November 12, 2010 18:30 - AML
 
	Created for EE 464 Project Pac-Man
 
#EOC*/

#include <mc9s12dp512.h>     /* derivative information */
#include "SCI1.h"
#include "xbee.h"
#include "queue.h"

typedef struct {
  unsigned char * buf;
  int size;
}txBufferEntry;


//-------------------------Module Globals------------------------
#define RDRF 0x20   // Receive Data Register Full Bit
#define TDRE 0x80   // Transmit Data Register Empty Bit
#define FIFOSIZE 20

txBufferEntry * TXFIFO[FIFOSIZE];
queue txFifo = {TXFIFO, 0, 0, 0, FIFOSIZE, 0,};

extern xbModule xbee;
          
#define CLOCKS_PER_US 24//(CLOCKS_PER_SEC / 1000000)

time_t startTime;

xbModule xbee;

xbPacketRX16 * rxPacket_Q[MAX_QUEUED_PACKETS];
queue rxQ = {
	rxPacket_Q, 0, 0, 0, MAX_QUEUED_PACKETS
};

xbAckQEntry * txACKWaitlist_Q[MAX_WAITLIST_SIZE];
queue txackQ = {
	txACKWaitlist_Q, 0, 0, 0, MAX_QUEUED_PACKETS
};



/****************Serial_WriteQ****************
 * Write bytes to serial port specified by file descriptor returned
 * by previous call to Serial_Open().
 * buf[0] -> buf[bytes - 1] will be written
 * Inputs:	buf - pointer to bytes to be written
 *			bytes - number of bytes to write
 * Returns the number of bytes successfully written.
 */
int Serial_WriteQ(SerialPort port, unsigned char * buf, int bytes) {
	int i;
  txBufferEntry * tx = malloc(sizeof(txBufferEntry));
  
  tx->buf = buf;
  tx->size = bytes;
  
  QPut(&txFifo, tx);
  
  //Enable TDRE interrupts
  SCI1CR2 |= 0x80;

	return bytes;                                                                       	                                
}

interrupt 21 void sci1Handler(void) {
  unsigned char sr1 = SCI1SR1;
  static int txInProgress = 0, i;
  static txBufferEntry * tx;
  
  if (sr1 & TDRE) {
   // Transmit register is empty. This may be a TDRE interrupt
   
    if (txInProgress) {
      SCI1DRL = tx->buf[i++];
      if ( i >= tx->size ) {
        txInProgress = 0;
        //free(tx->buf);
        free(tx);
      }
    } else if (txFifo.size > 0) {
      tx = (char *)QGet(&txFifo);
      i = 0;
      SCI1DRL = tx->buf[i++];
      if ( i >= tx->size ) {
        free(tx->buf);
        free(tx);
      } else txInProgress = 1;
    } else {
      //Fifo empty, disable transmit interrupts until new data is ready
      SCI1CR2 &= ~0x80;
    }
        
  } 
  
  if (sr1 & RDRF) {
    // Receive register is full. This may be an RDRF interrupt
    packetProcessorNASM(SCI1DRL);
  
  }
  
}


#define DEBUG_NO_XBEE 0

int checkOK(void) {

#if (DEBUG_NO_XBEE == 0)	
	unsigned char resp[10] = {0};
	int i;
	
	//wait for three bytes, confirm they are OK<CR>
	Serial_ReadnX(xbee.port, resp, 3);
	
	if (strcmp(resp, "OK\r")) {
		return -2;
	}
	return 0;
#else
	return 0;
#endif
}

//address must be a null-terminated ascii string
int xb_Init(char * address, char * devpath) {
	unsigned char msg[10] = {0};
	int checkStatus = 0;
	
	startTime = time(NULL);
	
	xbee.port = Serial_Open(devpath, 9600, 0); 
	
	usleep(1000000);
	strcpy((char *)msg, "+++");
	Serial_Write(xbee.port, msg, 3);
	usleep(1000000);
/*	if (SCI1_InStatus == 0) {
	    //OK is not ready, so there must be a connection issue, probably a baud mismatch
	    Serial_SetSpeed(1, 115200);
	    
	    //Retry the write
	    	usleep(1000000);
	      Serial_Write(xbee.port, msg, 3);
	      usleep(1000000);
	}*/
	
	checkStatus = checkOK();
	if (checkStatus  < 0) {
		return checkStatus;
	}
	
	xbee.address = atoi( address );
	
	//Set Address, wait for acknowledge
	sprintf(msg, "%s %s%c","ATMY", address, '\r');
	Serial_Write(xbee.port, msg , 7 + strlen(address));
	
	checkStatus = checkOK();
	if (checkStatus  < 0) {
		return checkStatus;
	}
	
	//Enter API mode, wait for acknowledge
	strcpy((char *)msg, "ATAP 1\r");
	Serial_Write(xbee.port, msg, 7);     

	checkStatus = checkOK();
	if (checkStatus  < 0) {
		return checkStatus;
	}
	
	/*//Change baud rate to 115200
	sprintf((char *) msg, "ATBD 7\r");
	Serial_Write(xbee.port, msg, 7);
	
	checkStatus = checkOK();
	if (checkStatus  < 0) {
		return checkStatus;
	} */
	
	//Exit command mode, wait for acknowledge
	Serial_Write(xbee.port, (unsigned char *)"ATCN\r", 5);       
	
	checkStatus = checkOK();
	if (checkStatus  < 0) {
		return checkStatus;
	}
	
	//Change baud rate of Serial port to match new speed set on xbee
	//Serial_SetSpeed(xbee.port, 115200);
	
  //Enable SCI1 RDRF interrupts, TCIE will be set on first write
  SCI1CR2 |= 0x20;
  
	return xbee.port;
}

int xb_RXStat(void) {
	return rxQ.size;
}

int xb_TXStat(void) {
	return txackQ.size;
}


//Receieve a processed xb packet from the rx fifo
xbPacketRX16 * xb_ReceivePacket(void) {
	// Dequeue a processed packet
	return (xbPacketRX16 *) QGet(&rxQ);
}
            
 
  
#define RX16_START 4
#define TXSTAT_START 8

//Non-Atomic State Machine Based Packet Processor
int packetProcessorNASM(unsigned char data){
  
  static int state;
  
  static int i, length, APIIdent;
  static xbPacketRX16 * rxPt;
  static xbPacketTXstat * txStatPt;
  xbAckQEntry * ackEntry; 
 
  int waitTime, j, srcHI;
  err_t exitVal = OK;
 
  //Switch on SM State
  switch (state) {
  
    case 0:
      if (data == 0x7E) {
        // Start Delimiter found
        state = 1;
      } else {
        // Wrong value found, packet processor has become desynchronized 
        // or there is an error
        state = 0;
        exitVal = PACKET_DESYNC;
      }
      
      break;
      
    case 1:
      //Read packet length MSB
      length = (data & 0xFF) << 8;
      state = 2;
      
      break;
      
    case 2:
      //Read packet length LSB
      length += (data & 0xFF);
      state = 3;
      
      break;
      
    case 3:
      //Read API Identifier
      APIIdent = data;
      
      // Switch next state based on APIIdent
      switch (APIIdent) {
        
        case 0x81:
        
          state = RX16_START;
          break;
          
        case 0x89:
        
          state = TXSTAT_START;
          break;
          
        default:
          state = 0;
          exitVal = UNKNOWN_PACKETID;
          break;
      
      }
      
      break;
            
    case 4:
      //RX16 packet, Read source MSB
      rxPt = (xbPacketRX16 *) malloc(length + 4);
      if (!rxPt) {
        exitVal = MALLOC_FAIL;
        state = 0;
      }
      
      rxPt->delim = 0x7E;
      rxPt->length = length;
      rxPt->APIIdent = APIIdent;
      srcHI = (data & 0xFF) << 8;
      rxPt->source = srcHI;
      
      i = 0;
      state = 5;
      
      break;
       
    case 5:
      //Read source LSB
      rxPt->source += (data & 0xFF);
    
      state = 6;
      break;      
      
    case 6:
      //Read rest (all but last) of packet into the packet structure, length - 3 bytes
      ((unsigned char *) rxPt + 6)[i++] = data;
      
      if (i < length - 3) state = 6;
      else state = 7;
      
      break;
            
    case 7:
      //Finish RX packet: Read last byte, stats, enqueue
      ((unsigned char *) rxPt + 6)[i++] = data;
      
      xbee.statistics.messagesReceived++;
			xbee.statistics.dataBytesReceived += (length - 4);
			
			//place pointer to packet on rx packet queue
			if (QPut(&rxQ, rxPt) == -1) exitVal = ERROR; //fifo full
      
      state = 0;
      
      break;    
 
    case 8:
      //TXStat Packet, allocate, store first 4 bytes + new byte
      txStatPt = (xbPacketTXstat *) malloc(length + 4);
      if (txStatPt == NULL) {
        return MALLOC_FAIL;
      }
      
      txStatPt->delim = 0x7E;
      txStatPt->length = length;
      txStatPt->APIIdent = APIIdent;
      
      txStatPt->frameID = data;
      
      state = 9;
      
      break;
      
    case 9:
      //Read ack status
      
      txStatPt->status = data;
      
      state = 10;
      break;
      
    case 10:
      //Read checksum
      
      txStatPt->checksum = data;
    
  	  //pull ack frame off of ack waitlist queue
			while (1) {
				ackEntry = QGet(&txackQ);
				if (ackEntry == 0) {
					//printf("ACK message Q mismatch, something is horribly wrong!\n");
					return OK;
				}
				if (txStatPt->frameID == ackEntry->frame_id) {
					break;
				} else {
					QPut(&txackQ, txStatPt);
				}
				
			}
			
			if (txStatPt->status) {
				//printf("Warning: TXStat Failure: No ACK Received for frame %x from dest %x", ackEntry->frame_id, ackEntry->packet->destination);
			}
			LCD_OutChar('0' + txStatPt->frameID);
			waitTime = (clock() - ackEntry->qtime) / CLOCKS_PER_US;
			txackQ.statistics.waitTime += waitTime;
			txackQ.statistics.total++;
			free(ackEntry->packet);
			free(ackEntry);
			free(txStatPt);
			
		
      
      
      state = 0;
      break;


  }
 
 
  return exitVal;
  
}                                          


//Be very careful specifying addresses, they are configured in hex, be sure to pass as such

int xb_SendPacket(short address, char frame_id, char options, unsigned char * data, char data_len) {
	unsigned int checksum = 0, i;
	xbAckQEntry * ack;
	xbPacketTX16 * txPt = malloc(9 + data_len);
	
	if (txPt == NULL) {
		return MALLOC_FAIL;
	}
	
	if (data_len > 100) {
		return -1;
	}
	
	txPt->delim = 0x7E;
	((unsigned char *) txPt)[1] = (unsigned char) (data_len + 5) >> 8;
	((unsigned char *) txPt)[2] = (unsigned char) (data_len + 5) & 0xFF;
	((unsigned char *) txPt)[3] = (unsigned char) 0x01;
	((unsigned char *) txPt)[4] = (unsigned char) frame_id;
	((unsigned char *) txPt)[5] = (unsigned char) (address & 0xFF00) >> 8;
	((unsigned char *) txPt)[6] = (unsigned char) (address) & 0xFF;
	txPt->options = options;
	memcpy(txPt->data, data, data_len);
	
	// To calculate checksum: Not including frame delimiters and length, add all bytes
	//keeping only the lowest 8 bits of the result and subtract from 0xFF.
	
	checksum += ( (unsigned char *) txPt)[3] & 0xff;
	for (i = 4; i < data_len + 8; i++) {
		checksum += ( (unsigned char *) txPt)[i] & 0xff;	
	} 
	checksum &= 0xFF;
	checksum = 0xFF - checksum;
		
	((char *)txPt)[data_len + 8] = checksum;
	
	if (Serial_WriteQ(xbee.port, (unsigned char *) txPt, data_len + 9) < 0) return -1;

	//Queue into ACK waitlist
	ack = (xbAckQEntry *) malloc(sizeof(xbAckQEntry));
	if (!ack) {
		return MALLOC_FAIL;
	}
	ack->frame_id = frame_id;
	ack->qtime = clock();
	ack->packet = txPt;
	
	QPut(&txackQ, ack);
	
	xbee.statistics.messagesSent++;
	xbee.statistics.dataBytesSent += data_len;
	
	
	return OK;
}

