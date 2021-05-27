/*************************************************************************************************
  Autor: Daniel Mundo
  Carnet:19508
  Nombre del archivo: main_webserver.c
  Proyecto 4: CONTROL DE REGISTRO PARA PARQUEO
  Configuracion del servidor web
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <WebServer.h>
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "Tu";  // Enter your SSID here
const char* password = "clashmundoll";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

String parqueo1;
String parqueo2;
String parqueo3;
String parqueo4;
String totalpark;
String datos = "";
String getValue(String data, char separator, int index);

String estadopark1;
String estadopark2;
String estadopark3;
String estadopark4;

uint8_t LED1pin = 2;
bool LED1status = LOW;
//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  pinMode(LED1pin, OUTPUT);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.8
  //server.on("/led1on", handle_led1on);
  //server.on("/led1off", handle_led1off);

  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  server.handleClient();
  while (Serial2.available()) {
    char inByte = Serial2.read();
    if (inByte != '\n') {
      //convierto todo a datos
      // los datos enviados de la TivaC separados por comas
      datos.concat(inByte);
    } else {
      //Se utiliza la función getValue para separarlos según las comas, debido a que los datos no tienen la misma longitud siempre
      parqueo1 = getValue(datos, ',', 0);
      parqueo2 = getValue(datos, ',', 1);
      parqueo3 = getValue(datos, ',', 2);
      parqueo4 = getValue(datos, ',', 3);
      totalpark = getValue(datos, ',', 4);
      //se limpia la cadena para la proxima lectura
      datos = "";
    }
    if (parqueo1 == "1")estadopark1 = "Disponible";
    else estadopark1 = "Ocupado";
    if (parqueo2 == "1")estadopark2 = "Disponible";
    else estadopark2 = "Ocupado";
    if (parqueo3 == "1")estadopark3 = "Disponible";
    else estadopark3 = "Ocupado";
    if (parqueo4 == "1")estadopark4 = "Disponible";
    else estadopark4 = "Ocupado";
  }
}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() {
  LED1status = LOW;
  Serial.println("GPIO2 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status));
}
//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML(uint8_t led1stat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<title> Proyecto4 </title>\n";
  ptr += "<title>LED Control</title>\n";
  ptr +=  "<script>\n"
          "<!--\n"
          "function timedRefresh(timeoutPeriod) {\n"
          "\tsetTimeout(\"location.reload(true);\",timeoutPeriod);\n"
          "}\n"
          "\n"
          "window.onload = timedRefresh(1000);\n"
          "\n"
          "//   -->\n"
          "</script>";
  ptr += "<style>table.GeneratedTable {width: 100%; background-color: #ffffff; border-collapse: collapse; border-width: 3px; border-color: #000000;border-style: solid; color: #000000; text-align:center; font-family:Times, serif;}\n";
  ptr += "table.GeneratedTable th {border-width: 3px; border-color: #000000; border-style: solid; color:#ffffff; padding: 3px;}\n";
  ptr += "table.GeneratedTable td{border-width: 3px; border-color: #000000; border-style: solid;padding: 2px;}\n";
  ptr += "table.GeneratedTable thead {background-color: #ff0000;}\n";
  ptr += "</style>\n";
  ptr += "<table class= \"GeneratedTable\">\n";
  ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
  ptr += "<style>h1{text-align:center;font-family:Times, serif;color:#000000;background-color:#ffffff;}\n";
  ptr += "p {text-align:center;font-family:Times, serif;font-size:12px;font-style:normal;font-weight:normal;color:#000000;background-color:#ffffff;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<h1>Parqueo-matic</h1>\n";
  ptr += "<h3>Elaborado por Daniel Mundo</h3>\n";
  ptr += "<thead>\n";
  ptr += "<tr>\n";
  ptr += "<th>No. Parqueo</th>\n";
  ptr += "<th>Estado</th>\n";
  ptr += "</tr>\n";
  ptr += "<thead>\n";
  ptr += "<tbody>\n";

  ptr += "<tr>\n";
  ptr += "<td>Parqueo 1</td>\n";
  ptr += "<td>" + estadopark1 + "</td>\n";
  ptr += "</tr>\n";

  ptr += "<tr>\n";
  ptr += "<td>Parqueo 2</td>\n";
  ptr += "<td>" + estadopark2 + "</td>\n";
  ptr += "</tr>\n";

  ptr += "<tr>\n";
  ptr += "<td>Parqueo 3</td>\n";
  ptr += "<td>" + estadopark3 + "</td>\n";
  ptr += "</tr>\n";

  ptr += "<tr>\n";
  ptr += "<td>Parqueo 4</td>\n";
  ptr += "<td>" + estadopark4 + "</td>\n";
  ptr += "</tr>\n";

  ptr += "<tr>\n";
  ptr += "<td>Total de Parqueos</td>\n";
  ptr += "<td>"+totalpark+"</td>\n";
  ptr += "</tr>\n";
  ptr += "</tbody>\n";

  ptr += "</table>\n";
  ptr += "</html>\n";
  return ptr;
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
  if (LED1status)
  {
    digitalWrite(LED1pin, HIGH);
  }
  else
  {
    digitalWrite(LED1pin, LOW);
  }
}
//************************************************************************************************
// Se obtiene el valor del serial
//************************************************************************************************
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
