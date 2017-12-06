#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

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

  
WiFiClient espClient;
PubSubClient client(espClient);
ESP8266WebServer server(80);

const char* mqtt_server = "10.0.139.212";
const char* clientID = "chipmunk";
const char* outTopic = "thing";
const char* inTopic = "thing";
const char* ssid = "WaveG Public WiFi";
const char* password =  "";

const int DIGITAL_PIN = 5; // maps to d1

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientID)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, clientID);
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup() {
    pinMode(DIGITAL_PIN, OUTPUT);
    
    Serial.begin(115200);
    WiFi.begin(ssid);  //Connect to the WiFi network
 
    while (WiFi.status() != WL_CONNECTED) {  //Wait for connection
 
        delay(500);
        Serial.println("Waiting to connect...");
 
    }

    Serial.println("settings as string:");
    Serial.println(getSettingsAsString());
 
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //Print the local IP

    server.on("/", serve_index);
    server.on("/turn_on_led", turn_on_led);
    server.on("/turn_off_led", turn_off_led);
    server.on("/settings", send_wireless_settings);
    server.on("/save_settings", save_wireless_settings);
   
    server.begin(); //Start the server
    Serial.println("Server listening");

    client.setServer(mqtt_server, 1883);
    client.connect(clientID);
}

int i = 0; 
void loop() {
     server.handleClient(); //Handling of incoming requests

    digitalWrite(DIGITAL_PIN, 1);
    delay(40);  // 25 = 1second 
    i++;

   if (!client.connected()){
      if (i % 25 == 0){
              Serial.println("warning client not connected");    
      }
    }
    
    if (i % 100 == 0){
      i = 0;
      Serial.println("publishing topic thing");
      bool success = client.publish("thing", "hello");
      if (success){
        Serial.println("publish success");
      }else{
        Serial.println("publish failure");
      }
    }
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





