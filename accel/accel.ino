#include "const.h"
#include "chars.h"
#include "filtering.h"
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

joystick_packet_t current = { 0 };

bool pixels[7][7] = { false };

bool movement = false;

int x_map;
int y_map;
int millis_xy;

Adafruit_LSM6DSOX sox;

CRGB leds[NUM_LEDS];

int note_duration;
unsigned long pause_note = 0, start_time;
unsigned long vibration_last_time = 0;

const int buttonPins[] = { CUCANJ_SAG, SHIELD, PUCANJE, SKOK, JOYBTN };

const int button_notes[] = {
  NOTE_G4,
  NOTE_F4,
  NOTE_A4,
  NOTE_B4,
  NOTE_D4
};

const int BUTTON_COUNT = 5;

int start_notes[] = {
  NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_DS5, NOTE_AS4, NOTE_G4,
  NOTE_DS5, NOTE_AS4, NOTE_G4
};
int notes1[] = {
  NOTE_G4, NOTE_G4
};
int notes2[] = {
  NOTE_DS5, NOTE_AS4
};
int notes3[] = {
  NOTE_DS5, NOTE_AS4
};
int notes4[] = {
  NOTE_G4, NOTE_AS4
};
int durations[] = {
  4, 4, 4,
  3, 3, 7,
  3, 3, 9
};

int event_durations[] = {
  4, 4
};

const int* note_sets[] = {
  notes1,
  notes2,
  notes3,
  notes4
};

int button_note_duration = 10;

void range_attune(int* x, int* y) {
  if (*x <= 3050) {

    *x = map(*x, 0, 3050, 0, 2048);
  } else {

    *x = map(*x, 3050, 4095, 2048, 4095);
  }

  if (*y <= 3050) {

    *y = map(*y, 0, 3050, 0, 2048);
  } else {

    *y = map(*y, 3050, 4095, 2048, 4095);
  }
}

void determine_action_end(int x, int y) {


  if (x > 1750 && x < 2350 && y > 1750 && y < 2350 && digitalRead(DISPATCH)) {
    Serial.print("-");
  } else {
    millis_xy = millis();
    movement = true;
    //  Serial.print(".");
  }
}

void finish_recording() {
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
            counter -= 2;
          }
        } else {
          if (pixels[j][k] == 1) {
            counter -= 1;
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

  if (movement) {
    Serial.println("detected: ");
    Serial.println(real_chars[max_index]);
    movement = false;
  }



  /// delay(3000);

  millis_xy = millis();
}

void setup() {
  Serial.begin(115200);

  FastLED.addLeds<WS2812, 2>(leds, NUM_LEDS);

  Wire.begin(SDA, SCL);

  if (!sox.begin_I2C(I2CPROTOCOL)) {
    leds[0] = 0x00FF00;
    FastLED.show();
    while (1)
      ;
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
  pinMode(VIBRATOR, OUTPUT);

  pinMode(JOYX, INPUT);
  pinMode(JOYY, INPUT);
  pinMode(DISPATCH, INPUT_PULLUP);

  millis_xy = millis();

  for (int note = 0; note < 9; note++) {
    note_duration = 1000 / durations[note];
    tone(BUZZER, start_notes[note], note_duration);
    pause_note = note_duration * 1.2;
    delay(pause_note);

    noTone(BUZZER);
  }
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

    int x = (int)((analogRead(JOYX) - 2048) * 0.0488519785051294577430385930);
    int y = (int)((analogRead(JOYY) - 2048) * 0.0488519785051294577430385930);
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

    if (millis() - vibration_last_time >= 100) {
      digitalWrite(VIBRATOR, LOW);
    }


    if (Serial.available()) {
      byte incomingByte = Serial.read();

      if (incomingByte >= 1 && incomingByte <= 4) {
        int i = incomingByte - 1;

        digitalWrite(VIBRATOR, HIGH);
        vibration_last_time = millis();

        tone(BUZZER, note_sets[i][0], 250);
        tone(BUZZER, note_sets[i][1], 250);
      }
    }


    for (int i = 0; i < BUTTON_COUNT; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        tone(BUZZER, button_notes[i], 1000 / button_note_duration);
        noTone(BUZZER);
        digitalWrite(VIBRATOR, HIGH);
        vibration_last_time = millis();
      }
    }
  }

  uint8_t r, g, b;
  ColorConverter::HsvToRgb(millis() / 3600.0, 1.0, 1.0, r, g, b);
  leds[0] = (r << 16) | (g << 8) | b;
  FastLED.show();

  if (memcmp(&current, &previous, sizeof(current)) != 0) {
    millis_var = millis();
    sleep_ = false;
  }

  if (millis() - millis_var > 29297) {
    sleep_ = true;
  }

  if (digitalRead(SKOK) == 0) {
    sleep_ = false;
    millis_var = millis();
  }


  int x = analogRead(JOYX);
  int y = analogRead(JOYY);

  // range_attune(&x, &y);

  x_map = map(x, 0, 4095, 0, 6);
  y_map = map(y, 0, 4095, 6, 0);

  determine_action_end(x, y);

  if (!digitalRead(DISPATCH)) {
    pixels[y_map][x_map] = 1;
  }

  // Serial.println(digitalRead(17));

  if (millis() - millis_xy > 1200) {
    finish_recording();
  }
}
