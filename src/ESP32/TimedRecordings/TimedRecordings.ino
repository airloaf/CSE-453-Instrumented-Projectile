#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define RECORDING_LENGTH (5 * 1000)
#define RECORDING_PERIOD 10

#define START_BUFFER_SIZE 21

#define MAG_CHECK_PERIOD 10
#define START_MAGNITUDE_THRESHOLD 30

#define IDLE_STATE 1
#define RECORD_STATE 2
#define OFFLOAD_STATE 3
#define RESET_STATE 4

Adafruit_MPU6050 mpu;
BluetoothSerial SerialBT;

static float dataBuf[10000];

unsigned int count;
unsigned int numDataPoints;

static float startBuf[START_BUFFER_SIZE];
unsigned int startCount;
unsigned int prevStartBufTime;

unsigned int recordingStartTime;
unsigned int currentState;
unsigned int prevMagTime;
bool hasRecorded;
float prevMag;

void idle();
void record();
void recordDataPoint();
void offloadData();
void reset();
void maintainStartBuffer();
bool checkMag();

void setup(void)
{
    Serial.begin(9600);
    SerialBT.begin("Instrumented_Projectile");
    Serial.println("Instrumented Projectile Setup");

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

    count = 0;
    numDataPoints = 0;
    currentState = IDLE_STATE;
    prevMag = 10;
    prevMagTime = millis();
    startCount = 0;
    prevStartBufTime = millis();
    hasRecorded = false;

    Serial.print("Setup complete.\n");
    delay(100);
}

void loop()
{
    switch (currentState) {
    case IDLE_STATE:
        idle();
        break;

    case RECORD_STATE:
        record();
        break;

    case OFFLOAD_STATE:
        offloadData();
        break;

    case RESET_STATE:
        reset();
        break;

    default:
        Serial.print("Error: currentState is invalid.");
        SerialBT.print("Error: currentState is invalid.");
    }
}

void idle()
{
    if (SerialBT.available()) {
        // Newline on other device should be using 'LF' setting for proper parsing with this method.
        String btMessage = SerialBT.readStringUntil('\n');
        Serial.print("Recieved message: '" + btMessage + "'\n");
        if (btMessage == "record") {
            // Does not use startBuffer if using mesage to start.
            Serial.print("Entering RECORD_STATE.\n");
            recordingStartTime = millis();
            currentState = RECORD_STATE;
        } else if (btMessage == "data") {
            currentState = OFFLOAD_STATE;
        } else if (btMessage == "reset") {
            currentState = RESET_STATE;
        } else {
            SerialBT.print("Invalid message: '" + btMessage + "'\n");
            SerialBT.print("String length: " + String(btMessage.length()) + "\n");
            Serial.print("Invalid message: '" + btMessage + "'\n");
            Serial.print("String length: " + String(btMessage.length()) + "\n");
        }
    }

    // Starts recording if the acceleration magnitude is greater than the threshold.
    if (!hasRecorded && millis() - prevMagTime > MAG_CHECK_PERIOD && checkMag()) {
        Serial.print("Exceeded starting magnitude threshhold..\n");
        Serial.print("Entering RECORD_STATE.\n");
        for (int i = 0; i < startCount; i++) {
            dataBuf[count++] = startBuf[i];
        }
        numDataPoints += startCount/7;
        currentState = RECORD_STATE;
    }
    else if (!hasRecorded) {
      maintainStartBuffer();
    }
}


void record()
{
  Serial.println("Entering Recording State");

  recordingStartTime = millis();
  unsigned long int curTime = recordingStartTime;
  unsigned long int maxTime = curTime + RECORDING_LENGTH;
  unsigned long int magCheckTime = curTime + MAG_CHECK_PERIOD;

  while (curTime < maxTime /* TODO: Maybe put MAG check here */)
  {
    // Record a single data point
    recordDataPoint();
    curTime += RECORDING_PERIOD;
    delay(RECORDING_PERIOD);
  }

  Serial.println("Leaving Recording State");
  hasRecorded = true;
  currentState = IDLE_STATE;
}

void recordDataPoint()
{
    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Store each reading
    dataBuf[count++] = (float) (millis());
    dataBuf[count++] = a.acceleration.x;
    dataBuf[count++] = a.acceleration.y;
    dataBuf[count++] = a.acceleration.z;
    dataBuf[count++] = g.gyro.x;
    dataBuf[count++] = g.gyro.y;
    dataBuf[count++] = g.gyro.z;

    // increment number of data points
    numDataPoints++;
}

void offloadData()
{
    Serial.println("Entering OFFLOAD_STATE.");
    Serial.print(numDataPoints); Serial.println(" number of data points");
    // Output the data.
    SerialBT.print("Timestamp,Acceleration_x,Acceleration_y,Acceleration_z,Gyro_x,Gyro_y,Gyro_z\n");

    for (int i = 0; i < numDataPoints; i++) {
        for (int j = 0; j < 6; j++) {
            SerialBT.print(dataBuf[(i*7) + j]);
            SerialBT.print(',');
        }
        SerialBT.print(dataBuf[(i*7) + 6]);
        SerialBT.print('\n');
    }

    SerialBT.println("END");
    Serial.println("Leaving OFFLOAD_STATE.");
    currentState = IDLE_STATE;
}

void reset() {
    count = 0;
    numDataPoints = 0;
    currentState = IDLE_STATE;
    prevMag = 10;
    prevMagTime = millis();
    startCount = 0;
    prevStartBufTime = millis();
    hasRecorded = false;
}

void maintainStartBuffer() {
  if (millis() - prevStartBufTime > RECORDING_PERIOD) {
    if (startCount == START_BUFFER_SIZE) {
      for (int i = 7; i < START_BUFFER_SIZE; i++) {
        startBuf[i-7] = startBuf[i];
      }
      startCount -= 7;
    }
    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Store each reading
    startBuf[startCount++] = (float) (millis());
    startBuf[startCount++] = a.acceleration.x;
    startBuf[startCount++] = a.acceleration.y;
    startBuf[startCount++] = a.acceleration.z;
    startBuf[startCount++] = g.gyro.x;
    startBuf[startCount++] = g.gyro.y;
    startBuf[startCount++] = g.gyro.z;
  }
}

bool checkMag()
{
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
