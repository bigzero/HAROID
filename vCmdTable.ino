
ER vCmdIllegal(ID id, PCOMMAND_STRUCT src);
// ServoTask Command
ER vServoForward(ID id, PCOMMAND_STRUCT src);
ER vServoLeft(ID id, PCOMMAND_STRUCT src);
ER vServoRight(ID id, PCOMMAND_STRUCT src);
ER testtest(ID id, PCOMMAND_STRUCT src)
{
 
}

ER vCmdIllegal(ID id, PCOMMAND_STRUCT src)
{
	int ercd = E_OK;


	return ercd;
};

ER vTest(ID id, PCOMMAND_STRUCT src)
{
	ER er = E_OK;
        int i;
        
   	return er;
};



ER vReadDevice(ID id, PCOMMAND_STRUCT src)
{
    DWORD addr,len;
    ER er = E_OK;
    PR_DEVICE_STRUCT dev = (PR_DEVICE_STRUCT)&(src);   
    
    return er;  
};
ER vWriteDevice(ID id, PCOMMAND_STRUCT src)
{
    ER er = E_OK;
    PW_DEVICE_STRUCT dev = (PW_DEVICE_STRUCT)&(src);
     
     
    
    
    
    return er;
};


ER (* const Haroid_FuncTbl[])(ID id, PCOMMAND_STRUCT src) = {
  vTest,   // 0x00
  vCmdIllegal,   // 0x01
  vCmdIllegal,   // 0x02
  vCmdIllegal,   // 0x03
  vCmdIllegal,   // 0x04
  vCmdIllegal,   // 0x05
  vCmdIllegal,   // 0x06
  vCmdIllegal,   // 0x07
  vCmdIllegal,   // 0x08
  vCmdIllegal,   // 0x09
  vCmdIllegal,   // 0x0A
  vCmdIllegal,   // 0x0B
  vCmdIllegal,   // 0x0C
  vCmdIllegal,   // 0x0D
  vCmdIllegal,   // 0x0E
  vCmdIllegal,   // 0x0F
  vReadDevice,   // 0x10
  vWriteDevice,   // 0x11
  vCmdIllegal,   // 0x12
  vCmdIllegal,   // 0x13
  vCmdIllegal,   // 0x14
  vCmdIllegal,   // 0x15
  vCmdIllegal,   // 0x16
  vCmdIllegal,   // 0x17
  vCmdIllegal,   // 0x18
  vCmdIllegal,   // 0x19
  vCmdIllegal,   // 0x1A
  vCmdIllegal,   // 0x1B
  vCmdIllegal,   // 0x1C
  vCmdIllegal,   // 0x1D
  vCmdIllegal,   // 0x1E
  vCmdIllegal,   // 0x1F
  vServoForward,   // 0x20
  vServoLeft,    // 0x21
  vServoRight,   // 0x22
  vCmdIllegal,   // 0x23
  vCmdIllegal,   // 0x24
  vCmdIllegal,   // 0x25
  vCmdIllegal,   // 0x26
  vCmdIllegal,   // 0x27
  vCmdIllegal,   // 0x28
  vCmdIllegal,   // 0x29
  vCmdIllegal,   // 0x2A
  vCmdIllegal,   // 0x2B
  vCmdIllegal,   // 0x2C
  vCmdIllegal,   // 0x2D
  vCmdIllegal,   // 0x2E  
  vCmdIllegal,   // 0x2F
  vDcForward,    // 0x30
  vDcReverse,    // 0x31
  
};
