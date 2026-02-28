#include "const.h"
#include <Wire.h>
#include <Adafruit_LSM6DSOX.h>
#include <FastLED.h>
#include "ColorConverterLib.h"

typedef struct {
  int16_t buttons;
  int16_t joy_x;
  int16_t joy_y;
  int16_t tilt_x;
  int16_t tilt_y;
  int16_t tilt_z;
} joystick_packet_t;

int32_t millis_var;
bool sleep_ = false;

joystick_packet_t current = {0};

Adafruit_LSM6DSOX sox;

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  
  FastLED.addLeds<WS2812, 2>(leds, NUM_LEDS); 
  
  Wire.begin(SDA, SCL);

  if (!sox.begin_I2C(I2CPROTOCOL)) {
    leds[0] = 0x00FF00;
    FastLED.show();
    while (1);
  }
  leds[0] = 0xFF0000;
  FastLED.show();
  delay(2000);

  pinMode(JOYX, INPUT);
  pinMode(JOYY, INPUT);
  pinMode(JOYBTN, INPUT_PULLUP);

  pinMode(CUCANJ_SAG, INPUT_PULLUP);
  pinMode(SHIELD, INPUT_PULLUP);
  pinMode(PUCANJE, INPUT_PULLUP);
  pinMode(SKOK, INPUT_PULLUP);
}



void loop() {
  delay(50);
  
  joystick_packet_t previous; 

  memcpy(&previous, &current, sizeof(current));
  
  if (!sleep_) {
    int16_t buttons = 0;
    buttons |= (!digitalRead(CUCANJ_SAG) ? 1 : 0) << 0;
    buttons |= (!digitalRead(SHIELD) ? 1 : 0) << 1;
    buttons |= (!digitalRead(PUCANJE) ? 1 : 0) << 2;
    buttons |= (!digitalRead(SKOK) ? 1 : 0) << 3;
    buttons |= (!digitalRead(JOYBTN) ? 1 : 0) << 4;
    current.buttons = buttons;

    int x = (int)((analogRead(JOYX)-2048) * 0.0488519785051294577430385930);
    int y = (int)((analogRead(JOYY)-2048) * 0.0488519785051294577430385930);
    current.joy_x = x;
    current.joy_y = y;

    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;
    sox.getEvent(&accel, &gyro, &temp);

    current.tilt_x = map(accel.acceleration.x, -5, 5, MIN_RANGE, MAX_RANGE);
    current.tilt_y = map(accel.acceleration.y, -5, 5, MIN_RANGE, MAX_RANGE);
    current.tilt_z = map(accel.acceleration.z, -5, 5, MIN_RANGE, MAX_RANGE);

    Serial.write((uint8_t*)&current, sizeof(current));
  }

  uint8_t r, g, b;
  ColorConverter::HsvToRgb(millis()/3600.0, 1.0, 1.0, r, g, b);
  leds[0] = (r << 16) | (g << 8) | b;
  FastLED.show();
  
  if(memcmp(&current, &previous, sizeof(current)) != 0) {
    millis_var = millis();
    sleep_ = false;
  }
  
  if (millis() - millis_var > 29297) {
    sleep_ = true;
  }

  if(digitalRead(CUCANJ_SAG) == 0) {
    sleep_ = false;
    millis_var = millis();
  }
}