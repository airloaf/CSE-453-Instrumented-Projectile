// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <string>

#define RECORDING_LENGTH (5 * 1000)

typedef union {
  float floatingPoint;
  byte binary[4];
} binaryFloat;

Adafruit_MPU6050 mpu;

binaryFloat dataBuf[6000];

unsigned int timeRecording;
unsigned int count;


void setup(void) {

  Serial.begin(9600);
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
  dataBuf[count++].floatingPoint = a.acceleration.x;
  dataBuf[count++].floatingPoint = a.acceleration.y;
  dataBuf[count++].floatingPoint = a.acceleration.z;
  dataBuf[count++].floatingPoint = g.gyro.x;
  dataBuf[count++].floatingPoint = g.gyro.y;
  dataBuf[count++].floatingPoint = g.gyro.z; 
}

void loop() {

  if (timeRecording != RECORDING_LENGTH) {
    // Record data to buffer
    recordData();
    timeRecording += 100;
    delay(100);
  }else {
    // Check if serial is connected
    while(!Serial) {
      delay(100);
    }
    delay(100);

    // Output the data
    int i = 0;
    while(i != count) {
      Serial.print(dataBuf[i++].floatingPoint);
      Serial.print(',');
      Serial.print(dataBuf[i++].floatingPoint);
      Serial.print(',');
      Serial.print(dataBuf[i++].floatingPoint);
      Serial.print(',');
      Serial.print(dataBuf[i++].floatingPoint);
      Serial.print(',');
      Serial.print(dataBuf[i++].floatingPoint);
      Serial.print(',');
      Serial.print(dataBuf[i++].floatingPoint);
      Serial.print('\n');
    }
    
    while(1) {
      delay(100);
    }
  }
}
