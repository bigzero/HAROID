#include "HAROID.h"
#include "Queue2.h"
#include "RoseState.h"

//extern CRoseManager roseMgr;


extern "C" const char HAROPANG_ACTIVITY[][6] = {
  {0xFF,0x0E,0xFF,0x60,0x00,0xFF},  // good morning
  {0xFF,0x0E,0xFF,0x61,0x00,0xFF},  // man 
  {0xFF,0x0E,0xFF,0x62,0x00,0xFF},  // woman
  {0xFF,0x0E,0xFF,0x63,0x00,0xFF},  // good byte
  {0xFF,0x0E,0xFF,0x64,0x00,0xFF},  // music man
  {0xFF,0x0E,0xFF,0x65,0x00,0xFF},   // music woman
  {0xFF,0x0E,0xFF,0x66,0x00,0xFF},    // who are you?
  {0xFF,0x0E,0xFF,0x67,0x00,0xFF},    // sunny
  {0xFF,0x0E,0xFF,0x68,0x00,0xFF},    // windy
  {0xFF,0x0E,0xFF,0x69,0x00,0xFF},    // rainy
  
  
  
};

#define HAROPANG_ACTIVITY_GOOD_MORNING   0
#define HAROPANG_ACTIVITY_MAN            1
#define HAROPANG_ACTIVITY_WOMAN          2
#define HAROPANG_ACTIVITY_GOOD_BYE       3
#define HAROPANG_ACTIVITY_MUSIC_MAN      4
#define HAROPANG_ACTIVITY_MUSIC_WOMAN    5
#define HAROPANG_ACTIVITY_SOUND_WHO      6
#define HAROPANG_ACTIVITY_MUSIC_SUNNY    7
#define HAROPANG_ACTIVITY_MUSIC_WINDY    8
#define HAROPANG_ACTIVITY_MUSIC_RAINY    9


#define HARO_ACTIVITY_VOICE_START          0x30
#define HARO_ACTIVITY_VOICE_SUCESS         0x31
#define HARO_ACTIVITY_VOICE_FAIL           0x32
//#define HARO_ACTIVITY_          0x33
#define HARO_ACTIVITY_SERVO_YES            0x34
#define HARO_ACTIVITY_SERVO_NO             0x35
#define HARO_ACTIVITY_SERVO_HEARTBIT       0x36
#define HARO_ACTIVITY_SLEEP_MODE           0x37
#define HARO_ACTIVITY_MUSIC_FFT_START      0x38
#define HARO_ACTIVITY_MUSIC_FFT_STOP       0x39
#define HARO_ACTIVITY_WAKEUP_MODE          0x3A
#define HARO_ACTIVITY_GUARD_MODE           0x3B
#define HARO_ACTIVITY_MODE_EXIT            0x3C
#define HARO_ACTIVITY_NATURE_MODE          0x3D
//#define HARO_ACTIVITY_NATURE_MODE_EXIT     0x3E
//#define HARO_ACTIVITY_                     0x3F



