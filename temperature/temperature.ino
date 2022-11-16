//lämpötilasensori DHT 11
#include "DHT.h"

#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#include <Tiny4kOLED.h>
#include <Servo.h>

#include <WiFiNINA.h>

#include "salaiset_kansiot.h"

char ssid[] = SECRET_SSID;                // verkon käyttäjätunnus
char pass[] = SECRET_PASS;                // verkon salasana
char servername[] = SECRET_SERVERNAME;    // verkon ip, ipconfig

//serverin portti, testaa portteja 49152 - 65535 väliltä, muista vaihtaa se myös serverille
int serverport = 65535;

//asenna ArduinoJson !!
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

//kertoo wifin tiedot, Thw WiFi radio's status
int wifi_status = WL_IDLE_STATUS;
unsigned long previousMillisInfo = 0;     

WiFiClient client;

//testaa onko onlinessa
bool offline_mode = false;

Servo servo;
int angle = 10;

#define DHTPIN 2
#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN, DHTTYPE);

float temperature;  //current
float humidity;     //current

String x; //Serveriltä tuleva viesti 

//Json tallentaa tietoja, muuttujat ovat:
//["temperature"]
//["humidity"]
const size_t json_capasity = JSON_OBJECT_SIZE(2);
DynamicJsonDocument temphum_json(json_capasity);

int upload_data();
int upload_data(char*, int);

//Tämä tekee fifiyhdistyksen
void wifi_connection() {
    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("Ei saada yhteyttä wifii");
        // don't continue
    } else {
        String fv = WiFi.firmwareVersion();
        if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
            Serial.println("Please upgrade the firmware");
        }

        wifi_connect();
        delay(10000);


        //keep connecting until success
        unsigned long TIME = millis();
        while (WiFi.status() != WL_CONNECTED) {
            if (millis() - TIME > 10000L) {
                wifi_connect();
                TIME = millis();
            }
        }
        Serial.println("Wifi yhdistetty");
    }
}

//yhdistetään verkkoon
void wifi_connect() {

    while (wifi_status != WL_CONNECTED) {
        Serial.print("Yritetään yhdistää verkkoon: ");
        Serial.println(ssid);
        
        wifi_status = WiFi.begin(ssid, pass);
        
    }
}


void show_display() {

	float t = temperature;
	float h = humidity;
 
    if ((t >= 21) || (t <= 24)) {
      //tyhjennä ruutu
      display.clearDisplay();
      //aseta teksti paikkaan
      display.setCursor(5,5);
      display.print(x);
      display.setCursor(5,15);
      display.print(String(t) + "C ja " + String(h) + "%");

      //näytä näytöllä
      display.display();
    }     
    else if (t > 24) {
      
      //tyhjennä ruutu
      display.clearDisplay();
      //aseta teksti paikkaan
      display.setCursor(5,5);
      display.print(x);
      display.setCursor(5,15);
      display.print(String(t) + "C ja " + String(h) + "%");

      //näytä näytöllä
      display.display();
     }
     else {
      //tyhjennä ruutu
      display.clearDisplay();
      //aseta teksti paikkaan
      display.setCursor(5,5);
      display.print(x);
      display.setCursor(5,15);
      display.print(String(t) + "C ja " + String(h) + "%");

      //näytä näytöllä
      display.display();
     }
}

int read_sensor_data() {
    float h = dht.readHumidity();

    float t = dht.readTemperature();

    bool update_display = false;

    if (isnan(h) || isnan(t)) {
        Serial.println("Ei pysty lukemaan lämpösensoria");
        return -1;
    } else {
        if (t != temperature) {
            temperature = t;
            update_display = true;
        }

        if (h != humidity) {
            humidity = h;
            update_display = true;
        }
    }
    if (update_display) {
        return 0;
    }
    return 1;
}

int upload_data() {
    return upload_data(servername, serverport);
}
int upload_data(char *host, int port) {
    if (WiFi.status() == WL_CONNECTED) {
			//jos on yhdistetty wifi ni yhdistyy serveriin
        if (client.connect(host, port)) {
            Serial.println("Yhdistetty serveriin " + String(host));
						Serial.println("Siis tää oikeesti toimii!");
            String temphum_string;
            serializeJson(temphum_json, temphum_string);
            Serial.println(temphum_string);
            client.println("POST /api/upload HTTP/1.1");
            client.println("Host: " + String(host));
            client.println("Content-Type: application/json;charset=UTF-8");
            client.print("Content-Length: ");
            client.println(measureJson(temphum_json));
            client.println();
            serializeJson(temphum_json, client);

            //tässä luetaan serveriltä juduja
            x = client.readString();
            x.trim();

            int startof = x.indexOf("&");
            int stopof = x.indexOf("#");
            x = x.substring(startof +1, stopof);

            Serial.println(x);
            
            
        } else {
            Serial.println("juu ei toimi tää paske");
        }
        if (client.connected()) {
            client.stop();
            return 0;
        }
    } else {
        Serial.println("server: ei oo wifii");
    }
    return -1;
}

void setup() {
	//yhdistetään nettiin jollei ole yhteydessä
    if (!offline_mode) {
        wifi_connection();
    }

		Serial.begin(9600);

    //starttaa mittarin
    dht.begin();

		//näyttö
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.clearDisplay();
    display.setCursor(0, 15);
    display.print("Tervetuloa!");

    display.display();
    delay(1000);

		//servo
    servo.attach(8);
		servo.write(angle);
}

void loop() {

	show_display();

	    //Servo liikkuu edestakaisin
//     scan from 0 to 180 degrees

    for(angle = 0; angle < 200; angle++) {                                  
      servo.write(angle);               
     delay(7);                   
    } 
     // now scan back from 180 to 0 degrees
    for(angle = 200; angle > 0; angle--) {                                
      servo.write(angle);           
      delay(2);       
  	} 

    bool update_display = false;

        Serial.println("Haetaan lämpöjä");

        switch (read_sensor_data()) {
        case -1:
            //jos ei toimi
            break;

        case 1:
            //luetaan, mutta arvot ei muutu
            Serial.println("Lämpötila: " + String(temperature) + "°C");
            Serial.println("Kosteus: " + String(humidity));
            break;

        case 0:

            //luetaan ja arvot muuttuu
            Serial.println("Lämpötila: " + String(temperature) + "°C");
            Serial.println("Kosteus: " + String(humidity));
						//ja lähetetään se sitten jsonille ja sitä kautta serverille
            temphum_json["temperature"] = temperature;
            temphum_json["humidity"] = humidity;
            
            break;

        default:
            
            break;
        }

   // }
    if (!offline_mode) {
        //upload new readings...
        if (WiFi.status() != WL_CONNECTED) {
            //ERROR
            Serial.println("Olet offlinessa");
            wifi_connection();
        }
        upload_data();
    }
}
