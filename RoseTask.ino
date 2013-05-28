#include "haroid.h"
#include "RoseState.h"


//xQueueHandle hndHeartbitQueue;
//xQueueHandle hndMissionQueue;

extern ER (* const Haroid_FuncTbl[255])(PCMD_PKT src);


extern const char RosePersonality[6][6];
extern void RosePersonalityRequest(const char *rose);


CRoseManager roseMgr;
/*
ROSE_STATE_ID GetRoseState(void) {
   return roseMgr.m_pState->GetState();
};
*/
static void RoseTask(void* arg) {

  /*  
     pinMode(2,INPUT);
  */
  //   hndHeartbitQueue = xQueueCreate( 1, sizeof(portCHAR));
  //   hndMissionQueue = xQueueCreate(1, sizeof(portCHAR));
     
     
     while(1) {

           roseMgr.m_pState->Do();
       //    DEBUG("Sleep\n");
       
       //    vTaskDelay(100 / portTICK_RATE_MS);
     } 
}

