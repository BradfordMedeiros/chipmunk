// Import required libraries
#include "ESP8266WiFi.h"
int ledPin = 2; //On the ESP-12, the onboard LED is connected to pin 2
 
// WiFi parameters
const char* ssid = "WaveG Public WiFi";
const char* password = "<redacted>";

double convertToVoltage(unsigned int analogReading){
  return 3.3 * analogReading / 1024.0;
}
 
void setup(void)
{ 
// Start Serial
Serial.begin(115200);
 
// Connect to WiFi
//WiFi.begin(ssid, password);
Serial.println("trying to connect");
//WiFi.begin(ssid);
//while (WiFi.status() != WL_CONNECTED) {
//  delay(500);
//  Serial.println(WiFi.status());
//}
//Serial.println("WiFi connected");
 
// Print the IP address
//Serial.println(WiFi.localIP());

 Serial.println("hello this is now started");
pinMode(ledPin, OUTPUT);       // Initialize the BUILTIN_LED pin as an output
}
 
void loop() {
  Serial.println('-');
  digitalWrite(ledPin, LOW);   // Turn the LED on (Note that LOW is the voltage level
                               // but actually the LED is on; this is because 
                               // it is acive low on the ESP-01)
  delay(10);                 // Wait for a second
  digitalWrite(ledPin, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(10);                 // Wait for two seconds (to demonstrate the active low LED)

  unsigned int pin0Voltage = analogRead(0);
  Serial.println(convertToVoltage(pin0Voltage));
}
