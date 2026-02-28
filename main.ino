#include "const.h"
#include <esp_system.h>

typedef struct {
  int16_t buttons;
  int16_t joy_x;
  int16_t joy_y;
  int16_t tilt_x;
  int16_t tilt_y;
  int16_t tilt_z;
} joystick_packet_t;

joystick_packet_t joystick = {0};

TaskHandle_t melodija_handler = nullptr;
int note = 0, note_duration;
unsigned long pause_note = 0, start_time, vibration_last_time = 0;

/*
void melodija(void *param){
  (void)param;
  int note_duration;
  int notes[] = {
    NOTE_G4, NOTE_G4, NOTE_G4,
    NOTE_DS5, NOTE_AS4,NOTE_G4,
    NOTE_DS5, NOTE_AS4, NOTE_G4
  };
  int durations[] = {
    4, 4, 4,
    3, 3, 7,
    3, 3, 9
  };
  
    Serial.println("Melody loop");
    for (int note = 0; note < 9; note++){
      note_duration = 1000/durations[note];
      tone(BUZZER,notes[note],note_duration);
      pause = note_duration * 1.2;
      vTaskDelay(pdMS_TO_TICKS(pause));

      noTone(BUZZER);

    }
    vTaskDelete(NULL);
    
}
*/
  int notes[] = {
    NOTE_G4, NOTE_G4, NOTE_G4,
    NOTE_DS5, NOTE_AS4,NOTE_G4,
    NOTE_DS5, NOTE_AS4, NOTE_G4
  };
  int durations[] = {
    4, 4, 4,
    3, 3, 7,
    3, 3, 9
  };
int button_note_duration = 10;
void setup() {
  
  Serial.begin(115200);
  pinMode(JOYX, INPUT);
  pinMode(JOYY, INPUT);
  pinMode(JOYBTN, INPUT_PULLUP);

  pinMode(CUCANJ_SAG, INPUT_PULLUP);
  pinMode(SHIELD, INPUT_PULLUP);
  pinMode(PUCANJE, INPUT_PULLUP);
  pinMode(SKOK, INPUT_PULLUP);
  pinMode(BUZZER,OUTPUT);
  pinMode(VIBRATOR,OUTPUT);

    for (int note = 0; note < 9; note++){
      note_duration = 1000/durations[note];
      tone(BUZZER,notes[note],note_duration);
      pause_note = note_duration * 1.2;
      delay(pause_note);

      noTone(BUZZER);

    }

  //ledcAttachChannel(BUZZER, 2000, 8, BUZZER_CH);
  //ledcAttachChannel(VIBRATOR,2000,8,VIBRATOR_CH);
  }


void loop() {

  int16_t buttons = 0;
  // buttons bit shifting
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
  
  if (millis() - vibration_last_time >= 100){
    digitalWrite(VIBRATOR,LOW);
  }

  if (digitalRead(CUCANJ_SAG) == LOW){
    tone(BUZZER,NOTE_G4,1000/button_note_duration);
    noTone(BUZZER);
    digitalWrite(VIBRATOR,HIGH);
    vibration_last_time = millis();
  }
    if (digitalRead(SHIELD) == LOW){
    tone(BUZZER,NOTE_F4,1000/button_note_duration);
    noTone(BUZZER);
    digitalWrite(VIBRATOR,HIGH);
    vibration_last_time = millis();
  }
    if (digitalRead(PUCANJE) == LOW){
    tone(BUZZER,NOTE_A4,1000/button_note_duration);
    noTone(BUZZER);
    digitalWrite(VIBRATOR,HIGH);
    vibration_last_time = millis();
  }
    if (digitalRead(SKOK) == LOW){
    tone(BUZZER,NOTE_B4,1000/button_note_duration);
    noTone(BUZZER);
    digitalWrite(VIBRATOR,HIGH);
    vibration_last_time = millis();
  }
    if (digitalRead(JOYBTN) == LOW){
    tone(BUZZER,NOTE_D4,1000/button_note_duration);
    noTone(BUZZER);
    digitalWrite(VIBRATOR,HIGH);
    vibration_last_time = millis();
  }

  //Serial.println(buttons);
  /*Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.println(digitalRead(10 ) == LOW);*/
  delay(20);
}


