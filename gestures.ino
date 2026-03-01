// #include "const.h"
#include "chars.h"
//#include <Wire.h>
//#include <Adafruit_LSM6DSOX.h>
//#include <FastLED.h>
//#include "ColorConverterLib.h"
#include <cstring>

#define JOYX 26
#define JOYY 27
#define DISPATCH 17

int x_precise;
int y_precise;


typedef struct {
  int16_t buttons;
  int16_t joy_x;
  int16_t joy_y;
  int16_t tilt_x;
  int16_t tilt_y;
  int16_t tilt_z;
} joystick_packet_t;

bool pixels[7][7] = { false };



joystick_packet_t current = { 0 };

void setup() {
  Serial.begin(115200);

  pinMode(JOYX, INPUT);
  pinMode(JOYY, INPUT);
  pinMode(DISPATCH, INPUT_PULLUP);

  millis_xy = millis();
}




void loop() {

  int x = analogRead(JOYX);
  int y = analogRead(JOYY);

  range_attune(&x, &y);

  x_map = map(x, 0, 4095, 0, 6);
  y_map = map(y, 0, 4095, 0, 6);

  determine_action_end(x, y);

  if (digitalRead(DISPATCH)) {
    pixels[y_map][x_map] = 1;
  }

  if (millis() - millis_xy > 3000) {
    finish_recording();
  }
}
