# IOT-Fire-alarm

------
Amer Armoush aa225pk
------

This project is a about a high temprature alarm made with a Heltec wifi LoRa 32 board and a BMP180 sensor.
The device will moniter the room temprature and notify the user if the temprature gets too high via an sms.
This tutorial can take a couple hours to set up.
![](https://i.imgur.com/k8huNj6.jpg)

# Purpose
This project is an example of an IOT device that takes data from the real world with the help of sensors and sends the data to a helping website to show and demonstrate the data. The device created is a thermometer that acts as a fire alarm and senses the temprature and notifies the user when temprature gets too high. the device can be further developed with the hekp of more sensors depending on the use situation.

# Materials
To follow the tutorial and make the device you need to buy a set of itmes as follows:


- Heltec WiFi LoRa 32: An IOT plaform that can be easily programmed using Arduino IDE.
- Jumper wire: to connect the sensor with the Heltec board.
- Antennae: for better Wifi signal.
- Micro USB cable: to power the Heltec board.
- MBP180 sensor: temperature sensor.

The temprature sensor can be bought from this [webbsite1] and rest of the matrials comes as a bundle and can be bought from [webbsite2] and all the material cost around 650 kr..

[webbsite1]: https://www.electrokit.com/en/product/bmp180-lufttryckssensor-monterad-pa-kort/
[webbsite2]: https://www.electrokit.com/en/product/lnu-1dt305-tillampad-iot-heltec-lora-32-and-sensors-bundle/


# IDE setup
The first thing you will need to do is to download an Arduino IDE to program the device, which can be installed by following this  [tutorial].
After downloading Arduino IDE you will need to download the needed libraries in order to work with the board.
The libraries can be downloaded from [library1] and [library2] and to download them you press on the green button that says "code" then "download Zip".
To link the libraries to the arduino IDE you need to open the IDE the click on sketch, then include library, then add .ZIP library, and then locate and choose the downloaded libraries and you're done.

[tutorial]:https://docs.arduino.cc/software/ide-v1/tutorials/Windows?_gl=1*qa4bhj*_ga*MTM4NDUxODU5Mi4xNjU5OTk3OTk5*_ga_NEXN8H46L5*MTY2MDM0OTI2NC45LjEuMTY2MDM0OTQyMS42MA..
[library1]:https://github.com/HelTecAutomation/Heltec_ESP32
[library2]:https://github.com/sparkfun/BMP180_Breakout_Arduino_Library

# Putting everything together

![](https://i.imgur.com/LNrTDmv.jpg)
![](https://i.imgur.com/C0VNhgG.jpg)

The first step is to connect the antennae with the Heltec as shown in the picture. 
The second step is connecting the sensor to the Heltec board as follows:

- BMP180 ------ Heltec
- GND    ------ GND
- VIN    ------ 3V3
- SDA    ------ 21
- SCL    ------ 22

 
# Internet connection
The Heltec must be connected to the internet network to send the data from the sensor. With the Heltec device can be connected to the internet via Wifi, Sigfox, LoRa. Sigfox and LoRa can be used if the device is placed outside in a place far from a wifi connection. But in my use case it will be fine to connect it to the Wifi because it will be used in my home near Wifi connection.

# Dashboard
After connecting all hardware together and connecting the Heltec to the computer, you will need a dashboard that represents your data sent by the Heltec. 
I used Ubidots to represent the data which is very easy to use and manage, and i chose to send the data via HTTP connection. 
![](https://i.imgur.com/vP4AUJz.png)

# Sending a phone notification
When the room temperature gets too high you will need to receive a warning. A phone sms can be used as one. And to set this up and adjust the warning temprature you can follow this [tutorial2].
![](https://i.imgur.com/wPRVrWG.jpg)

[tutorial2]:https://help.ubidots.com/en/articles/1784986-control-and-automation-with-ubidots

# The code
This code is used to to run the Heltec board and get the temprature from the sensor and then send the data to Ubidots over HTTP.

```python=
// This example sends data to multiple variables to
// Ubidots through HTTP protocol.
#include "Ubidots.h"
#include <SFE_BMP180.h>
#include <Wire.h>

SFE_BMP180 pressure;

#define ALTITUDE 92.0 // My altitude in Fagersta in meters

// Put here your Ubidots TOKEN
const char* UBIDOTS_TOKEN = "BBFF-fWWKs92Hd63MZPzSDpnqDfn43FxR7P";  
// Put here your Wi-Fi SSID
const char* WIFI_SSID = "Myhome"; 
// Put here your Wi-Fi SSID     
const char* WIFI_PASS = "32152799";      
// Put here your Wi-Fi password
Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);

void setup() {
  Serial.begin(115200);
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
  // Initialize the sensor 

  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.
    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }
}

void loop() {
  char status;
  double T;

  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      Serial.print("temperature: ");
      Serial.print(T,2);
      Serial.print(" deg C, ");
      
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

  

  ubidots.add("sensor", T);  // Change for your variable name
  

  bool bufferSent = false;
 // Will send data to a device label that matches the device Id
  bufferSent = ubidots.send("HeltecBoard"); 

  if (bufferSent) {
    // Do something if values were sent properly
    Serial.println("Values sent by the device");
  }
  else{
    Serial.println("data not send");
  }

  delay(5000);
}

