

class CState
{
  protected:
    enum  STATE_ID {PREFIX_S, CMD_S, RSVD_S,SUBCMD_S,LENGTH_S,PAYLOAD_S, POSTFIX_S, COMPLETE_S, FAIL_S};
    const STATE_ID m_stateID;
  public:
    char parsePacket[21]; 
    char m_nLength;         // payload length
    char m_nPayload;        // couting variable
    char m_PostfixPredict;
    char m_fSync;
    
    CState();
    CState(STATE_ID stateID);
    ~CState();
    virtual STATE_ID Do(char pk)=0;
};


class CParser 
{
  public:
    enum  STATE_ID {PREFIX_S, CMD_S, RSVD_S,SUBCMD_S,LENGTH_S,PAYLOAD_S, POSTFIX_S, COMPLETE_S, FAIL_S};
    CParser();
    CParser(CState *pNewState);
    ~CParser();
    

    
  void Update(char pk);
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
   
  protected:
    CState *m_pState;
    void ChangeState(CState* pNewState);
    
};
    
