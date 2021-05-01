#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <String>

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define RECORDING_LENGTH (10 * 1000)
#define RECORDING_PERIOD 50

Adafruit_MPU6050 mpu;
BluetoothSerial SerialBT;

static float dataBuf[10000];

unsigned int timeRecording;
unsigned int count;

void setup(void) {

  Serial.begin(9600);
  SerialBT.begin("Instrumented_Projectile");
  
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  timeRecording = 0;
  count = 0;
  delay(100);
}

void recordData() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Store each reading
  dataBuf[count++] = a.acceleration.x;
  dataBuf[count++] = a.acceleration.y;
  dataBuf[count++] = a.acceleration.z;
  dataBuf[count++] = g.gyro.x;
  dataBuf[count++] = g.gyro.y;
  dataBuf[count++] = g.gyro.z; 
}

void loop() {

  if (timeRecording != RECORDING_LENGTH) {
    // Record data to buffer
    recordData();
    timeRecording += RECORDING_PERIOD;
    delay(RECORDING_PERIOD);
  }else {
    Serial.println("Waiting for bluetooth connection to send \"read\" message");

    // Check if the bluetooth device is connected
    // Bluetooth needs to send any character over bluetooth
    while(!SerialBT.available()) {
      delay(100);
    }

    Serial.println("Message received, printing data");

    // Output the data
    int i = 0;
    while(i != count) {
      SerialBT.print(dataBuf[i++]);
      SerialBT.print(',');
      SerialBT.print(dataBuf[i++]);
      SerialBT.print(',');
      SerialBT.print(dataBuf[i++]);
      SerialBT.print(',');
      SerialBT.print(dataBuf[i++]);
      SerialBT.print(',');
      SerialBT.print(dataBuf[i++]);
      SerialBT.print(',');
      SerialBT.print(dataBuf[i++]);
      SerialBT.print('\n');
      
      Serial.print(dataBuf[i++]);
      Serial.print(',');
      Serial.print(dataBuf[i++]);
      Serial.print(',');
      Serial.print(dataBuf[i++]);
      Serial.print(',');
      Serial.print(dataBuf[i++]);
      Serial.print(',');
      Serial.print(dataBuf[i++]);
      Serial.print(',');
      Serial.print(dataBuf[i++]);
      Serial.print('\n');
    }
    
    while(1) {
      delay(100);
    }
  }
}
