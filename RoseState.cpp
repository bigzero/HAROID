#include "HAROID.h"
#include "Queue2.h"
#include "RoseState.h"

//extern CRoseManager roseMgr;


extern "C" const char RosePersonality[6][6] = {
  {0xFF,0x0E,0xFF,0x60,0x00,0xFF},
  {0xFF,0x0E,0xFF,0x61,0x00,0xFF},
  {0xFF,0x0E,0xFF,0x62,0x00,0xFF},
  {0xFF,0x0E,0xFF,0x63,0x00,0xFF},
  {0xFF,0x0E,0xFF,0x64,0x00,0xFF},
  {0xFF,0x0E,0xFF,0x65,0x00,0xFF}
};

void RosePersonalityRequest(const char *rose) {
  int i;
  for(i=0;i<6;i++) {
      wrbPutbyte(rose[i]);
  }
};
  
  
  
CRoseState::~CRoseState() 
{
};

ROSE_STATE_ID CRoseState::GetState(void)
{
  return m_stateID;  
};

extern xQueueHandle hndHeartbitQueue;
extern xQueueHandle hndMissionQueue;

/*
portCHAR  ApplauseCount = 0;

 MSG_STATUS st;
 char count;




byte val;
int distance;


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
  if(ApplauseCount > 5) {
       xTaskWokenByPost = pdFALSE;
       xQueueSendFromISR( hndApplauseQueue, &ApplauseCount,(signed char*) &xTaskWokenByPost );
  }

  if(xTaskWokenByPost) {
    portYIELD_FROM_ISR();
  }  
 
}
*/

void BeepPiPi(void)
{
      byte val;
      
                         HaroidIoControl(ME,
                                           ROSE_TASKID,
                                           0x25, 
                                           NULL,
                                           0,
                                           NULL,
                                           0,
                                           &val,
                                           NOSYNC);    

  
  
}


MSG_STATUS Wakeup(void)
{
      byte val;
      
                   return      HaroidIoControl(ME,
                                           ROSE_TASKID,
                                           0x23, 
                                           NULL,
                                           0,
                                           NULL,
                                           0,
                                           &val,
                                           SYNC);    

  
}


MSG_STATUS CdsRecognition(void)
{
      byte val;
      
                   return      HaroidIoControl(ME,
                                           ROSE_TASKID,
                                           0x26, 
                                           NULL,
                                           0,
                                           NULL,
                                           0,
                                           &val,
                                           SYNC);    

  
}

void ServoSleepModeAction(void)
{
      byte val;
      
                         HaroidIoControl(ME,
                                           ROSE_TASKID,
                                           0x20, 
                                           NULL,
                                           0,
                                           NULL,
                                           0,
                                           &val,
                                           SYNC);    
                    
                         
                        // vTaskDelay(5000 / portTICK_RATE_MS);
}

extern void MeasureDistance(void) ;
extern byte FrontObject; 
//char buf[10];

// 
ROSE_STATE_ID CRoseSleepState::Do(void) {
  // wrbPutbyte(no);
   ROSE_STATUS st = ROSE_SLEEP_S;
//   MSG_STATUS ret;

//   DEBUG("Sleep");
   




   //sprintf(buf,"%f",cm2);
//   DEBUG(buf);
//   DEBUG("\n");
    MeasureDistance();
    MeasureDistance();
    MeasureDistance();
    MeasureDistance();
    MeasureDistance();
   
         
   if(FrontObject == 1)
   {
                  FrontObject = 0;
//                  BeepPiPi();

                  //vTaskDelay(1000 / portTICK_RATE_MS);
 
                  
                  m_RoseMgr->RoseState_Ready();
                  st =  ROSE_READY_S;
     
                  // 2step , Applause sensor 
   /*        
                 attachInterrupt(0, DetectApplauseIsr, FALLING);
                 ret = xQueueReceive(hndApplauseQueue, (char*) &count ,portMAX_DELAY );
                 if(ret == pdTRUE) {
 
                        RosePersonalityRequest(RosePersonality[1]);
                         
                        // Up
                        HaroidIoControl(ME,
                                           DC_TASKID,
                                           0x21, 
                                           NULL,
                                           0,
                                           NULL,
                                           0,
                                           &val,
                                           NOSYNC);    
                
                      
                       m_RoseMgr->RoseState_Ready();
                       st = ROSE_READY_S;
     
                 }
                 
                 detachInterrupt(0); 
                 ApplauseCount = 0;
         } else {
           
                        RosePersonalityRequest(RosePersonality[1]);
                         
                        // Down
                        HaroidIoControl(ME,
                                           DC_TASKID,
                                           0x22, 
                                           NULL,
                                           0,
                                           NULL,
                                           0,
                                           &val,
                                           NOSYNC);    
                
                      
                       m_RoseMgr->RoseState_Sleep();
                       st =  ROSE_SLEEP_S;
           
         
         }
 
*/
     } else {
       
         ServoSleepModeAction();
         //vTaskDelay(5000 / portTICK_RATE_MS);

         m_RoseMgr->RoseState_Sleep();
         st =  ROSE_SLEEP_S;
         
       
     }


      return st;

 };
 
 ROSE_STATE_ID CRoseReadyState::Do(void) {
   MSG_STATUS ret;
   char count;
   ROSE_STATUS st = ROSE_READY_S;
//    static int mission = 0;
   
   DEBUG("Ready");
   
    ret = CdsRecognition();   
   //ret = xQueueReceive(hndHeartbitQueue, (char*) &count ,15000 / portTICK_RATE_MS );
   if(ret == pdTRUE) {
         m_RoseMgr->RoseState_Wakeup();
         st =  ROSE_WAKEUP_S;

   } else {
         m_RoseMgr->RoseState_Sleep();
         st =  ROSE_SLEEP_S;
   }
   
           
      
   return st;     
   
 };

