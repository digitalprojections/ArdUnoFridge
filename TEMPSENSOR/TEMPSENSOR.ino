#include "thermistor.h"
#include "HardwareSerial.h"


// Analog pin used to read the NTC
#define NTC_PIN               A0
#define NTC_PIN2               A1

int LED_R = 10;
int LED_G = 8;

int FT = -70;
int MT = 0;

// Thermistor objects
THERMISTOR thermistor(NTC_PIN,        // Analog pin
                      400,          // Nominal resistance at 25 ºC
                      3000,           // thermistor's beta coefficient
                      400);         // Value of the series resistor

// Global temperature reading
int temp;
int temp2;

bool defrost = false;
/**
 * setup
 *
 * Arduino setup function
 */
void setup()
{
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  //digitalWrite (LED_G, HIGH);
  //digitalWrite (LED_R, LOW);
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
  //temp2 = thermistor2.read();   // Read temperature
  Serial.print("Temp in 1/10 ºC : ");
  Serial.println(temp);
  //Serial.println(temp2);

  //Cooling
  if(temp>=MT && defrost){
    //Melting temperature met and defrost mode was ON
    digitalWrite (LED_G, HIGH);
    digitalWrite (LED_R, LOW);
    defrost = false;
    
  }else if(temp<=FT && !defrost){    
    //it is freezing. Cold enough to rest    
    digitalWrite (LED_G, LOW);
    digitalWrite (LED_R, HIGH);
    //because the defrost mode not yet started, start defrost
    defrost = true;
    
  } else if(temp>FT && !defrost){
    //temp is below melting point and no defrost mode
    //probably a restart in the middle of operation
    //must continue freezing
    digitalWrite (LED_G, HIGH);
    digitalWrite (LED_R, LOW);    
  }
  

  delay(5000);
}
