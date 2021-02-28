#define FORMAT_SPIFFS_IF_FAILED true

const char* ssid = "sparkpad-test";
const char* pass = "123";

IPAddress localIP(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

String configPath = "/WiFiSettings.json";

String sendHtml()
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Sparkpad Web Server</h1>\n";
  ptr += "<h3>Please enter your Wifi Network name and password</h3>\n";
  ptr += "<form action='store'>";
  ptr += "<input type='text' name='name' placeholder='Wifi Network' />";
  ptr += "<input type='text' name='pass' placeholder=' Wifi Password' />";
  ptr += "<input type='submit' name='submit' />";
  ptr += "</form>";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

String sendConfigHtml(String name, String pass)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Sparkpad Config</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Sparkpad Config Edditor</h1>\n";
  ptr += "<h3>Update your settings from here</h3>\n";
  ptr += "<form action='configSet'>";
  ptr += "<input type='text' name='name' value='" + name + "' />";
  ptr += "<input type='password' name='pass' value='" + pass + "' />";
  ptr += "<input type='submit' name='submit' />";
  ptr += "</form>";
  ptr += "<br /><br />";
  ptr += "<a href='/reset'>Reset esp32 config</a>";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

String sendHomePage()
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Sparkpad Config</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Sparkpad</h1>\n";
  ptr += "<h3>Select a place to go below</h3>\n";
  ptr += "<br /><br />";
  ptr += "<a href='/reset'>Reset config</a>    ";
  ptr += "<a href='/config'>Edit config</a>";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

void handleGet()
{
  server.send(200, "text/plain", "Thanks, please bear with us");
  // store the name and pass in strings to be used
  String name = server.arg(0);
  String pass = server.arg(1);

  DynamicJsonDocument doc(150);
  File configFile = SPIFFS.open(configPath, "w");

  doc["name"] = name;
  doc["pass"] = pass;

  serializeJson(doc, configFile);
  serializeJson(doc, Serial);
  configFile.close();
  Serial.println("\nSaved Config settings... Rebooting in 3 seconds");
  delay(3000);
  ESP.restart();
}

void handleNotFound()
{
  server.send(404, "text/plain", "You are in the wrong place buddy");
}

void handleOnConnect()
{
    server.send(200, "text/html", sendHtml());
    Serial.println("Client Connected");
}

void handleOnConnectSSID()
{
  server.send(200, "text/html", sendHomePage());
}

void handleReset()
{
  server.send(200, "text/plain", "Resetting Config, please bear with us.");
  Serial.println("removing config.....");
  SPIFFS.remove(configPath);
  Serial.println("Config removed rebooting");
  delay(3000);
  ESP.restart();
}

void handleconfigGet()
{
  File configFile = SPIFFS.open(configPath, "r");

  StaticJsonDocument<150> doc;

  DeserializationError error = deserializeJson(doc, configFile);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  String name = doc["name"];
  String pass = doc["pass"];

  server.send(200, "text/html", sendConfigHtml(name, pass));
}

void handleConfigSet()
{
  SPIFFS.remove(configPath);

  String name = server.arg(0);
  String pass = server.arg(1);

  DynamicJsonDocument doc(150);
  File configFile = SPIFFS.open(configPath, "w");

  doc["name"] = name;
  doc["pass"] = pass;

  serializeJson(doc, configFile);
  serializeJson(doc, Serial);
  configFile.close();
  Serial.println("\nSaved Config settings... Rebooting in 3 seconds");
  delay(3000);
  ESP.restart();
}

bool readConfig(String fileName)
{
  if(SPIFFS.exists(fileName)) {
    File file = SPIFFS.open(fileName, "r");
    DynamicJsonDocument doc(150);
    deserializeJson(doc, file);
    serializeJson(doc, Serial);
    file.close();
    return true;
  } else {
    Serial.println("File not Found");
    return false;
  }
}

void setupEndpoints(String type)
{
  if (type == "AP")
  {
    server.on("/", handleOnConnect);
    server.on("/store", handleGet);
    server.onNotFound(handleNotFound);
  }
  else if (type == "ST")
  {
    server.on("/", handleOnConnectSSID);
    server.on("/reset", handleReset);
    server.on("/config", handleconfigGet);
    server.on("/configSet", handleConfigSet);
    server.onNotFound(handleNotFound);
  }
}

void bootAP(){
  // WIFI
  Serial.println("Starting Wifi Server (AP)");
  WiFi.softAP(ssid);
  delay(100);

  IPAddress ip = WiFi.softAPIP();
  Serial.print("AP IP Address: ");
  Serial.println(ip);

  setupEndpoints("AP");

  server.begin();
  Serial.println("Server started!");
}

void bootSSID(){
  Serial.println("Connecting to WiFi using credentials from settings");

  File configFile = SPIFFS.open(configPath, "r");

  StaticJsonDocument<150> doc;

  DeserializationError error = deserializeJson(doc, configFile);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  const char* name = doc["name"];
  const char* pass = doc["pass"];

  WiFi.begin(name, pass);

  int i = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Establishing connection to WiFi..");
    i++;
    if(i > 12) {
      Serial.println("Cannot connect to network try again!");
      SPIFFS.remove(configPath);
      ESP.restart();
    }
  }
 
  Serial.println("Connected to network");

  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());

  setupEndpoints("ST");

  server.begin();
  Serial.println("Server started!");
}

void WIFI_setup() {

    // SPIFFS
  Serial.println("Starting Spiffs");
   if(!SPIFFS.begin(true)) {
     Serial.println("Error mounting filesystem");
     return;
   }

   //SPIFFS.remove(configPath);

  if(readConfig(configPath)) {
    bootSSID();
  } else {
    bootAP();
  }
}

void WIFI_loop() {

    server.handleClient();

    // delay(10000);
    // Serial.println("WiFi Status: " + WiFi.status());
}