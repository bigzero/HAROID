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
#define BUFF_SIZE (12)


#define E_OK        0       /* Normal Completion */

#define SYNC_CMD_FLAG   1
#define WRITE_CMD_FLAG  2
#define CONTROL_CMD_FLAG 4 

// The number 0,1 is system id. So you dont have to change the number.
#define BLUTOOTH_TASKID  0  // Bluetooth is a default channel.
#define PROTOCAL_TASKID  1  // 

//-------------------begin USER setting area---------------------------------
#define TASK_COUNT 4        // You must set a TASK count.
// from number 2, it is user id. You can set a arbitrary number.
#define SERVO_TASKID     2
#define DC_TASKID        3
#define UART_TASKID      4

typedef char ID;
typedef enum tagTO { ME, YOU } TO;
typedef enum tagSYNCTYPE { NOSYNC, SYNC } SYNCTYPE;

typedef portBASE_TYPE MSG_STATUS;

// User task declaration.
static void ServoTask(void* arg);
static void DcTask(void* arg);
//-------------------end  USER setting area-----------------------------------
typedef	 union	{							//Command structure
		BYTE	cmd;							//BYTE access
		 struct	{						//it access
			BITFLD	sync:1;						//sync command bit
			BITFLD	:1;						//
			BITFLD	:1;					// 
			BITFLD	:1;			                // 
			BITFLD	:1;						// default : 0
			BITFLD	:1;						// default : 0
			BITFLD	:1;						// default : 0
			BITFLD	:1;						// default : 0
		} BIT;
	 
} T_CMD;

typedef	 union	{							//Command structure
		BYTE	cmd;							//BYTE access
		 struct	{						//it access
			BITFLD	stat:1;						// status (1:ok, 0:fail)
			BITFLD	:1;						// default : 0
			BITFLD	:1;					// default : 0
			BITFLD	:1;			                // default : 0
			BITFLD	:1;						// default : 0
			BITFLD	:1;						// default : 0
			BITFLD	:1;						// default : 0
			BITFLD	:1;						// default : 0
		} BIT;
	 
} T_SYNC_CMD;


typedef struct tagCOMMAND_STRUCT {
  T_CMD   CMD;       
  char   Reserved;  //0xFF
  char   SubCMD;    //
  char   length;    //payload length
  char   payload[8];  // payload : max 256 . but you have to care a RAM 2K size.
} COMMAND_STRUCT, *PCOMMAND_STRUCT __attribute__((packed));

typedef struct tagSYNC_STRUCT {
  char  Status; // 1:ok, 0: fail
  char  length; // data length
  char data[4]; // data : max 256 . but you have to care a RAM 2K size.
} SYNC_STRUCT, *PSYNC_STRUCT __attribute__((packed));


typedef union tagW_DEVICE_DATA {
  char data[4];
  struct {
    WORD word1;
    WORD word2;
  } WORD_DATA;
  struct {
    DWORD dword;
  } DWORD_DATA;
} DEVICE_DATA __attribute__((packed));



typedef struct tagW_DEVICE_STRUCT {
  T_CMD   CMD;       
  char   Reserved;  //0xFF
  char   SubCMD;    //
  char   length;    // 5,6,8
  unsigned short int addr;  // payload
  DEVICE_DATA data;  
} W_DEVICE_STRUCT, *PW_DEVICE_STRUCT __attribute__((packed));

typedef  struct tagR_DEVICE_STRUCT {
  T_CMD   CMD;       
  char   Reserved;  //0xFF
  char   SubCMD;    //
  char   length;    // 8
  unsigned short int addr;  // payload
  char len;   // 1,2,4
} R_DEVICE_STRUCT, *PR_DEVICE_STRUCT __attribute__((packed));



typedef struct tagCMD_PKT {
  char  SendID;
  COMMAND_STRUCT cmd;
} CMD_PKT, *PCMD_PKT __attribute__((packed));

typedef struct tagSYNC_PKT {
  char  SendID;
  SYNC_STRUCT syncpkt;
} SYNC_PKT, *PSYNC_PKT __attribute__((packed));






typedef struct tagPRTCL {
   char  SendID;
   char pkt;
} PROTOCAL_PKT, PPROTOCAL_PKT __attribute__((packed));

typedef enum  STATE_ID {PREFIX_S, CMD_S, RSVD_S,SUBCMD_S,LENGTH_S,PAYLOAD_S, POSTFIX_S, COMPLETE_S, FAIL_S,
                                  SYNC_LENGTH_S, SYNC_PAYLOAD_S, SYNC_POSTFIX_S, SYNC_COMPLETE_S, SYNC_FAIL_S } PARSER_STATUS;

extern "C" PARSER_STATUS Update(char pk) ;
extern "C" COMMAND_STRUCT GetCommand(void);

extern "C" PARSER_STATUS Update2(char pk);
extern "C" COMMAND_STRUCT GetCommand2(void);

// decalre a message function.
MSG_STATUS SendMessage(ID id, PCMD_PKT cmd);
MSG_STATUS ReceiveMessage(ID id, PCMD_PKT pkt, int ms);
MSG_STATUS ReceiveSyncMessage(ID id, PSYNC_STRUCT pkt, int ms);
// 
MSG_STATUS CompleteSyncMessage(ID id, PSYNC_STRUCT pkt);

MSG_STATUS HaroidIoControl(TO who,
                     ID id,
                     BYTE byIoControlCode, 
                     BYTE* pInBuffer,
                     BYTE nInBufferSize,
                     BYTE* pOutBuffer,
                     BYTE nOutBufferSize,
                     BYTE* nBytesReturned,
                     SYNCTYPE syntype);

//portBASE_TYPE ReceiveMessageFromSerial(int rcvID, PPROTOCAL_PKT pkt, int ms);
//portBASE_TYPE ReceiveMessageAtProtocal(int rcvID, PSYNC_PKT pkt, int ms);



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
