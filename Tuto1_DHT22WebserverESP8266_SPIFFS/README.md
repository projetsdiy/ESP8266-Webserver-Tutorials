# ESP8266 Web Server + SPIFFS + Bootstrap - Part 1
How to create Web Server with ESP8266 
* Measure temperature and humidity (DHT22), atmospheric pressure (BMP180)
* Store HTML, CSS and IMG file on SPIFFS system file
* Use Pug (old Jade) to create HTML interface
* Use Bootstrap to create modern interface
* Use the plugin Bootstrap-table to create table

Full tutorial on http://www.diyprojects.io/esp8266-web-server-part-1-store-web-interface-spiffs-area-html-css-js/

## Equipment used
* Any ESP8266 ESP-12 module, for example <a href="http://www.banggood.com/search/esp8266-nodemcu/0-0-0-1-3-45-0-price-0-0_p-1.html?sortType=asc?p=RA18043558422201601Y" target="_blank" rel="nofollow" data-mce-href="http://www.banggood.com/search/esp8266-nodemcu/0-0-0-1-3-45-0-price-0-0_p-1.html?sortType=asc?p=RA18043558422201601Y">Wemos D1 mini</a>
* <a href="http://s.click.aliexpress.com/e/eM72zRrbE" target="_blank" rel="nofollow" data-mce-href="http://s.click.aliexpress.com/e/eM72zRrbE">BMP180</a> : Atmospheric pressure sensor
* <a href="http://s.click.aliexpress.com/e/BeYvjyfim" target="_blank" rel="nofollow" data-mce-href="http://s.click.aliexpress.com/e/BeYvjyfim">DHT22</a> Temperature and humidity sensor

## Wiring
|Sensor |Pin |ESP8266 Pin|
|-------|----|----------:|
|DHT22  |VCC |5V         |
|       |GND |G          |
|       |Data|D4         |
|BMP180 |VCC |5V         |
|       |GND |G          |
|       |SDA |D2         |
|       |SCL |D1         |

## More ressources
* How to install Geany editor on Raspberry Pi, Orange Pi... (ARM SoC) :  
http://www.diyprojects.io/geany-text-editor-develop-raspberry-pi-orange-pi-arm/
* How to prepare HTML interface with Geany and Pug :  
http://www.diyprojects.io/pug-jade-prepare-html-interfaces-raspberry-pi-geany/
* How to install Arduino IDE for ARM plateform :  
http://www.diyprojects.io/install-arduino-ide-linux-arm-raspberry-pi-orange-pi-odroid/
* How to configure Arduino IDE and use ESP8266 :  
http://www.diyprojects.io/adafruit-huzzah-esp8266-test-firmware-upgrade-nodemcu/

# ESP8266 Web Server + SPIFFS + Bootstrap - Partie 1
Comment créer un serveur web avec un ESP8266
* Mesurer la température et l'humidité (DHT22), ainsi que la pression atmosphérique (BMP180)
* Enregistrer les fichiers HTML, CSS et IMG sur le système de fichier SPIFFS
* Utiliser Pug (ancien Jade) pour développer rapidement les interfaces HTML
* Comment utiliser Bootstrap pour créer des interfaces modernes
* Utiliser le plugin Bootstrap-table pour créer des tableaux

Tutoriel complet sur http://www.projetsdiy.fr/esp8266-web-server-partie1-stockage-spiffs-interface-web/

## Matériel utilisé
* N'importe quel ESP8266 (ESP-12x), par exemple <a href="http://www.banggood.com/search/esp8266-nodemcu/0-0-0-1-3-45-0-price-0-0_p-1.html?sortType=asc?p=RA18043558422201601Y" target="_blank" rel="nofollow" data-mce-href="http://www.banggood.com/search/esp8266-nodemcu/0-0-0-1-3-45-0-price-0-0_p-1.html?sortType=asc?p=RA18043558422201601Y">Wemos D1 mini</a>
* <a href="http://s.click.aliexpress.com/e/eM72zRrbE" target="_blank" rel="nofollow" data-mce-href="http://s.click.aliexpress.com/e/eM72zRrbE">BMP180</a> : Capteur de pression atmosphérique
* <a href="http://s.click.aliexpress.com/e/BeYvjyfim" target="_blank" rel="nofollow" data-mce-href="http://s.click.aliexpress.com/e/BeYvjyfim">DHT22</a> capteur de température et d'humitité

## Cablage
|Sensor |Broche |Broche ESP8266|
|-------|-------|-------------:|
|DHT22  |VCC    |5V            |
|       |GND    |G             |
|       |Data   |D4            |
|BMP180 |VCC    |5V            |
|       |GND    |G             |
|       |SDA    |D2            |
|       |SCL    |D1            |

## Autres ressources
* Comment installer et configurer Geany pour coder en Pug :  
http://www.projetsdiy.fr/geany-editeur-texte-arm-raspberry-pi-orange/
* Découverte du langage Pug (ancien Jade):  
http://www.projetsdiy.fr/pug-jade-preparer-interfaces-html-raspberry-pi-geany/
* Installer l'IDE Arduino sur plateforme ARM (Orange Pi, Raspberry Pi...) :  
http://www.projetsdiy.fr/installer-ide-arduino-linux-arm-raspberry-pi-orange-pi-odroid-armbian/
* Comment programmer en code Arduino l'ESP8266 :  
http://www.projetsdiy.fr/adafruit-huzzah-esp8266-test-tuto-update-firmware/


<img align="center" src="https://github.com/projetsdiy/ESP8266-Webserver-Tutorials/blob/master/Tuto1_DHT22WebserverESP8266_SPIFFS/esp8266-spiffs-wemos-d1-mini-web-server-bootstrap-dht22-bmp180.png" style="max-width:100%;">

Licence : MIT

Copyright : www.projetsdiy.fr and www.diyprojects.io
