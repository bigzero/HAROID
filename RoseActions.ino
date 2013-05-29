#include "HAROID.h"

const int ServoUpDown = 9;
const int ServoLeftRight = 11;

extern Servo servo1;
extern Servo servo2;

#define SVR_MAX_UP 110
#define SVR_MAX_DN 180
#define SVR_MAX_LF 120
#define SVR_MAX_RT 60

void ServoOrigin(int Vangle, int Hangle)
{
    int angle;
    int V,H;

    
    //            maximum UP is   110
    // maximum Left is 120  maximum Right is 60
               // maximum DOWN is 180



    
    servo1.attach(ServoUpDown);
    servo2.attach(ServoLeftRight);
  
    V = servo1.read();
    H = servo2.read();
    
    // target is UP
    if(V > Vangle) {
       for(angle=V;angle>Vangle;angle--) {
            servo1.write(angle);
            vTaskDelay(15 / portTICK_RATE_MS);        
       }
      
    } else {
       for(angle=V;angle<Vangle;angle++) {
             servo1.write(angle);
            vTaskDelay(15 / portTICK_RATE_MS);                
       }
    }


    if(H > Hangle) {
       for(angle=H;angle>Hangle;angle--) {
             servo2.write(angle);
            vTaskDelay(15 / portTICK_RATE_MS);                
       }
      
    } else {
       for(angle=H;angle<Hangle;angle++) {
             servo2.write(angle);
            vTaskDelay(15 / portTICK_RATE_MS);                
       }
    }
   
    servo1.detach();
    servo2.detach();
}


void vServoWakeup(void)
{
   int angle = 0;

   servo1.attach(ServoUpDown);
 
    // up/down
   for (angle = 180; angle > 100; angle--) {
     servo1.write(angle);
     //delay(35);
     vTaskDelay(35 / portTICK_RATE_MS);
   }

  servo1.detach();

  vTaskDelay(100 / portTICK_RATE_MS);



  
}


void ServoASleep(void)
{
   int angle = 0;

   servo1.attach(ServoUpDown);
 
    // up/down
   for (angle = 100; angle < 180; angle++) {
     servo1.write(angle);
     //delay(35);
     vTaskDelay(35 / portTICK_RATE_MS);
   }

  servo1.detach();

  vTaskDelay(100 / portTICK_RATE_MS);



  
}

void ServoHello(void)
{
   int angle = 0;
   
//   Serial.println("ServoAction");
    ServoOrigin(100,90);
 
     servo1.attach(ServoUpDown);
    //servo1.write(110);
    //delay(1000);
    vTaskDelay(1000 / portTICK_RATE_MS);

  // down/up
 
   for (angle = 100; angle < 180; angle++) {
     servo1.write(angle);
     //delay(35);
     vTaskDelay(35 / portTICK_RATE_MS);
   }
 
   for (angle = 179; angle > 100; angle--) {
     servo1.write(angle);
     //delay(35);
     vTaskDelay(35 / portTICK_RATE_MS);
   }
   

    //   delay(1000);
    //   delay(1000);
  servo1.detach();
//  servo2.detach();


       vTaskDelay(500 / portTICK_RATE_MS);
}

#define DORI_RATE  35
void ServoDoriDori(void)
{
   int angle = 0;
   
//   Serial.println("ServoAction");
    ServoOrigin(110,90);
 
     servo2.attach(ServoLeftRight);
    //servo1.write(110);
    //delay(1000);
    vTaskDelay(1000 / portTICK_RATE_MS);

  // down/up
 
   for (angle = 90; angle < 110; angle++) {
     servo2.write(angle);
     //delay(35);
     vTaskDelay(DORI_RATE / portTICK_RATE_MS);
   }
 
   for (angle = 110; angle > 70; angle--) {
     servo2.write(angle);
     //delay(35);
     vTaskDelay(DORI_RATE / portTICK_RATE_MS);
   }
    for (angle = 70; angle < 90; angle++) {
     servo2.write(angle);
     //delay(35);
     vTaskDelay(DORI_RATE / portTICK_RATE_MS);
   }  

    //   delay(1000);
    //   delay(1000);
  servo2.detach();
//  servo2.detach();


       vTaskDelay(500 / portTICK_RATE_MS);
}

#define YES_RATE  20
void ServoYes(void)
{
   int angle = 0;
   
//   Serial.println("ServoAction");
    ServoOrigin(110,90);
 
     servo1.attach(ServoUpDown);
    //servo1.write(110);
    //delay(1000);
    vTaskDelay(100 / portTICK_RATE_MS);

  // down/up
 
   for (angle = 110; angle < 140; angle++) {
     servo1.write(angle);
     //delay(35);
     vTaskDelay(YES_RATE / portTICK_RATE_MS);
   }
 
   for (angle = 140; angle > 110; angle--) {
     servo1.write(angle);
     //delay(35);
     vTaskDelay(YES_RATE / portTICK_RATE_MS);
   }
   

    //   delay(1000);
    //   delay(1000);
  servo1.detach();
//  servo2.detach();


       vTaskDelay(500 / portTICK_RATE_MS);
}


