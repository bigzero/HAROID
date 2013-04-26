#include <haroid.h>
#include "RoseState.h"


xQueueHandle hndApplauseQueue;

extern ER (* const Haroid_FuncTbl[255])(PCMD_PKT src);


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

  if(xTaskWokenByPost) {
    portYIELD_FROM_ISR();
  }  
 
}


extern const char RosePersonality[6][6];
extern void RosePersonalityRequest(const char *rose);

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
             st = xQueueReceive(hndApplauseQueue, (char*) &count ,( 4000 / portTICK_RATE_MS));
             roseMgr.m_pState->Do(ApplauseCount);
           }
           
           detachInterrupt(0); 
           ApplauseCount = 0;
           
         } else if(roseMgr.m_pState->GetState() == ROSE_READY_S) {
           
           attachInterrupt(0, DetectApplauseIsr, FALLING);
           st = xQueueReceive(hndApplauseQueue, (char*) &count , ( 12000 / portTICK_RATE_MS));
           if(st == pdTRUE) {
               st = xQueueReceive(hndApplauseQueue, (char*) &count ,( 4000 / portTICK_RATE_MS));
               roseMgr.m_pState->Do(ApplauseCount); 
         } else {
               roseMgr.m_pState->Do(0);
               RosePersonalityRequest(RosePersonality[2]);
           }
           
           detachInterrupt(0); 
           ApplauseCount = 0;
           
         } else {
           roseMgr.m_pState->Do(0);
         }      
       
          // vTaskDelay(500 / portTICK_RATE_MS);
     } 
}

