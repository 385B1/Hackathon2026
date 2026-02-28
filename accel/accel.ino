#include <Wire.h>
#include <Adafruit_LSM6DSOX.h>

#define MIN_RANGE -100
#define MAX_RANGE 100

typedef struct { 
int16_t buttons; 
int16_t joy_x; 
int16_t joy_y; 
int16_t tilt_x; 
int16_t tilt_y; 
int16_t tilt_z; 
} joystick_packet_t; 

Adafruit_LSM6DSOX sox;

int32_t millis;
bool sleep = false;

joystick_packet_t current = {0};

void setup() {
  Serial.begin(115200);

  Wire.begin(8, 9);   // correct pins for your board

  if (!sox.begin_I2C(0x6B)) {
    Serial.println("Failed to find LSM6DSOX chip");
    while (1);
  }

  Serial.println("LSM6DSOX Found!");

   // sox.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS );
  Serial.print("Gyro range set to: ");
  switch (sox.getGyroRange()) {
  case LSM6DS_GYRO_RANGE_125_DPS:
    Serial.println("125 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_250_DPS:
    Serial.println("250 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_500_DPS:
    Serial.println("500 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_1000_DPS:
    Serial.println("1000 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_2000_DPS:
    Serial.println("2000 degrees/s");
    break;
  case ISM330DHCX_GYRO_RANGE_4000_DPS:
    break; // unsupported range for the DSOX
  }
}

void loop() {
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  sox.getEvent(&accel, &gyro, &temp);


  struct joystick_packet_t previous = memcpy(&previous, &current, sizeof(struct joystick_packet_t));

  delay(50);

  if (!sleep) {

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

  Serial.write((uint8_t*)&current, sizeof(joystick));
  }


  if(memcmp(&current, &previous, sizeof(struct joystick_packet_t)) != 0) {
      int32_t millis = millis();
      sleep = false;
      // current = memcpy(&current, &previous, sizeof(struct joystick_packet_t));
  }

  if (millis() - millis > 2930) {
    sleep = true;
  }

  if(digitalRead(CUCANJ_SAG) == 1) {
    sleep = false;
    int32_t millis = millis();
  }

  

    Serial.print("\t\tAccel X: ");
  //Serial.print(accel.acceleration.x);
  Serial.print(map(accel.acceleration.x, -5, 5, MIN_RANGE, MAX_RANGE));
  Serial.print(" \tY: ");
  //Serial.print(accel.acceleration.y);
  Serial.print(map(accel.acceleration.y, -5, 5, MIN_RANGE, MAX_RANGE));
  Serial.print(" \tZ: ");
  //Serial.print(accel.acceleration.z);
  Serial.print(map(accel.acceleration.z, -5, 5, MIN_RANGE, MAX_RANGE));
  Serial.println(" m/s^2 ");

  joystick_packet_t.tilt_x = map(accel.acceleration.x, -5, 5, MIN_RANGE, MAX_RANGE);
  joystick_packet_t.tilt_y = map(accel.acceleration.y, -5, 5, MIN_RANGE, MAX_RANGE);
  joystick_packet_t.tilt_z = map(accel.acceleration.z, -5, 5, MIN_RANGE, MAX_RANGE);

  delay(100);
}