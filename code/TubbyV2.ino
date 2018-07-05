 
#define BLYNK_PRINT Serial
#include "Wire.h"
#include "WEMOS_Motor.h"
#include <Arduino.h>
#include <U8g2lib.h>
#include "musical_notes.h"
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
int pwm;
int pwmA = 0;
int pwmB = 0;
int gX = 0;
int gY =0;
int direction = 1;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

//Motor shiled I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)
Motor M1(0x30,_MOTOR_A, 1000);//Motor A
Motor M2(0x30,_MOTOR_B, 1000);//Motor B
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "";
BLYNK_WRITE(V0)
{
  int x = param[0].asInt();
  
  // Do something with x and y
  Serial.print("X = ");
  Serial.print(x);
  if( x == 512)
  {
    pwmA = 0;
    pwmB = 0;
  }
  else if( x < 512 )
  {
    pwmA = x;
    pwmB = 1024 - x;
  }
  else if( x > 512 )
  {
    pwmB = x;
    pwmA = 1024 - x;
  }
  gX = x;

}
BLYNK_WRITE(V1) {
  int y = param[0].asInt();
  gY = y;
  // Do something with x and y
  Serial.print("; Y = ");
  Serial.println(y);
  if( y < 512 )
     direction = 0;
  else 
     direction = 1;
  
}
BLYNK_WRITE(V2) {
r2D2();  
}
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";
int speakerPin = 14; // speaker connected to digital pin 9 

void setup()
{
  // Debug console
  Serial.begin(115200);
    pinMode(speakerPin, OUTPUT); // sets the speakerPin to be an output 
  u8g2.begin();
  Blynk.begin(auth, ssid, pass);
}

void eyes(int angle)
{
  u8g2.setDrawColor(255);
  u8g2.drawDisc(25, 35, 15 );
  u8g2.setDrawColor(0);
  u8g2.drawDisc(25, 35, 5 );
  u8g2.setDrawColor(255);

  u8g2.drawDisc(100, 35, 15 );
  u8g2.setDrawColor(0);
  u8g2.drawDisc(100, 35, 5 );
}
void blink(int angle)
{
  int i =0 ;
  u8g2.setDrawColor(255);
  u8g2.drawDisc(25, 35, 15 );
  u8g2.drawDisc(100, 35, 15 );
  u8g2.setDrawColor(0);

  for ( i = 0; i<8;i++)
  {
    u8g2.drawDisc(25, 35, i );
    u8g2.drawDisc(100, 35, i);
    u8g2.sendBuffer();          // transfer internal memory to the display

    delay(50);
  }

  u8g2.drawDisc(25, 35, 15 );
  u8g2.drawDisc(100, 35, 15 );
}

void wink(int x,int y)
{
  int i =0 ;
  u8g2.clearBuffer();         // clear the internal memory
  u8g2.setDrawColor(255);
  u8g2.drawDisc(25, 35, 15 );
  u8g2.drawDisc(100, 35, 15 );
  u8g2.setDrawColor(0);

  int lx = map(x, 0,1024, 0,20); 
  int ly = map(y, 0,1024, 0,20);
  Serial.println(lx);
Serial.println(ly);
  u8g2.drawDisc(lx+15, ly+25, 8 );
  u8g2.drawDisc(lx+90, ly+25, 8);
  u8g2.sendBuffer();          // transfer internal memory to the display



  //u8g2.drawDisc(25, 35, 15 );
  //u8g2.drawDisc(100, 35, 15 );
}

void loop()
{
  
  //blink(10);
 // wink(pwmA, pwmB);
    wink(gX,gY);
  Blynk.run();
  if(direction)
  {
      
      M1.setmotor(_CW, pwmA);
      M2.setmotor(_CCW,   pwmB);
  }
  else
  {
      M1.setmotor(_CCW, pwmA);
      M2.setmotor(_CW,   pwmB);

  }
}

void beep (int speakerPin, float noteFrequency, long noteDuration)
{    
  int x;
  // Convert the frequency to microseconds
  float microsecondsPerWave = 1000000/noteFrequency;
  // Calculate how many HIGH/LOW cycles there are per millisecond
  float millisecondsPerCycle = 1000/(microsecondsPerWave * 2);
  // Multiply noteDuration * number or cycles per millisecond
  float loopTime = noteDuration * millisecondsPerCycle;
  // Play the note for the calculated loopTime.
  for (x=0;x<loopTime;x++)   
          {   
              digitalWrite(speakerPin,HIGH); 
              delayMicroseconds(microsecondsPerWave); 
              digitalWrite(speakerPin,LOW); 
              delayMicroseconds(microsecondsPerWave); 
          } 
}     
     
void r2D2(){
          beep(speakerPin, note_A7,100); //A 
          beep(speakerPin, note_G7,100); //G 
          beep(speakerPin, note_E7,100); //E 
          beep(speakerPin, note_C7,100); //C
          beep(speakerPin, note_D7,100); //D 
          beep(speakerPin, note_B7,100); //B 
          beep(speakerPin, note_F7,100); //F 
          beep(speakerPin, note_C8,100); //C 
          beep(speakerPin, note_A7,100); //A 
          beep(speakerPin, note_G7,100); //G 
          beep(speakerPin, note_E7,100); //E 
          beep(speakerPin, note_C7,100); //C
          beep(speakerPin, note_D7,100); //D 
          beep(speakerPin, note_B7,100); //B 
          beep(speakerPin, note_F7,100); //F 
          beep(speakerPin, note_C8,100); //C 
}




