#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define DS_PIN D3

// for example, random values are set here. change to your own before starting:
#define SSID "DSS WLAN S2" 
#define PASS "HoWXx4p1l0Rt"

OneWire oneWire(DS_PIN);
ESP8266WebServer server(80);

DallasTemperature sensors(&oneWire);

int temp;
int max_temp;
int last_temp;

void setup() {
  Serial.begin(9600);
  sensors.begin();
  WiFi.begin(SSID, PASS);

  // connecting to your AP
  Serial.print("Connecting to "); Serial.print(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  } 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  // run http server
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
}
  
void loop() {  
  // get the temp
  sensors.requestTemperatures();
  last_temp = temp;
  temp = sensors.getTempCByIndex(0) * 100;
  if (temp >= max_temp) max_temp = temp;

  // update server
  server.handleClient();
  delay(1000);
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML()); 
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML() {
  String ptr = "<!DOCTYPE html> <html lang=\"ru-RU\">\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<meta http-equiv=\"refresh\" content=\"10\">";
  ptr += "<meta charset=\"UTF-8\">";
  ptr += "<style>html{height:100vh;max-height:100vh;overflow:hidden;font-family:'Open Sans',sans-serif;display:block;margin:0 auto;text-align:center;color:#333}body{height:100%;max-height:100%;overflow:hidden}.wrap{display:flex;height:100%;flex-direction:column;justify-content:center}</style>";
 
  ptr +="<title>Температура: отчет</title>\n";
  ptr +="</head>\n";
  ptr +="<body>\n"; 
  ptr +="<div class=\"wrap\">\n";
  ptr +="<h1>Фигня для удаленного мониторинга температуры</h1>\n";
  ptr += "<p><i>Мне было лень прикручивать ajax, поэтому нажми F5</i></p>";
  
  ptr +="<p><b>Температура сейчас:</b> ";
  ptr += (int)((float)temp / 100.0);
  ptr +=",";  
  ptr += (int)(temp % 100);
  ptr +=" °C</p>";

  ptr +="<p><b>Максимальная температура:</b> ";
  ptr += (int)((float)max_temp / 100.0);
  ptr +=",";  
  ptr += (int)(max_temp % 100);
  ptr +=" °C</p>";
  
  ptr +="<p><b>Дельта:</b> ";
  ptr += (temp-last_temp);
  ptr +="*10<sup>2</sup> °C</p>";

  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}