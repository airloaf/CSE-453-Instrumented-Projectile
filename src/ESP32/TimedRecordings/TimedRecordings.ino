#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define RECORDING_LENGTH (10 * 1000)
#define RECORDING_PERIOD 50
#define START_TIME_BUFFER 1000

#define MAG_CHECK_PERIOD 10
#define START_MAGNITUDE_THRESHOLD 30

#define IDLE_STATE 1
#define RECORD_STATE 2
#define OFFLOAD_STATE 3


Adafruit_MPU6050 mpu;
BluetoothSerial SerialBT;

static float dataBuf[10000];
unsigned int count;
static float startBuf[70];
unsigned int startCount;

unsigned int timeRecording;
unsigned int recordingStartTime;
unsigned int prevTimestamp;
unsigned int currentState;
unsigned int prevMagTime;
float prevMag;

bool hasRecorded;

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
  prevTimestamp = millis();
  currentState = IDLE_STATE;
  prevMag = 10;
  prevMagTime = millis();
  startCount = 0;
  hasRecorded = false;

  Serial.print("Setup complete.\n");
  delay(100);
}

void recordData() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Store each reading
  dataBuf[count++] = (float) millis();
  dataBuf[count++] = a.acceleration.x;
  dataBuf[count++] = a.acceleration.y;
  dataBuf[count++] = a.acceleration.z;
  dataBuf[count++] = g.gyro.x;
  dataBuf[count++] = g.gyro.y;
  dataBuf[count++] = g.gyro.z;
}

bool checkMag() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float mag = sqrt(sq(a.acceleration.x) + sq(a.acceleration.y) + sq(a.acceleration.z));

  if (abs(mag - prevMag) > START_MAGNITUDE_THRESHOLD) {
    prevMagTime = millis();
    prevMag = 10;
    return true;
  }
  prevMagTime = millis();
  prevMag = mag;
  return false;
}

void maintainStartBuffer() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  if (startCount > 70) {
    startCount = 0;
  }

  // Store each reading
  startBuf[startCount++] = (float) millis();
  startBuf[startCount++] = a.acceleration.x;
  startBuf[startCount++] = a.acceleration.y;
  startBuf[startCount++] = a.acceleration.z;
  startBuf[startCount++] = g.gyro.x;
  startBuf[startCount++] = g.gyro.y;
  startBuf[startCount++] = g.gyro.z;
}

void loop() {
  switch(currentState) {
    case IDLE_STATE :
      if (SerialBT.available()) {
        // Newline on other device should be using 'LF' setting for proper parsing with this method.
        String btMessage = SerialBT.readStringUntil('\n');
        Serial.print("Recieved message: '" + btMessage + "'\n");
        if (btMessage == "record") {
          // Does not use startBuffer if using mesage to start.
          Serial.print("Entering RECORD_STATE.\n");
          recordingStartTime = millis();
          currentState = RECORD_STATE;
        }
        else if (btMessage == "data") {
          currentState = OFFLOAD_STATE;
        }
        else {
          SerialBT.print("Invalid message: '" + btMessage + "'\n");
          SerialBT.print("String length: " + String(btMessage.length()) + "\n");
        }
      }

      // Starts recording if the acceleration magnitude is greater than the threshold.
      if (!hasRecorded && millis() - prevMagTime > MAG_CHECK_PERIOD && checkMag()) {
        Serial.print("Exceeded starting magnitude threshhold..\n");
        Serial.print("Entering RECORD_STATE.\n");
        for (int i = 0; i < startCount; i++) {
          dataBuf[count++] = startBuf[i];
        }
        recordingStartTime = millis();
        currentState = RECORD_STATE;
      }
      else {
        maintainStartBuffer();
      }
      
      break;
      
    case RECORD_STATE :
      if (timeRecording < RECORDING_LENGTH) {
        // Checks to see if magnitude threshold was reached again and, if so, sets the timeRecording to only record 10 more cycles.
        if (millis() - recordingStartTime > START_TIME_BUFFER && millis() - prevMagTime > MAG_CHECK_PERIOD && checkMag()) {
          Serial.print("Found ending magnitude.\n");
          timeRecording = RECORDING_LENGTH - 10*RECORDING_PERIOD;
        }
        if (millis() - prevTimestamp >= RECORDING_PERIOD) {
          // Record data to buffer.
          recordData();
          timeRecording += millis() - prevTimestamp;
          prevTimestamp = millis();
        }
      }
      else {
        Serial.print("Leaving RECORD_STATE.\n");
        hasRecorded = true;
        currentState = IDLE_STATE;
      }
      
      break;
      
    case OFFLOAD_STATE :
      Serial.print("Entering OFFLOAD_STATE.\n");
      // Output the data.
      SerialBT.print("Timestamp,Acceleration_x,Acceleration_y,Acceleration_z,Gyro_x,Gyro_y,Gyro_z\n");
      for (int i = 0; i < count; i++) {
        for (int j = 0; j < 7; j++) {
          SerialBT.print(dataBuf[i++]);
          SerialBT.print(',');
        }
        SerialBT.print('\n');
      }
      Serial.print("Leaving OFFLOAD_STATE.\n");
      currentState = IDLE_STATE;
      
      break;
      
    default :
      Serial.print("Error: currentState is invalid.");
      SerialBT.print("Error: currentState is invalid.");
  }
}
