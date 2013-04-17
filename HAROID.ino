#include "haroid.h"         // HAROID structure
#include "Parser.h"
#include "Queue2.h"
#include <Servo.h>
//#include <stdlib.h>
#include <SoftwareSerial.h>


// declare & initialize IO
const uint8_t LED_PIN = LED;            // setting LED
const uint8_t SPEAKER_PIN = SPEAKER;    // setting buzer

const int bluetoothTx = BLUETOOTH_TX;   // setting bluetooth tx(bluetooth side) 
const int bluetoothRx = BLUETOOTH_RX;   // setting bluetooth rx(bluetooth side)
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
//xSemaphoreHandle xSemaphore;

 
MSG_STATUS HaroidIoControl(TO who,
                     ID id,
                     BYTE byIoControlCode, 
                     BYTE* pInBuffer,
                     BYTE nInBufferSize,
                     BYTE* pOutBuffer,
                     BYTE nOutBufferSize,
                     BYTE* nBytesReturned,
                     SYNCTYPE syntype) {

     static  CMD_PKT HCcmd;
     static  SYNC_STRUCT HCsyncSt;
     static  MSG_STATUS HCret;
                       
     BYTE i;
     
     HCcmd.cmd.CMD.BIT.sync = (syntype == SYNC) ? 1 : 0;
     HCcmd.cmd.Reserved = 0xFF;
     HCcmd.cmd.SubCMD =  byIoControlCode;
     HCcmd.cmd.length = nInBufferSize;
     if(nInBufferSize != 0)
       memcpy(HCcmd.cmd.payload, (BYTE*)pInBuffer, nInBufferSize);
     
     if(who == ME) {
       HCret = SendMessage(id, &HCcmd);
   
       if(syntype == SYNC) {
           HCret = ReceiveSyncMessage(id, &HCsyncSt, 100);
           if(HCret == pdTRUE) {
               if(HCsyncSt.Status == 0) {  // fail
                   HCret = pdFALSE;
               } else { // true
                   *nBytesReturned = (nOutBufferSize > HCsyncSt.length) ? HCsyncSt.length : nOutBufferSize;
                   memcpy(pOutBuffer,HCsyncSt.data,*nBytesReturned);
                   
                   HCret = pdTRUE;
               }
               
           }
           
       } // if(syntype == SYNC)
     
    } 
    
    else { // outside 
        // wrbPutbyte
        wrbPutbyte(0xFF);
        wrbPutbyte(*((BYTE*)(&HCcmd.cmd.CMD)));
        wrbPutbyte(HCcmd.cmd.Reserved);
        wrbPutbyte(HCcmd.cmd.SubCMD);
        wrbPutbyte(HCcmd.cmd.length);
        for(i=0;i<HCcmd.cmd.length;i++) {
          wrbPutbyte(HCcmd.cmd.payload[i]);
        }
        wrbPutbyte(0xFF);
    }    
      
     // Command Packing
     
                       
     return HCret;                      
}


// declare TASK QUEUE
xQueueHandle hndQueue[TASK_COUNT];
xQueueHandle hndSyncQueue[TASK_COUNT];

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
  const char command[6]={0xFF,0x0E,0xFF,0x20,0x00,0xFF};
  COMMAND_STRUCT cntrCMD;
  CMD_PKT cmdPkt;
  PROTOCAL_PKT ptPkt;  

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

//CMD_PKT cmdSndPkt;

MSG_STATUS  SendMessage(ID id, PCMD_PKT cmdpkt)
{
 //  int  rcvID;
   BYTE  toSendID;
   MSG_STATUS ret;
//  if( xSemaphore != NULL ) {

 //        if(xSemaphoreTake(xSemaphore,10/portTICK_RATE_MS) == pdTRUE)
 //        {
              // cmdSndPkt.SendID = PROTOCAL_TASKID;
              // cmdSndPkt.cmd = *cmd;
               taskENTER_CRITICAL();
               ret = pdFALSE;
               
               cmdpkt->SendID = id;
               toSendID = ((cmdpkt->cmd.SubCMD) >> 4) & 0x0F;
               taskEXIT_CRITICAL() ;



               ret = xQueueSend(hndQueue[toSendID], cmdpkt, 0 / portTICK_RATE_MS);
             
               if(ret == pdFALSE)
               {
                   wrbPutbyte(0xFF);
               }
          
   
            
   //             xSemaphoreGive( xSemaphore );  
 //        }      
 //  }
   return ret;
}

MSG_STATUS ReceiveMessage(ID id, PCMD_PKT pkt, int ms)
{
   return xQueueReceive(hndQueue[id], (PCMD_PKT) pkt,( ms / portTICK_RATE_MS)); 
}

