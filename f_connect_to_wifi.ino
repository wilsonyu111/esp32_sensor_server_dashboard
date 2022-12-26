bool wifiConnect(const char* ssid, const char* password)
{
  // Connect to Wi-Fi
  if (ssid != "" && password != "")
  {
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi");
    int attempt = 500; 
    while (WiFi.status() != WL_CONNECTED && attempt > 0) // loop that wait for wifi to connect before moving to reading sensors
    {
  
      delay(800);
      Serial.print(".");
      delay(800);
      attempt --;
    }
    Serial.println("IP address: ");
    Serial.println( WiFi.localIP());
    return true;
  }
  return false;
  
}

bool testWifi(const char* ssid, const char* password)
{
  Serial.println("testing wifi");
  int attempts = 120;
  WiFi.begin(ssid, password);

  // try to connect to the wifi for 20 times
  // if it fails return false and don't store the credential
  while (WiFi.status() != WL_CONNECTED && attempts > 0) // loop that wait for wifi to connect before moving to reading sensors
  {
    delay(200);
    Serial.print(".");
    delay(200);
    attempts--;
  }

  if (attempts == 0)
  {
    Serial.println("attempt timed out");
    return false;
  }
  else
  {
    return true;
  }
}
