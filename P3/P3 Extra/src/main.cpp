#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32_AP_Extra";
const char* password = "clave123456";

// Creamos un servidor en el puerto 80 (opcional para darle funcionalidad)
WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<h1>Hola desde el ESP32 en modo AP!</h1>");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Cambiamos a modo AP (Access Point)
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  // Mostramos la IP del AP
  IPAddress IP = WiFi.softAPIP();
  Serial.println("Punto de acceso creado:");
  Serial.print("Nombre de red (SSID): ");
  Serial.println(ssid);
  Serial.print("IP del ESP32: ");
  Serial.println(IP);

  // Iniciamos el servidor y definimos una respuesta a "/"
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Servidor HTTP iniciado en modo AP");
}

void loop() {
  server.handleClient(); // Gestiona las peticiones del navegador
}
