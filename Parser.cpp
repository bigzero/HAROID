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
   STATE_ID ret;
   
   if(((pk&0xF0)>>4) == 0x00) {  // CMD
        m_Parse->m_parsePacket[0] = pk;
        m_Parse->m_fSync = pk & 0x01;
        
        m_Parse->State_Reserved();
        
        ret = RSVD_S;
      
    } else {  // SYNC_CMD
        m_Parse->m_parsePacket[0] = pk & b0;
      
        m_Parse->State_SyncLength();
        ret = SYNC_LENGTH_S;
    }
    return ret;  
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



STATE_ID CSyncLengthState::Do(char pkk) {
  byte pk = pkk;
  
  m_Parse->m_parsePacket[1] = pkk;
  m_Parse->m_nLength = pk;
  m_Parse->m_PostfixPredict = m_Parse->m_nLength^0xFF;  
  
  if(pk != 0)
  {
    m_Parse->State_SyncPayload();
    return SYNC_PAYLOAD_S;
  }
  else
  {
      m_Parse->State_SyncPostfix();
     return SYNC_POSTFIX_S;
  }
  
};


STATE_ID CSyncPayloadState::Do(char pk) {
  
  m_Parse->m_parsePacket[2+m_Parse->m_nPayload]=pk;
  ++m_Parse->m_nPayload;
  if(m_Parse->m_nPayload < m_Parse->m_nLength)
  {
    m_Parse->State_SyncPayload();
    return SYNC_PAYLOAD_S;
  }
  else
  {
    m_Parse->State_SyncPostfix();
    return SYNC_POSTFIX_S;
  }    
};


STATE_ID CSyncPostfixState::Do(char pk) {
  // byte pk = pkk;

   if(pk == m_Parse->m_PostfixPredict)
   {
     m_Parse->State_SyncComplete();
     return SYNC_COMPLETE_S;
   } 
   else
   {
     m_Parse->State_SyncFail();
     return SYNC_FAIL_S;
   }
};


STATE_ID CSyncCompleteState::Do(char pk) {
  // very good!!! 
  m_Parse->m_syncPkt = (SYNC_STRUCT)(*(PSYNC_STRUCT)m_Parse->m_parsePacket);  
       
  m_Parse->m_nLength = 0;         // payload length
  m_Parse->m_nPayload = 0;        // couting variable
  m_Parse->m_PostfixPredict = 0;
  
  // the Complete state is null state. because nothing Do!!!
//  m_Parse->State_Prefix();

  return SYNC_COMPLETE_S;  
};


STATE_ID CSyncFailState::Do(char pk) {
  // initialize    
  m_Parse->m_nLength = 0;         // payload length
  m_Parse->m_nPayload = 0;        // couting variable
  m_Parse->m_PostfixPredict = 0;  

//  m_Parse->State_Prefix(); 

  return SYNC_FAIL_S;   
 
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
PARSER_STATUS CParser::Update(char pk) 
{ 
    PARSER_STATUS ret = FAIL_S;
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
        ret = COMPLETE_S;
        
        m_pState->Do(aa);
        //SendMessage(&m_cmdPkt);
        
        State_Prefix();
        
        
    } else if(m_pState->GetState() == FAIL_S) {
        ret = FAIL_S;
      
        m_pState->Do(aa);    
        
        State_Prefix();
      
    } else
    {
        ret = m_pState->GetState();
    }
    
    return ret;
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
// Sync 
void CParser::State_SyncLength() { ChangeState((CState *)new CSyncLengthState(this));};
void CParser::State_SyncPayload() { ChangeState((CState *)new CSyncPayloadState(this));};
void CParser::State_SyncPostfix() { ChangeState((CState *)new CSyncPostfixState(this));};  
void CParser::State_SyncComplete() { ChangeState((CState *)new CSyncCompleteState(this));};
void CParser::State_SyncFail() { ChangeState((CState *)new CSyncFailState(this));};


void CParser::ChangeState(CState* pNewState)
{
  delete m_pState;
  m_pState = pNewState;
};    


CParser gParser1;
extern "C" PARSER_STATUS Update(char pk) {
  return gParser1.Update(pk);
};
extern "C" COMMAND_STRUCT GetCommand(void) {
  return gParser1.m_cmdPkt;
};

CParser gParser2;
extern "C" PARSER_STATUS Update2(char pk) {
  return gParser2.Update(pk);
};
extern "C" COMMAND_STRUCT GetCommand2(void) {
  return gParser2.m_cmdPkt;
};

