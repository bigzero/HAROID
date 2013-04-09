#include "Parser.h"
#include <string.h>
#include <stdlib.h>

//extern BOOL wrbPutbyte(char b);

CState::~CState() 
{
};

STATE_ID CState::GetState(void)
{
  return m_stateID;  
};


STATE_ID CPrefixState::Do(char pkk) {
  byte pk = pkk;
  if(pk == 0xff)
    {
       m_Parse->State_Cmd();
      return CMD_S;
    }
    else
    {
      
     m_Parse->State_Fail();
      return FAIL_S;
    }
 };
      

STATE_ID CCmdState::Do(char pk) {
    m_Parse->m_parsePacket[0] = pk;
    m_Parse->m_fSync = pk & 0x01;
    
    m_Parse->State_Reserved();
    return RSVD_S;  
};


STATE_ID CReservedState::Do(char pkk) {
  byte pk = pkk;
  if(pk == 0xFF)
  {
    m_Parse->m_parsePacket[1] = pkk;
    m_Parse->State_SubCmd();
    return SUBCMD_S;
    
  } else {
    m_Parse->State_Fail();
    return FAIL_S;
  }
};


STATE_ID CSubCmdState::Do(char pk) {
  m_Parse->m_parsePacket[2] = pk;
  
  m_Parse->State_Length();
  return LENGTH_S;
};


STATE_ID CLengthState::Do(char pkk) {
  byte pk = pkk;
  
  m_Parse->m_parsePacket[3] = pkk;
  m_Parse->m_nLength = pk;
  m_Parse->m_PostfixPredict = m_Parse->m_nLength^0xFF;  
  
  if(pk != 0)
  {
    m_Parse->State_Payload();
    return PAYLOAD_S;
  }
  else
  {
      m_Parse->State_Postfix();
     return POSTFIX_S;
  }
  
};


STATE_ID CPayloadState::Do(char pk) {
  
  m_Parse->m_parsePacket[4+m_Parse->m_nPayload]=pk;
  ++m_Parse->m_nPayload;
  if(m_Parse->m_nPayload < m_Parse->m_nLength)
  {
    m_Parse->State_Payload();
    return PAYLOAD_S;
  }
  else
  {
    m_Parse->State_Postfix();
    return POSTFIX_S;
  }    
};


STATE_ID CPostfixState::Do(char pk) {
  // byte pk = pkk;

   if(pk == m_Parse->m_PostfixPredict)
   {
     m_Parse->State_Complete();
     return COMPLETE_S;
   } 
   else
   {
     m_Parse->State_Fail();
     return FAIL_S;
   }
};


STATE_ID CCompleteState::Do(char pk) {
  // very good!!! 
  m_Parse->m_cmdPkt = (COMMAND_STRUCT)(*(PCOMMAND_STRUCT)m_Parse->m_parsePacket);  
       
  m_Parse->m_nLength = 0;         // payload length
  m_Parse->m_nPayload = 0;        // couting variable
  m_Parse->m_PostfixPredict = 0;
  
  // the Complete state is null state. because nothing Do!!!
//  m_Parse->State_Prefix();

  return COMPLETE_S;  
};


STATE_ID CFailState::Do(char pk) {
  // initialize    
  m_Parse->m_nLength = 0;         // payload length
  m_Parse->m_nPayload = 0;        // couting variable
  m_Parse->m_PostfixPredict = 0;  

//  m_Parse->State_Prefix(); 

  return FAIL_S;   
 
};



CParser::CParser()
{
    m_pState = (CState *)new CPrefixState(this);
};

CParser::CParser(CState *pNewState)
{ 
  m_pState = pNewState;
};

CParser::~CParser() {
      delete m_pState;
      m_pState = NULL;
};
 
extern "C" BOOL wrbPutbyte(byte b);
void CParser::Update(char pk) 
{ 
    byte aa = pk;
 /*   
    if(aa == 0xff) {
            pinMode(LED,OUTPUT);
      digitalWrite(LED,LOW);
      delay(500);
      digitalWrite(LED,HIGH);
      delay(500);
    }
   */
    m_pState->Do(aa);

   
    if(m_pState->GetState() == COMPLETE_S) {
        //wrbPutbyte(0x00);
        m_pState->Do(aa);
        SendMessage(&m_cmdPkt);
        State_Prefix(); 
      
    } else if(m_pState->GetState() == FAIL_S) {
        m_pState->Do(aa);    
       State_Prefix();
      
    } else
    {
      
    }
};


/*
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
*/


void CParser::State_Prefix() { ChangeState((CState *)new CPrefixState(this));};
void CParser::State_Cmd() { ChangeState((CState *)new CCmdState(this));};
void CParser::State_Reserved() { ChangeState((CState *)new CReservedState(this));};
void CParser::State_SubCmd() { ChangeState((CState *)new CSubCmdState(this));};
void CParser::State_Length() { ChangeState((CState *)new CLengthState(this));};
void CParser::State_Payload() { ChangeState((CState *)new CPayloadState(this));};
void CParser::State_Postfix() { ChangeState((CState *)new CPostfixState(this));};  
void CParser::State_Complete() { ChangeState((CState *)new CCompleteState(this));};
void CParser::State_Fail() { ChangeState((CState *)new CFailState(this));};

void CParser::ChangeState(CState* pNewState)
{
  delete m_pState;
  m_pState = pNewState;
};    


CParser gParser1;
extern "C" void Update(char pk) {
  gParser1.Update(pk);
};
CParser gParser2;
extern "C" void Update2(char pk) {
  gParser2.Update(pk);
};

