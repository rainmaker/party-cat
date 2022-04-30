#include <Adafruit_CircuitPlayground.h>
#include <stdlib.h>
#include <time.h>
#include <string>

// Adjust this number for the sensitivity of the 'click' force
// this strongly depend on the range! for 16G, try 5-10
// for 8G, try 10-20. for 4G try 20-40. for 2G try 40-80
#define CLICKTHRESHHOLD 200
bool rightButtonPressed, leftButtonPressed;
int mode = 0;

void setup() {
   // Initialize the circuit playground
  CircuitPlayground.begin();
  Serial.begin(9600);

  // setupTap();
}

void loop() {
  CircuitPlayground.setBrightness(150);
  rightButtonPressed = CircuitPlayground.rightButton();
  leftButtonPressed = CircuitPlayground.leftButton();

  if(rightButtonPressed || leftButtonPressed) {
    changeMode();
  }

  switch(mode){
    case 0:
      loudnessReact();
      break;
    case 1:
      rainbowFade();
      break;
    case 2:
      slowBlink(0xff0000);
      break;
    case 3:
      rotateColor(0x00ff00);
      break;
  }
}

void changeMode() {
  Serial.println("Changing mode");
  if (mode == 3) {
    mode = 0;
  } else {
    mode++;
  }
}

bool modeChangeInterrupt() {
  rightButtonPressed = CircuitPlayground.rightButton();
  leftButtonPressed = CircuitPlayground.leftButton();
  if (leftButtonPressed || rightButtonPressed) {
    return true;
  }
  return false;
}

void setupTap() {
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!

  // 0 = turn off click detection & interrupt
  // 1 = single click only interrupt output
  // 2 = double click only interrupt output, detect single click
  // Adjust threshhold, higher numbers are less sensitive
  CircuitPlayground.setAccelTap(1, CLICKTHRESHHOLD);
  
  //mode: defines when the interrupt should be triggered. Four constants are predefined as valid values:
  // LOW to trigger the interrupt whenever the pin is low,
  // CHANGE to trigger the interrupt whenever the pin changes value
  // RISING to trigger when the pin goes from low to high,
  // FALLING for when the pin goes from high to low.

  // have a procedure called when a tap is detected
  attachInterrupt(digitalPinToInterrupt(CPLAY_LIS3DH_INTERRUPT), changeMode, FALLING);
}

void temperatureReact() {
  float tempF = CircuitPlayground.temperatureF();

  Serial.print("  tempF: ");
  Serial.println(tempF);
}

void loudnessReact() {
  Serial.println("Loudness react");

  // takes time to sample in ms, returns value that tends to be between 40-120db
  float pressure = CircuitPlayground.mic.soundPressureLevel(5);

  // setting 150 as likely ceiling for loudness
  float loudness = pressure / 150;

  // scale brightness
  CircuitPlayground.setBrightness(loudness * 255);

  // scale color
  uint32_t color = loudness * 0xffffff;
  setAllPixels(color);
}

// use fft function
// https://learn.adafruit.com/circuit-playground-sound-controlled-robot/using-the-microphone
void freqReact(){}

const uint32_t rainbow[] = {0xc70039, 0xe17102, 0xffc300, 0x61d802, 0x022bd8, 0x4b0082, 0x9400d3, 0xaa336a};

void rainbowFade() {
  Serial.println("Rainbow fade");
  for (int i = 0; i < 8; i++){
    setAllPixels(rainbow[i]);
    if (modeChangeInterrupt()) {
      return;
    }
    else {
      delay(250);
    }
  }
}

void setAllPixels(uint32_t color) {
  for(int i = 0; i < 10; i++) {
      CircuitPlayground.setPixelColor(i, color);
      delay(50);
    }
}

void rotateColor(uint32_t color) {
  Serial.println("Rotate color");
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, color);
    if (modeChangeInterrupt()) {
      return;
    }
    else {
      delay(250);
    }
  }

  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 0);
    if (modeChangeInterrupt()) {
      return;
    }
    else {
      delay(250);
    }
  }
}

void slowBlink(uint32_t color) {
  Serial.println("Slow blink");
  //get brighter
  for (int i = 0; i < 250; i++) {
    CircuitPlayground.setBrightness(i);
      for (int k = 0; k < 10; k++) {
        CircuitPlayground.setPixelColor(k, color);
      }

    if (modeChangeInterrupt()) {
      return;
    }
    else {
      delay(10);
    }
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
