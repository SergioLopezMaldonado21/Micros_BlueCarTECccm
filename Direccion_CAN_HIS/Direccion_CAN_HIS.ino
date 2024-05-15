#include "config.h"
#include <BlueCarCAN.h>
#include <TimerLib.h>


// Selección de pines basada en la plataforma de hardware
#if defined(ESP32)
BlueCarCAN can(5, 21);  // Pines para ESP8266
#else
BlueCarCAN can(10, 2);  // Pines para Arduino
#endif
TimerLib timerCAN(5, 5, TimerLib::MILLIS); 
void setup() {
  can.config_node("Direccion");
  timerCAN.addCallback(delay_send_CAN);
  for (int i = 0; i < 8; i++) {
    pinMode(signalColors[i], INPUT);
  }
  pinMode(RPWM_Output, OUTPUT);
  pinMode(LPWM_Output, OUTPUT);

  int sensorValue = 512;

  ledcSetup(0, 1000, 8); // Channel 0, 1 MHz frequency, 8-bit resolution
  ledcAttachPin(RPWM_Output, 0); // Attach PWM channel 0 to `outputPin`

  ledcSetup(1, 1000, 8); // Channel 0, 1 MHz frequency, 8-bit resolution
  ledcAttachPin(LPWM_Output, 1); // Attach PWM channel 0 to `outputPin`


}

void loop() {
  
  timerCAN.update();
  value = getPosition();
  possibleAngles = farRightAngle-farLeftAngle;
  angle = map(value, farLeft, farRight, farLeftAngle, farRightAngle);
  can.is4Me(1);  
  motor_control();

  /*Serial.print("Reading: ");
  Serial.print(value);

  Serial.print(" Angle: ");
  Serial.println(angle);*/

}

void delay_send_CAN(){
  can.setWriteMessage(0, angle + 35);
  desiredAngle = map(can.getReadMessage(0),0,70,-35,35);  
}

void motor_control(){

  int maxPower = 55; // Value from 0-100% for maximum power.
  int minPower = 25; // Value from 0-100% for the minimum power.
  
  maxPower = maxPower * 2.55;
  minPower = minPower * 2.55;
  
  if (desiredAngle == 200){
    ledcWrite(1, 0);
    ledcWrite(0, 0);
    return;
  }
  
  if (angle < desiredAngle +2 and angle > desiredAngle -2){
    // Angle in correct position
    // angle +=1        angle -=1

    ledcWrite(1, 0);
    ledcWrite(0, 0);
    
    Serial.print("Correct position ");
    /*Serial.print("Angle: "); Serial.print(angle); Serial.print(" Desired angle: "); Serial.println(desiredAngle);*/
  }
  
  else if(angle < desiredAngle){
    // angle less than desired. Wheels should turn to the right.
    // get the duty cycle based on the difference in angle.
    // turn off left, turn on right.
    
    int angleDiff = abs(angle-desiredAngle);
    int dutyCycle = map(angleDiff, 0, possibleAngles, minPower, maxPower);

    ledcWrite(1, 0);
    ledcWrite(0, dutyCycle);

   Serial.print("Angle is Increasing! ");
    /*Serial.print("Angle: "); Serial.print(angle); Serial.print(" Desired angle: "); Serial.print(desiredAngle); Serial.print("  Cycle: "); Serial.println(dutyCycle);*/
  }
  
  else if (angle > desiredAngle){
    // angle more than desired. Wheels should turn to the left.
    // get the duty cycle based on the difference in angle.
    // turn off right, turn on left.

    int angleDiff = abs(angle-desiredAngle);
    int dutyCycle = map(angleDiff, 0, possibleAngles, minPower, maxPower);

    ledcWrite(1, dutyCycle);
    ledcWrite(0, 0);

    Serial.print("Angle is decreasing! ");
    /*Serial.print("Angle: "); Serial.print(angle); Serial.print(" Desired angle: "); Serial.print(desiredAngle); Serial.print(" Duty Cycle: "); Serial.println(dutyCycle);*/
  }
}

int getPosition() {

  //// TESTING
  
  uint8_t gray = 0;

  
  for (int i = 0; i < 8; i++) {
    if (digitalRead(signalColors[i]) == LOW) {
      gray |= 1 << i;
    }
  }

  uint8_t binary = grayToBinary(gray);

  int position = binary;

  return position;
}

uint8_t grayToBinary(uint8_t num) {
  uint8_t mask;
  for (mask = num >> 1; mask != 0; mask >>= 1) {
    num ^= mask;
  }
  return num;
}
