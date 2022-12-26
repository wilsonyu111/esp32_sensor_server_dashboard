template <typename Data>
void putData (Data *data, int putLocation)
{
  EEPROM.put(putLocation, *data);
  EEPROM.commit ();
}

template <typename Data>
void getData (Data *data, int getLocation)
{
  EEPROM.get(getLocation, *data);
}

void initializeStartupConfig()
{
  EEPROM.get(config_index, esp_config);
  Serial.println(esp_config.ssid);
  Serial.println(esp_config.password);
  Serial.println(esp_config.locationName);
  Serial.println(esp_config.serverIP);
  Serial.println(esp_config.installed_light);
  Serial.println(esp_config.destPort);
  Serial.println(esp_config.modifiedTime);
}
