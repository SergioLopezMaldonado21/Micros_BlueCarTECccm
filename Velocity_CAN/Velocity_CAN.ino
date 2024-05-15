#include <BlueCarCAN.h>
#include <TimerLib.h>
int speedValue = 0;

#if defined(ESP32)
BlueCarCAN can(5, 21);  // Pines para ESP8266
#else
BlueCarCAN can(10, 2);  // Pines para Arduino
#endif
TimerLib timerCAN(5, 5, TimerLib::MILLIS); 

void setup() {
  can.config_node("Propulsion");
  timerCAN.addCallback(delay_send_CAN);

  pinMode(15, OUTPUT);
  pinMode(2, OUTPUT);


  ledcSetup(0, 1000, 8);
  ledcAttachPin(15, 0);
}

void loop() {

  timerCAN.update();
  can.is4Me(1);  
  speedValue = can.getReadMessage(0); 
  Serial.println(speedValue) ;
  ledcWrite(0, speedValue);

}
void delay_send_CAN(){
  can.setWriteMessage(0, 69);

}
