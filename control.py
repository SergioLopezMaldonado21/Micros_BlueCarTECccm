
import pygame
import serial
import time
# Inicializar Pygame y el joystick
pygame.init()
pygame.joystick.init()
joystick_count = pygame.joystick.get_count()

port = 'COM5'  # Cambia esto al puerto correcto
baud_rate = 115200  # Ajusta la velocidad de baudios según tu configuración
ser = serial.Serial(port, baud_rate, timeout=1) 

if joystick_count == 0:
    print("No hay joysticks conectados.")
else:
    joystick = pygame.joystick.Joystick(0)
    joystick.init()

def get_joystick_values():
    # Procesar eventos de Pygame
    pygame.event.pump()
    
    # Obtener valores del joystick
    # La 'Dirección' se controla con el eje horizontal del joystick (eje 0)
    # La 'Propulsión' se controla con el eje vertical del joystick (eje 1)
    direccion = ((joystick.get_axis(0) + 1 )/2)* 70  # Normalizar a 70
    propulsion = ((joystick.get_axis(5) +1 )/2) * 255  # Escalar el eje Y para 'Propulsión' directamente de -255 a 255
    radar = 0;

    return {"Propulsion": propulsion, "Direccion": direccion, "Radar": radar}

try:
    while True:
        output_values = get_joystick_values()
        output_string = "%d,%d\n" % (
             output_values["Direccion"], output_values["Propulsion"])
        print(output_string)
        ser.write(output_string.encode())  # Envía la cadena codificada por serial
       # print(output_string)  # También imprime en la consola para depuración
        time.sleep(0.1) 
        #pygame.time.wait(100)  # Pequeña espera para no saturar la salida

except KeyboardInterrupt:
    # Limpiar antes de salir
    pygame.quit()
    print("Programa terminado.")

