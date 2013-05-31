#include <stdint.h>
#include <ffft.h>

#define  IR_AUDIO  0 // ADC channel to capture


volatile  byte  position = 0;
volatile  long  zero = 0;

int16_t capture[FFT_N];			/* Wave captureing buffer */
complex_t bfly_buff[FFT_N];		/* FFT buffer */
uint16_t spektrum[FFT_N/2];		/* Spectrum output buffer */
uint16_t sex[5]; // 11,12,13,14,15


byte __ADMUX__;
byte __ADCSRA__;


void FftInit()
{
  //Serial.begin(57600);
  adcInit();
  adcCalb();
  establishContact();  // send a byte to establish contact until Processing respon
}

void FftExit()
{
    ADMUX = __ADMUX__;
    ADCSRA = __ADCSRA__ ; 
}

int Fftloop()
{
        static unsigned long startTime;
      static unsigned long elapsedTime;        
      int i;
      
      for(i=0;i<4;i++)
      {
        sex[i] = 0;
      }
      
      startTime = xTaskGetTickCount();
      elapsedTime = 0;
      while( elapsedTime <  1500 ) 
      {

            if (position == FFT_N)
            {
              fft_input(capture, bfly_buff);
              fft_execute(bfly_buff);
              fft_output(bfly_buff, spektrum);
          /*
              for (byte i = 0; i < 64; i++){
                Serial.write(spektrum[i]);
              }
          */
          
            if(spektrum[0] > 80)
            {
                sex[0] = spektrum[11] > 50 ? ++sex[0] : sex[0];
                sex[1] = spektrum[12] > 50 ? ++sex[1] : sex[1];        
                sex[2] = spektrum[13] > 50 ? ++sex[2] : sex[2];
                sex[3] = spektrum[14] > 50 ? ++sex[3] : sex[3];
                //sex[4] = spektrum[10] > 50 ? ++sex[4] : sex[4];
            }
            
             
             position = 0;
            }
            
            elapsedTime = (xTaskGetTickCount() - startTime)*portTICK_RATE_MS;
      }
      
      Serial.println("SEX");
      Serial.println(sex[0]+sex[1]+sex[2]+sex[3],HEX);
      
      return (sex[0]+sex[1]+sex[2]+sex[3]);   
}



void ServoHeadbang(void)
{
    static byte flag = 1;
    byte val;
 
                 flag ^= 1;
                 
                 if(flag == 1)
                 {
                         HaroidIoControl(ME,
                                           ETC_TASKID,
                                           0x26, 
                                           NULL,
                                           0,
                                           NULL,
                                           0,
                                           &val,
                                           NOSYNC);    
                 } else {
                   ResetMessageQueue(SERVO_TASKID);
                 }
                 
                 
  
}

//extern void MusicBumpBase(void);
//extern void MusicBumpVocal(void);
extern void MusicBump(void);
extern byte gMusicFlag;
void FftMusicloop()
{
        static unsigned long startTime;
      static unsigned long elapsedTime;        
      int i;
      
      for(i=0;i<4;i++)
      {
        sex[i] = 0;
      }
      
      startTime = xTaskGetTickCount();
      elapsedTime = 0;


     // while( elapsedTime <  100000 )
     while( 1) 
      {
        
            if(gMusicFlag == 0)
            {
              ResetMessageQueue(SERVO_TASKID);
              break;
            }
            
            if (position == FFT_N)
            {
                    fft_input(capture, bfly_buff);
                    fft_execute(bfly_buff);
                    fft_output(bfly_buff, spektrum);
                /*
                    for (byte i = 0; i < 64; i++){
                      Serial.write(spektrum[i]);
                     
                    }
                */
                

                     // Serial.write(spektrum[i]);
                     //MusicBump(spektrum[6], spektrum[11]);
                     
                  if(spektrum[6] > 40)
                  {
                    MusicBumpBase();
                    ServoHeadbang();
                  } 
                  if(spektrum[11] > 40)
                   MusicBumpVocal(); 
                
                  if(spektrum[0] > 80)
                  {
                      sex[0] = spektrum[0] > 50 ? ++sex[0] : sex[0];
                      sex[1] = spektrum[1] > 50 ? ++sex[1] : sex[1];        
                      sex[2] = spektrum[2] > 50 ? ++sex[2] : sex[2];
                      sex[3] = spektrum[3] > 50 ? ++sex[3] : sex[3];
                      //sex[4] = spektrum[10] > 50 ? ++sex[4] : sex[4];

                  }
            
             
                   position = 0;
                   

           

            }
            
 
            


//            vTaskDelay(100/ portTICK_RATE_MS);            
           
            
            elapsedTime = (xTaskGetTickCount() - startTime)*portTICK_RATE_MS;
      }
      
      
     
      
}


void establishContact() {
/*
  while (Serial.available() <= 0) {
      Serial.write('A');   // send a capital A
      delay(300);
  }
*/

  vTaskDelay(300/ portTICK_RATE_MS);
}

// free running ADC fills capture buffer
ISR(ADC_vect)
{
  if (position >= FFT_N)
    return;
  
  capture[position] = ADC + zero;
  if (capture[position] == -1 || capture[position] == 1)
    capture[position] = 0;

  position++;
}


void adcInit(){
   __ADMUX__ = ADMUX;
   __ADCSRA__ = ADCSRA;
 
  /*  REFS0 : VCC use as a ref, IR_AUDIO : channel selection, ADEN : ADC Enable, ADSC : ADC Start, ADATE : ADC Auto Trigger Enable, ADIE : ADC Interrupt Enable,  ADPS : ADC Prescaler  */
  // free running ADC mode, f = ( 16MHz / prescaler ) / 13 cycles per conversion 
  ADMUX = _BV(REFS0) | IR_AUDIO; // | _BV(ADLAR); 
//  ADCSRA = _BV(ADSC) | _BV(ADEN) | _BV(ADATE) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) //prescaler 64 : 19231 Hz - 300Hz per 64 divisions
  ADCSRA = _BV(ADSC) | _BV(ADEN) | _BV(ADATE) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); // prescaler 128 : 9615 Hz - 150 Hz per 64 divisions, better for most music
  sei();
}
void adcCalb(){
//  Serial.println("Start to calc zero");
  long midl = 0;
  // get 2 meashurment at 2 sec
  // on ADC input must be NO SIGNAL!!!
  for (byte i = 0; i < 2; i++)
  {
    position = 0;
    //delay(100);
      vTaskDelay(100/ portTICK_RATE_MS);
    midl += capture[0];
    //delay(900);
      vTaskDelay(900/ portTICK_RATE_MS);
  }
  zero = -midl/2;
//  Serial.println("Done.");
}
