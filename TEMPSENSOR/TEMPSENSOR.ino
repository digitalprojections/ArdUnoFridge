#include "thermistor.h"
#include "HardwareSerial.h"


// Analog pin used to read the NTC
#define NTC_PIN               A0
#define NTC_PIN2               A1

int LED_R = 10;
int LED_G = 9;

int FT = -70;
int MT = 0;

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
int temp;
int temp2;
/**
 * setup
 *
 * Arduino setup function
 */
void setup()
{
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  digitalWrite (LED_G, HIGH);
  digitalWrite (LED_R, LOW);
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

  //Cooling
  if(temp>=MT){
    digitalWrite (LED_G, HIGH);
    digitalWrite (LED_R, LOW);
  }else if(temp<=FT){
    //both sensors are freezing. Cold enough to rest
    //start defrost
    digitalWrite (LED_G, LOW);
    digitalWrite (LED_R, HIGH);
    //delay(360000);
  }
  

  delay(5000);
}
