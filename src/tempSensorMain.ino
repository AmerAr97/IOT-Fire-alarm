// This example sends data to multiple variables to
// Ubidots through HTTP protocol.

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"
#include <SFE_BMP180.h>
#include <Wire.h>

/****************************************
 * Define Instances and Constants
 ****************************************/

SFE_BMP180 pressure;

#define ALTITUDE 92.0 // My altitude in Fagersta in meters

const char* UBIDOTS_TOKEN = "BBFF-fWWKs92Hd63MZPzSDpnqDfn43FxR7P";  // Put here your Ubidots TOKEN
const char* WIFI_SSID = "Myhome";      // Put here your Wi-Fi SSID
const char* WIFI_PASS = "32152799";      // Put here your Wi-Fi password
Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);

void setup() {
  Serial.begin(115200);
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
  // ubidots.setDebug(true);  // Uncomment this line for printing debug messages

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
  bufferSent = ubidots.send("HeltecBoard");  // Will send data to a device label that matches the device Id

  if (bufferSent) {
    // Do something if values were sent properly
    Serial.println("Values sent by the device");
  }
  else{
    Serial.println("data not send");
  }

  delay(5000);
}
