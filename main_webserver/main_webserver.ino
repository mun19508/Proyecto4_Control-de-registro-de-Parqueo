/*************************************************************************************************
 *Autor: Daniel Mundo                                                                            *
 *Carnet:19508                                                                                   *
 *Nombre del archivo: main_webserver.c                                                           *
 *Proyecto 4: CONTROL DE REGISTRO PARA PARQUEO                                                   *
 *Configuracion del servidor web                                                                 *
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
const char* ssid = "DIGITAL2";  // Enter your SSID here
const char* password = "digital2";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200);
  Serial.println("Try Connecting to ");
  Serial.println(ssid);


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

}
//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML(uint8_t led1stat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<title> Proyecto4 </title>\n";
  ptr += "<title>LED Control</title>\n";
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
  ptr += "<td>Disponible</td>\n";
  ptr += "</tr>\n";

  ptr += "<tr>\n";
  ptr += "<td>Parqueo 2</td>\n";
  ptr += "<td>Disponible</td>\n";
  ptr += "</tr>\n";

  ptr += "<tr>\n";
  ptr += "<td>Parqueo 3</td>\n";
  ptr += "<td>Disponible</td>\n";
  ptr += "</tr>\n";

  ptr += "<tr>\n";
  ptr += "<td>Parqueo 4</td>\n";
  ptr += "<td>Disponible</td>\n";
  ptr += "</tr>\n";

  ptr += "<tr>\n";
  ptr += "<td>Total de Parqueos</td>\n";
  ptr += "<td>4</td>\n";
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
}
