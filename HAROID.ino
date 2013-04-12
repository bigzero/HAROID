#include "haroid.h"         // HAROID structure
#include "Parser.h"
#include "Queue2.h"
#include <Servo.h>
#include <SoftwareSerial.h>


// declare & initialize IO
const uint8_t LED_PIN = LED;            // setting LED
const uint8_t SPEAKER_PIN = SPEAKER;    // setting buzer

const int bluetoothTx = BLUETOOTH_TX;   // setting bluetooth tx(bluetooth side) 
const int bluetoothRx = BLUETOOTH_RX;   // setting bluetooth rx(bluetooth side)
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);



// declare TASK QUEUE
xQueueHandle hndQueue[TASK_COUNT];

static void UART_TASK(void* arg) {
  //char buf[11]={0,};
  int i=0;
  int j=0;
//  int cmdParseDone=0;
  byte toRcv;
//  COMMAND_STRUCT cntrCMD;
//  CMD_PKT cmdPkt;
  PROTOCAL_PKT ptPkt;

  while (1) {

    if(Serial.available())
    {
      char toSend = (char)Serial.read();
      rdbPutbyte(toSend);
      //wrbPutbyte(toSend);
      xQueueSend(hndQueue[PROTOCAL_TASKID],&ptPkt, 0 / portTICK_RATE_MS);
    }
     
    if(wrbGetbyte((char*)&toRcv))
    {
       Serial.write(toRcv);
    }  
    vTaskDelay(0L / portTICK_RATE_MS);
  }
 }

static void BlueTooth_TASK(void* arg) {
  //char buf[11]={0,};
  int i=0;
  int j=0;
  int cmdParseDone=0;
  byte toRcv;
  COMMAND_STRUCT cntrCMD;
  CMD_PKT cmdPkt;
  PROTOCAL_PKT ptPkt;
  const char command[6]={0xFF,0x0E,0xFF,0x20,0x00,0xFF};
  

  while(1) {
    vTaskDelay(3000L / portTICK_RATE_MS);
/*    
    for(i=0;i<6;i++) {
      if(Update2(command[i]) == COMPLETE_S) {
         COMMAND_STRUCT cmd = GetCommand2();
         SendMessage(&cmd); 
      }
    }
*/
  }
/*
  while (1) {

    if(bluetooth.available())
    {
      char toSend = (char)bluetooth.read();
      rdbPutbyte(toSend);
      xQueueSend(hndQueue[PROTOCAL_TASKID],&ptPkt, 20 / portTICK_RATE_MS);
      
    }
     
   if(wrbGetbyte((char*)&toRcv))
     {
       bluetooth.write(toRcv);
     }   
     
 
    vTaskDelay(0L / portTICK_RATE_MS);
  }
 */ 
}


CMD_PKT cmdSndPkt;
void  SendMessage(PCOMMAND_STRUCT cmd)
{
   int  rcvID;
   int  toSendID;
   portBASE_TYPE ret;
   
   cmdSndPkt.SendID = PROTOCAL_TASKID;
   cmdSndPkt.cmdpkt = *cmd;
   
//   is_SYNC = (cmd.CMD & 0x01);
   toSendID = ((*cmd).SubCMD >> 4) & 0x0F;
   wrbPutbyte(toSendID); 
   ret = xQueueSend(hndQueue[toSendID],&cmdSndPkt, 0 / portTICK_RATE_MS);
   if(ret == pdFALSE)
   {
       wrbPutbyte(0xFF);
   }  
}

portBASE_TYPE ReceiveMessage(int rcvID, PCMD_PKT pkt, int ms)
{
   return xQueueReceive(hndQueue[rcvID], (PCMD_PKT) pkt,( ms / portTICK_RATE_MS)); 
}

/*
portBASE_TYPE ReceiveMessageFromSerial(int rcvID, PPROTOCAL_PKT pkt, int ms)
{
   return xQueueReceive(hndQueue[rcvID], (PPROTOCAL_PKT) pkt,( ms / portTICK_RATE_MS)); 
}
*/

