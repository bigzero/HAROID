#include <haroid.h>

extern ER (* const Haroid_FuncTbl[255])(PCMD_PKT src);

ER vDcStop(PCMD_PKT src)
{

}


ER vDcForward(PCMD_PKT src)
{
     char val;
     val = src->cmdpkt.payload[0];
     
     digitalWrite(9, LOW);
     analogWrite(10, val);
     vTaskDelay(150 / portTICK_RATE_MS);
     digitalWrite(10, LOW);
//     taskYIELD();

}

ER vDcReverse(PCMD_PKT src)
{
      char val;
      val = src->cmdpkt.payload[0];

      digitalWrite(9, HIGH);
      analogWrite(10,val);
      vTaskDelay(150 / portTICK_RATE_MS);
      digitalWrite(10, HIGH);
 //     taskYIELD();
}


CMD_PKT dcpkt;
static void DcTask(void* arg) {
	  portBASE_TYPE ret;
	  int i;
	  byte val;
	  
    	 while(1)
	 { 
	          ret = ReceiveMessage(DC_TASKID, &dcpkt, portMAX_DELAY);
	          if(ret == pdTRUE)
	          {
                        val = dcpkt.cmdpkt.SubCMD;
                        Haroid_FuncTbl[val](&dcpkt);  
		        //vTaskDelay(80 / portTICK_RATE_MS);
	  	   } 
	 } 
}
