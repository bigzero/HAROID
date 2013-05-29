#include "haroid.h"

Servo servo;

extern ER (* const Haroid_FuncTbl[255])(PCMD_PKT src);

//const int ServoUpDown = 9;
//const int ServoLeftRight = 11;

Servo servo1;
Servo servo2;





ER vServoLeft(PCMD_PKT src)
{
      int j;

    vTaskSuspendAll();
    servo.attach(11);
   //servo.write(90);
   //vTaskDelay(50/portTICK_RATE_MS);       
   delay(50);
//   servo.write(70);
   for(j=5;j<90;j++)
   {
     
   //    for(i=90;i<10;i-=1)
        {
          servo.write(j);
          //vTaskDelay(2 / portTICK_RATE_MS);
          delay(50);
        }
        
       //vTaskDelay(2 / portTICK_RATE_MS);   
//        delay(2);
  /*      
        for(i=10;i<90 ;i+=1)
        {
          servo.write(i);
         // vTaskDelay(2 / portTICK_RATE_MS);
         delay(2);
        }
  */        
   }       
   servo.detach();

   xTaskResumeAll();
   
   return pdTRUE;
}

ER vServoRight(PCMD_PKT src)
{
      int j;
    vTaskSuspendAll();  
      
    servo.attach(11);


   for(j=90;j>5;j--)
   {
     
   //    for(i=90;i<10;i-=1)
        {
          servo.write(j);
          //vTaskDelay(2 / portTICK_RATE_MS);
          delay(50);
        }
        
       //vTaskDelay(2 / portTICK_RATE_MS);   
//        delay(2);
  /*      
        for(i=10;i<90 ;i+=1)
        {
          servo.write(i);
         // vTaskDelay(2 / portTICK_RATE_MS);
         delay(2);
        }
  */        
   } 
  
   servo.detach();
 
  //      servo.write(90);
        //vTaskDelay(50/portTICK_RATE_MS);
  //      delay(50);
      
/*      
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
        */
        
    xTaskResumeAll();    
    
    return pdTRUE;
}

ER vServoUpDown(PCMD_PKT src)
{
   int i, j;
   
   vTaskSuspendAll();
   
   servo.attach(12);
   delay(50);
   
    for(i=0;i<5;i++)
    {
            for(j=10;j<130;j++)
           {
             
           //    for(i=90;i<10;i-=1)
                {
                  servo.write(j);
                  //vTaskDelay(2 / portTICK_RATE_MS);
                  delay(1);
                }
           }
        
            for(j=130;j>10;j--)
           {
             
           //    for(i=90;i<10;i-=1)
                {
                  servo.write(j);
                  //vTaskDelay(2 / portTICK_RATE_MS);
                  delay(1);
                }
           }
    }
    servo.detach();
 
   xTaskResumeAll();
   
   return pdTRUE;
}


ER vServoForward(PCMD_PKT src)
{
       int i;
       static SYNC_STRUCT ss;
       ss.Status = 0;
       
       
     //  CompleteSyncMessage(src->SendID , &ss);
       
        for(i=0;i<2;i++) {
            pinMode(LED,OUTPUT);
            digitalWrite(LED, LOW);
            // This is very important!!!!
            // You have to care about "delay" function.
            //delay(100);
            vTaskDelay(100/portTICK_RATE_MS);
            digitalWrite(LED, HIGH);
            //delay(100);
            vTaskDelay(100/portTICK_RATE_MS);
        }
        
             pinMode(LED,INPUT);
       
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

  return pdTRUE;
}



CMD_PKT srvpkt;
static void ServoTask(void* arg) {
  MSG_STATUS ret;
  byte val;
  
  while (1) {
  
    ret = ReceiveMessage(SERVO_TASKID, &srvpkt, portMAX_DELAY);
    if(ret == pdTRUE)
    { 
        val = srvpkt.cmd.SubCMD;
     
     //   vTaskSuspendAll();
     //    servo.attach(11);
         Haroid_FuncTbl[val](&srvpkt);
     //    servo.detach();
     //  xTaskResumeAll();

    //    taskYIELD();
        
      // this is very stranged!!!!!!
      // I can't understand.
      // why the servotask queue is empted by dctask? 
      // If I comment vTaskDelay( from 2 over), it  disapear.
      //  vTaskDelay(5 / portTICK_RATE_MS);

   
    }
    
  }
//  wrbPutbyte(0xDD);  
}