void __FIRE_TO_HAROPANG(int act) {
  int i;
  for(i=0;i<6;i++) {
      wrbPutbyte(HAROPANG_ACTIVITY[act][i]);
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

void MusicStart(void)
{
    byte val;
 
                         HaroidIoControl(ME,
                                           ROSE_TASKID,
                                           0x01, 
                                           NULL,
                                           0,
                                           NULL,
                                           0,
                                           &val,
                                           NOSYNC);    
  
  
}


void NatureStart(void)
{
    byte val;
 
                         HaroidIoControl(ME,
                                           ROSE_TASKID,
                                           0x02, 
                                           NULL,
                                           0,
                                           NULL,
                                           0,
                                           &val,
                                           NOSYNC);    
  
  
}

extern byte gMusicFlag;
void MusicStop(void)
{
                 gMusicFlag = 0;
}

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

/*
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
*/
/*
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
*/
extern void vServoDozeoff(void);
extern void vServoGlimpse(void);
extern void MeasureDistance(void) ;
extern void LedAllOn();
extern void LedAllOff();

extern byte FrontObject; 
//char buf[10];

// 
ROSE_STATE_ID CRoseSleepState::Do(void) {
  // wrbPutbyte(no);
   ROSE_STATUS st = ROSE_SLEEP_S;
//   MSG_STATUS ret;

   DEBUG("Sleep");

    MeasureDistance();
   // MeasureDistance();
   // MeasureDistance();
   // MeasureDistance();
   // MeasureDistance();
   
         
   if(FrontObject == 1)
   {
                  LedAllOn();
                  FrontObject = 0;
//                  BeepPiPi();

                  //vTaskDelay(1000 / portTICK_RATE_MS);
 
                  
                  m_RoseMgr->RoseState_Ready();
                  st =  ROSE_READY_S;
     

     } else {
       
         LedAllOff();
       
         int rand=random(1,4);
         switch(rand)
         {
           case 1:
              vServoDozeoff();
              vTaskDelay(1000/ portTICK_RATE_MS);
           //   vServoDozeoff();
           //   vTaskDelay(1000/ portTICK_RATE_MS);
              break;
           case 2:
              vServoGlimpse();
              break;
           case 3:
              vTaskDelay(1000/ portTICK_RATE_MS);
              break;
         }          

        // m_RoseMgr->RoseState_Sleep();
         st =  ROSE_SLEEP_S;
         
       
     }


      return st;

 };
 
 extern int vCdsRecognition(void);
 ROSE_STATE_ID CRoseReadyState::Do(void) {
   char count;
   ROSE_STATUS st = ROSE_READY_S;
   CMD_PKT rosepkt;
   MSG_STATUS ret;
   byte val;
//    static int mission = 0;
   
   DEBUG("Ready");
/*
        ret = vCdsRecognition();   
   
       if(ret == 1) {
             m_RoseMgr->RoseState_Wakeup();
             st =  ROSE_WAKEUP_S;
    
       } else {
             m_RoseMgr->RoseState_Sleep();
             st =  ROSE_SLEEP_S;
       }
   
 */

        ret = ReceiveMessage(ROSE_TASKID, &rosepkt, 5000 / portTICK_RATE_MS);
            //ret=pdTRUE;
       if(ret == pdTRUE)
       { 
            val = rosepkt.cmd.SubCMD;
            switch(val)
            {
                case 0x3A :
                           m_RoseMgr->RoseState_Wakeup();
                           st =  ROSE_WAKEUP_S;  
                           break;
                           
            }
       }
        
   return st;     
   
 };

extern void vServoWakeup();
ROSE_STATE_ID CRoseWakeupState::Do(void) {
      //Do your job
      MSG_STATUS st;
       
      DEBUG("Wakeup");
      
      vServoWakeup();
      
      __FIRE_TO_HAROPANG(HAROPANG_ACTIVITY_GOOD_MORNING);    
      
    //  m_RoseMgr->RoseState_Mission1();
    //  return ROSE_MISSION_1_S;
    
      m_RoseMgr->RoseState_Mission2();
      return ROSE_MISSION_2_S;
 };


extern void FftInit();
extern int Fftloop();
extern void FftExit();

// voice recognition
 ROSE_STATE_ID CRoseMission1State::Do(void) {
      //Do your job
      CMD_PKT rosepkt;
      MSG_STATUS ret;
      ROSE_STATUS st = ROSE_MISSION_1_S;
      byte val;
      int  sex;
       
      DEBUG("Mission1\n");
   
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
                      
                                      DEBUG("A");
                                      DEBUG(sex);
                                     if(sex > 0)
                                         m_RoseMgr->SetGenus(ROSE_YOUNGBOY);
                                     else
                                         m_RoseMgr->SetGenus(ROSE_OLDBOY);
                                
                                }
                                break;
                        case 0x31 :   // 
                                {
                                    BeepPiPi();
                                    BeepPiPi();                                    
 
 
                                    if(m_RoseMgr->GetGenus() == ROSE_OLDBOY )
                                          __FIRE_TO_HAROPANG(HAROPANG_ACTIVITY_MAN);
                                    else
                                          __FIRE_TO_HAROPANG(HAROPANG_ACTIVITY_WOMAN);
 

                                    m_RoseMgr->RoseState_Mission2();
                                    st = ROSE_MISSION_2_S;
                                   
                                   
                                  
                                }
                                break;
                         case 0x32:
                               {
                                    BeepPiPi();
                                    BeepPiPi();             
                                    BeepPiPi();
                               }   
                                    
                                break;
                }
                  
            } else {
           //    DEBUG("timeout"); 
                 //m_RoseMgr->RoseState_Mission1();
                 st = ROSE_MISSION_1_S;
                 break; 
            }
            
            if(st != ROSE_MISSION_1_S)
                break;
    }
        
    
    FftExit();