portBASE_TYPE ReceiveMessageAtProtocal(int rcvID, PSYNC_PKT pkt, int ms)
{
   return (portBASE_TYPE)xQueueReceive(hndQueue[rcvID], (PSYNC_PKT) pkt,( portTickType )( ms / portTICK_RATE_MS)); 
}


/*
byte cmdPacket[21];   
COMMAND_STRUCT stCmdPkt;
SYNC_PKT sync_pkt;
CMD_PKT cmdPkt;
*/

 PROTOCAL_PKT ptpkt; 
 PARSER_STATUS pret; 
 COMMAND_STRUCT cmd;

static void Protocal_TASK(void* arg) {
 byte pk;
 portBASE_TYPE ret;
 
 while(1)
 {
   //ret = xSemaphoreTake(semaFireReceiveCommand,50 / portTICK_RATE_MS);  //portMAX_DELAY
   //ret = ReceiveMessageFromSerial(PROTOCAL_TASKID, &ptpkt, portMAX_DELAY);
   ret = xQueueReceive(hndQueue[PROTOCAL_TASKID],  &ptpkt,( 5000 / portTICK_RATE_MS));
   //ret = pdTRUE;
   if(ret == pdTRUE)
   {

       if(rdbGetbyte((char*)&pk) == TRUE)
         {
//           wrbPutbyte(pk);
           pret = Update(pk);
           if(pret == COMPLETE_S) {
                 cmd = GetCommand();
                 SendMessage(&cmd);
                 if(cmd.CMD.BIT.sync == 1)
                 {
                    wrbPutbyte(0x11);
                    wrbPutbyte(0x11);
                 } else {
                   
                   
                 }             
           }

         }
  
         
    } else {
       wrbPutbyte(0xAA); 
    }
    vTaskDelay(0L / portTICK_RATE_MS); 
 
 } 
}


void setup() {
  portBASE_TYPE s1, s2, s3, s4, s5;

  Serial.begin(9600);
  delay(100);
  pinMode(SPEAKER_PIN, OUTPUT);
  digitalWrite(SPEAKER_PIN,HIGH);
  bluetooth.begin(9600);  

  pinMode(DC_MOTOR1, OUTPUT);
  pinMode(DC_MOTOR2, OUTPUT);
  digitalWrite(DC_MOTOR1, LOW);
  digitalWrite(DC_MOTOR2, LOW);


  // CAUTION!!!! stack size is very small. so you need a heap approach.
//  s1 = xTaskCreate(BlueTooth_TASK, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  s2 = xTaskCreate(ServoTask, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  s3 = xTaskCreate(DcTask, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  s4 = xTaskCreate(Protocal_TASK, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);  
  s5 = xTaskCreate(UART_TASK, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);  
  
    hndQueue[BLUTOOTH_TASKID] = xQueueCreate( 10, sizeof( CMD_PKT ) );
    if(hndQueue[BLUTOOTH_TASKID] == 0) {}
    hndQueue[PROTOCAL_TASKID] = xQueueCreate( 10, sizeof( PROTOCAL_PKT) );
    if(hndQueue[PROTOCAL_TASKID] == 0) {}
    hndQueue[SERVO_TASKID] = xQueueCreate( 10, sizeof( CMD_PKT ) );
    if(hndQueue[SERVO_TASKID] == 0) {}
    hndQueue[DC_TASKID] = xQueueCreate( 10, sizeof( CMD_PKT) );
    if(hndQueue[DC_TASKID] == 0) {}
     hndQueue[UART_TASKID] = xQueueCreate( 10, sizeof( CMD_PKT ) );
    if(hndQueue[UART_TASKID] == 0) {}   

    
    
  // Queue Initialize
  rdbClear();
  wrbClear();



  // start scheduler
  vTaskStartScheduler();
  Serial.println(F("Insufficient RAM"));
  while(1);
}

//------------------------------------------------------------------------------
// WARNING idle loop has a very small stack (configMINIMAL_STACK_SIZE)
// loop must never block
void loop() {
  // Not used.
}
