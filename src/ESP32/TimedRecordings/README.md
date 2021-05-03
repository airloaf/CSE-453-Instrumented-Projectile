This code will record accelerometer and gyroscopic data for 5 seconds, then it will output the data over bluetooth.

## Format

There will be x lines sent over bluetooth and serial, where x is the total number of readings taken during the 5 second period.
Each line will have the following format

```
Timestamp,Acceleration_x,Acceleration_y,Acceleration_z,Gyro_x,Gyro_y,Gyro_z
```

Note: each value is separated by a comma. After gyro.z, a new line character is printed.

If the accelerometer was read from 1000 times, ther should be 1000 lines of data with 7000 values total (7 per line)