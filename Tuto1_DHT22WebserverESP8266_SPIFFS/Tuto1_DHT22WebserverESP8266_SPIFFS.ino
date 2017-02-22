/*
 * ESP8266 + DHT22 + BMP180 + BOOTSTRAP + SPIFFS
 * 
 * Step 1 - Start Project ESP8266 Web Server + SPIFFS + Bootstrap
 * Full tutorial on http://www.diyprojects.io/esp8266-web-server-part-1-store-web-interface-spiffs-area-html-css-js/
 * 
 * Etape 1 - Démarrage du projet ESP8266 Web Serveur + Bootstrap
 * Tutoriel complet sur http://www.projetsdiy.fr/esp8266-web-server-partie1-stockage-spiffs-interface-web/
 *  
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
 * 
 * Licence : MIT
 * Copyright (C) 2017 : www.projetsdiy.fr and www.diyprojects.io
 */
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <FS.h>

#define ssid      "yourSSID"      // WiFi SSID
#define password  "yourPASSWORD"  // WiFi password
#define DHTTYPE   DHT22           // DHT type (DHT11, DHT22)
#define DHTPIN    D4              // Broche du DHT - DHT Pin
const uint8_t GPIOPIN[4] = {D5,D6,D7,D8};  // Led
float   t = 0 ;
float   h = 0 ;
float   pa = 0;

// Création des objets - create Objects
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
ESP8266WebServer server ( 80 );

void setup() {
  for ( int x = 0 ; x < 5 ; x++ ) {
    pinMode(GPIOPIN[x], OUTPUT);
  }
  
  Serial.begin ( 115200 );
  // Initialisation du BMP180 - Init BMP180
  if ( !bmp.begin() ) {
    Serial.println("BMP180 KO!");
    while (1);
  } else {
    Serial.println("BMP180 OK");
  }

  WiFi.begin ( ssid, password );
  // Attente de la connexion au réseau WiFi - Wait for WiFi connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); Serial.print ( "." );
  }
  // Connexion WiFi établie - WiFi connexion is OK
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

  // Rend disponible la page index et les images - Makes index page and images available
  server.serveStatic("/img", SPIFFS, "/img");
  server.serveStatic("/", SPIFFS, "/index.html");

  // Démarre le serveur web - Start Web Server
  server.begin();
  Serial.println ( "HTTP server started" );

}

void loop() {
  // Vérifie régulière la connexion de nouveaux clients - Regularly checks the connection of new clients
  server.handleClient();
  t = dht.readTemperature();
  h = dht.readHumidity();
  pa = bmp.readPressure() / 100.0F;
  delay(100);
}

