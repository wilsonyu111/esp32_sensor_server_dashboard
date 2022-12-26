int waitTypes = 3;
int waited [] = {0,0,0};

void calcDelay(int delayTime)
{
  for (int i=0; i < waitTypes; i++)
  {
    waited [i] = waited[i] + delayTime;
  }
  delay(delayTime);
}

void loop() {
  // put your main code here, to run repeatedly:
  drd->loop();
  portal_server.handleClient();
  
  if (WiFi.status() != WL_CONNECTED)
  {
    wifiConnect(esp_config.ssid, esp_config.password);
  }
  
  calcDelay(500);

  if (waited[0] >= esp_config.sleepTimer * 1000 * 60)
  {
    waited[0] = 0;
    Serial.println("sending data");
    sendPost ();
  } 
}
