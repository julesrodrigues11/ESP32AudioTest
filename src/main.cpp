/**
 * @file streams-sd_mp3-i2s.ino
 * @author Phil Schatzmann
 * @brief decode MP3 file and output it on I2S
 * @version 0.1
 * @date 2021-96-25
 * 
 * @copyright Copyright (c) 2021 
 */


#include <SPI.h>
#include <SD.h>
#include "AudioTools.h"
#include "AudioCodecs/CodecWAV.h"
#include "Adafruit_VEML7700.h"


const int chipSelect = D9;
I2SStream i2s; // final output of decoded stream
EncodedAudioStream decoder(&i2s, new WAVDecoder()); // Decoding stream
StreamCopy copier; 
File audioFile;

Adafruit_VEML7700 lightSensor = Adafruit_VEML7700();
int lux;

void lightSensorSetup()
{
    if (!lightSensor.begin())
    {
        Serial.println("Sensor not found");
        while(1);
    }
    Serial.println("Sensor found");

    Serial.print(F("Gain: "));
    switch (lightSensor.getGain())
    {
        case VEML7700_GAIN_1: Serial.println("1"); break;
        case VEML7700_GAIN_2: Serial.println("2"); break;
        case VEML7700_GAIN_1_4: Serial.println("1/4"); break;
        case VEML7700_GAIN_1_8: Serial.println("1/8"); break;
    }

    Serial.print(F("Integration Time (ms): "));
    switch (lightSensor.getIntegrationTime())
    {
        case VEML7700_IT_25MS: Serial.println("25"); break;
        case VEML7700_IT_50MS: Serial.println("50"); break;
        case VEML7700_IT_100MS: Serial.println("100"); break;
        case VEML7700_IT_200MS: Serial.println("200"); break;
        case VEML7700_IT_400MS: Serial.println("400"); break;
        case VEML7700_IT_800MS: Serial.println("800"); break;
    }

    lightSensor.setLowThreshold(10000);
    lightSensor.setHighThreshold(20000);
    lightSensor.interruptEnable(true);

}


void setup(){
  Serial.begin(115200);
  lightSensorSetup();
  AudioLogger::instance().begin(Serial, AudioLogger::Info);  

  // setup file
  SD.begin(chipSelect);
  audioFile = SD.open("/purr10.wav");

  // setup i2s
  auto config = i2s.defaultConfig(TX_MODE);
  i2s.begin(config);

  // setup I2S based on sampling rate provided by decoder
  decoder.begin();

  // begin copy
  copier.begin(decoder, audioFile);

}

void loop()
{
  //lux = (int)lightSensor.readLux();
  //Serial.println(lux);
  
  /*
  if(lux <= 30)
  {
    copier.begin(decoder, audioFile);
    if(!copier.copy())
    {
      stop();
    }
  }
  */

  
  if (!copier.copy()) {
    stop();
  }
  
}