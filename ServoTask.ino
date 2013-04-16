#include <haroid.h>

Servo servo;

extern ER (* const Haroid_FuncTbl[255])(ID id, PCOMMAND_STRUCT src);

ER vServoLeft(ID id, PCOMMAND_STRUCT src)
{
      int i,j;
    
   servo.write(90);
   //vTaskDelay(50/portTICK_RATE_MS);       
   delay(50);
   
   for(j=0;j<3;j++)
   {
     
       for(i=90;i<10;i-=1)
        {
          servo.write(i);
          //vTaskDelay(2 / portTICK_RATE_MS);
          delay(2);
        }
        
       //vTaskDelay(2 / portTICK_RATE_MS);   
        delay(2);
        
        for(i=10;i<90 ;i+=1)
        {
          servo.write(i);
         // vTaskDelay(2 / portTICK_RATE_MS);
         delay(2);
        }
   }  
        servo.write(90);
        //vTaskDelay(50/portTICK_RATE_MS);
        delay(50);  
}

ER vServoRight(ID id, PCOMMAND_STRUCT src)
{
      int i,j;
      
    servo.write(90);
   //vTaskDelay(50/portTICK_RATE_MS);       
    delay(50);
    
   for(j=0;j<3;j++)
   {
     
       for(i=90;i<160;i+=1)
        {
          servo.write(i);
          //vTaskDelay(2 / portTICK_RATE_MS);
          delay(2);
        }
        
       //vTaskDelay(2 / portTICK_RATE_MS);   
        delay(2);
        
        for(i=160;i<90 ;i-=1)
        {
          servo.write(i);
          //vTaskDelay(2 / portTICK_RATE_MS);
          delay(2);
        }
   }  
        servo.write(90);
        //vTaskDelay(50/portTICK_RATE_MS);  
        delay(50);
}

ER vServoForward(ID id, PCOMMAND_STRUCT src)
{
       int i,j;
       
        for(i=0;i<2;i++) {
            pinMode(LED,OUTPUT);
            digitalWrite(LED, LOW);
            delay(100);
            digitalWrite(LED, HIGH);
            delay(100);
        }
        /*
    servo.write(90);
    //vTaskDelay(50/portTICK_RATE_MS);     
    delay(50);
        
   for(j=0;j<1;j++)
   {   
       for(i=50;i<130;i+=1)
        {
          servo.write(i);
          //vTaskDelay(2 / portTICK_RATE_MS);
          delay(2);
         //delay(15);
        }
       //vTaskDelay(2 / portTICK_RATE_MS);   
       delay(2);
        for(i=130;i<50;i-=1)
        {
          servo.write(i);
          //vTaskDelay(2 / portTICK_RATE_MS);
         delay(2);
        }
    }   
    servo.write(90);
    //vTaskDelay(50/portTICK_RATE_MS);
    delay(50);
    
//    taskYIELD();
*/
}

CMD_PKT srvpkt;

static void ServoTask(void* arg) {
  MSG_STATUS ret;
  byte val;
  ID  id;
  
  while (1) {
  
    ret = ReceiveMessage(SERVO_TASKID, &srvpkt, portMAX_DELAY);
    if(ret == pdTRUE)
    {
        id = srvpkt.SendID;   
        val = srvpkt.cmd.SubCMD;
     
        vTaskSuspendAll();
         servo.attach(11);
         Haroid_FuncTbl[val](id, &(srvpkt.cmd));
         servo.detach();
        xTaskResumeAll();

        taskYIELD();
        
      // this is very stranged!!!!!!
      // I can't understand.
      // why the servotask queue is empted by dctask? 
      // If I comment vTaskDelay( from 2 over), it  disapear.
      //  vTaskDelay(5 / portTICK_RATE_MS);

   
    }
    
  }
//  wrbPutbyte(0xDD);  
}
