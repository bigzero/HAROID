#include "haroid.h"

Servo servo;

extern ER (* const Haroid_FuncTbl[255])(PCMD_PKT src);

//const int ServoUpDown = 9;
//const int ServoLeftRight = 11;

Servo servo1;
Servo servo2;


ER vServoWakeup(PCMD_PKT src)
{
   int angle = 0;
   SYNC_STRUCT pkt;

   servo1.attach(ServoUpDown);
 
    // up/down
   for (angle = 180; angle > 100; angle--) {
     servo1.write(angle);
     //delay(35);
     vTaskDelay(35 / portTICK_RATE_MS);
   }

  servo1.detach();

  vTaskDelay(100 / portTICK_RATE_MS);

//   xTaskResumeAll();
   pkt.Status = 0;
   pkt.length = 0;
   CompleteSyncMessage(src->SendID, &pkt);
   
   return pdTRUE;

  
}

ER vServoSleepAction(PCMD_PKT src)
{
   int angle = 0;
   SYNC_STRUCT pkt;
   
//   Serial.println("ServoAction");
   
   servo1.attach(ServoUpDown);
   servo2.attach(ServoLeftRight);  

 //   vTaskSuspendAll();
  
    servo1.write(180);
    //delay(1000);
    vTaskDelay(1000 / portTICK_RATE_MS);

 
   // left 
   for (angle = 90; angle < 120; angle++) {
     servo2.write(angle);
     //delay(25);
     vTaskDelay(20 / portTICK_RATE_MS);

   } 


   // up/down
   for (angle = 180; angle > 160; angle--) {
     servo1.write(angle);
     //delay(35);
     vTaskDelay(35 / portTICK_RATE_MS);
   }
   for (angle = 160; angle < 180; angle++) {
     servo1.write(angle);
     //delay(35);
     vTaskDelay(35 / portTICK_RATE_MS);
   }
   

 //  delay(1000);
//    vTaskDelay(1000 / portTICK_RATE_MS);
   // right
   for (angle = 120; angle > 60; angle--) {
     servo2.write(angle);
     //delay(25);
     vTaskDelay(20 / portTICK_RATE_MS);
   }

   // up/down
   for (angle = 180; angle > 160; angle--) {
     servo1.write(angle);
     //delay(35);
     vTaskDelay(35 / portTICK_RATE_MS);
   }
   for (angle = 160; angle < 180; angle++) {
     servo1.write(angle);
     //delay(35);
     vTaskDelay(35 / portTICK_RATE_MS);
   }

     //delay(1000);
     //vTaskDelay(1000 / portTICK_RATE_MS);

  // center
   for (angle = 60; angle < 90; angle++) {
     servo2.write(angle);
     //delay(25);
     vTaskDelay(20 / portTICK_RATE_MS);
   }

    //   delay(1000);
    //   delay(1000);
  servo1.detach();
  servo2.detach();


       vTaskDelay(2000 / portTICK_RATE_MS);
  

//       vTaskDelay(100 / portTICK_RATE_MS);

 //  xTaskResumeAll();
   pkt.Status = 0;
   pkt.length = 0;
   CompleteSyncMessage(src->SendID, &pkt);
//   Serial.println("YYY");
   
   return pdTRUE;

}

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


ER vCdsRecognition(PCMD_PKT src)
{
        float cds;
        char count=0;
        SYNC_STRUCT pkt;
        static unsigned long startTime;
        static unsigned long elapsedTime;        
       
        startTime = xTaskGetTickCount();
        elapsedTime = 0;
        while( elapsedTime <  15000 ) 
        {
            
              cds = MeasureCds();
              if(cds < 30)
              {
               // BeepPiPi();  
                HeartBit();
                ++count;
              }
              
              if(count == 5)
              {
                 break;
              } 
              
              elapsedTime = (xTaskGetTickCount() - startTime)*portTICK_RATE_MS;
              
         //     vTaskDelay(100/portTICK_RATE_MS);
        }
        
        
        if(count == 5)
        {
             pkt.Status = 1;
             pkt.length = 0;
        } else {
             pkt.Status = 0;
             pkt.length = 0;
        }
          


        CompleteSyncMessage(src->SendID, &pkt);

        return pdTRUE;
        
}


