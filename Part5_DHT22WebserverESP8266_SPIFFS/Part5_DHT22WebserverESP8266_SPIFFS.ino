/*
 * ESP8266 + DHT22 + BMP180 + BOOTSTRAP + SPIFFS + GOOGLE CHARTS
 * Copyright (C) 2017 http://www.projetsdiy.fr - http://www.diyprojects.io
 * 
 * Part 5 : 
 *  - Create History based ArduinoJSON
 *  - Prepare the histogram graph data for the average temperature and humidity measurements
 *  - Add Google Charts to the web page
 *  - Full tutorial here : http://www.diyprojects.io/esp8266-web-server-part-5-add-google-charts-gauges-and-charts/
 *  
 *  Partie 5 :
 *  - Enregistre un historique de mesure à l'aide d'ArduinoJSON
 *  - Prepare les données du graphique en histogramme des mesures moyennes de température et d'humidité
 *  - Ajout des graphiques Google Charts à la page web
 *  - Tutoriel complet :  http://www.projetsdiy.fr/esp8266-web-serveur-partie5-gauges-graphiques-google-charts/
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
 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <FS.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <ArduinoJson.h>

#define ssid      "yourSSID"        // WiFi SSID
#define password  "yourPASSWORD"    // WiFi password
#define DHTTYPE   DHT22             // DHT type (DHT11, DHT22)
#define DHTPIN    D4                // Broche du DHT / DHT Pin
#define HISTORY_FILE "/history.json"
const uint8_t GPIOPIN[4] = {D5,D6,D7,D8};  // Led
float   t = 0 ;
float   h = 0 ;
float   pa = 0;
int     sizeHist = 84 ;        // Taille historique (7h x 12pts) - History size 

const long intervalHist = 1000 * 60 * 5;  // 5 mesures / heure - 5 measures / hours
unsigned long previousMillis = intervalHist;  // Dernier point enregistré dans l'historique - time of last point added

// Création des objets / create Objects
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
ESP8266WebServer server ( 80 );

StaticJsonBuffer<10000> jsonBuffer;                 // Buffer static contenant le JSON courant - Current JSON static buffer
JsonObject& root = jsonBuffer.createObject();
JsonArray& timestamp = root.createNestedArray("timestamp");
JsonArray& hist_t = root.createNestedArray("t");
JsonArray& hist_h = root.createNestedArray("h");
JsonArray& hist_pa = root.createNestedArray("pa");
JsonArray& bart = root.createNestedArray("bart");   // Clé historgramme (temp/humidité) - Key histogramm (temp/humidity)
JsonArray& barh = root.createNestedArray("barh");   // Clé historgramme (temp/humidité) - Key histogramm (temp/humidity)

char json[10000];                                   // Buffer pour export du JSON - JSON export buffer

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
  Serial.println("GPIO updated");
}

void sendMesures() {
  String json = "{\"t\":\"" + String(t) + "\",";
  json += "\"h\":\"" + String(h) + "\",";
  json += "\"pa\":\"" + String(pa) + "\"}";

  server.send(200, "application/json", json);
  Serial.println("Send measures");
}

void calcStat(){
  float statTemp[7] = {-999,-999,-999,-999,-999,-999,-999};
  float statHumi[7] = {-999,-999,-999,-999,-999,-999,-999};
  int nbClass = 7;  // Nombre de classes - Number of classes                         
  int currentClass = 0;
  int sizeClass = hist_t.size() / nbClass;  // 2
  double temp;
  //
  if ( hist_t.size() >= sizeHist ) {
    //Serial.print("taille classe ");Serial.println(sizeClass);
    //Serial.print("taille historique ");Serial.println(hist_t.size());
    for ( int k = 0 ; k < hist_t.size() ; k++ ) {
      temp = root["t"][k];
      if ( statTemp[currentClass] == -999 ) {
        statTemp[ currentClass ] = temp;
      } else {
        statTemp[ currentClass ] = ( statTemp[ currentClass ] + temp ) / 2;
      }
      temp = root["h"][k];
      if ( statHumi[currentClass] == -999 ) {
        statHumi[ currentClass ] = temp;
      } else {
        statHumi[ currentClass ] = ( statHumi[ currentClass ] + temp ) / 2;
      }
         
      if ( ( k + 1 ) > sizeClass * ( currentClass + 1 ) ) {
        //Serial.print("k ");Serial.print(k + 1);Serial.print(" Cellule statTemp = ");Serial.println(statTemp[ currentClass ]);
        currentClass++;
      } else {
        //Serial.print("k ");Serial.print(k + 1);Serial.print(" < ");Serial.println(sizeClass * currentClass);
      }
    }
    
    Serial.println("Histogram - Temperature"); 
    for ( int i = 0 ; i < nbClass ; i++ ) {
      Serial.print(statTemp[i]);Serial.print('|');
    }
    Serial.println("Histogram - Humidity "); 
    for ( int i = 0 ; i < nbClass ; i++ ) {
      Serial.print(statHumi[i]);Serial.print('|');
    }
    Serial.print("");
    if ( bart.size() == 0 ) {
      for ( int k = 0 ; k < nbClass ; k++ ) { 
        bart.add(statTemp[k]);
        barh.add(statHumi[k]);
      }  
    } else {
      for ( int k = 0 ; k < nbClass ; k++ ) { 
        bart.set(k, statTemp[k]);
        barh.set(k, statHumi[k]);
      }  
    }
  }
}

void sendTabMesures() {
  double temp = root["t"][0];      // Récupère la plus ancienne mesure (temperature) - get oldest record (temperature)
  String json = "[";
  json += "{\"mesure\":\"Température\",\"valeur\":\"" + String(t) + "\",\"unite\":\"°C\",\"glyph\":\"glyphicon-indent-left\",\"precedente\":\"" + String(temp) + "\"},";
  temp = root["h"][0];             // Récupère la plus ancienne mesure (humidite) - get oldest record (humidity)
  json += "{\"mesure\":\"Humidité\",\"valeur\":\"" + String(h) + "\",\"unite\":\"%\",\"glyph\":\"glyphicon-tint\",\"precedente\":\"" + String(temp) + "\"},";
  temp = root["pa"][0];             // Récupère la plus ancienne mesure (pression atmospherique) - get oldest record (Atmospheric Pressure)
  json += "{\"mesure\":\"Pression Atmosphérique\",\"valeur\":\"" + String(pa) + "\",\"unite\":\"mbar\",\"glyph\":\"glyphicon-dashboard\",\"precedente\":\"" + String(temp) + "\"}";
  json += "]";
  server.send(200, "application/json", json);
  Serial.println("Send data tab");
}

void sendHistory(){  
  root.printTo(json, sizeof(json));             // Export du JSON dans une chaine - Export JSON object as a string
  server.send(200, "application/json", json);   // Envoi l'historique au client Web - Send history data to the web client
  Serial.println("Send History");   
}

void loadHistory(){
  File file = SPIFFS.open(HISTORY_FILE, "r");
  if (!file){
    Serial.println("Aucun historique existe - No History Exist");
  } else {
    size_t size = file.size();
    if ( size == 0 ) {
      Serial.println("Fichier historique vide - History file empty !");
    } else {
      std::unique_ptr<char[]> buf (new char[size]);
      file.readBytes(buf.get(), size);
      JsonObject& root = jsonBuffer.parseObject(buf.get());
      if (!root.success()) {
        Serial.println("Impossible de lire le JSON - Impossible to read JSON file");
      } else {
        Serial.println("Historique charge - History loaded");
        root.prettyPrintTo(Serial);  
      }
    }
    file.close();
  }
}

void saveHistory(){
  Serial.println("Save History");            
  File historyFile = SPIFFS.open(HISTORY_FILE, "w");
  root.printTo(historyFile); // Exporte et enregsitre le JSON dans la zone SPIFFS - Export and save JSON object to SPIFFS area
  historyFile.close();  
}

void setup() {
  NTP.onNTPSyncEvent([](NTPSyncEvent_t error) {
    if (error) {
      Serial.print("Time Sync error: ");
      if (error == noResponse)
        Serial.println("NTP server not reachable");
      else if (error == invalidAddress)
        Serial.println("Invalid NTP server address");
      }
    else {
      Serial.print("Got NTP time: ");
      Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));
    }
  });
  // Serveur NTP, decalage horaire, heure été - NTP Server, time offset, daylight 
  NTP.begin("pool.ntp.org", 0, true); 
  NTP.setInterval(60000);
  delay(500);
     
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
  int tentativeWiFi = 0;
  // Attente de la connexion au réseau WiFi / Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); Serial.print ( "." );
    tentativeWiFi++;
    if ( tentativeWiFi > 20 ) {
      ESP.reset();
      while(true)
        delay(1);
    }
  }
  // Connexion WiFi établie / WiFi connexion is OK
  Serial.println ( "" );
  Serial.print ( "Connected to " ); Serial.println ( ssid );
  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );
  
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS Mount failed");        // Problème avec le stockage SPIFFS - Serious problem with SPIFFS 
  } else { 
    Serial.println("SPIFFS Mount succesfull");
    loadHistory();
  }
  delay(50);
  
  server.on("/tabmesures.json", sendTabMesures);
  server.on("/mesures.json", sendMesures);
  server.on("/gpio", updateGpio);
  server.on("/graph_temp.json", sendHistory);

  server.serveStatic("/js", SPIFFS, "/js");
  server.serveStatic("/css", SPIFFS, "/css");
  server.serveStatic("/img", SPIFFS, "/img");
  server.serveStatic("/", SPIFFS, "/index.html");

  server.begin();
  Serial.println ( "HTTP server started" );

  Serial.print("Uptime :");
  Serial.println(NTP.getUptime());
  Serial.print("LastBootTime :");
  Serial.println(NTP.getLastBootTime());
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  t = dht.readTemperature();
  h = dht.readHumidity();
  pa = bmp.readPressure() / 100.0F;
  if ( isnan(t) || isnan(h) ) {
    //Erreur, aucune valeur valide - Error, no valid value
  } else {
    addPtToHist();
  }
  //delay(5);
}

void addPtToHist(){
  unsigned long currentMillis = millis();
  
  //Serial.println(currentMillis - previousMillis);
  if ( currentMillis - previousMillis > intervalHist ) {
    long int tps = NTP.getTime();
    previousMillis = currentMillis;
    //Serial.println(NTP.getTime());
    if ( tps > 0 ) {
      timestamp.add(tps);
      hist_t.add(double_with_n_digits(t, 1));
      hist_h.add(double_with_n_digits(h, 1));
      hist_pa.add(double_with_n_digits(pa, 1));

      //root.printTo(Serial);
      if ( hist_t.size() > sizeHist ) {
        //Serial.println("efface anciennes mesures");
        timestamp.removeAt(0);
        hist_t.removeAt(0);
        hist_h.removeAt(0);
        hist_pa.removeAt(0);
      }
      //Serial.print("size hist_t ");Serial.println(hist_t.size());
      calcStat();
      delay(100);
      saveHistory();
      //root.printTo(Serial);  
    }  
  }
}
