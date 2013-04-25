#include <haroid.h>
#include "RoseState.h"


 xQueueHandle hndApplauseQueue;

extern ER (* const Haroid_FuncTbl[255])(PCMD_PKT src);

ER vDcStop(PCMD_PKT src)
{

}


ER vDcForward(PCMD_PKT src)
{
     char val;
     val = src->cmd.payload[0];
     
     digitalWrite(9, LOW);
     analogWrite(10, val);
     vTaskDelay(150 / portTICK_RATE_MS);
     digitalWrite(10, LOW);
//     taskYIELD();

}

ER vDcReverse(PCMD_PKT src)
{
      char val;
      val = src->cmd.payload[0];

      digitalWrite(9, HIGH);
      analogWrite(10,val);
      vTaskDelay(150 / portTICK_RATE_MS);
      digitalWrite(10, HIGH);
 //     taskYIELD();
}


CRoseManager roseMgr;
portCHAR  ApplauseCount = 0;

// debounce code : http://arduino.cc/en/Tutorial/Debounce
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 300;    // the debounce time; increase if the output flickers
 

void DetectApplauseIsr()
{
  portBASE_TYPE xTaskWokenByPost;
  //cli();
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
      ++ApplauseCount;
      lastDebounceTime = millis();
  }
     
  //sei();
  if(ApplauseCount == 1) {
       xTaskWokenByPost = pdFALSE;
       xQueueSendFromISR( hndApplauseQueue, &ApplauseCount,(signed char*) &xTaskWokenByPost );
  }
  
 
}




static void RoseTask(void* arg) {
     static char count;
     static MSG_STATUS st;
  
     pinMode(2,INPUT);
     hndApplauseQueue = xQueueCreate( 1, sizeof(portCHAR));
     
     while(1) {
 
         if(roseMgr.m_pState->GetState() == ROSE_SLEEP_S )
         {     

           attachInterrupt(0, DetectApplauseIsr, FALLING);
           st = xQueueReceive(hndApplauseQueue, (char*) &count ,portMAX_DELAY );
           if(st == pdTRUE) {
             st = xQueueReceive(hndApplauseQueue, (char*) &count ,( 3000 / portTICK_RATE_MS));
             roseMgr.m_pState->Do(ApplauseCount);
           }
           
           detachInterrupt(0); 
           ApplauseCount = 0;
           
         } else if(roseMgr.m_pState->GetState() == ROSE_READY_S) {
           
           attachInterrupt(0, DetectApplauseIsr, FALLING);
           st = xQueueReceive(hndApplauseQueue, (char*) &count , ( 1000 / portTICK_RATE_MS));
           if(st == pdTRUE) {
               st = xQueueReceive(hndApplauseQueue, (char*) &count ,( 3000 / portTICK_RATE_MS));
               roseMgr.m_pState->Do(ApplauseCount); 
         } else {
               roseMgr.m_pState->Do(0);
           }
           
           detachInterrupt(0); 
           ApplauseCount = 0;
           
         } else {
           roseMgr.m_pState->Do(0);
         }      
       
          // vTaskDelay(500 / portTICK_RATE_MS);
     } 
}