ROSE_STATE_ID CRoseWakeupState::Do(void) {
      //Do your job
      MSG_STATUS st;
       
      DEBUG("Wakeup\n");
      
      //Wakeup();    
      
      m_RoseMgr->RoseState_Mission1();
      return ROSE_MISSION_1_S;
 };


extern void FftInit();
extern int Fftloop();

// voice recognition
 ROSE_STATE_ID CRoseMission1State::Do(void) {
      //Do your job
      CMD_PKT rosepkt;
      MSG_STATUS ret;
      ROSE_STATUS st = ROSE_MISSION_1_S;
      byte val;
      int  sex;
       
  //    DEBUG("Mission1\n");
   
      FftInit();
      
        
    // You must send command to phone.
    // Command : who are you?
    // FF 0E FF 60 00 FF  
      
    // Message loop   
    // 1. voice start command
    // 2. voice end command
    while(1)
    {   
            ret = ReceiveMessage(ROSE_TASKID, &rosepkt, 15000 / portTICK_RATE_MS);
            //ret=pdTRUE;
            if(ret == pdTRUE)
            { 
                val = rosepkt.cmd.SubCMD;
                switch(val)
                {
                        case 0x30 :    //
                                {
                                      BeepPiPi();
                                      sex = Fftloop(); // during about 6 second.
                      
                                      Serial.println("A");
                                      Serial.println(sex);
                                }
                                break;
                        case 0x31 :   // 
                                {
                                    BeepPiPi();
                                    BeepPiPi();                                    
 

                                    m_RoseMgr->RoseState_Mission2();
                                    st = ROSE_MISSION_2_S;
                                  
                                }
                                break;
                }
                  
            } else {
           //    Serial.println("timeout"); 
                 m_RoseMgr->RoseState_Mission1();
                 st = ROSE_MISSION_1_S;
                 break; 
            }
            
            if(st != ROSE_MISSION_1_S)
                break;
    }
        
    


//      vTaskDelay(5000 / portTICK_RATE_MS);

//      m_RoseMgr->RoseState_Ready();
//      return ROSE_READY_S;
      
      return st;
 };

extern void ServoHello(void);
extern void ServoDoriDori(void);
extern void ServoYes(void);
extern void ServoNo(void);
extern void HeartBitOnly();

 ROSE_STATE_ID CRoseMission2State::Do(void) {
      MSG_STATUS ret;
      ROSE_STATUS st = ROSE_MISSION_2_S;
      CMD_PKT rosepkt;
      byte val;
      
      DEBUG("Mission2\n");


    while(1)
    {   
            ret = ReceiveMessage(ROSE_TASKID, &rosepkt, 5000 / portTICK_RATE_MS);
            //ret=pdTRUE;
            if(ret == pdTRUE)
            { 
              
                  val = rosepkt.cmd.SubCMD;
                  switch(val) 
                  {
                    case 0x30 :
                               ServoYes();
                               break;
                    case 0x31 :
                               ServoNo();
                               break;
                    case 0x32  :
                            HeartBitOnly();
                               break;
                  }
                               
            } else {
              Serial.println("timeout");
              ServoDoriDori();
            }
    }

 
      vTaskDelay(2000 / portTICK_RATE_MS);
 
      m_RoseMgr->RoseState_Mission2();
      return ROSE_MISSION_2_S;

 };


void CRoseManager::RoseState_Sleep() { ChangeState((CRoseState *)new CRoseSleepState(this));};
void CRoseManager::RoseState_Ready() { ChangeState((CRoseState *)new CRoseReadyState(this));};
void CRoseManager::RoseState_Wakeup() { ChangeState((CRoseState *)new CRoseWakeupState(this));};
void CRoseManager::RoseState_Mission1() { ChangeState((CRoseState *)new CRoseMission1State(this));};
void CRoseManager::RoseState_Mission2() { ChangeState((CRoseState *)new CRoseMission2State(this));};



CRoseManager::CRoseManager()
{
    //m_pState = (CRoseState *)new CRoseSleepState(this);
    //m_pState = (CRoseState *)new CRoseReadyState(this);
    m_pState = (CRoseState *)new CRoseMission2State(this);
};

CRoseManager::CRoseManager(CRoseState *pNewState)
{ 
  m_pState = pNewState;
};

CRoseManager::~CRoseManager() {
      delete m_pState;
      m_pState = NULL;
};


void CRoseManager::ChangeState(CRoseState* pNewState)
{
  if(pNewState == NULL)
  {
    DEBUG("fuck\n");
  }
  delete m_pState;
  m_pState = pNewState;
};    

void CRoseManager::ResetState()
{  
  RoseState_Ready();
};    





