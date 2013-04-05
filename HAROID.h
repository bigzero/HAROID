#ifndef __HAROID_H__
#define __HAROID_H__

typedef int ER;
typedef bool BOOL;

#define NULL    0
#define TRUE    1
#define FALSE   0
#define BUFF_SIZE (32)


#define E_OK        0       /* Normal Completion */

#define SYNC_CMD_FLAG   1
#define WRITE_CMD_FLAG  2
#define CONTROL_CMD_FLAG 4 

// The number 0,1 is system id. So you dont have to change the number.
#define BLUTOOTH_TASKID  0  // Bluetooth is a default channel.
#define PROTOCAL_TASKID  1  // 

//-------------------begin USER setting area---------------------------------
#define TASK_COUNT 5        // You must set a TASK count.
// from number 2, it is user id. You can set a arbitrary number.
#define SERVO_TASKID     2
#define DC_TASKID        3
#define UART_TASKID      4

// User task declaration.
static void ServoTask(void* arg);
static void DcTask(void* arg);
//-------------------end  USER setting area-----------------------------------


typedef struct tagCOMMAND_STRUCT {
  char   CMD;       
  char   Reserved;  //0xFF
  char   SubCMD;    //
  char   length;    //payload length
  char   payload[16];  // payload
} COMMAND_STRUCT, *PCOMMAND_STRUCT;


typedef struct tagCMD_PKT {
  char  SendID;
  COMMAND_STRUCT cmdpkt;
} CMD_PKT, *PCMD_PKT;

typedef struct tagSYNC_PKT {
  char  Status; // 1:ok, 0: fail
  char  length; // data length
  char data[16]; // max 256 ?
} SYNC_PKT, *PSYNC_PKT;


typedef struct tagPRTCL {
   char  SendID;
   char pkt;
} PROTOCAL_PKT, PPROTOCAL_PKT;

#endif
