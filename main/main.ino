#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define DS_PIN D3
#define SSID "DSS WLAN S2"
#define PASS "HoWXx4p1l0Rt"

OneWire oneWire(DS_PIN);
ESP8266WebServer server(80);

DallasTemperature sensors(&oneWire);

float temp;

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

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
}
  
void loop() {  
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
  server.handleClient();
  delay(1000);
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(temp)); 
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float Temp)
{
  String ptr = "<!DOCTYPE html> <html lang=\"ru-RU\">\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<meta charset=\"UTF-8\">";
  ptr +="<title>Температура: отчет</title>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div>\n";
  ptr +="<h1>Фигня для удаленного мониторинга температуры</h1>\n";
  ptr += "<p><i>Мне было лень прикручивать ajax, поэтому нажми F5</i></p>";
  
  ptr +="<p>Температура сейчас: ";
  ptr +=(int)(Temp*100);
  ptr +="*10<sup>2</sup>C</p>";
  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}