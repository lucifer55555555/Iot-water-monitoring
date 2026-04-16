#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

#define PH_PIN 34
#define TDS_PIN 35

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void reconnect() {
  while (!client.connected()) {
    client.connect("ESP32Client");
  }
}

void setup() {
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  int phValue = analogRead(PH_PIN);
  int tdsValue = analogRead(TDS_PIN);

  float ph = phValue * (14.0 / 4095.0);
  float tds = tdsValue * (1000.0 / 4095.0);

  String payload = "{";
  payload += "\"ph\":";
  payload += ph;
  payload += ",";
  payload += "\"tds\":";
  payload += tds;
  payload += "}";

  client.publish("water/sensors", payload.c_str());

  delay(5000);
}