#define NO_RATE  20
void ServoNo(void)
{
   int angle = 0;
   
//   Serial.println("ServoAction");
    ServoOrigin(110,90);
 
     servo2.attach(ServoLeftRight);
    //servo1.write(110);
    //delay(1000);
    vTaskDelay(100 / portTICK_RATE_MS);

  // down/up
 
   for (angle = 90; angle < 110; angle++) {
     servo2.write(angle);
     //delay(35);
     vTaskDelay(NO_RATE / portTICK_RATE_MS);
   }
 
   for (angle = 110; angle > 70; angle--) {
     servo2.write(angle);
     //delay(35);
     vTaskDelay(NO_RATE / portTICK_RATE_MS);
   }
    for (angle = 70; angle < 90; angle++) {
     servo2.write(angle);
     //delay(35);
     vTaskDelay(NO_RATE / portTICK_RATE_MS);
   }  

    //   delay(1000);
    //   delay(1000);
  servo2.detach();
//  servo2.detach();


       vTaskDelay(500 / portTICK_RATE_MS);
}



void vServoDozeoff() 
{
   int angle = 0;
   
//   Serial.println("ServoAction");
    ServoOrigin(170,90);
 
     servo1.attach(ServoUpDown);
    //servo1.write(110);
    //delay(1000);
    vTaskDelay(1000 / portTICK_RATE_MS);

  // down/up
 
   for (angle = 170; angle < 180; angle++) {
     servo1.write(angle);
     //delay(35);
     vTaskDelay(20 / portTICK_RATE_MS);
   }
 
   for (angle = 180; angle > 170; angle--) {
     servo1.write(angle);
     //delay(35);
     vTaskDelay(40 / portTICK_RATE_MS);
   }
   

    //   delay(1000);
    //   delay(1000);
  servo1.detach();
//  servo2.detach();


       vTaskDelay(500 / portTICK_RATE_MS);  
  
}

void vServoGlimpse(void)
{
   int angle = 0;
 
//   Serial.println("ServoAction");
   ServoOrigin(180,90);
   
   
   servo1.attach(ServoUpDown);
   servo2.attach(ServoLeftRight);  

 //   vTaskSuspendAll();
  
   // left 
   for (angle = 90; angle < 100; angle++) {
     servo2.write(angle);
     //delay(25);
     vTaskDelay(50 / portTICK_RATE_MS);

   } 


   // up/down
   for (angle = 180; angle > 170; angle--) {
     servo1.write(angle);
     //delay(35);
     vTaskDelay(50 / portTICK_RATE_MS);
   }
   for (angle = 170; angle < 180; angle++) {
     servo1.write(angle);
     //delay(35);
     vTaskDelay(50 / portTICK_RATE_MS);
   }
   

 //  delay(1000);
//    vTaskDelay(1000 / portTICK_RATE_MS);
   // right
   for (angle = 100; angle > 80; angle--) {
     servo2.write(angle);
     //delay(25);
     vTaskDelay(50 / portTICK_RATE_MS);
   }

   // up/down
   for (angle = 180; angle > 170; angle--) {
     servo1.write(angle);
     //delay(35);
     vTaskDelay(50 / portTICK_RATE_MS);
   }
   for (angle = 170; angle < 180; angle++) {
     servo1.write(angle);
     //delay(35);
     vTaskDelay(50 / portTICK_RATE_MS);
   }

     //delay(1000);
     //vTaskDelay(1000 / portTICK_RATE_MS);

  // center
   for (angle = 80; angle < 90; angle++) {
     servo2.write(angle);
     //delay(25);
     vTaskDelay(50 / portTICK_RATE_MS);
   }

    //   delay(1000);
    //   delay(1000);
  servo1.detach();
  servo2.detach();


       vTaskDelay(500 / portTICK_RATE_MS);
  

//       vTaskDelay(100 / portTICK_RATE_MS);


}


        unsigned long startTime;
        unsigned long elapsedTime;    

int vCdsRecognition(void)
{
        float cds;
        char count=0;
        int ret;
    
       
        startTime = xTaskGetTickCount();
        DEBUG("startTime");
        DEBUG(startTime);
        elapsedTime = 0;
        while( elapsedTime <  15000 ) 
        {
              DEBUG("previous CDS");
              cds = MeasureCds();
              
              DEBUG("C");
              DEBUG(cds);
              
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
             ret = 1;
        } else {
             ret = 0;
        }
          
    DEBUG("exit CDS");

        return ret;
        
}


const int analPin = 1;

float MeasureCds(void) 
{
  int val = 0;       // variable to store the value coming from the sensor
  long x=0, vcc=4840;
  float th=0,cds=0;

  //
  // VERY IMPORTANT!!!!!!!
  // We use the FFT library. so FFT is reset ADC setting. 
  // consequently, conflict FFT and AnalogRead.
  //
  
  val = analogRead(analPin);    // read the value from the sensor
  //val = 730;
  DEBUG(val);
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

