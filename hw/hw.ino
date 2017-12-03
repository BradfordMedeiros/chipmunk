#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

/*
 * 1. parse json and get appropriate settings
 * 2. save settings into eeprom
 * 3. create way to load settings out of eeprom
 * 4. create way to switch access point settings (probably reboot + read?)
 * 5. sample  mqtt at rate posted via webserver
 * 
 *
 * 
 * 
 */
 
ESP8266WebServer server(80);
 
const char* ssid = "WaveG Public WiFi";
const char* password =  "";

const int DIGITAL_PIN = 5; // maps to d1
 
void setup() {
    pinMode(DIGITAL_PIN, OUTPUT);
    
    Serial.begin(115200);
    WiFi.begin(ssid);  //Connect to the WiFi network
 
    while (WiFi.status() != WL_CONNECTED) {  //Wait for connection
 
        delay(500);
        Serial.println("Waiting to connect...");
 
    }
 
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //Print the local IP

    server.on("/", serve_index);
    server.on("/turn_on_led", turn_on_led);
    server.on("/turn_off_led", turn_off_led);
    server.on("/settings", send_wireless_settings);
    server.on("/save_settings", save_wireless_settings);
   
    server.begin(); //Start the server
    Serial.println("Server listening");
 
}
 
void loop() {
     server.handleClient(); //Handling of incoming requests

    digitalWrite(DIGITAL_PIN, 1);
    delay(40);  // 25 = 1second 
}

void save_wireless_settings(){
     if (server.hasArg("plain")== false){ //Check if body received
            server.send(200, "text/plain", "Body not received");
            return;
     }

     String json = server.arg("plain");

     Serial.println("json is: ");
     Serial.println(json);
     Serial.println("--");
     
     StaticJsonBuffer<200> jsonBuffer;
     JsonObject& root = jsonBuffer.parseObject(json);

     bool is_ap    = root["is_access_point"];
     const char* network_name = root["network_name"];
     const char* network_password = root["network_password"];
     unsigned int sampling_rate = root["sampling_rate"];
     bool use_mqtt = root["use_mqtt"];
     bool is_output = root["is_output"];

     set_use_mqtt(use_mqtt);
     set_mode(is_output);
     set_sampling_frequency(sampling_rate);
     set_use_own_access_point(is_ap);
     Serial.println(String(sampling_rate));
     Serial.println(network_name);
     Serial.println(network_password);
     
     Serial.println("-----------");

     server.send(200, "text/plain", "ok");
}

void send_wireless_settings(){
  server.send(200, "text/plain", getSettingsAsString());
}

void turn_off_led() {
    digitalWrite(DIGITAL_PIN, 0);
    server.send(200,"text/plain", "ok");
}

void turn_on_led() {
    server.send(200,"text/plain", "ok");
}


void serve_index()  {
    server.send(200, "text/html", read_html());
}





