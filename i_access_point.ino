void setupAP()
{
  APactive = true;
  Serial.println("starting AP");
  WiFi.softAP("esp", ""); // launch access point
}
