// helper function that creates the web server and launch it
void launchWeb()
{
  Serial.println("starting web site");
  createWebServer();
  wifi_server.begin();
}

// make a web server that sends a html file
// and handles submission from client
void createWebServer()
{
  wifi_server.on("/", handleMain);
  wifi_server.on("/setting", handleSubmitValue);
  wifi_server.on("/style.css", sendWifiCssFile);
}

void handleMain()
{
  // File html_file = SPIFFS.open("/index.html", "r"); // working html submission template
  File html_file = SPIFFS.open("/main.html", "r");
  wifi_server.streamFile(html_file, "text/html");
  html_file.close();
}

void sendWifiCssFile()
{
  File css_file = SPIFFS.open("/style.css", "r");
  wifi_server.streamFile(css_file, "text/css");
  css_file.close();
}

unsigned long getTime() {
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, "pool.ntp.org");
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}

// accept values from client and write it to eeprom
void handleSubmitValue()
{
  int nextAddress = 0;
  char wifiName [25] = "";
  char wifiPass [25] = "";
  wifi_server.arg("wifiname").toCharArray(wifiName, sizeof(wifiName));
  wifi_server.arg("pass").toCharArray(wifiPass, sizeof(wifiPass));
  String serverIP = wifi_server.arg("serverIP");
  String locationName = wifi_server.arg("location_name");
  String destPort = wifi_server.arg("destination_port");
  String listenPort = wifi_server.arg("listening_port");
  String sleepTime = wifi_server.arg("sleep_timer");
  String sensorPresent = wifi_server.arg("installed_sensor");

  Serial.println(wifiName);
  Serial.println(wifiPass);
  Serial.println(serverIP);
  Serial.println(locationName);
  Serial.println(destPort);
  Serial.println(listenPort);
  Serial.println(sleepTime);
  Serial.println(sensorPresent);
  

  if (testWifi(wifiName, wifiPass))
  {
    wifi_server.arg("wifiname").toCharArray(esp_config.ssid, sizeof(esp_config.ssid));
    wifi_server.arg("pass").toCharArray(esp_config.password, sizeof(esp_config.password));
    wifi_server.arg("location_name").toCharArray(esp_config.locationName, sizeof(esp_config.locationName));
    wifi_server.arg("serverIP").toCharArray(esp_config.serverIP, sizeof(esp_config.serverIP));
    esp_config.destPort = wifi_server.arg("destination_port").toInt();
    esp_config.sleepTimer = wifi_server.arg("sleep_timer").toInt();
    esp_config.installed_light = wifi_server.arg("installed_sensor").toInt();
    esp_config.modifiedTime = getTime();
    
    putData (&esp_config, config_index); // store data into eeprom
    byte temp = contain_data;
    putData (&temp, reset_data_index); // set the data index to contain data
    wifi_server.send(200, "text/plain", "credential recorded!!");
    delay(200);
    Serial.println("restarting esp");
    ESP.restart(); // restart the esp to close all server and access point
  }
  else
  {
    wifi_server.send(200, "text/plain", "incorrect values!");
  }
}
