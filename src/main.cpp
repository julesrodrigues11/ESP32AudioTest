#include <SPI.h>
#include <SD.h>
#include "AudioTools.h"
#include "AudioCodecs/CodecWAV.h"

const int chipSelect = D9;
I2SStream i2s; // final output of decoded stream
EncodedAudioStream decoder(&i2s, new WAVDecoder()); // Decoding stream
StreamCopy copier; 
File audioFile;

int lux;

void setup(){
  Serial.begin(115200);
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

  copier.copy();
  if (audioFile.size() > 0 && audioFile.available()==0)
  {
    audioFile.seek(0);
    Serial.println("Replay");
  }
  
}