#include "joystick.h"

#define CUCANJ_SAG 41 // Cucanj i saginjanje
#define SHIELD 39
#define PUCANJE 40
#define SKOK 14

void setup(){
  // Definiranje pinova
  Serial.begin(115200);
  pinMode(CUCANJ_SAG,INPUT_PULLUP);
  pinMode(SHIELD,INPUT_PULLUP);
  pinMode(PUCANJE,INPUT_PULLUP);
  pinMode(SKOK, INPUT_PULLUP);
  Serial.println("Setup complete.");
}
void loop(){
  if (digitalRead(CUCANJ_SAG) == LOW){
    Serial.println("cucanj je pritisnut.");

  }
  if (digitalRead(SHIELD) == LOW){
    Serial.println("shield je pritisnut.");

  }
  if (digitalRead(PUCANJE) == LOW){
    Serial.println("pucanje je pritisnut.");

  }
  if (digitalRead(SKOK) == LOW){
    Serial.println("skok je pritisnut.");

  }
}
