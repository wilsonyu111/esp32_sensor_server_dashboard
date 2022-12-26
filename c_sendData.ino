String tempType = "F";

void sendPost ()
{ 
  String server_path = "http://" + String(esp_config.serverIP) + ":" + String(esp_config.destPort) + "/sendtoserver";
  Serial.println(server_path);
  HTTPClient http; // make http object
  http.begin(client, server_path); // connect to the server ip address
  http.addHeader("Content-Type", "application/json");
  String post_data = initialJSON();
  int httpResponseCode = http.POST(post_data);
  Serial.println (post_data);
  Serial.println ("send post, response: " + (String)httpResponseCode);
  http.end();
}

String initialJSON()
{
  
  String jsonMessage = OPEN_BRACKET;
  jsonMessage += QUOTE_MARK + "data" + QUOTE_MARK + COLON + dataJSON() +COMMA;
  jsonMessage += QUOTE_MARK + "config" + QUOTE_MARK + COLON + configJSON();
  jsonMessage += CLOSE_BRACKET;
  return jsonMessage;
}

String configJSON()
{
  String jsonMessage = OPEN_BRACKET;
  jsonMessage += QUOTE_MARK + "sensor_id" + QUOTE_MARK + COLON + QUOTE_MARK + WiFi.macAddress() + QUOTE_MARK + COMMA;
  jsonMessage += QUOTE_MARK + "location_name" + QUOTE_MARK + COLON + QUOTE_MARK + esp_config.locationName + QUOTE_MARK + COMMA;
  jsonMessage += QUOTE_MARK + "sensor_ip" + QUOTE_MARK + COLON + QUOTE_MARK + WiFi.localIP().toString() + QUOTE_MARK + COMMA;
  jsonMessage += QUOTE_MARK + "listen_port" + QUOTE_MARK + COLON + QUOTE_MARK + String(WIFI_PORT) + QUOTE_MARK;
  jsonMessage += CLOSE_BRACKET;
  return jsonMessage;
}

String dataJSON()
{
  String jsonMessage = OPEN_BRACKET;
  jsonMessage += QUOTE_MARK + "temperature" + QUOTE_MARK + COLON + QUOTE_MARK + getTemp () + QUOTE_MARK + COMMA;  
  jsonMessage += QUOTE_MARK + "humidity" + QUOTE_MARK + COLON + QUOTE_MARK + getHumd () + QUOTE_MARK + COMMA;
  jsonMessage += QUOTE_MARK + "light_status" + QUOTE_MARK + COLON + QUOTE_MARK + getLightStat() + QUOTE_MARK;
  jsonMessage += CLOSE_BRACKET;
  return jsonMessage;
}

String getHumd ()// get hudmidity 
{
//  float newH = dht.readHumidity();
//  // if humidity read failed, don't change h value 
//  if (isnan(newH))// if 
//  {
//    return "-123";
//  }
//
//  return (String) newH;

  return "11.2%";
}

String getTemp ()
{
//  float newT;
//  
//  if (tempType == "C")// if temperture type is C get temp in celsius
//  {
//    newT = dht.readTemperature();
//  }
//  else // if temperture type is F get temperature as Farenheit
//  {
//    newT = dht.readTemperature(true);
//  }
//  
//  if (isnan(newT))
//  {
//    return "-321";
//  }
//  
//  return (String)newT;
  return "99.1";
}

/*
 * The get light function returns "off" state when the pin is high
 * otherwise it'll return "on" state when the pin is low
 * this can be changed by making the pin pulling low
 * during the startup state
 * The input pin is D2
 */
String getLightStat()
{
  if (!esp_config.installed_light)
  {
    return "N/A";
  }
  int light_sensor_state = digitalRead(LIGHT_INPUT_PIN);
  if (light_sensor_state)
  {
    return "off";
  }
  else
  {
    return "on";
  }
}
