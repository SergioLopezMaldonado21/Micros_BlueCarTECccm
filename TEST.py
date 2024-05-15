import pygame

# Inicializar Pygame y el joystick
pygame.init()
pygame.joystick.init()
joystick_count = pygame.joystick.get_count()

if joystick_count == 0:
    print("No hay joysticks conectados.")
else:
    joystick = pygame.joystick.Joystick(0)
    joystick.init()

def check_buttons_and_triggers():
    # Procesar eventos de Pygame
    pygame.event.pump()
    
    # Comprobar todos los botones del joystick
    num_buttons = joystick.get_numbuttons()
    for i in range(num_buttons):
        if joystick.get_button(i):
            print(f"Botón {i} presionado")
    
    # Comprobar todos los ejes del joystick
    num_axes = joystick.get_numaxes()
    for j in range(num_axes):
        axis_value = joystick.get_axis(j)
        if axis_value > 0.5:  # Umbral para detectar movimiento, ajusta según necesidad
            print(f"Eje {j} en movimiento, valor: {axis_value:.2f}")

try:
    print("Interactúa con el joystick para ver los índices y valores de botones y ejes. Presiona CTRL+C para salir.")
    while True:
        check_buttons_and_triggers()
        pygame.time.wait(100)  # Pequeña espera para limitar el número de verificaciones por segundo

except KeyboardInterrupt:
    # Limpiar antes de salir
    pygame.quit()
    print("Programa terminado.")
