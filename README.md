# ESP32_MultipleTM1637_Temp_Time_Display
This repository demo is to show the ESP32 receiving Date/Time and Sensor and display on TM1637 display

###  

#### Demo to show the ESP32 looking at NTP for date and time, along with the Temperature sensor DHT11
and rendering it on TM1637 four 7 segment display.

In this project - I have used two 7 segment displays, one for the temperature and one for showing the time.

The ESP32 microcontroller uses WiFi and connects to the Wireless router and probes the NTP (Network Time Protocol)
and displays it on 7 segment display.

On the other hand, The ESP32 reads the temperature sensor (using the DHT11 sensor) and displays the current temperature
on the second 7 segment display.

The code runs on ESP32 and NOT on ESP8266.

The firmware is developed on an Arduino IDE, Uploaded by configuring appropriate serial port driver and installation of ESP32 libraries, DHT11, and TM1637 libraries.

Libraries required:

DHT11
TM1637

