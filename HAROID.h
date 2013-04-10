#ifndef __HAROID_H__
#define __HAROID_H__

#define MEM_USE_POOLS

#include <FreeRTOS_AVR.h>
#include "haroid_config.h"


typedef int                     ER;
typedef bool                    BOOL;
typedef unsigned int	        BITFLD;
typedef unsigned char	        BYTE;				// unsigned  8bit value
typedef unsigned short	        WORD;				// unsigned 16bit value
typedef unsigned long	        DWORD;				// unsigned 32bit value
typedef	void			VOID,*PVOID;		// alias for VOID
typedef signed char		SBYTE;				// signed  8bit value
typedef signed short	        SWORD;				// signed 16bit value
typedef signed long		SDWORD;				// signed 32bit value
typedef SDWORD			FIXED;				// fixed point


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

typedef union tagW_DEVICE_DATA {
  char data[4];
  struct {
    WORD word1;
    WORD word2;
  } WORD_DATA;
  struct {
    DWORD dword;
  } DWORD_DATA;
};


typedef	 union	{							//Command structure
		BYTE	cmd;							//BYTE access
		 struct	{						//it access
			BITFLD	sync:1;						//sync command bit
			BITFLD	wr:1;						//
			BITFLD	control:1;					// control(1), data(0) command
			BITFLD	command:1;			                // command(1), response(0) packet
			BITFLD	:1;						//Samusung original  Atapi status
			BITFLD	:1;						//Device fault bit
			BITFLD	:1;						//Device ready bit
			BITFLD	:1;						//Busy bit
		} BIT;
	 
} T_CMD;

typedef struct tagW_DEVICE_STRUCT {
  T_CMD   CMD;       
  char   Reserved;  //0xFF
  char   SubCMD;    //
  char   length;    // 5,6,8
  unsigned int addr;  // payload
  unsigned int data;  
} W_DEVICE_STRUCT, *PW_DEVICE_STRUCT;

typedef struct tagR_DEVICE_STRUCT {
  T_CMD   CMD;       
  char   Reserved;  //0xFF
  char   SubCMD;    //
  char   length;    // 8
  unsigned int addr;  // payload
  unsigned int len;   // 1,2,4
} R_DEVICE_STRUCT, *PR_DEVICE_STRUCT;



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

typedef enum  STATE_ID {PREFIX_S, CMD_S, RSVD_S,SUBCMD_S,LENGTH_S,PAYLOAD_S, POSTFIX_S, COMPLETE_S, FAIL_S} PARSER_STATUS;

extern "C" PARSER_STATUS Update(char pk) ;
extern "C" COMMAND_STRUCT GetCommand(void);

extern "C" PARSER_STATUS Update2(char pk);
extern "C" COMMAND_STRUCT GetCommand2(void);

// decalre a message function.
extern "C" void  SendMessage(PCOMMAND_STRUCT cmd);
portBASE_TYPE ReceiveMessage(int rcvID, PCMD_PKT pkt, int ms);
portBASE_TYPE ReceiveMessageFromSerial(int rcvID, PPROTOCAL_PKT pkt, int ms);
portBASE_TYPE ReceiveMessageAtProtocal(int rcvID, PSYNC_PKT pkt, int ms);



#define	ReadDEV_1B(ADDR)			(*((volatile BYTE *)(ADDR)))
#define	ReadDEV_2B(ADDR)			(*((volatile WORD *)(ADDR)))
#define	ReadDEV_4B(ADDR)			(*((volatile DWORD*)(ADDR)))

#define WriteDEV_1B(ADDR,DATA)		((*(volatile BYTE *)(ADDR)) = (DATA))
#define WriteDEV_2B(ADDR,DATA)		((*(volatile WORD *)(ADDR)) = (DATA))
#define WriteDEV_4B(ADDR,DATA)		((*(volatile DWORD*)(ADDR)) = (DATA))

#define GetBitsDEV_1B(ADDR,MASK)	(*(volatile BYTE *)(ADDR) & (BYTE)(MASK))
#define SetBitsDEV_1B(ADDR,MASK)	(*(volatile BYTE *)(ADDR) |= (BYTE)(MASK))
#define ClrBitsDEV_1B(ADDR,MASK)	(*(volatile BYTE *)(ADDR) &= (BYTE)(~(MASK)))
#define SetBitsDEV_4B(ADDR,MASK)	(*(volatile DWORD *)(ADDR) |= (DWORD)(MASK))
#define ClrBitsDEV_4B(ADDR,MASK)	(*(volatile DWORD *)(ADDR) &= (DWORD)(~(MASK)))

#define ReadBitsDEV_1B(ADDR, MASK)   ((*(volatile BYTE *)(ADDR)) & (BYTE)(MASK))
#define MaskSetDEV_1B(ADDR, MASK, DATA) 	(*(volatile BYTE *)(ADDR) = ((*(volatile BYTE *)(ADDR)) & ((BYTE)(~(MASK)))) | (DATA)) 

///////////////////////////////////////////////////////////////////////////////
//		Bit define.
///////////////////////////////////////////////////////////////////////////////
#define	b15		(0x8000)				// Bit15.
#define	b14		(0x4000)				// Bit14.
#define	b13		(0x2000)				// Bit13.
#define	b12		(0x1000)				// Bit12.
#define	b11		(0x0800)				// Bit11.
#define	b10		(0x0400)				// Bit10.
#define	b9		(0x0200)				// Bit9.
#define	b8		(0x0100)				// Bit8.
#define	b7		(0x80)					// Bit7.
#define	b6		(0x40)					// Bit6.
#define	b5		(0x20)					// Bit5.
#define	b4		(0x10)					// Bit4.
#define	b3		(0x08)					// Bit3.
#define	b2		(0x04)					// Bit2.
#define	b1		(0x02)					// Bit1.
#define	b0		(0x01)					// Bit0.


#endif
