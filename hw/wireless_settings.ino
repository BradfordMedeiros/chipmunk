#include <ArduinoJson.h>

struct wireless_settings {
  bool is_using_own_access_point;
  String access_point_name;
  String access_point_password;
  bool is_output_mode;
  bool is_mqtt;
  unsigned int sampling_frequency;
};

struct wireless_settings load_settings(){
  wireless_settings settings;
  settings.is_using_own_access_point = true;
  settings.access_point_name = "";
  settings.access_point_password = "";
  settings.is_output_mode = false;
  settings.is_mqtt = false;
  settings.sampling_frequency = 1000;
  return settings;
}

wireless_settings settings = load_settings();

struct wireless_settings get_settings() {
  return settings;
}
void set_use_own_access_point(bool isOwnAccessPoint) {
  settings.is_using_own_access_point = isOwnAccessPoint;
}

void set_use_mqtt(bool use_mqtt){
  settings.is_mqtt = use_mqtt;
}
void set_mode(bool is_output_mode){
  settings.is_output_mode = is_output_mode;
}

void set_sampling_frequency(unsigned int sampling_frequency){
  settings.sampling_frequency = sampling_frequency;
}

void set_wifi_settings(String access_point_name, String access_point_password){
  settings.access_point_name = access_point_name;
  settings.access_point_password = access_point_password;
}

String getSettingsAsString(){
  /*
   * JsonObject& root = jsonBuffer.createObject();

  // Add values in the object
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do root.set<long>("time", 1351824120);
  root["sensor"] = "gps";
  root["time"] = 1351824120;
   * String output;
  root.printTo(output);
   */
   StaticJsonBuffer<400> jsonBuffer; //@todo should calculate this better
   JsonObject& root = jsonBuffer.createObject();
   root["is_access_point"] = settings.is_using_own_access_point;
   root["network_name"] = settings.access_point_name;
   root["network_password"] = settings.access_point_password;
   root["mode"] = settings.is_mqtt ? "mqtt": "http";
   root["pin_mode"] = settings.is_output_mode ? "output" : "input";
   root["sampling_frequency"] = settings.sampling_frequency;
   String output;
   root.printTo(output);
   return output;
}


