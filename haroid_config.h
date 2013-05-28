#define __M2560__

#ifdef __M2560__ //atmega2560

#define LED		8
#define SPEAKER		3

#define BLUETOOTH_TX	12
#define BLUETOOTH_RX	13

#define DC_MOTOR1       9
#define DC_MOTOR2      10

//Heartbeat LED
//elCalvoMike 12-6-2008
#define LED1  4 //analog pin location for ArduinoMini - change for other boards
#define LED2  5
#define LED3  6
#define LED4  7


#else  // promini




#endif
