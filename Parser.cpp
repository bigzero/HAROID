#include "HAROID.h"
#include "Parser.h"

#include <string.h>

CState::~CState() 
{
};

class CPrefixState : CState
{
  public:
    CPrefixState():CState(PREFIX_S) {   };
    ~CPrefixState();
    STATE_ID Do(char pk) {
        if(pk == 0xFF)
          return CMD_S;
        else
          return FAIL_S;
     };
      
};
class CCmdState : CState
{
  public:
    CCmdState():CState(CMD_S) {   };
    ~CCmdState();
    STATE_ID Do(char pk) {
        parsePacket[0] = pk;
        m_fSync = pk & 0x01;
        return RSVD_S;  
    };
};
class CReservedState : CState
{
  public:
    CReservedState():CState(RSVD_S) {   };
    ~CReservedState();
    STATE_ID Do(char pk) {
      if(pk == 0xFF)
      {
        parsePacket[1] = pk;
        return SUBCMD_S;
      } else {
        return FAIL_S;
      }
    };
};

class CSubCmdState : CState
{
  public:
    CSubCmdState():CState(SUBCMD_S) {   };
    ~CSubCmdState();
    STATE_ID Do(char pk) {
      parsePacket[2] = pk;
      return LENGTH_S;
    };
};

class CLengthState : CState
{
  public:
    CLengthState():CState(LENGTH_S) {   };
    ~CLengthState();
    STATE_ID Do(char pk) {
      parsePacket[3] = pk;
      m_nLength = pk;
      m_PostfixPredict = m_nLength^0xFF;  
      if(pk != 0)
        return PAYLOAD_S;
       else
         return FAIL_S;
      
    };
};

class CPayloadState : CState
{
  public:
    CPayloadState():CState(PAYLOAD_S) {   };
    ~CPayloadState();
    STATE_ID Do(char pk) {
      parsePacket[++m_nPayload];
      if(m_nPayload < m_nLength)
        return PAYLOAD_S;
      else
        return POSTFIX_S;    
    };
};

class CPostfixState : CState
{
  public:
    CPostfixState():CState(POSTFIX_S) {   };
    ~CPostfixState();
    STATE_ID Do(char pk) {
       if(pk == m_PostfixPredict)
         return COMPLETE_S;
       else
         return FAIL_S;
    };
};

class CCompleteState : CState
{
  public:
    CCompleteState():CState(COMPLETE_S) {   };
    ~CCompleteState();
    STATE_ID Do(char pk) {
      // very good!!! 
   
      m_nLength = 0;         // payload length
      m_nPayload = 0;        // couting variable
      m_PostfixPredict = 0;
    
      return PREFIX_S;  
    };
};

class CFailState : CState
{
  public:
    CFailState():CState(FAIL_S) {   };
    ~CFailState();
    STATE_ID Do(char pk) {
      // initialize    
      m_nLength = 0;         // payload length
      m_nPayload = 0;        // couting variable
      m_PostfixPredict = 0;  
   
      return PREFIX_S;   
 
    };
};



CParser::CParser()
{
    //m_pState = (CState *)new CPrefixState();
};

CParser::CParser(CState *pNewState)
{ 
  m_pState = pNewState;
};

CParser::~CParser() {
      delete m_pState;
      m_pState = NULL;
};
 

void CParser::Update(char pk) 
{ 
    NextState((STATE_ID)m_pState->Do(pk));
    if(m_pState->m_stateID == COMPLETE_S) {
      
      
      
    } else if(m_pState->m_stateID == FAIL_S) {
    
     
      
    } else
    {
      
    }
};



void CParser::NextState(STATE_ID id)
{
  //PREFIX_S, CMD_S, RSVD_S,SUBCMD_S,LENGTH_S,PAYLOAD_S, POSTFIX_S, COMPLETE_S, FAIL_S
    switch(id)
    {
      case PREFIX_S:
                      State_Prefix();
                      break;
      case CMD_S:  
                      State_Cmd();
                      break;
      case RSVD_S:  
                      State_Reserved();
                      break;
      case SUBCMD_S:
                      State_SubCmd();
                      break;
      case LENGTH_S:
                      State_Length();
                      break;
      case PAYLOAD_S:
                      State_Payload();
                      break;
      case POSTFIX_S:
                      State_Postfix();
                      break;
      case COMPLETE_S:
                      State_Complete();
                      break;
      case FAIL_S:  
                      State_Fail();
                      break;
    }
};

void CParser::State_Prefix() { ChangeState((CState *)new CPrefixState);};
void CParser::State_Cmd() { ChangeState((CState *)new CCmdState);};
void CParser::State_Reserved() { ChangeState((CState *)new CReservedState);};
void CParser::State_SubCmd() { ChangeState((CState *)new CSubCmdState);};
void CParser::State_Length() { ChangeState((CState *)new CLengthState);};
void CParser::State_Payload() { ChangeState((CState *)new CPayloadState);};
void CParser::State_Postfix() { ChangeState((CState *)new CPostfixState);};  
void CParser::State_Complete() { ChangeState((CState *)new CCompleteState);};
void CParser::State_Fail() { ChangeState((CState *)new CFailState);};

void CParser::ChangeState(CState* pNewState)
{
  delete m_pState;
  m_pState = pNewState;
};    

