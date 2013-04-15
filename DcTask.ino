#include <haroid.h>

extern ER (* const Haroid_FuncTbl[255])(PCOMMAND_STRUCT src);

ER vDcStop(PCOMMAND_STRUCT src)
{

}


ER vDcForward(PCOMMAND_STRUCT src)
{
     char val;
     val = src->payload[0];
     
     digitalWrite(9, LOW);
     analogWrite(10, val);
     vTaskDelay(150 / portTICK_RATE_MS);
     digitalWrite(10, LOW);
//     taskYIELD();

}

ER vDcReverse(PCOMMAND_STRUCT src)
{
      char val;
      val = src->payload[0];

      digitalWrite(9, HIGH);
      analogWrite(10,val);
      vTaskDelay(150 / portTICK_RATE_MS);
      digitalWrite(10, HIGH);
 //     taskYIELD();
}

COMMAND_STRUCT dcpkt;
BYTE flag=0;
BYTE val;
WORD val2=0;
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
                        HaroidIoControl(YOU,
                                 DC_TASKID,
                                 0x20, 
                                 (BYTE*)&val2,
                                 2,
                                 NULL,
                                 0,
                                 &val,
                                 NOSYNC);
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
                               
              vTaskDelay(2000/portTICK_RATE_MS);
	 } 
}
