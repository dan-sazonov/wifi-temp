#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>

#define DS_PIN D3
#define SSID "DSS WLAN S2"
#define PASS "HoWXx4p1l0Rt"

OneWire oneWire(DS_PIN);

DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  sensors.begin();
  WiFi.begin(SSID, PASS);

  Serial.print("Connecting to "); Serial.print(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  } 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}
  
void loop() {
  sensors.requestTemperatures();
  float temperature_Celsius = sensors.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.print(temperature_Celsius);
  Serial.println(" ºC");
  Serial.println("");
  delay(1000);
}