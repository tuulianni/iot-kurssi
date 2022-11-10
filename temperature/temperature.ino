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
int serverport = 8080;

//asenna ArduinoJson !!
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

//TÄMÄ?
#include <Wire.h>

//kertoo wifin tiedot, Thw WiFi radio's status
int wifi_status = WL_IDLE_STATUS;
//will store last time Wi-Fi information was updated
unsigned long previousMillisInfo = 0;     

WiFiClient client;

//TÄMÄ!!
bool offline_mode = false;  // for testing...

Servo servo;
int angle = 10;

#define DHTPIN 2
#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN, DHTTYPE);


//TÄMÄ?

float temperature = -1.0;  //current
float humidity = -1.0;     //current

bool override_temp = false;
float min_temp = 17;
float max_temp = 25;

//Local json object to save sensor data into
//["temperature"]
//["humidity"]
const size_t capacity_sdj = JSON_OBJECT_SIZE(2);
DynamicJsonDocument sensor_data_json(capacity_sdj);

int upload_data();
int upload_data(char*, int);

//----------------------------------------------------------------------------

String IpAddress2String(const IPAddress &ipAddress) {
    return String(ipAddress[0]) + String(".") + String(ipAddress[1]) + String(".") + String(ipAddress[2]) + String(".") + String(ipAddress[3]);
}

//NÄÄ VISSII VAA HAKEE NOITA IP-OSOTTEITA, EI TARVI ?
String getlocalip() {
    IPAddress localip = WiFi.localIP();
    return IpAddress2String(localip);
}

String getgatewayip() {
    IPAddress gatewayip = WiFi.gatewayIP();
    return IpAddress2String(gatewayip);
}

//Yhdistetään wifii
void wifi_init() {
    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("Communication with WiFi module failed!");
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
        Serial.println("Wifi connected");

        //TARVIIKO NÄITÄ 
        byte mac[6];
          WiFi.macAddress(mac);

        pingServer();
    }
}

//yhdistetään verkkoon
void wifi_connect() {

		//jos while ei toimi niin if (Wifi.status() != WL_CONNECTED) toimii

    while (wifi_status != WL_CONNECTED) {
        Serial.print("Attempting to connect to network: ");
        Serial.println(ssid);
        
        wifi_status = WiFi.begin(ssid, pass);
        
    }
}


void display_sensor_data() {
	//tyhjennä ruutu
    display.clearDisplay();
		//aseta teksti paikkaan
    display.setCursor(5,5);
    display.println(String(temperature) + " C");
    display.println(String(humidity) + " %");

    display.display();
}

int read_sensor_data() {
    float hNow = dht.readHumidity();

    float tNow = dht.readTemperature();

    bool update_display = false;

    if (isnan(hNow) || isnan(tNow)) {
        Serial.println("Failed to read from DHT sensor!");
        return -1;
    } else {
        if (tNow != temperature) {
            temperature = tNow;
            update_display = true;
        }

        if (hNow != humidity) {
            humidity = hNow;
            update_display = true;
        }
    }
    if (update_display) {
        return 0;
    }
    return 1;
}
//NÄITÄ EI VARMAAN TARVI?
//int update_targets() {

//    float targ_temperature = -99;
//    float targ_humidity = -99;


    //check connection
//    if (WiFi.status() != WL_CONNECTED) {
        //ERROR
//        Serial.println("wifi ei toimi");
//        return -1;
//    }


//    Serial.println("Yhdistetään");

//    if (!client.connect(servername, serverport)) {
//        Serial.println("ei yhteyttä serveriin");
//        return -1;
//    }

//    Serial.println("Serveriyhteys saatu");

    // Send HTTP request
//    client.println(F("GET /api/targets HTTP/1.0"));
//    client.println("Host: " + String(servername));
//    client.println(F("Connection: close"));
//    if (client.println() == 0) {
//        Serial.println("Ei saatu yhteyttö");
//        client.stop();
//        return -1;
//    }

    // Skip HTTP headers
//    char endOfHeaders[] = "\r\n\r\n";
 //   if (!client.find(endOfHeaders)) {
//        Serial.println("Invalid response");
//        client.stop();
//        return -1;
//    }



    // Allocate the JSON document
 //   const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_ARRAY_SIZE(2) + 60;
    //const size_t capacity = JSON_OBJECT_SIZE(4);
//    DynamicJsonDocument json_response(capacity);

    // Parse JSON object
//    DeserializationError error = deserializeJson(json_response, client);
//    if (error) {
//        Serial.print("deserializeJson() failed: ");
//        Serial.println(error.f_str());
//        client.stop();
//        return -1;
//    }
    // Disconnect
//    client.stop();
//    Serial.print("target json response: ");

//    String response_string;
//    serializeJson(json_response, response_string);
//    Serial.print(response_string);
//    Serial.println("");
		//tässä pystyis lähettää sitä servotietoa 
    //joku_oikea_muuttuja = json_response["servocontrol_json"]["servo_order"];

