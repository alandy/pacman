/*Message passing scheme:

{ PACKET_PREFIX } {opcode} {data_length N} {data0} {data1} ... {dataN-1} {0}

*/

#define PACKET_PREFIX 0xA5

//Network Control
#define OPCODE_NET_JOIN                           0x01
#define OPCODE_NET_REASSIGN_ADDRESS               0x02

  #define NET_ID_SERVER   0xFA
  #define NET_ID_PACMAN   0xFB
  #define NET_ID_GHOST1   0xFC
  #define NET_ID_GHOST2   0xFD
  #define NET_ID_MAZE     0xFE

//Debugging 0x8{...}
#define OPCODE_CHARACTER_MESSSAGE 0xD0  //Should be treated as a printf

//Robot Control   0x1{...}
#define OPCODE_START                              0x10
#define OPCODE_STOP                               0x11
#define OPCODE_PAUSE                              0x12
#define OPCODE_DISABLE_MOTORS                     0x13
#define OPCODE_ENABLE_MOTORS                      0x14
#define OPCODE_MOVE                               0x15
#define OPCODE_SET_MOTOR_POWER                    0x16

    //Robot Calibration
    #define OPCODE_GET_FULL_MOTOR_POWER           0x17
    #define OPCODE_SET_FULL_MOTOR_POWER           0x18
    #define OPCODE_GET_CALIBRATION_COEFF          0x19
    #define OPCODE_SET_CALIBRATION_COEFF          0x1A

//Data        0x5{...}
#define OPCODE_DATA_STATUS                        0x50
#define OPCODE_DATA_STATUS_LONG                   0x51
#define OPCODE_DATA_SENSOR_REQUEST_IR             0x55
#define OPCODE_DATA_SENSOR_REQUEST_HEADING        0x56
#define OPCODE_DATA_SET_MAZE_REF_HEADING          0x57
#define OPCODE_DATA_RFID_TAG                      0x58

//Maze Control    0xC{...}
#define OPCODE_MAZE_STATUS                        0xC0
#define OPCODE_MAZE_GET_LEDS                      0xC1
#define OPCODE_MAZE_SET_LEDS                      0xC3
#define OPCODE_MAZE_SET_LEDS_SPEC                 0xC4
