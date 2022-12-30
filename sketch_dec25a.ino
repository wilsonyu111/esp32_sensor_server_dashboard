// this is an esp32 version of the sensor server
// ported over from the esp8266 server

#if defined(ESP32)
  #define USE_SPIFFS            true
  #define ESP_DRD_USE_EEPROM    true
#else  
  #error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.  
#endif
#include <Arduino.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ESP_DoubleResetDetector.h>
#include "EEPROM.h"
#include "SPIFFS.h"


#define DRD_TIMEOUT 10 // double reset timeout
#define DRD_ADDRESS 0  // double reset address
#define baud_rate               115200
#define eeprom_size             2048
#define RESTART_VALUE           199       // software restart byte indicator
#define contain_data            123       // contain data byte indicator
#define restart_bit_index       1         // restart status, if 0 means no software reset, 1 means software reset. This prevents dobule reset button logic from running
#define reset_data_index        2         // if sensor does not have any data to start with or a reset data is triggered. (other values means no data, 255 means it have data)
#define config_index            3         // 100 bytes
#define DHTPIN                  5         // Digital pin connected to the DHT sensor, D1 pin on esp 8266
#define DHTTYPE                 DHT11     // DHT 11
#define LIGHT_INPUT_PIN         4
#define WIFI_PORT               80
#define PORTAL_PORT             3000
#define HTTP_TIMEOUT            15000     // the library defaults to 5000
#define DELAY_BEFORE_RESTART    3500      // wait for response to be sent before restarting

const String OPEN_BRACKET = "{";
const String CLOSE_BRACKET = "}";
const String QUOTE_MARK = "\"";
const String COLON = ":";
const String COMMA = ",";


struct startConfig{
  char ssid [25] = "";
  char password[25] = "";
  char locationName[4] = "";
  char serverIP[16] = "";
  bool installed_light = false;
  int destPort = -1;
  int sleepTimer = 1;
  int modifiedTime = -1;
  
};

bool resetWifi = false;
bool APactive = false; // if the AP is active make sure wifi cannot be connected
DoubleResetDetector* drd;
WebServer wifi_server(WIFI_PORT); // used for initializing wifi, and other settings
WebServer portal_server(PORTAL_PORT); // used for listening to portal server object
startConfig esp_config = {};
WiFiClient client;     // for invoking wifi connection
//DHT dht(5, DHT11);

void startSPIFFS()
{
  // start SPIFFS for reading HTML and CSS file
  // the true flag is used when the file system is not formatted
  // once it's formatted this flag will not be needed
  if (SPIFFS.begin(true)) 
  {
    Serial.println("opened SPIFFS");
  }
  else
  {
    Serial.println("failed to open SPIFFS");
  }
}
void startEEPROM()
{
  if (EEPROM.begin(eeprom_size)) 
  {
    Serial.println("opened EEPROM");
  }
  else
  {
    Serial.println("failed to open EEPROM");
  }
}

void checkDoubleReset()
{
  if (drd->detectDoubleReset()) 
  { 
    Serial.println("double resetting");
    resetWifi = true;
  }
  else
  {
    Serial.println("no double reset");
  }
}

void printDebug()
{
  Serial.print("ssid: ");
  Serial.println(esp_config.ssid);
  Serial.print("password: ");
  Serial.println(esp_config.password);
  Serial.print("location: ");
  Serial.println(esp_config.locationName);
  Serial.print("server IP: ");
  Serial.println(esp_config.serverIP);
  Serial.print("installed light?: ");
  Serial.println(esp_config.installed_light);
  Serial.print("server port: ");
  Serial.println(esp_config.destPort);
  Serial.print("sleep timer: ");
  Serial.println(esp_config.sleepTimer);
  Serial.print("modified time: ");
  Serial.println(esp_config.modifiedTime);
}

void setup() {
  Serial.begin(baud_rate);        // initialize baud rate
  drd = new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);
  startEEPROM();
  startSPIFFS();
  checkDoubleReset(); 
  
  if (EEPROM.read(reset_data_index) != contain_data || (resetWifi && EEPROM.read(restart_bit_index) == 0))
  {
    // start the web server and AP to get the credentials
    setupAP();   // launch AP for device to connect into
    launchWeb(); // launch wifi portal webserver
    Serial.println("waiting for credentials");
    Serial.println("");
    while ((WiFi.status() != WL_CONNECTED) && APactive)
    {

      Serial.print(".");
      delay(200);
      wifi_server.handleClient();
    }
    EEPROM.write(reset_data_index, contain_data);
  }
  // credential already exist
  // read the info from EEPROM
  else if (EEPROM.read(restart_bit_index) != 0)
  {
    EEPROM.write(restart_bit_index, 0);
    EEPROM.commit();
  }
  initializeStartupConfig();
//  printDebug();
  wifiConnect(esp_config.ssid, esp_config.password);
  launchDashboard();
  
}