//    bool update_display = false;
	//TÄN SAA KANS VARMAA POISTAA
    //if (targ_temperature != wants_temp || targ_humidity != wants_humidity) {
    //    update_display = true;
    //}
//TÄMÄ
    //wants_temp = targ_temperature;
    //wants_humidity = targ_humidity;

   // if (update_display) {

//        return 1;
   // }

//    return 0;
//}

int upload_data() {
    return upload_data(servername, serverport);
}
int upload_data(char *host, int port) {
    if (WiFi.status() == WL_CONNECTED) {
			//jos on yhdistetty wifi ni yhdistyy serveriin
        if (client.connect(host, port)) {
            Serial.println("Yhdistetty serveriin " + String(host));
						Serial.println("Siis tää oikeesti toimii!");
            String sensor_data_string;
            serializeJson(sensor_data_json, sensor_data_string);
            Serial.println(sensor_data_string);
            client.println("POST /api/upload HTTP/1.1");
            client.println("Host: " + String(host));
            client.println("Content-Type: application/json;charset=UTF-8");
            client.print("Content-Length: ");
            client.println(measureJson(sensor_data_json));
            client.println();
            serializeJson(sensor_data_json, client);
        } else {
            Serial.println("juu ei toimi tää paske");
        }
        if (client.connected()) {
            Serial.println("\ndisconnecting from server.");
            client.stop();
            return 0;
        }
    } else {
        Serial.println("server: ei oo wifii");
    }
    return -1;
}

void setup() {
	//yhdistetään nettiin
    if (!offline_mode) {
        wifi_init();
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
    display.print("terve");

    display.display();
    delay(1);
    display_sensor_data();

		//tä tekee jotain nyt sit tolle 
    sensor_data_json["temperature"] = "-99";
    sensor_data_json["humidity"] = "-99";

		//servo
    servo.attach(8);
		servo.write(angle);
}

unsigned long SENSOR_UPDATE_DELAY = 5000; //tää vois varmaan olla isompiki
unsigned long LAST_SENSOR_UPDATE_TIME = 0;

unsigned long TARGET_UPDATE_DELAY = 5000; //tää vois varmaa olla isompiki
unsigned long LAST_TARGET_UPDATE_TIME = 0;

float react_sensitivity_delta = 0.1;

void loop() {

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

    // check if enough time has passed for us to get new measurements.
    //...
    unsigned long TIME = millis();

//TÄTÄ EI EHKÄ TARVII?
  //  if (TIME - LAST_TARGET_UPDATE_TIME >= TARGET_UPDATE_DELAY || LAST_TARGET_UPDATE_TIME == 0) {
   //     if (!offline_mode) {
            //check connection
   //         if (WiFi.status() != WL_CONNECTED) {
                //ERROR
   //             Serial.println("wifi ei oo yhteydes, yhdistetään uusiks");
   //             wifi_init();
   //         } else {
   //             if (update_targets() == 1) {
   //                 update_display = true;
   //             }
   //         }
   //         LAST_TARGET_UPDATE_TIME = TIME;
   //     }
   // }


    if (TIME - LAST_SENSOR_UPDATE_TIME >= SENSOR_UPDATE_DELAY || LAST_SENSOR_UPDATE_TIME == 0) {
        Serial.println("SENSOR_UPDATE...");

        switch (read_sensor_data()) {
        case -1:
            //jos ei toimi
            break;

        case 1:
            //luetaan, mutta arvot ei muutu
            Serial.println("Lämpötila: " + String(temperature) + "°C");
            Serial.println("Kosteus: " + String(humidity));
            LAST_SENSOR_UPDATE_TIME = TIME;
            break;

        case 0:
            LAST_SENSOR_UPDATE_TIME = TIME;
            //luetaan ja arvot muuttuu
            Serial.println("Lämpötila: " + String(temperature) + "°C");
            Serial.println("Kosteus: " + String(humidity));
						//tässä se varmaan lähettää sen sitten jsonille ja sitä kautta serverille
            sensor_data_json["temperature"] = temperature;
            sensor_data_json["humidity"] = humidity;
            update_display = true;
            break;

        default:
            
            break;
        }

    }
    if (!offline_mode) {
        //upload new readings...
        if (WiFi.status() != WL_CONNECTED) {
            //ERROR
            Serial.println("WIFI NOT CONNECTED! Attempting to reconnect.");
            wifi_init();
        }
        upload_data();
    }
}


//connectivity testing...
void pingServer() {
    int pingResult;
    Serial.print("Pinging ");
    Serial.print(servername);
    Serial.print(": ");

    pingResult = WiFi.ping(servername);

    if (pingResult >= 0) {
        Serial.print("Toimi! RTT = ");
        Serial.print(String(pingResult));
        Serial.println(" ms");
    } else {
        Serial.print("FAILED! Error code: ");
        Serial.println(String(pingResult));
    }
}


