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

CMD_PKT dcpkt;
BYTE flag=0;
BYTE val;
BYTE val2=0;
static void DcTask(void* arg) {
	  MSG_STATUS ret;
          	  
          
    	 while(1)
	 { 
#if 1  
              //val2 = analogRead(0);
               val2 = 200;
             // wrbPutbyte(val2);
              if(val2 > 100)
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
               
                            HaroidIoControl(ME,
                                               DC_TASKID,
                                               0x22, 
                                               NULL,
                                               0,
                                               NULL,
                                               0,
                                               &val,
                                               NOSYNC);    
 
                            HaroidIoControl(ME,
                                               DC_TASKID,
                                               0x24, 
                                               NULL,
                                               0,
                                               NULL,
                                               0,
                                               &val,
                                               NOSYNC);    
 
                                        
                                         
                                         HaroidIoControl(YOU,
                                               DC_TASKID,
                                               0x20, 
                                               NULL,
                                               0,
                                               NULL,
                                               0,
                                               &val,
                                               NOSYNC);             
                                               
                              
                                ret =  ReceiveMessage(DC_TASKID, &dcpkt, 10000);
                                if(ret == pdTRUE)
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
                                                           0x20, 
                                                           NULL,
                                                           0,
                                                           NULL,
                                                           0,
                                                           &val,
                                                           NOSYNC);   
                                  
                                      //    wrbPutbyte(0x18);
                                } else {
                                      //    wrbPutbyte(0x28);
                                              // Buzer sound
                                   //     DEBUG("Pang wait time out");
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
                                    
                                    vTaskDelay(5000/portTICK_RATE_MS);                   
                                }    
              }
   
/*   
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
                                 
                       if(val2 < 30 && val2 > 0) {
                                // Buzer sound
                                HaroidIoControl(ME,
                                                   DC_TASKID,
                                                   0x20, 
                                                   NULL,
                                                   0,
                                                   NULL,
                                                   0,
                                                   &val,
                                                   NOSYNC);
                                // Servo UP!!!
                                
                                HaroidIoControl(ME,
                                                   DC_TASKID,
                                                   0x24, 
                                                   NULL,
                                                   0,
                                                   NULL,
                                                   0,
                                                   &val,
                                                   NOSYNC);
                       }
              }                       
/*
                                ret =  ReceiveSyncMessage(DC_TASKID, &s_struct, 5000);
                                if(ret == pdTRUE)
                                {
                                  wrbPutbyte(0x18);
                                } else {
                                  wrbPutbyte(0x28);
                                      // Buzer sound
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
                                                       0x22, 
                                                       NULL,
                                                       0,
                                                       NULL,
                                                       0,
                                                       &val,
                                                       NOSYNC);
                                                       
                                }                       
 
 
           
                       }              
                       
   
                       
                   
              }
           */   
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

#endif                               
              vTaskDelay(1000/portTICK_RATE_MS);

	 } 
}
