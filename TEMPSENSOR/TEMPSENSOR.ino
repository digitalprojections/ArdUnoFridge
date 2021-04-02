#include "thermistor.h"
#include "HardwareSerial.h"


// Analog pin used to read the NTC
#define NTC_PIN A0
#define NTC_PIN2 A1

int LED_Red = 10;
int LED_Green = 8;

int FREEZING_TEMP = -70;
int MELT_TEMP = 20;

// Thermistor objects
THERMISTOR thermistor(NTC_PIN,        // Analog pin
                      400,          // Nominal resistance at 25 ºC
                      3000,           // thermistor's beta coefficient
                      400);         // Value of the series resistor

// Global temperature reading
int temp;
int temp2;

bool wasWorking = false;
/**
 * setup
 *
 * Arduino setup function
 */
void setup()
{
  pinMode(LED_Green, OUTPUT);
  pinMode(LED_Red, OUTPUT);
  digitalWrite (LED_Green, LOW);
  digitalWrite (LED_Red, LOW);
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
  

  //Cooling
  if(temp>=MELT_TEMP && !wasWorking){
    //Melting temperature met and defrost mode was ON
    //START
    motorRun(1);
    wasWorking=true;
    
  }else if(temp<=FREEZING_TEMP && wasWorking){    
    //it is freezing. Cold enough to rest   
    //because the defrost mode not yet started, start defrost
    motorRun(0);
    delay(900000);//30 minutes
    wasWorking=false;
  }



  delay(5000);
}
void motorRun(int allowed){
  Serial.println("motor runs: ");
  Serial.println(allowed);
  Serial.print("Temp in MY CODE ºC : ");  
  Serial.println(temp);
    if(allowed){
      digitalWrite (LED_Green, HIGH);
      digitalWrite (LED_Red, LOW);      
    }else{
      digitalWrite (LED_Green, LOW);
      digitalWrite (LED_Red, HIGH);
    }
  }