//      vTaskDelay(5000 / portTICK_RATE_MS);

//      m_RoseMgr->RoseState_Ready();
//      return ROSE_READY_S;
      
      return st;
 };


extern void ServoOrigin(int,int);
extern void ServoHello(void);
extern void ServoDoriDori(void);
extern void ServoYes(void);
extern void ServoNo(void);
extern void HeartBitOnly();


 ROSE_STATE_ID CRoseMission2State::Do(void) {
      MSG_STATUS ret;
      ROSE_STATUS st = ROSE_MISSION_2_S;
      CMD_PKT rosepkt;
      int  sex;
      byte val;
      
      DEBUG("Mission2\n");

      
    ServoOrigin(110,90);
    ResetMessageQueue(ROSE_TASKID);
    while(1)
    {   
                 
            ret = ReceiveMessage(ROSE_TASKID, &rosepkt, 5000 / portTICK_RATE_MS);
            //ret=pdTRUE;
            if(ret == pdTRUE)
            { 
              
                  val = rosepkt.cmd.SubCMD;
                  DEBUG("mission2 command");
                  //DEBUG(val);
                  Serial.println(val,HEX);
                  switch(val) 
                  {
                       case 0x30 :    //
                                {
                                      MusicStop();
                                      BeepPiPi();
                                      
                                      FftInit();    
                                  
                                     
                                      sex = Fftloop(); // during about 6 second.
                        
                                      FftExit();
                      
                                    
                      
                                      DEBUG("A");
                                      DEBUG(sex);
                                     if(sex > 0)
                                         m_RoseMgr->SetGenus(ROSE_YOUNGBOY);
                                     else
                                         m_RoseMgr->SetGenus(ROSE_OLDBOY);
                                
                                }
                                break;
                        case 0x31 :   // 
                                {
                                    BeepPiPi();
                                    BeepPiPi();                                    
 
 
                                    if(m_RoseMgr->GetGenus() == ROSE_OLDBOY )
                                          __FIRE_TO_HAROPANG(HAROPANG_ACTIVITY_MAN);
                                    else
                                          __FIRE_TO_HAROPANG(HAROPANG_ACTIVITY_WOMAN);
 

                                    m_RoseMgr->RoseState_Mission2();
                                    st = ROSE_MISSION_2_S;
                                   
                                   
                                  
                                }
                                break;
                         case 0x32:
                               {
                                    BeepPiPi();
                                    BeepPiPi();             
                                    BeepPiPi();
                               }   
                                    
                                break;
                                     
                    
                    case 0x34 :
                               ServoYes();
                               break;
                    case 0x35 :
                               ServoNo();
                               break;
                    case 0x36  :
                            HeartBitOnly();
                               break;
                    case 0x37  :
                       
                               
                               m_RoseMgr->RoseState_ASleep();
                               st = ROSE_ASLEEP_S;
                              break;       
                          
                    case 0x38 :
                                if(m_RoseMgr->GetGenus() == ROSE_OLDBOY )
                                          __FIRE_TO_HAROPANG(HAROPANG_ACTIVITY_MUSIC_MAN);
                                    else
                                          __FIRE_TO_HAROPANG(HAROPANG_ACTIVITY_MUSIC_WOMAN);
                                          
                                MusicStart();
                                break;    
                    case 0x39 :
                                MusicStop();
                                vTaskDelay(1000 / portTICK_RATE_MS);
                                break;
                       
                    case 0x3B :
                              m_RoseMgr->RoseState_Guard();
                              st = ROSE_GUARD_S;  
                              break;
                    case 0x3D :    // HARO_ACTIVITY_NATURE_MODE
                              m_RoseMgr->RoseState_Nature();
                              st = ROSE_NATURE_S;      
                              break;            
                    
                  }
                               
            } else {
              DEBUG("timeout");
            //  ServoDoriDori();
    
            }
            
            if(st != ROSE_MISSION_2_S)
              break;
    }

 
      vTaskDelay(500 / portTICK_RATE_MS);
 

      return st;

 };