MSG_STATUS ReceiveSyncMessage(ID id, PSYNC_STRUCT pkt, int ms)
{
   return xQueueReceive(hndSyncQueue[id], (PSYNC_STRUCT) pkt,( ms / portTICK_RATE_MS)); 
}

MSG_STATUS CompleteSyncMessage(ID id, PSYNC_STRUCT pkt)
{
  return xQueueSend(hndSyncQueue[id], pkt, 0 / portTICK_RATE_MS);
}




static void Protocal_TASK(void* arg) {
  static PARSER_STATUS pret; 
  static PROTOCAL_PKT ptpkt;
  static CMD_PKT cmdpkt;
  static SYNC_STRUCT ptsyncSt;
  static MSG_STATUS ptret;
 
  byte pk;
  portBASE_TYPE ret;
  BYTE nSize;
  BYTE i;


 while(1)
 {
   ret = xQueueReceive(hndQueue[PROTOCAL_TASKID], &ptpkt,( 5000 / portTICK_RATE_MS));
   if(ret == pdTRUE)
   {

       if(rdbGetbyte((char*)&pk) == TRUE)
       {
           pret = Update(pk);
           if(pret == COMPLETE_S) {
                 cmdpkt.SendID = PROTOCAL_TASKID;
                 cmdpkt.cmd = GetCommand();
            
                 SendMessage(PROTOCAL_TASKID, &cmdpkt);
                 if(cmdpkt.cmd.CMD.BIT.sync == 1) {
                     ptret = ReceiveSyncMessage(PROTOCAL_TASKID, &ptsyncSt, 1000);
  
                     if(ptret == pdTRUE) {
                    
                             wrbPutbyte(0xFF);                      // prefix
                             if(ptsyncSt.Status == 1)
                             {
                                wrbPutbyte(0x01);                   // status
                                wrbPutbyte(ptsyncSt.length);        // length
                                for(i=0;i<ptsyncSt.length;i++) {    //data
                                  wrbPutbyte(ptsyncSt.data[i]);
                                }
                             } else {
                               wrbPutbyte(0x00);                   // status
                             }
                             
                             wrbPutbyte(0xFF);                     // postfix
                             
                       
                         
                     } else {
                       
                            wrbPutbyte(0xFF);
                            wrbPutbyte(0x00);
                            wrbPutbyte(0xFF);  
                     }
                     
                 } // if(syntype == SYNC)

  
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


        pinMode(2, OUTPUT); 
        pinMode(3, INPUT); 


  //xSemaphore = xSemaphoreCreateMutex();

 // pinMode(DC_MOTOR1, OUTPUT);
 // pinMode(DC_MOTOR2, OUTPUT);
 // digitalWrite(DC_MOTOR1, LOW);
 // digitalWrite(DC_MOTOR2, LOW);


  // CAUTION!!!! stack size is very small. so you need a heap approach.
//  s1 = xTaskCreate(BlueTooth_TASK, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  s2 = xTaskCreate(ServoTask, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  s3 = xTaskCreate(DcTask, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  s4 = xTaskCreate(Protocal_TASK, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);  
  s5 = xTaskCreate(UART_TASK, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);  
  
 //   hndQueue[BLUTOOTH_TASKID] = xQueueCreate( 5, sizeof( CMD_PKT ) );
//    if(hndQueue[BLUTOOTH_TASKID] == 0) {}
    hndQueue[PROTOCAL_TASKID] = xQueueCreate( 5, sizeof( PROTOCAL_PKT) );
    hndSyncQueue[PROTOCAL_TASKID] = xQueueCreate( 1, sizeof(SYNC_STRUCT));
    if(hndQueue[PROTOCAL_TASKID] == 0) {}
    if(hndSyncQueue[PROTOCAL_TASKID] == 0) {}
    
    hndQueue[SERVO_TASKID] = xQueueCreate( 5, sizeof( CMD_PKT ) );
    hndSyncQueue[SERVO_TASKID] = xQueueCreate( 1, sizeof(SYNC_STRUCT) );
    if(hndQueue[SERVO_TASKID] == 0) {}
    if(hndSyncQueue[SERVO_TASKID] == 0) {}

    hndQueue[DC_TASKID] = xQueueCreate( 5, sizeof( CMD_PKT) );
    hndSyncQueue[DC_TASKID] = xQueueCreate( 1, sizeof( SYNC_STRUCT) );
    if(hndQueue[DC_TASKID] == 0) {}
    if(hndSyncQueue[DC_TASKID] == 0) {}

     hndQueue[UART_TASKID] = xQueueCreate( 5, sizeof( CMD_PKT ) );
     hndSyncQueue[UART_TASKID] = xQueueCreate( 1, sizeof( SYNC_STRUCT ) );
    if(hndQueue[UART_TASKID] == 0) {}   
    if(hndSyncQueue[UART_TASKID] == 0) {}   

    
    
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
