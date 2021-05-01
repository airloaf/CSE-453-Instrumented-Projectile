# Bluetooth on ESP32 development board Test

This test is designed to test the functionality of bluetooth on an ESP 32 development board. You should have some sort of bluetooth monitor capable of outputting the raw character stream into a terminal/textfile/whatever.

## Usage

Upload the Arduino Code to the ESP 32. When the ESP 32 is running, add a bluetooth device to your computer called "ESP32test". In your bluetooth serial monitor, connect to the ESP32test device. Open up the serial monitor from the Arduino IDE and enter anything into the serial input. You should see it output to the bluetooth serial monitor.

## References

[Bluetooth Guide](https://randomnerdtutorials.com/esp32-bluetooth-classic-arduino-ide/#:~:text=The%20ESP32%20comes%20with%20Wi,Low%20Energy%20and%20Bluetooth%20Classic.&text=We'll%20control%20an%20ESP32,only%20compatible%20with%20Android%20smartphones.)