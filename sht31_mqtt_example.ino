#include "SHT3x.h"
SHT3x Sensor;
#include <WiFi.h>
#include <PubSubClient.h>
 
const char* ssid = "<wifi-name>";
const char* password =  "<wifi-password>";
const char* mqttServer = "<server-ip>";
const int mqttPort = 1883;
const char* mqttUser = "<user>";
const char* mqttPassword = "<pw>";

int wifi_stop = 0;
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);
  Sensor.Begin();

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
    ++wifi_stop;
       if (wifi_stop > 30) {
         ESP.deepSleep(0);
       }
    
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("Wifi connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000); 
      
    }
  } 
}
 
 
void loop() {

  Sensor.UpdateData();
  Serial.print("Temperature: ");
  //String sensorTemp = Sensor.GetTemperature();
  Serial.print(Sensor.GetTemperature());
  Serial.write("\xC2\xB0"); //The Degree symbol
  Serial.println("C");
  Serial.print("Humidity: ");
  Serial.print(Sensor.GetRelHumidity());
  //String sensorHumity = Sensor.GetRelHumidity();
  Serial.println("%");

  String sensorTemp_str;
  char char_sensorTemp[50];
  sensorTemp_str = String(Sensor.GetTemperature()); 
  sensorTemp_str.toCharArray(char_sensorTemp, sensorTemp_str.length() + 1); 
  client.publish("betontest/temperatur", char_sensorTemp);

  delay(2000);

  String sensorHumity_str;
  char char_sensorHumity[50];
  sensorHumity_str = String(Sensor.GetRelHumidity()); 
  sensorHumity_str.toCharArray(char_sensorHumity, sensorHumity_str.length() + 1); 
  client.publish("betontest/feuchtigkeit", char_sensorHumity);

  //delay to enter deppsleep - needed sending message above
  delay(2000);

  //delay(5000);
  ESP.deepSleep(600000000);
  delay(500);
  
}