extern void ServoGuardUp(int,int);
extern void vServoGuard(void);
 ROSE_STATE_ID CRoseGuardState::Do(void) {
   char count;
   ROSE_STATUS st = ROSE_GUARD_S;
   CMD_PKT rosepkt;
   MSG_STATUS ret;
   byte val;
   int  sex;
//    static int mission = 0;
   
   DEBUG("Guard");
   
       

        ret = ReceiveMessage(ROSE_TASKID, &rosepkt, 1000 / portTICK_RATE_MS);
            //ret=pdTRUE;
       if(ret == pdTRUE)
       { 
            val = rosepkt.cmd.SubCMD;
            switch(val)
            {
                case 0x30 :    //
                        {
                              MusicStop();
                              BeepPiPi();
                              
                              FftInit();    
                          
                             
                              sex = Fftloop(); // during about 6 second.
                
                              FftExit();
              
                            
              
                              DEBUG("A");
                              DEBUG(sex);
                             if(sex > 0)
                                 m_RoseMgr->SetGenus(ROSE_YOUNGBOY);
                             else
                                 m_RoseMgr->SetGenus(ROSE_OLDBOY);
                                 
                             
                            ServoOrigin(110,90);
                            m_RoseMgr->RoseState_Mission2();
                            st = ROSE_MISSION_2_S;    
                        
                        }
                        break;
/*                        
                case 0x31 :   // 
                        {
                            BeepPiPi();
                            BeepPiPi();                                    
 
 
                            if(m_RoseMgr->GetGenus() == ROSE_OLDBOY )
                                  __FIRE_TO_HAROPANG(HAROPANG_ACTIVITY_MAN);
                            else
                                  __FIRE_TO_HAROPANG(HAROPANG_ACTIVITY_WOMAN);
 

                            m_RoseMgr->RoseState_Mission2();
                            st = ROSE_MISSION_2_S;
                           
                           
                          
                        }
                        break;
                 case 0x32:
                       {
                            BeepPiPi();
                            BeepPiPi();             
                            BeepPiPi();
                       }   
                       m_RoseMgr->RoseState_Mission2();
                       st = ROSE_MISSION_2_S;         
                        break;             
    */          
                case 0x3C :       // GUIDE exit
                                    m_RoseMgr->RoseState_Mission2();
                                    st = ROSE_MISSION_2_S;
                           break;
                           
            }
       } else
       {
             vServoGuard();    
             MeasureDistance();
 
            if(FrontObject == 1)
            {     
                  
                  FrontObject = 0;
                  
                 
                  __FIRE_TO_HAROPANG(HAROPANG_ACTIVITY_SOUND_WHO);
                  vTaskDelay(500 / portTICK_RATE_MS);
                  ServoGuardUp(110,90);
                  vTaskDelay(2000 / portTICK_RATE_MS);
            }
       }
        
   return st;     
   
 };



extern byte gNatureMode;

void PlayNatureMusic()
{
           int rand=random(1,4);
         switch(rand)
         {
           case 1 :
                       __FIRE_TO_HAROPANG(HAROPANG_ACTIVITY_MUSIC_SUNNY);  
                       gNatureMode = SERVO_SUNNY;    
                       break;
           case 2 : 
                       __FIRE_TO_HAROPANG(HAROPANG_ACTIVITY_MUSIC_WINDY);
                       gNatureMode = SERVO_WINDY;  
                       break;
           case 3 : 
                       __FIRE_TO_HAROPANG(HAROPANG_ACTIVITY_MUSIC_RAINY);
                       gNatureMode = SERVO_RAINY;
                       break;
         }           
}


