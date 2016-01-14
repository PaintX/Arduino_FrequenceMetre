#include <LiquidCrystal.h>
#include "avance_tab.h"



LiquidCrystal lcd(8,9,4,5,6,7);
int adc_key_in;
volatile int state = LOW;

void setup() 
{
  S_AVANCE * ptr;
  pinMode(A0, INPUT);
  Serial.begin(115200);
  lcd.begin(16,2);

  Serial.println("Hello World");
  analogReference(EXTERNAL);
  attachInterrupt(digitalPinToInterrupt(3), trigPIN3, RISING); 


  ptr = AVANCE_TAB_GetTab();
  for ( int i = 0 ; i < AVANCE_TAB_GetSize() ; i++ )
  {
    ptr[i].avance = (float)i;
  }

  Serial.println("**************************");
  Serial.println("table d'avance :");
    for ( int i = 0 ; i < AVANCE_TAB_GetSize() ; i++ )
  {
    Serial.println(ptr[i].avance);
  }
}

static unsigned long  periode;
static unsigned long ms;
float freq = 0.0;
float trmin = 0.0;



void loop() 
{
  
  
  lcd.setCursor(0,0);            // move cursor to second line "1" and 9 spaces over
  freq = 1.0/((float)periode/1000000.0);
  lcd.print(freq);
  lcd.print(" Hz");

  Serial.print(freq);
  Serial.println(" Hz");

  trmin = freq * 60.0;
  trmin /= 2.0;   //-- pour 2 cylindres
  Serial.print( trmin );
  Serial.println(" tr/min");
  delay(100);
}

unsigned long us;
void trigPIN3() 
{
  us = micros();
  periode = us - ms;
  ms = us;
}
