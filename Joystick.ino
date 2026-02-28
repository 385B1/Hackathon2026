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
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(10, INPUT_PULLUP);
}

void loop() {
  delay(100);
  int x = (int)((analogRead(11)-2048) * 0.0488519785051294577430385930);
  int y = (int)((analogRead(12)-2048) * 0.0488519785051294577430385930);
  joystick.joy_x = x;
  joystick.joy_y = y;
  Serial.write((uint8_t*)&joystick, sizeof(joystick));
  /*Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.println(digitalRead(10 ) == LOW);*/
}