#include "HAROID.h"
#include <arduino.h>


//enum  STATE_ID {PREFIX_S, CMD_S, RSVD_S,SUBCMD_S,LENGTH_S,      PAYLOAD_S,      POSTFIX_S, COMPLETE_S, FAIL_S, 
//                                                 SYNC_LENGTH_S, SYNC_PAYLOAD_S, SYNC_POSTFIX_S, SYNC_COMPLETE_S, SYNC_FAIL_S};
class CRoseManager;

class CRoseState
{
  protected:
    const ROSE_STATE_ID m_stateID;

  public:
   // CState();
    CRoseState(ROSE_STATE_ID state):m_stateID(state)
    {};
    //CState();
    ~CRoseState();
    ROSE_STATE_ID GetState(void);
    
    virtual ROSE_STATE_ID Do(char no)=0;
};

class CRoseSleepState : CRoseState
{
  public:
    CRoseManager *m_RoseMgr;

    //CPrefixState():CState(PREFIX_S) {};
    CRoseSleepState(CRoseManager *mgr):CRoseState(ROSE_SLEEP_S), m_RoseMgr(mgr)
    {};
    ~CRoseSleepState();
    ROSE_STATE_ID Do(char no);
};

class CRoseReadyState : CRoseState
{
  public:
    CRoseManager *m_RoseMgr;

    //CPrefixState():CState(PREFIX_S) {};
    CRoseReadyState(CRoseManager *mgr):CRoseState(ROSE_READY_S), m_RoseMgr(mgr)
    {};
    ~CRoseReadyState();
    ROSE_STATE_ID Do(char no);
};

class CRoseMission1State : CRoseState
{
  public:
    CRoseManager *m_RoseMgr;

    //CPrefixState():CState(PREFIX_S) {};
    CRoseMission1State(CRoseManager *mgr):CRoseState(ROSE_MISSION_1_S), m_RoseMgr(mgr)
    {};
    ~CRoseMission1State();
    ROSE_STATE_ID Do(char no);
};

class CRoseMission2State : CRoseState
{
  public:
    CRoseManager *m_RoseMgr;

    //CPrefixState():CState(PREFIX_S) {};
    CRoseMission2State(CRoseManager *mgr):CRoseState(ROSE_MISSION_2_S), m_RoseMgr(mgr)
    {};
    ~CRoseMission2State();
    ROSE_STATE_ID Do(char no);
};



class CRoseManager 
{
  public:
    CRoseManager();
    CRoseManager(CRoseState *pNewState);
    ~CRoseManager();
    
   
  ROSE_STATUS Update(void);
  void NextState(ROSE_STATE_ID st);
  void ResetState();

  void RoseState_Sleep();
  void RoseState_Ready();

  void RoseState_Mission1();
  void RoseState_Mission2();
   
  protected:
    CRoseState *m_pState;
    void ChangeState(CRoseState* pNewState);
    
};


