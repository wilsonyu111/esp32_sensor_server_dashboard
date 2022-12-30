void launchDashboard()
{
  Serial.println("starting web site");
  createWebPortal();
  portal_server.begin();
}

// make a web server that sends a html file
// and handles submission from client
void createWebPortal()
{
  portal_server.on("/online", checkServerOnline);
  portal_server.on("/serverStat", sendSensorStatus);
  portal_server.on("/changeStat", modifyStat);
  portal_server.on("/restart", restartESP);
  portal_server.on("/timestampCheck", checkTime);
}

void checkTime()
{
  if (portal_server.arg("time_stamp").toInt() == esp_config.modifiedTime)
  {
    portal_server.send(304, "text/plain", "Not modified");
  }

  portal_server.send(200, "application/json", getConfigJSON().c_str());
}

void checkServerOnline()
{
  portal_server.send(200, "text/plain", "online");
}

void sendSensorStatus()
{
  portal_server.send(200, "application/json", getConfigJSON().c_str());
}

String getConfigJSON()
{
  
  String jsonMessage = OPEN_BRACKET;

  jsonMessage += QUOTE_MARK + "sensor_id" + QUOTE_MARK + COLON + QUOTE_MARK + WiFi.macAddress() + QUOTE_MARK + COMMA;
  jsonMessage += QUOTE_MARK + "location_name" + QUOTE_MARK + COLON + QUOTE_MARK + esp_config.locationName + QUOTE_MARK + COMMA;
  jsonMessage += QUOTE_MARK + "sensor_IP" + QUOTE_MARK + COLON + QUOTE_MARK + WiFi.localIP().toString() + QUOTE_MARK + COMMA;
  jsonMessage += QUOTE_MARK + "server_ip" + QUOTE_MARK + COLON + QUOTE_MARK + esp_config.serverIP + QUOTE_MARK + COMMA;
  jsonMessage += QUOTE_MARK + "listen_port" + QUOTE_MARK + COLON + QUOTE_MARK + String(PORTAL_PORT) + QUOTE_MARK + COMMA;
  jsonMessage += QUOTE_MARK + "modified_time" + QUOTE_MARK + COLON + QUOTE_MARK + String(esp_config.modifiedTime) + QUOTE_MARK + COMMA;
  jsonMessage += QUOTE_MARK + "ssid" + QUOTE_MARK + COLON + QUOTE_MARK + esp_config.ssid + QUOTE_MARK + COMMA;
  jsonMessage += QUOTE_MARK + "installed_light" + QUOTE_MARK + COLON + QUOTE_MARK + String(esp_config.installed_light) + QUOTE_MARK + COMMA;  
  jsonMessage += QUOTE_MARK + "dest_port" + QUOTE_MARK + COLON + QUOTE_MARK + String(esp_config.destPort) + QUOTE_MARK + COMMA;
  jsonMessage += QUOTE_MARK + "sleep_timer" + QUOTE_MARK + COLON + QUOTE_MARK + String(esp_config.sleepTimer) + QUOTE_MARK ;
  
  
  jsonMessage += CLOSE_BRACKET;
  return jsonMessage;
}

void modifyStat()
{

  if (portal_server.hasArg("serverIP"))
  {
    Serial.println(portal_server.arg("serverIP"));
    portal_server.arg("serverIP").toCharArray(esp_config.serverIP, sizeof(esp_config.serverIP));
  }
  if (portal_server.hasArg("location_name"))
  {
    Serial.println(portal_server.arg("location_name"));
    portal_server.arg("location_name").toCharArray(esp_config.locationName, sizeof(esp_config.locationName));
  }
  if (portal_server.hasArg("destination_port"))
  {
    Serial.println(portal_server.arg("destination_port"));
    esp_config.destPort = portal_server.arg("destination_port").toInt();
  }
  if (portal_server.hasArg("sleep_timer") && portal_server.arg("sleep_timer").toInt() >= 1)
  {
    Serial.println(portal_server.arg("sleep_timer"));
    esp_config.sleepTimer = portal_server.arg("sleep_timer").toInt();
  }
  if (portal_server.hasArg("installed_sensor"))
  {
    Serial.println(portal_server.arg("installed_sensor"));
    esp_config.installed_light = portal_server.arg("installed_sensor").toInt();
  }

  if (portal_server.hasArg("modified_time"))
  {
    Serial.println(portal_server.arg("modified_time"));
    esp_config.modifiedTime = portal_server.arg("modified_time").toInt();
  }

  putData (&esp_config, config_index); // store data into eeprom
  
  portal_server.send(200, "text/plain", "got values");
  espRestart(); // restart esp and update restart tarcking bit
}

void restartESP()
{
  portal_server.send(200, "text/plain", "restarting");
  espRestart();// restart esp and update restart tarcking bit
}
