#include <SPI.h>
#include <SD.h>
#include "AudioTools.h"
#include "AudioCodecs/CodecWAV.h"
#include "Wire.h"

const int chipSelect = D9;
I2SStream i2s; // final output of decoded stream
EncodedAudioStream decoder(&i2s, new WAVDecoder()); // Decoding stream
StreamCopy copier; 
File audioFile;

#define SLAVE_ADDRESS 8

// Function to Receive Data and act based on input received
void receiveSignal(int numBytes)
{
  int c = Wire.read();
  if (c < 30)
  {
    Serial.print("Below threshold - ");
  }
  Serial.println(c);
}

void setup(){
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveSignal);

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