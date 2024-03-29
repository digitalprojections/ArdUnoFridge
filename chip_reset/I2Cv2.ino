// This code updates Ricoh Toner chip for Ricoh Aficio SP C250SF, SP C250DN, C250e, etc...
// Update EEPROM_I2C_ADDRESS define value with the chip you want to reprogram
// 83 is Chip K - black
// 82 is Chip C - cyan
// 81 is Chip M - magenta
// 80 is Chip Y - yellow

#define EEPROM_I2C_ADDRESS 83
#include <Wire.h>

// blank data for K, C, M & Y chip. 128 array of data. I got it from data dump of a replacement chip. 
byte KChipData[]={168,0,1,3,18,1,1,255,100,0,52,48,55,53,52,51,20,9,65,66,22,0,22,38,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,100,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0};
byte CChipData[]={168,0,1,3,14,2,1,255,100,0,49,49,49,53,52,54,20,2,65,66,23,0,7,1,255,255,255,255,255,255,255,255,88,48,56,54,80,52,48,49,50,48,56,0,68,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,1,85,0,18,0,40,0,5,184,230,50,0,128,0,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0};
byte MChipData[]={168,0,1,3,14,3,1,255,100,0,49,49,49,53,52,54,20,2,65,66,24,0,7,16,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,100,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0};
byte YChipData[]={168,0,1,3,14,4,1,255,100,0,49,49,49,53,52,55,20,2,65,66,25,0,3,7,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,100,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0};

void setup() {
  // Select correct data for chip then copy to WriteData array
  byte WriteData[128];
  switch (EEPROM_I2C_ADDRESS){
    case 83:
      memcpy(WriteData,KChipData,128*sizeof(byte));
      break;
    case 82:
      memcpy(WriteData,CChipData,128*sizeof(byte));
      break; 
    case 81:
      memcpy(WriteData,MChipData,128*sizeof(byte));
      break;
    case 80:
      memcpy(WriteData,YChipData,128*sizeof(byte));
      break;
    default: //default K chip data, no reason.
      memcpy(WriteData,KChipData,128*sizeof(byte));
      break;
  }
  
  // Start Wire and Serial bus
  Wire.begin();
  Serial.begin(9600);
  delay(100);

  Serial.println("Start");
  Serial.println(" ");

  // Start Write Chip with blank data
  Serial.println("Write 128 bytes:");
  byte wordaddress;

  for(byte i=0;i<128;i++){
        wordaddress = i;
        i2cwrite((byte)wordaddress,(byte)WriteData[i]);
        Serial.print(wordaddress);
        Serial.print(":");
        Serial.print(WriteData[i]);
        Serial.print(" ");
  }

  // Start Read chip
  Serial.println(" ");
  Serial.println("Read 128 bytes:");
  for(byte i=0;i<128;i++){
      byte readVale = i2cread(i);
      Serial.print(i);
      Serial.print(":");
      Serial.print(readVale);
      Serial.print(" ");
  } 
  Serial.println(" ");  
  Serial.println("End");

}

void loop() {

}

void i2cwrite(byte address, byte data) {
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  Wire.write((byte)address);
  Wire.write((byte)data);
  Wire.endTransmission();
  delay(20);
}

byte i2cread(byte address) {
  byte rData = 0;
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  Wire.write((byte)address);
  Wire.endTransmission();

  Wire.requestFrom(EEPROM_I2C_ADDRESS,1);
  while (Wire.available()){
    rData = Wire.read();
    return rData;
  }
}

void WhatI2CAddress() {
    for(int i=0;i<128;i++){
      Wire.requestFrom(i,1);  //request first data byte
      Serial.print(i);
      Serial.print(":");
      while(Wire.available()){
        byte c = Wire.read();
        Serial.print(c);      //if data exist, print it out. That way you can identify which address.
      }
      Serial.println(" ");
      delay(5);
  }
}


