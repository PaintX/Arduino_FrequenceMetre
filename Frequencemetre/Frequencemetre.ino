#include <LiquidCrystal.h>
#include "avance_tab.h"
#include "TimerOne.h"


LiquidCrystal lcd(8,9,4,5,6,7);
int adc_key_in;
volatile int state = LOW;
static unsigned int _Tick;
static unsigned int timerperiode;
void _pulseSim(void)
{
  state = state ^1;
  digitalWrite(2,state);
}


void setup() 
{
  S_AVANCE * ptr;
  
  pinMode(2,OUTPUT);
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
  _Tick = millis();

  timerperiode = 2500;
  Timer1.initialize(timerperiode);
  Timer1.attachInterrupt(_pulseSim);
}

static unsigned long  periode;
static unsigned long ms;
float freq = 0.0;
float trmin = 0.0;


#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
 
int read_LCD_buttons()
{               // read the buttons
  adc_key_in = analogRead(0);       // read the value from the sensor 
  
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  // We make this the 1st option for speed reasons since it will be the most likely result
  
  if (adc_key_in > 1000) return btnNONE; 
  
  // For V1.1 us this threshold
  if (adc_key_in < 50)   return btnRIGHT;  
  if (adc_key_in < 250)  return btnUP; 
  if (adc_key_in < 450)  return btnDOWN; 
  if (adc_key_in < 650)  return btnLEFT; 
  if (adc_key_in < 850)  return btnSELECT;  
  
  return btnNONE;                // when all others fail, return this.
}

 
void loop() 
{
  lcd.setCursor(0,0);            // move cursor to second line "1" and 9 spaces over
  freq = 1.0/((float)periode/1000000.0);
  
  if ( (millis() - _Tick ) > 1000 )
    freq = 0.0;
  
  if ( freq > 0.0 )
    _Tick = millis();
  
  lcd.print(freq);
  lcd.print(" Hz ");
  
  trmin = freq * 60.0;
  trmin /= 2.0;   //-- pour 2 cylindres
  lcd.setCursor(0,1);
  lcd.print(trmin);
  lcd.print(" tr/min  ");
  lcd.setCursor(10,0);             // move to the begining of the second line

  switch (read_LCD_buttons())
  {               // depending on which button was pushed, we perform an action
    case btnRIGHT:
    {             //  push button "RIGHT" and show the word on the screen
      lcd.print("RIGHT ");
      break;
    }
    case btnLEFT:
    {
      lcd.print("LEFT   "); //  push button "LEFT" and show the word on the screen
      break;
    }    
    case btnUP:
    {
      lcd.print("UP    ");  //  push button "UP" and show the word on the screen
      timerperiode--;
      Timer1.setPeriod(timerperiode);
      break;
    }
    case btnDOWN:
    {
      lcd.print("DOWN  ");  //  push button "DOWN" and show the word on the screen
      timerperiode++;
      Timer1.setPeriod(timerperiode);
      break;
    }
    case btnSELECT:
    {
      lcd.print("SELECT");  //  push button "SELECT" and show the word on the screen
      break;
    }
    case btnNONE:
    {
      lcd.print("NONE  ");  //  No action  will show "None" on the screen
      break;
    }
  }
  delay(100);
  Serial.print(trmin);
  Serial.println(" tr/min  ");
}

unsigned long us;
void trigPIN3() 
{
  us = micros();
  periode = us - ms;
  ms = us;
}
