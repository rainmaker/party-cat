#include <Adafruit_CircuitPlayground.h>
#include <stdlib.h>
#include <time.h>
#include <string>

void setup() {
   // Initialize the circuit playground
  CircuitPlayground.begin();
  Serial.begin(9600);
  srand( (unsigned)time(NULL) );
}

void loop() {
   CircuitPlayground.setBrightness(150);

   loudnessReact();

//  //sparkleRainbow
 for(int i = 0; i < 10; i++) {
   rainbow();
   delay(100);
 }
  
//  //red rotate
//  rotateColor(0xff0000);
//
//  // red / blue slow blink 10x
//  for(int i = 0; i < 10; i++) {
//    slowBlink(0xff0000);
//  }
}

void loudnessReact() {
    // takes time to sample in ms, returns value that tends to be between 40-120db
    float pressure = CircuitPlayground.mic.soundPressureLevel(10);
    Serial.print("pressure: ");
    Serial.print(pressure);
    Serial.println();

    // setting 150 as likely ceiling for loudness
    float loudness = pressure / 150;
    Serial.print("loudness: ");
    Serial.print(loudness);
    Serial.println();

    // scale brightness
    CircuitPlayground.setBrightness(loudness * 255);
    Serial.print("brightness: ");
    Serial.print((loudness * 255));
    Serial.println();

    // scale color
    uint32_t color = loudness * 0xffffff;
    Serial.print("color: ");
    Serial.print(color);
    Serial.println();
    setAllPixels(color);
}

// use fft function
// https://learn.adafruit.com/circuit-playground-sound-controlled-robot/using-the-microphone
void freqReact(){}

const uint32_t rainbow[] = {0xc70039, 0xe17102, 0xffc300, 0x61d802, 0x022bd8, 0x4b0082, 0x9400d3, 0xaa336a};

void rainbow() {
  for (int i = 0; i < 8; i++){
    setAllPixels(rainbow[i]);
    Serial.print(i);
    Serial.println();
    delay(250);
  }
}

void randomSparkle(){
  CircuitPlayground.setPixelColor(rand() % 8, 0xffffff);
}

void setAllPixels(uint32_t color) {
  for(int i = 0; i < 10; i++) {
      CircuitPlayground.setPixelColor(i, color);
      delay(50);
    }
}

void rotateColor(uint32_t color) {
    
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, color);
    delay(250);
  }

  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 0);
    delay(250);
  }
}

void slowBlink(uint32_t color) {
  //get brighter
  for (int i = 0; i < 250; i++) {
    CircuitPlayground.setBrightness(i);
      for (int k = 0; k < 10; k++) {
        CircuitPlayground.setPixelColor(k, color);
      }
    delay(10);
  }

  // gradually change color
  for (int k = 0; k < 10; k++) {
    CircuitPlayground.setPixelColor(k, 0x0000ff);
    delay(50);
  }
  
  // get dimmer
  for (int i = 250; i > 0; i--) {
    CircuitPlayground.setBrightness(i);
    for (int k = 0; k < 10; k++) {
      CircuitPlayground.setPixelColor(k, 0x0000ff);
    }
    delay(10);
  }

  // gradually change back
  for (int k = 10; k >= 0; k--) {
    CircuitPlayground.setPixelColor(k, 0xff0000);
    delay(50);
  }
  
 }
