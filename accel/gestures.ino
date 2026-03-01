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


int x_map;
int y_map;
int millis_xy;

joystick_packet_t current = { 0 };

void setup() {
  Serial.begin(115200);

  pinMode(JOYX, INPUT);
  pinMode(JOYY, INPUT);
  pinMode(DISPATCH, INPUT_PULLUP);

  millis_xy = millis();
}


void range_attune(int* x, int* y) {
  if (*x <= 3050) {
    // Map lower half: [0 to 3050] -> [0 to 2048]
    *x = map(*x, 0, 3050, 0, 2048);
  } else {
    // Map upper half: [3050 to 4095] -> [2048 to 4095]
    *x = map(*x, 3050, 4095, 2048, 4095);
  }

  if (*y <= 3050) {
    // Map lower half: [0 to 3050] -> [0 to 2048]
    *y = map(*y, 0, 3050, 0, 2048);
  } else {
    // Map upper half: [3050 to 4095] -> [2048 to 4095]
    *y = map(*y, 3050, 4095, 2048, 4095);
  }
}


void loop() {

  int x = analogRead(JOYX);
  int y = analogRead(JOYY);

  range_attune(&x, &y);

  int x_previous = x_precise;
  int y_previous = y_precise;

  x_precise = x;
  y_precise = y;

  x_map = map(x, 0, 4095, 0, 6);
  y_map = map(y, 0, 4095, 0, 6);


  if (x > 1750 && x < 2350 && y > 1750 && y < 2350) {
    Serial.print("-");
  } else {
    millis_xy = millis();
    Serial.print(".");
  }

  if (digitalRead(DISPATCH)) {
    pixels[y_map][x_map] = 1;
  }

  if (millis() - millis_xy > 3000) {
    Serial.println("record done");

    cleanNoise(pixels);

    for (int j = 0; j < 7; j++) {
      for (int k = 0; k < 7; k++) {

        Serial.printf("%d ", (int)pixels[j][k]);
      }
      Serial.println();
    }

    int max_counter = 0;
    int max_index = 0;

    for (int i = 0; i < 36; i++) {
      int counter = 0;
      for (int j = 0; j < 7; j++) {
        for (int k = 0; k < 7; k++) {
          if (letters[i][j][k] == 1) {
            if (hasNearbyPixel(j, k, pixels)) {
              counter += 3;
            } else {
              counter -= 4;
            }
          } else {
            if (pixels[j][k] == 1) {
              counter -= 3;
            }
          }
        }
      }
      if (counter > max_counter) {
        max_counter = counter;
        max_index = i;
      }
    }

    memset(pixels, 0, sizeof(pixels));  // sets all bytes to 0

    Serial.println("detected: ");
    Serial.println(real_chars[max_index]);



    delay(3000);

    millis_xy = millis();
  }
}
