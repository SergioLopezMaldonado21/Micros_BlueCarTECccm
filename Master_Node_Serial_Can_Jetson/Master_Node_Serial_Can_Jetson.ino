
#include <TimerLib.h>
#include <BlueCarCAN.h>
#if defined(ESP32)
BlueCarCAN can(5, 21);  // Pines para ESP8266
#else
BlueCarCAN can(10, 2);  // Pines para Arduino
#endif
TimerLib timer(5, 5, TimerLib::MILLIS); 
String num_string = "";
unsigned short int node = 0;
bool debug = 0;
void setup() {
    can.config_node("Master");
    //timer.addCallback(ReadSerialJetson);
    timer.addCallback(SendSerialJetson);
    timer.addCallback(Serial_update);
}

void loop() {
    timer.update();
    can.send2Jetson();
    can.is4Me();
     
  //= "{Propulsion:255, Direccion:150, Radar:255}"; // Lee la línea completa desde Serial

}
void Serial_update(){
  Serial.println(can.json);  
}
/*void ReadSerialJetson(){
void ReadSerialJetson(){
    char c = Serial.read();    // Lee un caracter
    if (c == '\n') {           // Si es el final del mensaje
      Serial.println("Mensaje recibido: " + message);
      //can.json_received  = message; // Imprime el mensaje
      can.json_received = message;
      message = ""; // Restablece el mensaje para el siguiente uso
    } else {
      message += c; // Añade el caracter al mensaje
    }                                                                      2        
}*/

void SendSerialJetson(){
    //can.readJetson();
  if(Serial.available() > 0){
    char c = (char)Serial.read();
        //Serial.print(c);

    if (c == ',' || c == '\n'){
      if(debug == true){
        Serial.print("Nodo: ");
        Serial.print(node + 1);
        Serial.print("Valor: ");
        Serial.println( num_string.toInt());

      }
      can.setWriteMessage(node,  num_string.toInt());
      
      num_string = "";
      node ++;
      if (c == '\n'){
        node = 0;
      }
    }
    else{
      num_string += c;
      //Serial.println(num_string);
    }
  }
                                                                           
}