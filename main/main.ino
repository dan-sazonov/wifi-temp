#include <OneWire.h>
#include <DallasTemperature.h>

#define DS_PIN D3

OneWire oneWire(DS_PIN);

DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  sensors.begin();
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