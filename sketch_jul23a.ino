
/*
 Stepper Motor Control - speed control

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.
 A potentiometer is connected to analog input 0.

 The motor will rotate in a clockwise direction. The higher the potentiometer value,
 the faster the motor speed. Because setSpeed() sets the delay between steps,
 you may notice the motor is less responsive to changes in the sensor value at
 low speeds.

 Created 30 Nov. 2009
 Modified 28 Oct 2010
*/

#include "I2Cdev.h" 
#include "MPU6050_6Axis_MotionApps20.h" 
#include "Wire.h"
#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor
MPU6050 mpu; 
bool initialization = false;  // set true if DMP init was successful 
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU 
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error) 
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes) 
uint16_t fifoCount;     // count of all bytes currently in FIFO 
uint8_t fifoBuffer[64]; // FIFO storage buffer 
Quaternion q;           // [w, x, y, z]         quaternion container 
VectorFloat gravity;    // [x, y, z]            gravity vector 
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector 
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin 

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

int stepCount = 0;  // number of steps the motor has taken

void setup() {
  // nothing to do inside the setup
  Serial.begin(9600);
  Serial.println("Program started"); 
  initialization = initializeGyroscope();   
}

void loop() {
  // read the sensor value:
  int sensorReading = analogRead(A0);
  // map it to a range from 0 to 100:
  int motorSpeed = map(sensorReading, 0, 1023, 0, 100);

  Serial.println(motorSpeed);
  // set the motor speed:
  if (motorSpeed > 0) {
    myStepper.setSpeed(motorSpeed);
    // step 1/100 of a revolution:
    myStepper.step(stepsPerRevolution / 100);
  }
  if (!initialization) { 
       return; 
   } 
   mpuInterrupt = false; 
   mpuIntStatus = mpu.getIntStatus(); 
   fifoCount = mpu.getFIFOCount(); 
   if (hasFifoOverflown(mpuIntStatus, fifoCount)) { 
       mpu.resetFIFO(); 
       return; 
   } 
   if (mpuIntStatus & 0x02) { 
       while (fifoCount < packetSize) { 
           fifoCount = mpu.getFIFOCount(); 
       } 
       mpu.getFIFOBytes(fifoBuffer, packetSize);         
       fifoCount -= packetSize; 
       mpu.dmpGetQuaternion(&q, fifoBuffer); 
       mpu.dmpGetGravity(&gravity, &q); 
       mpu.dmpGetYawPitchRoll(ypr, &q, &gravity); 
       //redrawLeds(ypr[0] * 180/M_PI, ypr[1] * 180/M_PI, ypr[2] * 180/M_PI); 
       Serial.print(ypr[0] * 180/M_PI);
       Serial.print(" ");
       Serial.print(ypr[1] * 180/M_PI);
       Serial.print(" ");
       Serial.print(ypr[2] * 180/M_PI);
   }
}
boolean hasFifoOverflown(int mpuIntStatus, int fifoCount)  
{ 
   return mpuIntStatus & 0x10 || fifoCount == 1024; 
} 
bool initializeGyroscope() { 
   Wire.begin(); 
   TWBR = 24;   
   mpu.initialize(); 
   Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed")); 
   Serial.println(F("Initializing DMP...")); 
   devStatus = mpu.dmpInitialize(); 
   mpu.setXGyroOffset(220); 
   mpu.setYGyroOffset(76); 
   mpu.setZGyroOffset(-85); 
   mpu.setZAccelOffset(1788); 
   if (devStatus != 0) { 
       Serial.print(F("DMP Initialization failed (code "));Serial.println(devStatus); 
       return false; 
   } 
   mpu.setDMPEnabled(true); 
   Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)...")); 
  attachInterrupt(0, dmpDataReady, RISING);   
   mpuIntStatus = mpu.getIntStatus(); 
   Serial.println(F("DMP ready! Waiting for first interrupt...")); 
   packetSize = mpu.dmpGetFIFOPacketSize(); 
   return true; 
} 
void dmpDataReady()  
{ 
   mpuInterrupt = true; 
} 
