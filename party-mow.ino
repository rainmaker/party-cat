#include <Adafruit_CircuitPlayground.h>
#include <stdlib.h>
#include <time.h>
#include <string>

bool rightButtonPressed, leftButtonPressed;
bool modeChangeEvent;
enum Mode { ModeRainbowFade, ModeSlowBlinkRed, ModeRotateGreen, ModeRotatePink, ModeRotateRed, ModeSolidWarmWhite, ModeFadeWarmWhite};
Mode selectedMode;

void setup() {
   // Initialize the circuit playground
  CircuitPlayground.begin();
  Serial.begin(9600);
  selectedMode = ModeRainbowFade;
  modeChangeEvent = false;
}

void loop() {
  CircuitPlayground.setBrightness(150);

  if(modeChangeEvent) {
    changeMode();
  }

  switch(selectedMode){
    case ModeRainbowFade:
      rainbowFade();
      break;
    case ModeSolidWarmWhite:
      solidWarmWhite();
      break;
    case ModeSlowBlinkRed:
      slowBlink(0xff0000);
      break;
    case ModeRotateGreen:
      rotateColor(0x00ff00);
      break;
    case ModeRotatePink:
      rotateColor(0xde028d);
      break;
    case ModeRotateRed:
      rotateColor(0xd40902);
      break;
  }

  Serial.println("sleeping");
  delay(500);
}

void changeMode() {
  Serial.println("Current mode is" + selectedMode);
  modeChangeEvent = false;

  switch(selectedMode) {
    case ModeRainbowFade:
      selectedMode = ModeSolidWarmWhite;
      break;
    case ModeSolidWarmWhite:
      selectedMode = ModeRotateGreen;
      break;
    case ModeRotateGreen:
      selectedMode = ModeRotatePink;
      break;
    case ModeRotatePink:
      selectedMode = ModeRotateRed;
      break;
    case ModeRotateRed:
      selectedMode = ModeRainbowFade;
      break;
  }

  Serial.println("Mode changed to" + selectedMode);
  Serial.flush();
}

bool modeChangeInterrupt() {
  rightButtonPressed = CircuitPlayground.rightButton();
  leftButtonPressed = CircuitPlayground.leftButton();
  if (leftButtonPressed || rightButtonPressed) {
    return true;
  }
  return false;
}

void temperatureReact() {
  float tempF = CircuitPlayground.temperatureF();

  Serial.print("  tempF: ");
  Serial.println(tempF);
}

void setAllPixels(uint32_t color) {
  for(int i = 0; i < 10; i++) {
      CircuitPlayground.setPixelColor(i, color);
      delay(50);
    }
}

void fadeAllPixels(uint32_t color) {
  for (int i = 0; i < 250; i++) {
    CircuitPlayground.setBrightness(i);
    for(int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, color);
        delay(10);
    }
  }
}

const uint32_t rainbow[] = {0xc70039, 0xe17102, 0xffc300, 0x61d802, 0x022bd8, 0x4b0082, 0x9400d3, 0xaa336a};

void rainbowFade() {
  Serial.println("Rainbow fade");
  for (int i = 0; i < 8; i++){
    setAllPixels(rainbow[i]);
    if (modeChangeInterrupt()) {
      modeChangeEvent = modeChangeInterrupt();
      return;
    }
    else {
      delay(250);
    }
  }
}

const uint32_t warmWhite = 0xffb026;
void solidWarmWhite() {
  setAllPixels(warmWhite);

  if (modeChangeInterrupt()) {
    modeChangeEvent = modeChangeInterrupt();
    return;
  }
  else {
    delay(250);
  }
}

const uint32_t black = 0x000000;
void rotateColor(uint32_t color) {
  Serial.println("Rotate color");
  setAllPixels(color);
  if (modeChangeInterrupt()) {
    modeChangeEvent = modeChangeInterrupt();
    return;
  }
  else {
    delay(250);
  }

  setAllPixels(black);
  if (modeChangeInterrupt()) {
    modeChangeEvent = modeChangeInterrupt();
    return;
  }
  else {
    delay(250);
  }
}

void slowBlink(uint32_t color) {
  Serial.println("Slow blink");
  //get brighter
  for (int i = 0; i < 250; i++) {
    CircuitPlayground.setBrightness(i);
      setAllPixels(color);
    if (modeChangeInterrupt()) {
      modeChangeEvent = modeChangeInterrupt();
      return;
    }
  }

  // get dimmer
  for (int i = 250; i > 0; i--) {
    CircuitPlayground.setBrightness(i);
    setAllPixels(color);
  }
 }
