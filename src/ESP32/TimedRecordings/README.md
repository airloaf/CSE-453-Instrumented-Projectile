This code will record accelerometer and gyroscopic data for 5 seconds, then it will output the data over bluetooth.

## Format

There will be x lines sent over bluetooth and serial, where x is the total number of readings taken during the 5 second period.
Each line will have the following format

```
accel.x, accel.y, accel.z, gyro.x, gyro.y, gryo.z
```

Note: each value is separated by a comma. After gyro.z, a new line character is printed.

If the accelerometer was read from 1000 times, ther should be 1000 lines of data with 6000 values total (6 per line)