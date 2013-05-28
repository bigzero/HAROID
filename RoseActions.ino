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
