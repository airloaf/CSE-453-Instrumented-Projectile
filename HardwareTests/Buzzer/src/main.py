import board
import pulseio

OFF = 0
ON = 2 ** 15

buzzer = pulseio.PWMOut(board.D4, variable_frequency=True) # Using Pin D4
buzzer.duty_cycle = ON # TURN THE BUZZER ON OR OFF
buzzer.frequency = 2000