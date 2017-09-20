// Светодиод подлкючен к 5 пину
// Датчик температуры ds18b20 к 2 пину

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid = "ap"; 
const char *pass = "pass";

const char *mqtt_server = "ip"; 
const int mqtt_port = 1883; 
const char *mqtt_user = "user"; 
const char *mqtt_pass = "pass"; 

#define BUFFER_SIZE 100

bool LedState = false;


void callback(const MQTT::Publish& pub)
{
Serial.print(pub.topic()); 
Serial.print(" => ");
Serial.print(pub.payload_string()); 

String payload = pub.payload_string();

if(String(pub.topic()) == "test/led") 
{
int stled = payload.toInt(); 
digitalWrite(2,stled); 
}
}

WiFiClient wclient; 
PubSubClient client(wclient, mqtt_server, mqtt_port);

void setup() {

Serial.begin(115200);
delay(10);
Serial.println();
Serial.println();
pinMode(2, OUTPUT);
}

void loop() {
// подключаемся к wi-fi
if (WiFi.status() != WL_CONNECTED) {
Serial.print("Connecting to ");
Serial.print(ssid);
Serial.println("...");
WiFi.begin(ssid, pass);

if (WiFi.waitForConnectResult() != WL_CONNECTED)
return;
Serial.println("WiFi connected");
}


if (WiFi.status() == WL_CONNECTED) {
if (!client.connected()) {
Serial.println("Connecting to MQTT server");
if (client.connect(MQTT::Connect("arduinoClient2")
.set_auth(mqtt_user, mqtt_pass))) {
Serial.println("Connected to MQTT server");
client.set_callback(callback);
client.subscribe("test/led"); 
} else {
Serial.println("Could not connect to MQTT server"); 
}
}

if (client.connected()){
client.loop();
delay(10); 
}

}
} 

