#include "const.h"
// #include "chars.cpp"
//#include <Wire.h>
//#include <Adafruit_LSM6DSOX.h>
//#include <FastLED.h>
//#include "ColorConverterLib.h"
#include <cstring>



int x_precise;
int y_precise;

bool letters[3][7][7] = 
{
/* A */
{
{0,0,1,1,1,0,0},
{0,1,0,0,0,1,0},
{1,0,0,0,0,0,1},
{1,1,1,1,1,1,1},
{1,0,0,0,0,0,1},
{1,0,0,0,0,0,1},
{1,0,0,0,0,0,1}
},

/* B */
{
{1,1,1,1,1,0,0},
{1,0,0,0,0,1,0},
{1,0,0,0,0,1,0},
{1,1,1,1,1,0,0},
{1,0,0,0,0,1,0},
{1,0,0,0,0,1,0},
{1,1,1,1,1,0,0}
},

/* C */
{
{0,1,1,1,1,1,0},
{1,0,0,0,0,0,1},
{1,0,0,0,0,0,0},
{1,0,0,0,0,0,0},
{1,0,0,0,0,0,0},
{1,0,0,0,0,0,1},
{0,1,1,1,1,1,0}
}
};

char real_chars[36] = {
'A','B','C','D','E','F','G','H','I','J',
'K','L','M','N','O','P','Q','R','S','T',
'U','V','W','X','Y','Z',
'0','1','2','3','4','5','6','7','8','9'
};

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

  millis_xy = millis();
}




void loop() {
  // delay(50);



  int x_previous = x_precise;
  int y_previous = y_precise;

  x_precise = map(analogRead(JOYX), 0, 4095, -100, 100);
  y_precise = map(analogRead(JOYX), 0, 4095, -100, 100);


  x_map = map(analogRead(JOYX) + 585, 0, 4095, 0, 6);
  y_map = map(analogRead(JOYY) + 585, 0, 4095, 6, 0);

  

  if (x_previous != x_precise && y_previous != y_precise) {
    millis_xy = millis();
    Serial.println("difference");
    pixels[y_map][x_map] = 1;
  } else {
     Serial.println("no difference");
  }

  if (millis() - millis_xy > 3000) {
    Serial.println("record done");

          for (int j = 0; j < 7; j++) {
        for (int k = 0; k < 7; k++) {
         
            Serial.printf("%d ", (int)pixels[j][k]);
          
        }
        Serial.println();
          }

    int max_counter = 0;
    int max_index = 0;

    for (int i = 0; i < 35; i++) {
      int counter = 0;
      for (int j = 0; j < 7; j++) {
        for (int k = 0; k < 7; k++) {
          if (pixels[j][k] == letters[i][j][k]) {
            counter++;
          }
        }
      }
      if (counter > max_counter) {
        max_counter = counter;
        max_index = i;
      }
    }

    memset(pixels, 0, sizeof(pixels)); // sets all bytes to 0

    Serial.println("detected: ");
    Serial.println(real_chars[max_index]);

    

    delay(3000);

    millis_xy = millis();

  }

  Serial.println("x");

  Serial.println(x_map);

  Serial.println(y_map);
}
