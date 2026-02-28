#include "const.h"
#include <Adafruit_LSM6DSOX.h>

typedef struct {
  int16_t buttons;
  int16_t joy_x;
  int16_t joy_y;
  int16_t tilt_x;
  int16_t tilt_y;
  int16_t tilt_z;
} joystick_packet_t;

joystick_packet_t joystick = {0};

Adafruit_LSM6DSOX sox;

void setup() {
  Serial.begin(115200);
  
  Wire.begin(SDA, SCL);

  if (!sox.begin_I2C(I2CPROTOCOL)) {
    while (1);
  }

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
  int16_t buttons = 0;
  buttons |= (!digitalRead(CUCANJ_SAG) ? 1 : 0) << 0;
  buttons |= (!digitalRead(SHIELD) ? 1 : 0) << 1;
  buttons |= (!digitalRead(PUCANJE) ? 1 : 0) << 2;
  buttons |= (!digitalRead(SKOK) ? 1 : 0) << 3;
  buttons |= (!digitalRead(JOYBTN) ? 1 : 0) << 4;
  joystick.buttons = buttons;
  
  int x = (int)((analogRead(JOYX)-2048) * 0.0488519785051294577430385930);
  int y = (int)((analogRead(JOYY)-2048) * 0.0488519785051294577430385930);
  joystick.joy_x = x;
  joystick.joy_y = y;
  
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  sox.getEvent(&accel, &gyro, &temp);

  joystick.tilt_x = map(accel.acceleration.x, -5, 5, MIN_RANGE, MAX_RANGE);
  joystick.tilt_y = map(accel.acceleration.y, -5, 5, MIN_RANGE, MAX_RANGE);
  joystick.tilt_z = map(accel.acceleration.z, -5, 5, MIN_RANGE, MAX_RANGE);

  Serial.write((uint8_t*)&joystick, sizeof(joystick));
}