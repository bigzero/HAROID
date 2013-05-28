#include <stdint.h>
#include <ffft.h>

#define  IR_AUDIO  0 // ADC channel to capture


volatile  byte  position = 0;
volatile  long  zero = 0;

int16_t capture[FFT_N];			/* Wave captureing buffer */
complex_t bfly_buff[FFT_N];		/* FFT buffer */
uint16_t spektrum[FFT_N/2];		/* Spectrum output buffer */
uint16_t sex[4]; // 12,13,14,15

void FftInit()
{
  //Serial.begin(57600);
  adcInit();
  adcCalb();
  establishContact();  // send a byte to establish contact until Processing respon
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
      while( elapsedTime <  6000 ) 
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
          
            if(spektrum[0] > 100)
            {
                sex[0] = spektrum[11] > 50 ? ++sex[0] : sex[0];
                sex[1] = spektrum[12] > 50 ? ++sex[1] : sex[1];        
                sex[2] = spektrum[13] > 50 ? ++sex[2] : sex[2];
                sex[3] = spektrum[14] > 50 ? ++sex[3] : sex[3];
            }
            
             
             position = 0;
            }
            
            elapsedTime = (xTaskGetTickCount() - startTime)*portTICK_RATE_MS;
      }
      
      return (sex[0]+sex[1]+sex[2]+sex[3]);   
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
