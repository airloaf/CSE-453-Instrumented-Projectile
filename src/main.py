import time
import math
import board
import busio
import pulseio
import adafruit_mpu6050

###################### BUZZER CODE
OFF = 0
ON = 2 ** 15

buzzer = pulseio.PWMOut(board.D4, variable_frequency=True)
buzzer.duty_cycle = OFF # TURN THE BUZZER ON OR OFF
buzzer.frequency = 2000
#######################

# Creating functions to save space (since the variables will be out of scope)
def initializeGravityVector():
    x = 0.0
    y = 0.0
    z = 0.0
    for i in range(1024):
        x += mpu.acceleration[0]
        y += mpu.acceleration[1]
        z += mpu.acceleration[2]
    x = x / 1024
    y = y / 1024
    z = z / 1024
    force = math.sqrt(math.pow(x,2) + math.pow(y,2) + math.pow(z,2))
    unitVector = (x/force, y/force, z/force)
    return unitVector

def updateGravityVector():
    updateVector = [gravityVector[0], gravityVector[1], gravityVector[2]]
    gyro = (-mpu.gyro[0], -mpu.gyro[1], -mpu.gyro[2])
    elapsedTime = time.monotonic() - timeSinceUpdate

    if (gyro[2] > 3 or gyro[2] < -3):
        xyCos = math.cos(math.radians(gyro[2] * elapsedTime))
        xySin = math.sin(math.radians(gyro[2] * elapsedTime))
        updateVector[0] = updateVector[0] * xyCos - updateVector[1] * xySin
        updateVector[1] = updateVector[0] * xySin + updateVector[1] * xyCos

    if (gyro[1] > 3 or gyro[1] < -3):
        xzCos = math.cos(math.radians(gyro[1] * elapsedTime))
        xzSin = math.sin(math.radians(gyro[1] * elapsedTime))
        updateVector[0] = updateVector[0] * xzCos - updateVector[2] * xzSin
        updateVector[2] = updateVector[0] * xzSin + updateVector[2] * xzCos

    if (gyro[0] > 3 or gyro[0] < -3):
        yzCos = math.cos(math.radians(gyro[0] * elapsedTime))
        yzSin = math.sin(math.radians(gyro[0] * elapsedTime))
        updateVector[1] = updateVector[1] * yzCos - updateVector[2] * yzSin
        updateVector[2] = updateVector[1] * yzSin + updateVector[2] * yzCos

    force = math.sqrt(math.pow(updateVector[0],2) + math.pow(updateVector[1],2) + math.pow(updateVector[2],2))
    unitVector = (updateVector[0]/force, updateVector[1]/force, updateVector[2]/force)
    return updateVector


i2c = busio.I2C(board.SCL, board.SDA)
mpu = adafruit_mpu6050.MPU6050(i2c)

IDLE = 0
CALIBRATING = 1
RECORDING = 2
SAVING = 3

state = 1
pollingRate = 2

print("hello world");

timeSinceUpdate = time.monotonic()
while True:
    # buzzer.duty_cycle = ON if (state == IDLE) else OFF

    if (state == IDLE):
        print("Executing Idle State.")
    elif (state == CALIBRATING):
        print("Executing Calibration State.")
        gravityVector = initializeGravityVector()
        state = 2
    elif (state == RECORDING):
        #print("Executing Recording State.")

        if (gravityVector):
            gravityVector = updateGravityVector()
        print(gravityVector)

        #print("Acceleration: X:%.2f, Y: %.2f, Z: %.2f m/s^2" % mpu.acceleration)
        #print("Gyro X:%.2f, Y: %.2f, Z: %.2f degrees/s" % (-mpu.gyro[0], -mpu.gyro[1], -mpu.gyro[2]))
        #print("Temperature: %.2f C\n" % mpu.temperature)
        time.sleep(1/pollingRate)
    elif (state == SAVING):
        print("Executing Saving State.")
    else:
        print("Error: state does not exist.")
