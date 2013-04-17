#include <haroid.h>

extern ER (* const Haroid_FuncTbl[255])(PCMD_PKT src);

ER vDcStop(PCMD_PKT src)
{

}


ER vDcForward(PCMD_PKT src)
{
     char val;
     val = src->cmd.payload[0];
     
     digitalWrite(9, LOW);
     analogWrite(10, val);
     vTaskDelay(150 / portTICK_RATE_MS);
     digitalWrite(10, LOW);
//     taskYIELD();

}

ER vDcReverse(PCMD_PKT src)
{
      char val;
      val = src->cmd.payload[0];

      digitalWrite(9, HIGH);
      analogWrite(10,val);
      vTaskDelay(150 / portTICK_RATE_MS);
      digitalWrite(10, HIGH);
 //     taskYIELD();
}

COMMAND_STRUCT dcpkt;
BYTE flag=0;
BYTE val;
BYTE val2=0;
static void DcTask(void* arg) {
	  portBASE_TYPE ret;
          	  
          
    	 while(1)
	 { 
              val2 = analogRead(0);
              if(val2 > 100)
                flag = 1;
               else
                 flag = 0;
                
              if(flag == 1)
              {
              HaroidIoControl(ME,
                                 DC_TASKID,
                                 0x20, 
                                 NULL,
                                 0,
                                 NULL,
                                 0,
                                 &val,
                                 NOSYNC);
                                 
                                                         
              } else {
                
                        HaroidIoControl(ME,
                                 DC_TASKID,
                                 0x23, 
                                 NULL ,
                                 0,
                                 (BYTE*)&val2,
                                 1,
                                 &val,
                                 SYNC);
                                 
                                 wrbPutbyte(val2);
                                 
                       if(val2 < 30) {
           
              HaroidIoControl(ME,
                                 DC_TASKID,
                                 0x20, 
                                 NULL,
                                 0,
                                 NULL,
                                 0,
                                 &val,
                                 NOSYNC);
 
              HaroidIoControl(ME,
                                 DC_TASKID,
                                 0x21, 
                                 NULL,
                                 0,
                                 NULL,
                                 0,
                                 &val,
                                 NOSYNC);
 
 
           
                       }              
                   
              }
	      /*
                  ret = ReceiveMessage(DC_TASKID, &dcpkt, portMAX_DELAY);
	          if(ret == pdTRUE)
	          {
                        val = dcpkt.SubCMD;
                        Haroid_FuncTbl[val](&dcpkt);  
		        //vTaskDelay(80 / portTICK_RATE_MS);
                       taskYIELD();
	  	   }
              */
                               
              vTaskDelay(1000/portTICK_RATE_MS);
	 } 
}