const int analPin = 2;

float MeasureCds(void) 
{
  int val = 0;       // variable to store the value coming from the sensor
  long x=0, vcc=4840;
  float th=0,cds=0;

  val = analogRead(analPin);    // read the value from the sensor
  x = map(val,0,1023,0,vcc);
  th = (((float)(vcc-x)*10.0)/(float)x)*1000.0;
  cds = ((log(4.0*th - 3000.0) / (-0.024119329) + 473)-32.0)/1.8;
 
  return cds;
}



int i = 0;
int pmw = 255;  //set PWM max - this can differ for other board pins
int rate = 25;  //this is the beats per minute (60000 ms)
                //because there are two beats to simulate the 'lub-dub' of the heart,
                // a 60 beat heart rate is only a value of 30 in the rate variable
                //the delay is the key to this programs realism - divide the rate into a minute, then weight it and divide by the pmw
                //you can modify the weight by changing the fractions (i.e .1, .2, .6) but to keep the timing correct, they should total 1
                //.1+.2+.1+.6 = 1


void HeartBit(){

       
   pinMode(LED,OUTPUT);
   digitalWrite(LED, LOW);
       

  for(i = 0; i < pmw; i++) {
    analogWrite(LED1,i);
    analogWrite(LED2,i);
    analogWrite(LED3,i);
    analogWrite(LED4,i);
    delay(((60000/rate)*.1)/pmw);
  }

   digitalWrite(LED, HIGH);

  
  for (i = pmw; i > 0; i--){
    analogWrite(LED1,i);
    analogWrite(LED2,i);
    analogWrite(LED3,i);
    analogWrite(LED4,i);
    
    delay(((60000/rate)*.2)/pmw);
  }

   digitalWrite(LED, LOW);
  
    for(i = 0; i < pmw; i++) {
    analogWrite(LED1,i);
    analogWrite(LED2,i);
    analogWrite(LED3,i);
    analogWrite(LED4,i);

      delay(((60000/rate)*.1)/pmw);
  }


   digitalWrite(LED, HIGH);
  
  for (i = pmw; i > 0; i--){
    analogWrite(LED1,i);
    analogWrite(LED2,i);
    analogWrite(LED3,i);
    analogWrite(LED4,i);

    
    delay(((60000/rate)*.6)/pmw);
  }

    analogWrite(LED1,0);
    analogWrite(LED2,0);
    analogWrite(LED3,0);
    analogWrite(LED4,0);

    pinMode(LED,INPUT);

  
}


void HeartBitOnly(){

       

  for(i = 0; i < pmw; i++) {
    analogWrite(LED1,i);
    analogWrite(LED2,i);
    analogWrite(LED3,i);
    analogWrite(LED4,i);
    delay(((60000/rate)*.1)/pmw);
  }



  
  for (i = pmw; i > 0; i--){
    analogWrite(LED1,i);
    analogWrite(LED2,i);
    analogWrite(LED3,i);
    analogWrite(LED4,i);
    
    delay(((60000/rate)*.2)/pmw);
  }

  
    for(i = 0; i < pmw; i++) {
    analogWrite(LED1,i);
    analogWrite(LED2,i);
    analogWrite(LED3,i);
    analogWrite(LED4,i);

      delay(((60000/rate)*.1)/pmw);
  }


  
  for (i = pmw; i > 0; i--){
    analogWrite(LED1,i);
    analogWrite(LED2,i);
    analogWrite(LED3,i);
    analogWrite(LED4,i);

    
    delay(((60000/rate)*.6)/pmw);
  }

    analogWrite(LED1,0);
    analogWrite(LED2,0);
    analogWrite(LED3,0);
    analogWrite(LED4,0);

  
}

