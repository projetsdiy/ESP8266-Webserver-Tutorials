/*
 * ESP8266 + DHT22 + BMP180 + BOOTSTRAP + SPIFFS + GOOGLE CHARTS
 * Copyright (C) 2017 http://www.projetsdiy.fr - http://www.diyprojects.io
 * 
 * Part 2 : Interaction between Arduino code and HTML interface, data exchange in JSON
 * - How to prepare a JSON object to send measurements to the HTML interface from Arduino code
 * - Add a Bootstrap-table table (http://bootstrap-table.wenzhixin.net.cn/) and refresh the display automatically
 * - Driving the ESP8266 GPIO from the HTML interface
 * - Full tutorial here http://www.diyprojects.io/esp8266-ftp-server-spiffs-file-exchange-rapid-development-web-server/
 *
 * Partie 2 (Web Serveur ESP8266) : Interaction entre le code Arduino et l'interface HTML, échange de données en JSON
 * - Comment préparer un objet JSON pour envoyer les mesures à l'interface HTML depuis le code Arduino
 * - Ajouter une table Bootstrap-table (http://bootstrap-table.wenzhixin.net.cn/) et actualiser l'affichage automatiquement
 * - Piloter le GPIO de l'ESP8266 depuis l'interface HTML
 * - Tutoriel complet http://www.projetsdiy.fr/esp8266-web-serveur-partie2-interaction-arduino-interface-html/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <FS.h>

#define ssid      "yourSSID"      // WiFi SSID
#define password  "yourPASSWORD"  // WiFi password
#define DHTTYPE   DHT22           // DHT type (DHT11, DHT22)
#define DHTPIN    D4              // Broche du DHT / DHT Pin
const uint8_t GPIOPIN[4] = {D5,D6,D7,D8};  // Led
float   t = 0 ;
float   h = 0 ;
float   pa = 0;
int     sizeHist = 100;           // Nombre de points dans l'historique - History size

// Création des objets / create Objects
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
ESP8266WebServer server ( 80 );
 
void updateGpio(){
  String gpio = server.arg("id");
  String etat = server.arg("etat");
  String success = "1";
  int pin = D5;
 if ( gpio == "D5" ) {
      pin = D5;
 } else if ( gpio == "D7" ) {
     pin = D7;
 } else if ( gpio == "D8" ) {
     pin = D8;  
 } else {   
      pin = D5;
  }
  Serial.println(pin);
  if ( etat == "1" ) {
    digitalWrite(pin, HIGH);
  } else if ( etat == "0" ) {
    digitalWrite(pin, LOW);
  } else {
    success = "1";
    Serial.println("Err Led Value");
  }
  
  String json = "{\"gpio\":\"" + String(gpio) + "\",";
  json += "\"etat\":\"" + String(etat) + "\",";
  json += "\"success\":\"" + String(success) + "\"}";
    
  server.send(200, "application/json", json);
  Serial.println("GPIO mis a jour");
}

void sendMesures() {
  String json = "{\"t\":\"" + String(t) + "\",";
  json += "\"h\":\"" + String(h) + "\",";
  json += "\"pa\":\"" + String(pa) + "\"}";

  server.send(200, "application/json", json);
  Serial.println("Mesures envoyees");
}

void sendTabMesures() {
  double temp = 0;      // Récupère la plus ancienne mesure (temperature) - get oldest record (temperature)
  String json = "[";
  json += "{\"mesure\":\"Température\",\"valeur\":\"" + String(t) + "\",\"unite\":\"°C\",\"glyph\":\"glyphicon-indent-left\",\"precedente\":\"" + String(temp) + "\"},";
  temp = 0;             // Récupère la plus ancienne mesure (humidite) - get oldest record (humidity)
  json += "{\"mesure\":\"Humidité\",\"valeur\":\"" + String(h) + "\",\"unite\":\"%\",\"glyph\":\"glyphicon-tint\",\"precedente\":\"" + String(temp) + "\"},";
  temp = 0;             // Récupère la plus ancienne mesure (pression atmospherique) - get oldest record (Atmospheric Pressure)
  json += "{\"mesure\":\"Pression Atmosphérique\",\"valeur\":\"" + String(pa) + "\",\"unite\":\"mbar\",\"glyph\":\"glyphicon-dashboard\",\"precedente\":\"" + String(temp) + "\"}";
  json += "]";
  server.send(200, "application/json", json);
  Serial.println("Tableau mesures envoyees");
}

void setup() {
  for ( int x = 0 ; x < 5 ; x++ ) {
    pinMode(GPIOPIN[x], OUTPUT);
  }
  
  Serial.begin ( 115200 );
  // Initialisation du BMP180 / Init BMP180
  if ( !bmp.begin() ) {
    Serial.println("BMP180 KO!");
    while (1);
  } else {
    Serial.println("BMP180 OK");
  }

  WiFi.begin ( ssid, password );
  // Attente de la connexion au réseau WiFi / Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); Serial.print ( "." );
  }
  // Connexion WiFi établie / WiFi connexion is OK
  Serial.println ( "" );
  Serial.print ( "Connected to " ); Serial.println ( ssid );
  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );

  if (!SPIFFS.begin())
  {
    // Serious problem
    Serial.println("SPIFFS Mount failed");
  } else {

    Serial.println("SPIFFS Mount succesfull");
  }

  server.on("/tabmesures.json", sendTabMesures);
  server.on("/mesures.json", sendMesures);
  server.on("/gpio", updateGpio);
  /*HTTP_POST, []() {
    updateGpio();
  });
  */
  server.serveStatic("/js", SPIFFS, "/js");
  server.serveStatic("/css", SPIFFS, "/css");
  server.serveStatic("/img", SPIFFS, "/img");
  server.serveStatic("/", SPIFFS, "/index.html");

  server.begin();
  Serial.println ( "HTTP server started" );

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  t = dht.readTemperature();
  h = dht.readHumidity();
  pa = bmp.readPressure() / 100.0F;
  delay(100);
}

