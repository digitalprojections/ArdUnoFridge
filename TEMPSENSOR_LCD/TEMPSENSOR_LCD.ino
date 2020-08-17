#include "thermistor.h"
#include "HardwareSerial.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Analog pin used to read the NTC
#define NTC_PIN               A0
#define NTC_PIN2               A1

int LED_G = 9;
int LED_R = 8;
int RR = 7;
int RL = 10;

int FT = -10;
int MT = 5;

// Thermistor objects
THERMISTOR thermistor(NTC_PIN,        // Analog pin
                      10000,          // Nominal resistance at 25 ºC
                      3950,           // thermistor's beta coefficient
                      10000);         // Value of the series resistor
THERMISTOR thermistor2(NTC_PIN2,        // Analog pin
                      10000,          // Nominal resistance at 25 ºC
                      3950,           // thermistor's beta coefficient
                      10000);         // Value of the series resistor
// Global temperature reading
uint16_t temp;
uint16_t temp2;
/**
 * setup
 *
 * Arduino setup function
 */
void setup()
{
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(RR, OUTPUT);
  pinMode(RL, OUTPUT);
  digitalWrite (LED_G, HIGH);
  digitalWrite (LED_R, LOW);
  digitalWrite (RR, HIGH);
  digitalWrite (RL, LOW);
  Serial.begin(9600);
}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{
  temp = thermistor.read();   // Read temperature
  temp2 = thermistor2.read();   // Read temperature
  Serial.print("Temp in 1/10 ºC : ");
  Serial.println(temp);
  Serial.println(temp2);
  lcd.print("Temp = ");
  lcd.print(temp);   
  lcd.print(" C");

  //Cooling
  if(temp>=MT && temp2>=MT){
    digitalWrite (LED_G, HIGH);
    digitalWrite (LED_R, LOW);
    digitalWrite (RR, HIGH);
    digitalWrite (RL, LOW);
  }else if(temp<=FT && temp2<=FT){
    //both sensors are freezing. Cold enough to rest
    //start defrost
    digitalWrite (LED_G, LOW);
    digitalWrite (LED_R, HIGH);
    digitalWrite (RR, LOW);
    digitalWrite (RL, HIGH);
  }
  
  delay(5000);              
  lcd.clear();
}
