/*
 * Read multiple One-Wire DS18B20 probes and publish value on Domoticz with HTTP request
 * Lecture multiple de sonde OneWire DS18B20 et plublication des mesures sur un serveur Domoticz requete HTTP
 * Code adapté - Code adaptated 
 * 
 */
//include <OneWire.h>
//#include <DallasTemperature.h>

// Pour une carte ESP8266 | For ESP8266 development board
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>
#include "DHT.h"


#define DHT1PIN 4     // what digital pin we're connected to
#define DHT2PIN 5

// Uncomment whatever type you're using!
#define DHT1TYPE DHT11   // DHT 11
#define DHT2TYPE DHT22
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht1(DHT1PIN, DHT1TYPE);
DHT dht2(DHT2PIN, DHT2TYPE);
// Data wire is plugged into port 4 on the Arduino or ESP32
//#define ONE_WIRE_BUS 4
//#define TEMPERATURE_PRECISION 10

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
//OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
//DallasTemperature sensors(&oneWire);

// Tableaux contenant l'adresse de chaque sonde OneWire | arrays to hold device addresses
//DeviceAddress insideThermometer = { 0x28,  0xD4,  0xB0,  0x26,  0x0,  0x0,  0x80,  0xBC };
//DeviceAddress outsideThermometer = { 0x28,  0xF4,  0xBC,  0x26,  0x0,  0x0,  0x80,  0x2B };

// Parametres WIFI - WiFi settings
#define wifi_ssid "KVKNETWERK"
#define wifi_password "VlieR300"

// Paramètres HTTP Domoticz - HTTP Domoticz settings
const char* host = "192.168.87.17";
const int   port = 8080;
#define IDX_greenhouseTemp    47
#define IDX_greenhouseHum    50
#define IDX_outsideTemp   51
#define IDX_outsideHum   52

int ledpin = 2;
HTTPClient http;
float temp_gh = 25;
float h_gh = 101;
float h_out = 102;
float temp_out = 25;

void setup() {
  Serial.begin(115200);

  // Connexion au réseau WiFi, connexion aux sondes
  // Start WiFi connexion and probes
  setup_wifi();           
//  sensors.begin();
  dht1.begin();
  dht2.begin();
  pinMode(ledpin, OUTPUT);

}

//void printTemperature(String label, DeviceAddress deviceAddress){
  void printTemperature(String label){
 
  

   if ( label == "Greenhouse : " ) {
//      h_gh = dht1.readHumidity();
  temp_gh = dht1.readTemperature();
      String url = "/json.htm?type=command&param=udevice&idx=";
        url += String(IDX_greenhouseTemp);
        url += "&nvalue=0&svalue=";    
        url += String(temp_gh); 
      sendToDomoticz(url);
   }      
    else if ( label == "Outside : " ) {
 h_out = dht2.readHumidity();
  temp_out = dht2.readTemperature();
      
    String url = "/json.htm?type=command&param=udevice&idx=";
    url += String(IDX_outsideTemp);
    url += "&nvalue=0&svalue=";    
    url += String(temp_out);  
     
    sendToDomoticz(url);
    
   
    }
    
}
void printHumidity(String label){
 
  
//  float tempC = sensors.getTempC(deviceAddress);
//  Serial.print(label);
//  if (temp_gh == -127.00) {
//    Serial.print("Error getting temperature");
//  } else {
    // Format JSON à respecter pour l'API Domoticz - Domoticz JSON API 
    // /json.htm?type=command&param=udevice&idx=IDX&nvalue=0&svalue=TEMP
    // https://www.domoticz.com/wiki/Domoticz_API/JSON_URL%27s#Temperature
   if ( label == "Greenhouse : " ) {
      h_gh = dht1.readHumidity();
//  temp_gh = dht1.readTemperature();
//      String url = "/json.htm?type=command&param=udevice&idx=";
//        url += String(IDX_greenhouseTemp);
 //       url += "&nvalue=0&svalue=";    
//        url += String(temp_gh); 
//      sendToDomoticz(url);
      
//delay(5000);
      String url = "/json.htm?type=command&param=udevice&idx=";
    url += String(IDX_greenhouseHum);
    url += "&nvalue=0&svalue=";    
    url += String(h_gh);  
    sendToDomoticz(url);
    }
  
    else if ( label == "Outside : " ) {
 h_out = dht2.readHumidity();
//  temp_out = dht2.readTemperature();
      
//    String url = "/json.htm?type=command&param=udevice&idx=";
//    url += String(IDX_outsideTemp);
//    url += "&nvalue=0&svalue=";    
//    url += String(temp_out);  
     
//    sendToDomoticz(url);
    
//delay(5000);
     String url = "/json.htm?type=command&param=udevice&idx=";
    url += String(IDX_outsideHum);
    url += "&nvalue=0&svalue=";    
    url += String(h_out);  
    sendToDomoticz(url);
    
   
    }
    
}

void loop() {

//  Serial.print("Requesting temperatures...");
//  sensors.requestTemperatures();
//  Serial.println("DONE");

  // print the device information
digitalWrite(ledpin, HIGH);

printTemperature("Greenhouse : ");

digitalWrite(ledpin, LOW);

delay(5000);

digitalWrite(ledpin, HIGH);

printHumidity("Greenhouse : ");

digitalWrite(ledpin, LOW);

delay(5000);
digitalWrite(ledpin, HIGH);

printTemperature("Outside : ");

digitalWrite(ledpin, LOW);  

digitalWrite(ledpin, HIGH);

printHumidity("Outside : ");

digitalWrite(ledpin, LOW);

delay(5000);
  //printTemperature("Outside : ", outsideThermometer);
//   float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
//  float hic = dht.computeHeatIndex(t, h, false);

//  Serial.print("Humidity: ");
//  Serial.print(h);
//  Serial.print(" %\t");
//  Serial.print("Temperature: ");
//  Serial.print(tempC);
//  Serial.print(" *C ");
  delay(10000);
}

//Connexion au réseau WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connexion OK ");
  Serial.print("=> Addresse IP : ");
  Serial.print(WiFi.localIP());
}


void sendToDomoticz(String url){
  Serial.print("Connecting to ");
  Serial.println(host);
  Serial.print("Requesting URL: ");
  Serial.println(url);
  http.begin(host,port,url);
  int httpCode = http.GET();
    if (httpCode) {
      if (httpCode == 200) {
        String payload = http.getString();
        Serial.println("Domoticz response "); 
        Serial.println(payload);
      }
    }
  Serial.println("closing connection");
  http.end();
}