#if 0
static void RoseTask(void* arg) {
          CMD_PKT dcpkt;
          BYTE flag=0;
          BYTE val;
          BYTE val2=0;

  
  	  MSG_STATUS ret;
          	  
          
    	 while(1)
	 { 
  
              //val2 = analogRead(0);
               val2 = 200;
             // wrbPutbyte(val2);
              if(val2 > 100)
              {
  
                
                         HaroidIoControl(ME,
                                               DC_TASKID,
                                               0x20, 
                                               NULL,
                                               0,
                                               NULL,
                                               0,
                                               &val,
                                               NOSYNC);
               
                            HaroidIoControl(ME,
                                               DC_TASKID,
                                               0x22, 
                                               NULL,
                                               0,
                                               NULL,
                                               0,
                                               &val,
                                               NOSYNC);    
 
                            HaroidIoControl(ME,
                                               DC_TASKID,
                                               0x24, 
                                               NULL,
                                               0,
                                               NULL,
                                               0,
                                               &val,
                                               NOSYNC);    
 
                                        
                                         
                                         HaroidIoControl(YOU,
                                               DC_TASKID,
                                               0x20, 
                                               NULL,
                                               0,
                                               NULL,
                                               0,
                                               &val,
                                               NOSYNC);             
                                               
                              
                                ret =  ReceiveMessage(DC_TASKID, &dcpkt, 10000);
                                if(ret == pdTRUE)
                                {
                                                                          HaroidIoControl(ME,
                                                           DC_TASKID,
                                                           0x20, 
                                                           NULL,
                                                           0,
                                                           NULL,
                                                           0,
                                                           &val,
                                                           NOSYNC);   
                                                           
                                                                                                   HaroidIoControl(ME,
                                                           DC_TASKID,
                                                           0x20, 
                                                           NULL,
                                                           0,
                                                           NULL,
                                                           0,
                                                           &val,
                                                           NOSYNC);   
                                                           
                                                                                                   HaroidIoControl(ME,
                                                           DC_TASKID,
                                                           0x20, 
                                                           NULL,
                                                           0,
                                                           NULL,
                                                           0,
                                                           &val,
                                                           NOSYNC);   
                                  
                                      //    wrbPutbyte(0x18);
                                } else {
                                      //    wrbPutbyte(0x28);
                                              // Buzer sound
                                   //     DEBUG("Pang wait time out");
                                        HaroidIoControl(ME,
                                                           DC_TASKID,
                                                           0x20, 
                                                           NULL,
                                                           0,
                                                           NULL,
                                                           0,
                                                           &val,
                                                           NOSYNC);    
                                                           
                                        HaroidIoControl(ME,
                                                       DC_TASKID,
                                                       0x21, 
                                                       NULL,
                                                       0,
                                                       NULL,
                                                       0,
                                                       &val,
                                                       NOSYNC);
                                    
                                    vTaskDelay(5000/portTICK_RATE_MS);                   
                                }    
              }
   
/*   
              flag = 0;   
              if(flag == 1)
              {
               
              HaroidIoControl(ME,
                                 DC_TASKID,
                                 0x20, 
                                 NULL,
                                 0,
                                 NULL,
                                 0,
                                 &val,
                                 NOSYNC);
                                 
                                                         
              } else {
                
                        HaroidIoControl(ME,
                                 DC_TASKID,
                                 0x23, 
                                 NULL ,
                                 0,
                                 (BYTE*)&val2,
                                 1,
                                 &val,
                                 SYNC);
                                 
                                 wrbPutbyte(val2);
                                 
                       if(val2 < 30 && val2 > 0) {
                                // Buzer sound
                                HaroidIoControl(ME,
                                                   DC_TASKID,
                                                   0x20, 
                                                   NULL,
                                                   0,
                                                   NULL,
                                                   0,
                                                   &val,
                                                   NOSYNC);
                                // Servo UP!!!
                                
                                HaroidIoControl(ME,
                                                   DC_TASKID,
                                                   0x24, 
                                                   NULL,
                                                   0,
                                                   NULL,
                                                   0,
                                                   &val,
                                                   NOSYNC);
                       }
              }                       
/*
                                ret =  ReceiveSyncMessage(DC_TASKID, &s_struct, 5000);
                                if(ret == pdTRUE)
                                {
                                  wrbPutbyte(0x18);
                                } else {
                                  wrbPutbyte(0x28);
                                      // Buzer sound
                                HaroidIoControl(ME,
                                                   DC_TASKID,
                                                   0x20, 
                                                   NULL,
                                                   0,
                                                   NULL,
                                                   0,
                                                   &val,
                                                   NOSYNC);                            
                                  
                                     HaroidIoControl(ME,
                                                       DC_TASKID,
                                                       0x22, 
                                                       NULL,
                                                       0,
                                                       NULL,
                                                       0,
                                                       &val,
                                                       NOSYNC);
                                                       
                                }                       
 
 
           
                       }              
                       
   
                       
                   
              }
           */   
	      /*
                  ret = ReceiveMessage(DC_TASKID, &dcpkt, portMAX_DELAY);
	          if(ret == pdTRUE)
	          {
                        val = dcpkt.SubCMD;
                        Haroid_FuncTbl[val](&dcpkt);  
		        //vTaskDelay(80 / portTICK_RATE_MS);
                       taskYIELD();
	  	   }
              */

              vTaskDelay(1000/portTICK_RATE_MS);

	 } 
}

#endif                               