extern byte gServoNatureFlag;

 ROSE_STATE_ID CRoseNatureState::Do(void) {
   char count;
   ROSE_STATUS st = ROSE_NATURE_S;
   CMD_PKT rosepkt;
   MSG_STATUS ret;
   byte val;
   int  sex;
//    static int mission = 0;
   
   DEBUG("Nature");
   
   
       PlayNatureMusic();
       NatureStart();

        ret = ReceiveMessage(ROSE_TASKID, &rosepkt, 30000 / portTICK_RATE_MS);
            //ret=pdTRUE;
       if(ret == pdTRUE)
       { 
            val = rosepkt.cmd.SubCMD;
            switch(val)
            {
                 case 0x30 :    //
                        {
                              gServoNatureFlag = 0;   // nature exit
                              MusicStop();
                              BeepPiPi();
                              
                              FftInit();    
                          
                             
                              sex = Fftloop(); // during about 6 second.
                
                              FftExit();
              
                            
              
                              DEBUG("A");
                              DEBUG(sex);
                             if(sex > 0)
                                 m_RoseMgr->SetGenus(ROSE_YOUNGBOY);
                             else
                                 m_RoseMgr->SetGenus(ROSE_OLDBOY);
                                 
                             
                            m_RoseMgr->RoseState_Mission2();
                            st = ROSE_MISSION_2_S;    
                        
                        }
                        break;
                        
 /*                       
                case 0x31 :   // 
                        {
                            BeepPiPi();
                            BeepPiPi();                                    
 
 
                            if(m_RoseMgr->GetGenus() == ROSE_OLDBOY )
                                  __FIRE_TO_HAROPANG(HAROPANG_ACTIVITY_MAN);
                            else
                                  __FIRE_TO_HAROPANG(HAROPANG_ACTIVITY_WOMAN);
 

                            m_RoseMgr->RoseState_Mission2();
                            st = ROSE_MISSION_2_S;
                           
                           
                          
                        }
                        break;
                 case 0x32:
                       {
                            BeepPiPi();
                            BeepPiPi();             
                            BeepPiPi();
                       }   
                       
                             m_RoseMgr->RoseState_Mission2();
                            st = ROSE_MISSION_2_S;
                            
                        break;              
*/
               case HARO_ACTIVITY_MODE_EXIT  :       // GUIDE exit  0x3C
                                    m_RoseMgr->RoseState_Mission2();
                                    st = ROSE_MISSION_2_S;
                           break;
                           
            }
       } else
       {
              gServoNatureFlag = 1;   
              PlayNatureMusic();
   
         
 
       }
        
   return st;     
   
 };


extern void ServoASleep();
 ROSE_STATE_ID CRoseASleepState::Do(void) {
  
   DEBUG("ASleep");
  
   __FIRE_TO_HAROPANG(HAROPANG_ACTIVITY_GOOD_BYE);
   
   ServoASleep();

     
 //  m_RoseMgr->RoseState_Sleep();
 //  return ROSE_SLEEP_S;
     m_RoseMgr->RoseState_Ready();
     return ROSE_READY_S;
 };


void CRoseManager::RoseState_Sleep() { ChangeState((CRoseState *)new CRoseSleepState(this));};
void CRoseManager::RoseState_Ready() { ChangeState((CRoseState *)new CRoseReadyState(this));};
void CRoseManager::RoseState_Wakeup() { ChangeState((CRoseState *)new CRoseWakeupState(this));};
void CRoseManager::RoseState_Mission1() { ChangeState((CRoseState *)new CRoseMission1State(this));};
void CRoseManager::RoseState_Mission2() { ChangeState((CRoseState *)new CRoseMission2State(this));};
void CRoseManager::RoseState_Guard() { ChangeState((CRoseState *)new CRoseGuardState(this));};
void CRoseManager::RoseState_Nature() { ChangeState((CRoseState *)new CRoseNatureState(this));};
void CRoseManager::RoseState_ASleep() { ChangeState((CRoseState *)new CRoseASleepState(this));};

void CRoseManager::SetGenus(int sex) {  m_genus = sex; };
int CRoseManager::GetGenus(void) { return m_genus; };

CRoseManager::CRoseManager()
{
    m_genus = ROSE_YOUNGBOY;
    m_pState = (CRoseState *)new CRoseSleepState(this);
    //m_pState = (CRoseState *)new CRoseReadyState(this);
    //m_pState = (CRoseState *)new CRoseMission1State(this);
   // m_pState = (CRoseState *)new CRoseMission2State(this);
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





