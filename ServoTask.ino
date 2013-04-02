#include <haroid.h>

Servo servo;

extern ER (* const Haroid_FuncTbl[255])(PCMD_PKT src);

ER vServoLeft(PCMD_PKT src)
{
      int i,j;
    
   vTaskSuspendAll();
   servo.write(90);
   vTaskDelay(50/portTICK_RATE_MS);       

   for(j=0;j<3;j++)
   {
     
       for(i=90;i<10;i-=1)
        {
          servo.write(i);
          vTaskDelay(4 / portTICK_RATE_MS);
         //delay(15);
        }
        
       vTaskDelay(4 / portTICK_RATE_MS);   
        
        for(i=10;i<90 ;i+=1)
        {
          servo.write(i);
          vTaskDelay(4 / portTICK_RATE_MS);
         //delay(15);
        }
   }  
        servo.write(90);
        vTaskDelay(50/portTICK_RATE_MS);  
    xTaskResumeAll();
    taskYIELD();        
}

ER vServoRight(PCMD_PKT src)
{
      int i,j;
      
    vTaskSuspendAll();
   servo.write(90);
   vTaskDelay(50/portTICK_RATE_MS);       

   for(j=0;j<3;j++)
   {
     
       for(i=90;i<170;i+=1)
        {
          servo.write(i);
          vTaskDelay(4 / portTICK_RATE_MS);
         //delay(15);
        }
        
       vTaskDelay(4 / portTICK_RATE_MS);   
        
        for(i=170;i<90 ;i-=1)
        {
          servo.write(i);
          vTaskDelay(4 / portTICK_RATE_MS);
         //delay(15);
        }
   }  
        servo.write(90);
        vTaskDelay(50/portTICK_RATE_MS);  
    xTaskResumeAll();
    taskYIELD();   
}

ER vServoForward(PCMD_PKT src)
{
       int i,j;
           vTaskSuspendAll();    
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
    
    xTaskResumeAll();
//    taskYIELD();
}

CMD_PKT srvpkt;

static void ServoTask(void* arg) {
  portBASE_TYPE ret;
  int i;
  int angle;
  byte val;
  //SYNC_PKT syn_pkt;
  
 // pinMode(LED_PIN, OUTPUT);
  //servo.attach(11); 
  //centerServo = maxPulse - ((maxPulse - minPulse)/2);  
  //pulseWidth = centerServo;
  while (1) {
  
    ret = ReceiveMessage(SERVO_TASKID, &srvpkt, portMAX_DELAY);
    if(ret == pdTRUE)
    {
        servo.attach(11);
        val = srvpkt.cmdpkt.SubCMD;
     
        Haroid_FuncTbl[val](&srvpkt);
          
       //        servo.write(val);
   
       
       
        
        servo.detach();
      // servoTest(val);
        vTaskDelay(5 / portTICK_RATE_MS);
      // hrdCmdServoTest(&srvpkt, 0);
   
    }
    
  }
//  wrbPutbyte(0xDD);  
}
