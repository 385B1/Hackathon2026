#include "const.h"

typedef struct {
  int16_t buttons;
  int16_t joy_x;
  int16_t joy_y;
  int16_t tilt_x;
  int16_t tilt_y;
  int16_t tilt_z;
} joystick_packet_t;

joystick_packet_t joystick = {0};

void setup() {
  Serial.begin(115200);
  pinMode(JOYX, INPUT);
  pinMode(JOYY, INPUT);
  pinMode(JOYBTN, INPUT_PULLUP);

  pinMode(CUCANJ_SAG, INPUT_PULLUP);
  pinMode(SHIELD, INPUT_PULLUP);
  pinMode(PUCANJE, INPUT_PULLUP);
  pinMode(SKOK, INPUT_PULLUP);
}

void loop() {
  delay(100);
  int16_t buttons = 0;
  buttons |= (!digitalRead(CUCANJ_SAG) ? 1 : 0) << 0;
  buttons |= (!digitalRead(SHIELD) ? 1 : 0) << 1;
  buttons |= (!digitalRead(PUCANJE) ? 1 : 0) << 2;
  buttons |= (!digitalRead(SKOK) ? 1 : 0) << 3;
  buttons |= (!digitalRead(JOYBTN) ? 1 : 0) << 4;
  int x = (int)((analogRead(11)-2048) * 0.0488519785051294577430385930);
  int y = (int)((analogRead(12)-2048) * 0.0488519785051294577430385930);
  joystick.joy_x = x;
  joystick.joy_y = y;
  joystick.buttons = buttons;
  Serial.write((uint8_t*)&joystick, sizeof(joystick));
  //Serial.println(buttons);
  /*Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.println(digitalRead(10 ) == LOW);*/
}