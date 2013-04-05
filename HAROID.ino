#include <FreeRTOS_AVR.h>
#include <SoftwareSerial.h>
#include "haroid_config.h"  // HAROID configuration
#include "haroid.h"         // HAROID structure
#include "Parser.h"
#include <Servo.h>


// declare & initialize IO
const uint8_t LED_PIN = LED;            // setting LED
const uint8_t SPEAKER_PIN = SPEAKER;    // setting buzer

const int bluetoothTx = BLUETOOTH_TX;   // setting bluetooth tx(bluetooth side) 
const int bluetoothRx = BLUETOOTH_RX;   // setting bluetooth rx(bluetooth side)
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);



// declare TASK QUEUE
xQueueHandle hndQueue[TASK_COUNT];

// decalre a message function.
void  SendMessage(PCOMMAND_STRUCT cmd);
portBASE_TYPE ReceiveMessage(int rcvID, PCMD_PKT pkt, int ms);
portBASE_TYPE ReceiveMessageFromSerial(int rcvID, PPROTOCAL_PKT pkt, int ms);
portBASE_TYPE ReceiveMessageAtProtocal(int rcvID, PSYNC_PKT pkt, int ms);



static void UART_TASK(void* arg) {
  //char buf[11]={0,};
  int i=0;
  int j=0;
  int cmdParseDone=0;
  byte toRcv;
  COMMAND_STRUCT cntrCMD;
  CMD_PKT cmdPkt;
  PROTOCAL_PKT ptPkt;

  while (1) {

    if(Serial.available())
    {
      char toSend = (char)Serial.read();
      rdbPutbyte(toSend);
      xQueueSend(hndQueue[PROTOCAL_TASKID],&ptPkt, 20 / portTICK_RATE_MS);
    }
     
    if(wrbGetbyte(&toRcv))
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


  while (1) {

    if(bluetooth.available())
    {
      char toSend = (char)bluetooth.read();
      rdbPutbyte(toSend);
      xQueueSend(hndQueue[PROTOCAL_TASKID],&ptPkt, 20 / portTICK_RATE_MS);
      
    }
     
   if(wrbGetbyte(&toRcv))
     {
       bluetooth.write(toRcv);
     }   
     
 
    vTaskDelay(0L / portTICK_RATE_MS);
  }
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



byte cmdPacket[21];   
COMMAND_STRUCT stCmdPkt;
SYNC_PKT sync_pkt;
CMD_PKT cmdPkt;
CParser parser;

void TEST(void) {
   byte pk;
   portBASE_TYPE ret;
   PROTOCAL_PKT ptpkt; 
   
   while(1)
   {
        ret = xQueueReceive(hndQueue[PROTOCAL_TASKID],  &ptpkt,( 50 / portTICK_RATE_MS));
        if(ret == pdTRUE)
        {
               parser.Update(pk);  
         
        } else
        {
          
        }

      
     
   }
}


static void Protocal_TASK(void* arg) {
 byte pk;
 int  cmdPacketSize =0;
 int  cmdPacketPaserState = 0;
 int  cmdDataLength = 0;
 int  cmdDataCount = 0;
 int  cmdResetCount = 0;

 portBASE_TYPE ret;
 byte temp;
 byte  isSYNC;
 int i;
 int post_fix;
 int Status ; 
 int  rcvID;
 int  toSendID; 

 PROTOCAL_PKT ptpkt; 
 
 while(1)
 {
   //ret = xSemaphoreTake(semaFireReceiveCommand,50 / portTICK_RATE_MS);  //portMAX_DELAY
   //ret = ReceiveMessageFromSerial(PROTOCAL_TASKID, &ptpkt, portMAX_DELAY);
   ret = xQueueReceive(hndQueue[rcvID],  &ptpkt,( 50 / portTICK_RATE_MS));
   if(ret == pdTRUE)
   {
         if(rdbGetbyte(&pk) == TRUE)
         {
           
//             wrbPutbyte(pk); 
     
             switch(cmdPacketPaserState)
             {
                case 0 : // prefix
                          if( pk == 0xFF )
                              cmdPacketPaserState = 1;
                          break;
                case 1 : // cmd
                      //    digitalWrite(SPEAKER,HIGH);
                          if( pk == 0xFF  )  // Reset command?
                          {
                              cmdResetCount = 2;
                              cmdPacketPaserState = 10;
                          } else {
                          
                             cmdPacket[cmdPacketSize++] = pk;
                          
                             cmdPacketPaserState = 2;
                          }
                          break;     
                case 2 : // Reserved
                          if( pk == 0xFF )
                          {
                            cmdPacket[cmdPacketSize++] = pk;
                            cmdPacketPaserState = 3;
                          } else {
                            cmdPacketSize = 0;   // initialize 
                            cmdPacketPaserState = 0;
                             cmdDataCount = 0;
                             cmdPacketSize = 0;
                             cmdResetCount = 0;  
                          }
                          break;
                case 3 : // SubCmd
                  //     digitalWrite(SPEAKER,LOW);
                          cmdPacket[cmdPacketSize++] = pk;
                          cmdPacketPaserState = 4;
                          break;
                case 4 : // length
                    //digitalWrite(SPEAKER,LOW);
                          cmdPacket[cmdPacketSize++] = pk;
                          if(pk == 0) {
                            cmdDataCount = 0;
                            cmdDataLength = 0;
                            cmdPacketPaserState = 6;   // exit
                          } else {
                            cmdDataCount = 0;   // init
                            cmdDataLength = pk;  // length
                            cmdPacketPaserState = 5;   // to read payload 
                          }
                          break;
                case 5 : // payload
                           cmdPacket[cmdPacketSize++] = pk;
                           if(++cmdDataCount >= cmdDataLength)
                              cmdPacketPaserState = 6;   // next state
                           else
                              cmdPacketPaserState = 5;   // loop
                          break;
                case 6 : // post_fix
    
                                
                          temp = cmdDataLength ^ 0xFF;
                         
                          if(pk == temp)
                          {
                             stCmdPkt = (COMMAND_STRUCT)(*(PCOMMAND_STRUCT)cmdPacket); 
                                           
                         
                            //ProcessPacket(&stCmdPkt);
                                          //  digitalWrite(SPEAKER,LOW);
                                        
                                         //SendMessage(&cmdPkt,
                                         //isSYNC = (stCmdPkt.CMD) & 0x01;
                                         isSYNC = 0;                                  
                                          
                                         if(isSYNC == 1)
                                         {
                                          
                                           SendMessage(&stCmdPkt);
                                           ret = ReceiveMessageAtProtocal(PROTOCAL_TASKID, &sync_pkt, portMAX_DELAY);
                                          
                                          
                                           if(ret == pdTRUE)  // sync_pkt success
                                           {
                                               Status = sync_pkt.Status;
                                               if(Status == 1) // success
                                               {
                                                 post_fix = sync_pkt.length ^ 0xFF;
                                                 
                                                 wrbPutbyte(0xFF);
                                                 for(i=0;i<sync_pkt.length;i++)
                                                 {
                                                   wrbPutbyte(sync_pkt.data[i]);
                                                 }
                                                 wrbPutbyte(post_fix);
                                                 
                                               } else {  // fail
                                                 wrbPutbyte(0xFF);
                                                 wrbPutbyte(0x00);
                                                 wrbPutbyte(0xFF);  
                                               }
                                            
                                           } else { // fail
                                               wrbPutbyte(0xFF);
                                               wrbPutbyte(0x00);
                                               wrbPutbyte(0xFF);
                                           }
                                           
                                         } else {
                                              //   digitalWrite(SPEAKER,LOW);                   
                                            SendMessage(&stCmdPkt);
                                                 wrbPutbyte(0x61);
                                               wrbPutbyte(0x62);
                                               wrbPutbyte(0x63);  
                                             //  cmdPkt.SendID = PROTOCAL_TASKID;
                                             //  cmdPkt.cmdpkt = stCmdPkt;
                                               
                                            //   is_SYNC = (cmd.CMD & 0x01);
                                            //   toSendID = (stCmdPkt.SubCMD >> 4) & 0x0F;
                                            //  toSendID = SERVO_TASKID;
                                            //  xQueueSend(hndQueue[toSendID],0 , 50 / portTICK_RATE_MS);  
                                        
                                      
                                     }
              
                            
                            
                            cmdPacketPaserState = 0;
                            cmdDataCount = 0;
                             cmdPacketSize = 0;
                            cmdResetCount = 0;
                          } else {
                            // fail!!!!
                            cmdPacketPaserState = 0;
                            cmdDataCount = 0;
                           cmdPacketSize = 0;
                           cmdResetCount = 0;
                          }
                          break;
                          
                    case 10 : 
                                if(pk == 0xFF)
                                {
                                      if(++cmdResetCount == 6)
                                      {
                                            BOOL bl;
                                            
                                            //bl = wrbPutbyte(0x41);
                                               // vTaskDelay(2L / portTICK_RATE_MS); 
                                            //bl = wrbPutbyte(0x42);
                                               // vTaskDelay(2L / portTICK_RATE_MS); 
                                            //bl = wrbPutbyte(0x43);
                                               // vTaskDelay(2L / portTICK_RATE_MS); 
       
                                            
                                           cmdPacketPaserState = 0;
                                           cmdResetCount = 0;
                                           cmdPacketSize = 0;
                                           cmdResetCount = 0;
                                      } else {
                                           cmdPacketPaserState = 10;
                                      }
                                } else {
                                     cmdPacketPaserState = 0;
                                     cmdResetCount = 0;
                                     cmdPacketSize = 0;
                                     cmdResetCount = 0;
                                }
                                
                                
                               break;
                  }
               
     

           }
   
   } else {
     cmdPacketPaserState = 0;
     cmdDataCount = 0;
     cmdPacketSize = 0;
     cmdResetCount = 0;
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
  s1 = xTaskCreate(BlueTooth_TASK, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
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
