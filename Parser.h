#include "HAROID.h"
#include <arduino.h>


//enum  STATE_ID {PREFIX_S, CMD_S, RSVD_S,SUBCMD_S,LENGTH_S,      PAYLOAD_S,      POSTFIX_S, COMPLETE_S, FAIL_S, 
//                                                 SYNC_LENGTH_S, SYNC_PAYLOAD_S, SYNC_POSTFIX_S, SYNC_COMPLETE_S, SYNC_FAIL_S};
extern "C" PARSER_STATUS Update(char pk);

class CParser;

class CState
{
  protected:
    const STATE_ID m_stateID;

  public:
   // CState();
    CState(STATE_ID state):m_stateID(state)
    {};
    //CState();
    ~CState();
    STATE_ID GetState(void);
    
    virtual STATE_ID Do(char pk)=0;
};


class CPrefixState : CState
{
  public:
    CParser *m_Parse;

    //CPrefixState():CState(PREFIX_S) {};
    CPrefixState(CParser *parse):CState(PREFIX_S), m_Parse(parse)
    {};
    ~CPrefixState();
    STATE_ID Do(char pk);
};
class CCmdState : CState
{
  public:
    CParser *m_Parse;

    //CCmdState():CState(CMD_S) {   };
    CCmdState(CParser *parse):CState(CMD_S), m_Parse(parse)
    {};
    ~CCmdState();
    STATE_ID Do(char pk);
};

class CReservedState : CState
{
  public:
    CParser *m_Parse;

    //CReservedState():CState(RSVD_S) {   };
    CReservedState(CParser *parse):CState(RSVD_S), m_Parse(parse)
    {};
    ~CReservedState();
    STATE_ID Do(char pk);
};

class CSubCmdState : CState
{
  public:
    CParser *m_Parse;

    //CSubCmdState():CState(SUBCMD_S) {   };
    CSubCmdState(CParser *parse):CState(SUBCMD_S), m_Parse(parse)
    {};
    ~CSubCmdState();
    STATE_ID Do(char pk);
};

class CLengthState : CState
{
  public:
    CParser *m_Parse;

    //CLengthState():CState(LENGTH_S) {   };
    CLengthState(CParser *parse):CState(LENGTH_S), m_Parse(parse)
    {};
    ~CLengthState();
    STATE_ID Do(char pk);
};

class CPayloadState : CState
{
  public:
    CParser *m_Parse;

    //CPayloadState():CState(PAYLOAD_S) {   };
    CPayloadState(CParser *parse):CState(PAYLOAD_S), m_Parse(parse)
    {};
    STATE_ID Do(char pk);
};

class CPostfixState : CState
{
  public:
    CParser *m_Parse;

    //CPostfixState():CState(POSTFIX_S) {   };
    CPostfixState(CParser *parse):CState(POSTFIX_S), m_Parse(parse)
    {};
    ~CPostfixState();
    STATE_ID Do(char pk);
};

class CCompleteState : CState
{
  public:
    CParser *m_Parse;

    //CCompleteState():CState(COMPLETE_S) {   };
    CCompleteState(CParser *parse):CState(COMPLETE_S), m_Parse(parse)
    {};
    ~CCompleteState();
    STATE_ID Do(char pk);
};

class CFailState : CState
{
  public:
    CParser *m_Parse;

    //CFailState():CState(FAIL_S) {   };
    CFailState(CParser *parse):CState(FAIL_S), m_Parse(parse)
    {
    };
    ~CFailState();
    STATE_ID Do(char pk);
};


class CSyncLengthState : CState
{
  public:
    CParser *m_Parse;

    //CLengthState():CState(LENGTH_S) {   };
    CSyncLengthState(CParser *parse):CState(SYNC_LENGTH_S), m_Parse(parse)
    {};
    ~CSyncLengthState();
    STATE_ID Do(char pk);
};

class CSyncPayloadState : CState
{
  public:
    CParser *m_Parse;

    //CPayloadState():CState(PAYLOAD_S) {   };
    CSyncPayloadState(CParser *parse):CState(SYNC_PAYLOAD_S), m_Parse(parse)
    {};
    STATE_ID Do(char pk);
};

class CSyncPostfixState : CState
{
  public:
    CParser *m_Parse;

    //CPostfixState():CState(POSTFIX_S) {   };
    CSyncPostfixState(CParser *parse):CState(SYNC_POSTFIX_S), m_Parse(parse)
    {};
    ~CSyncPostfixState();
    STATE_ID Do(char pk);
};

class CSyncCompleteState : CState
{
  public:
    CParser *m_Parse;

    //CCompleteState():CState(COMPLETE_S) {   };
    CSyncCompleteState(CParser *parse):CState(SYNC_COMPLETE_S), m_Parse(parse)
    {};
    ~CSyncCompleteState();
    STATE_ID Do(char pk);
};

class CSyncFailState : CState
{
  public:
    CParser *m_Parse;

    //CFailState():CState(FAIL_S) {   };
    CSyncFailState(CParser *parse):CState(SYNC_FAIL_S), m_Parse(parse)
    {
    };
    ~CSyncFailState();
    STATE_ID Do(char pk);
};




class CParser 
{
  public:
    CParser();
    CParser(CState *pNewState);
    ~CParser();
    
    char m_parsePacket[21]; 
    COMMAND_STRUCT m_cmdPkt;
    SYNC_STRUCT  m_syncPkt;
    char m_nLength;         // payload length
    char m_nPayload;        // couting variable
    char m_PostfixPredict;
    char m_fSync;

    
  PARSER_STATUS Update(char pk);
  void NextState(STATE_ID st);

  void State_Prefix();
  void State_Cmd();
  void State_Reserved();
  void State_SubCmd();
  void State_Length();
  void State_Payload();
  void State_Postfix();  
  void State_Complete();
  void State_Fail();

  void State_SyncLength();
  void State_SyncPayload();
  void State_SyncPostfix();  
  void State_SyncComplete();
  void State_SyncFail();

   
  protected:
    CState *m_pState;
    void ChangeState(CState* pNewState);
    
};
    
