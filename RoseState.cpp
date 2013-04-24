#include "RoseState.h"



CRoseState::~CRoseState() 
{
};

ROSE_STATE_ID CRoseState::GetState(void)
{
  return m_stateID;  
};


ROSE_STATE_ID CRoseSleepState::Do(char no) {
  if(no == 5)
    {
       m_RoseMgr->RoseState_Ready();
      return ROSE_READY_S;
    }
    else
    {
      
     m_RoseMgr->RoseState_Sleep();
      return ROSE_SLEEP_S;
    }
 };
 
 ROSE_STATE_ID CRoseReadyState::Do(char no) {
  if(no == 1)
    {
       m_RoseMgr->RoseState_Mission1();
      return ROSE_MISSION_1_S;
    }
    else if(no == 2)
    {
      
     m_RoseMgr->RoseState_Mission2();
      return ROSE_MISSION_2_S;
    }
    else {
      // You selected a wrong number
 
      m_RoseMgr->RoseState_Ready();
      return ROSE_READY_S;
    }
 };

 ROSE_STATE_ID CRoseMission1State::Do(char no) {
      //Do your job 


      m_RoseMgr->RoseState_Ready();
      return ROSE_READY_S;
 };


 ROSE_STATE_ID CRoseMission2State::Do(char no) {
     //Do your job


      m_RoseMgr->RoseState_Ready();
      return ROSE_READY_S;
 };


void CRoseManager::RoseState_Sleep() { ChangeState((CRoseState *)new CRoseSleepState(this));};
void CRoseManager::RoseState_Ready() { ChangeState((CRoseState *)new CRoseReadyState(this));};
void CRoseManager::RoseState_Mission1() { ChangeState((CRoseState *)new CRoseMission1State(this));};
void CRoseManager::RoseState_Mission2() { ChangeState((CRoseState *)new CRoseMission2State(this));};



CRoseManager::CRoseManager()
{
    m_pState = (CRoseState *)new CRoseSleepState(this);
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
  delete m_pState;
  m_pState = pNewState;
};    

void CRoseManager::ResetState()
{  
  RoseState_Ready();
};    

