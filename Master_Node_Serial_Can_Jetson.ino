
#include <TimerLib.h>
#include <BlueCarCAN.h>
#if defined(ESP32)
BlueCarCAN can(5, 21);  // Pines para ESP8266
#else
BlueCarCAN can(10, 2);  // Pines para Arduino
#endif
TimerLib timer(10, 5, TimerLib::MILLIS); 
String message = "";

void setup() {
    can.config_node("Master");
    timer.addCallback(ReadSerialJetson);


}

void loop() {
    timer.update();
  //= "{Propulsion:255, Direccion:150, Radar:255}"; // Lee la línea completa desde Serial
    can.readJetson();
    can.send2Jetson();
    can.is4Me();
    Serial.println(can.json);
    
}

void ReadSerialJetson(){
    char c = Serial.read();    // Lee un caracter
    if (c == '\n') {           // Si es el final del mensaje
      Serial.println("Mensaje recibido: " + message);
      //can.json_received  = message; // Imprime el mensaje
      can.json_received = message;
      
      message = ""; // Restablece el mensaje para el siguiente uso
    } else {
      message += c; // Añade el caracter al mensaje
    }                                                                              
  
  
}