/* Code of the SDC 2019 competition robot.
 *  
 * Coded by:
 * Jaime Ginorio: jaime.ginorio@gmail.com
 * Steven Ramírez: steven.ramirez3@upr.edu
 * Abdiel Cortés: abdiel.cortes@upr.edu
 */

#include <Servo.h>
#include <PS4BT.h>
#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb);
BTD Btd(&Usb);
PS4BT PS4(&Btd);

// Objects declarations
Servo front;
Servo rear;

Servo right;
Servo left;

Servo door;

Servo fence;

// Variables declarations
byte Ljoy_X_pos_1;
byte Ljoy_X_pos_2;
byte Ljoy_X_neg_1;
byte Ljoy_X_neg_2;

byte Ljoy_Y_pos_1;
byte Ljoy_Y_pos_2;
byte Ljoy_Y_neg_1;
byte Ljoy_Y_neg_2;

byte Rjoy_X_pos;
byte Rjoy_X_neg;


void setup() {
  Serial.begin(115200);
  #if!defined(__MIPSEL__)
    while (!Serial);
  #endif
  if(Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1);
  }
  // X coordinates servos
  front.attach(28); // rojos 1
  rear.attach(32); // rojos 2
  // Y coordinates servos
  right.attach(34); // rojos 3
  left.attach(36); // rojos 4
  // Door servo
  door.attach(46); // vex 1
  // Fence servo
  fence.attach(48); // vex 2
}

void loop() {
  Usb.Task();
  if(PS4.connected()) {
    // Left Joystick X coordinates map.
    Ljoy_X_pos_1 = map(PS4.getAnalogHat(LeftHatX), 185, 255, 100, 180);
    Ljoy_X_pos_2 = map(PS4.getAnalogHat(LeftHatX), 185, 255, 80, 0);
    Ljoy_X_neg_1 = map(PS4.getAnalogHat(LeftHatX), 70, 0, 80, 0);
    Ljoy_X_neg_2 = map(PS4.getAnalogHat(LeftHatX), 70, 0, 100, 180);

    // Conditions to move the robot in the X coordinates.
    if(PS4.getAnalogHat(LeftHatX) >= 185) {
      front.write(Ljoy_X_pos_1);
      rear.write(Ljoy_X_pos_2);
    }
    if(PS4.getAnalogHat(LeftHatX) <= 70) {
      front.write(Ljoy_X_neg_1);
      rear.write(Ljoy_X_neg_2);
    }
    if(PS4.getAnalogHat(LeftHatX) < 185 && PS4.getAnalogHat(LeftHatX) > 70 && PS4.getAnalogHat(RightHatX) < 185 && PS4.getAnalogHat(RightHatX) > 70) {
      front.write(93);
      rear.write(93);
    }

    // Left Joystick Y coordinates map.
    Ljoy_Y_pos_1 = map(PS4.getAnalogHat(LeftHatY), 70, 0, 100, 180);
    Ljoy_Y_pos_2 = map(PS4.getAnalogHat(LeftHatY), 70, 0, 80, 0);
    Ljoy_Y_neg_1 = map(PS4.getAnalogHat(LeftHatY), 185, 255, 80, 0);
    Ljoy_Y_neg_2 = map(PS4.getAnalogHat(LeftHatY), 185, 255, 100, 180);
    
    // Conditions to move the robot in the Y coordinates
    if(PS4.getAnalogHat(LeftHatY) <= 70) {
      right.write(Ljoy_Y_pos_1);
      left.write(Ljoy_Y_pos_2);
    }else if(PS4.getAnalogHat(LeftHatY) >= 185) {
      right.write(Ljoy_Y_neg_1);
      left.write(Ljoy_Y_neg_2);
    }else {
      right.write(93);
      left.write(93);
    }

    // Right Joystick X coordinates map.
    Rjoy_X_pos = map(PS4.getAnalogHat(RightHatX), 185, 255, 100, 180);
    Rjoy_X_neg = map(PS4.getAnalogHat(RightHatX), 70, 0, 80, 0);

    // Conditions to rotate the robot.
    if(PS4.getAnalogHat(RightHatX) >= 185) {
      front.write(Rjoy_X_pos);
      rear.write(Rjoy_X_pos);
      right.write(Rjoy_X_pos);
      left.write(Rjoy_X_pos);
    }
    if(PS4.getAnalogHat(RightHatX) <= 70) {
      front.write(Rjoy_X_neg);
      rear.write(Rjoy_X_neg);
      right.write(Rjoy_X_neg);
      left.write(Rjoy_X_neg);
    }

    // Conditions to move up or down the door plate
    if(PS4.getButtonPress(R1)) {
      door.write(11);
    }else if(PS4.getButtonPress(R2)) {
      door.write(169);
    }else {
      door.write(0);
    }

    // Conditions to move up or down the fence
    if(PS4.getButtonPress(L2)) {
      fence.write(11);
    }else if(PS4.getButtonPress(L1)) {
      fence.write(169);
    }else {
      fence.write(0);
    }
  }
}
