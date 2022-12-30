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
}

void espRestart()
{
  delay(DELAY_BEFORE_RESTART);
  EEPROM.write(restart_bit_index, RESTART_VALUE);
  EEPROM.commit();
  ESP.restart();  
}
