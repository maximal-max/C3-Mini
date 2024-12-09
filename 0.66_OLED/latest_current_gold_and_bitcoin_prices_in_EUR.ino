#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <time.h> 

// Define the server and storage
WebServer server(80);

// Define variables for storing credentials
String ssid = "";
String password = "";
String apiKey = "";
String apiUrl = "https://api.metalpriceapi.com/v1/latest?api_key=";
int fetchCycleTime = 8 * 60 * 60; // Fetch cycle time in seconds (default: 8 hours)

// OLED display
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 10, /* data=*/ 8, /* reset=*/ U8X8_PIN_NONE);

// Variables for holding prices and timing
float goldPrice = 0.0;
float bitcoinPrice = 0.0;
unsigned long lastUpdateTime = 0;
unsigned long lastDisplaySwitch = 0;
unsigned long lastDisplayUpdateTime = 0;
bool displayGold = true;

void setup() {
  Serial.begin(115200);
  SPIFFS.begin(true); // Initialize SPIFFS storage

  // Attempt to read credentials from SPIFFS
  ssid = readFile("/ssid.txt");
  password = readFile("/password.txt");
  apiKey = readFile("/apiKey.txt");
  String fetchCycleString = readFile("/fetchCycleTime.txt");
  if (fetchCycleString != "") {
    fetchCycleTime = fetchCycleString.toInt();
  }

  // Debugging: Print the values read from SPIFFS to the serial monitor
  Serial.println("Credentials from SPIFFS:");
  Serial.println("SSID: " + ssid);
  Serial.println("Password: " + password);
  Serial.println("API Key: " + apiKey);
  Serial.println("Fetch Cycle Time: " + String(fetchCycleTime) + " seconds");

  // Initialize OLED
  u8g2.begin();

  // Display a message about the connection status
  displayConnectionStatus();

  // If credentials are not set, start as Access Point
  if (ssid == "" || password == "" || apiKey == "") {
    startConfigMode();
  } else {
    // Connect to Wi-Fi with saved credentials
    connectToWiFi();
  }

  // Setup web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_GET, handleSave);
  server.begin();
  Serial.println("Web server started.");

  // Initialize time
  configTime(0, 0, "pool.ntp.org", "time.nist.gov"); // Set time servers
  delay(2000); // Wait for time to sync
}

void loop() {
  // Handle the web server requests
  server.handleClient();

  // Update prices at the user-defined cycle time
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= fetchCycleTime * 1000) {
    lastUpdateTime = currentTime;
    fetchPrices();
  }

  // Update the display every 10 seconds
  if (currentTime - lastDisplaySwitch >= 10000) {
    lastDisplaySwitch = currentTime;
    if (displayGold) {
      displayGoldPrice();
    } else {
      displayBitcoinPrice();
    }
    displayGold = !displayGold;
  }

  // Display the last update time every 60 seconds
  if (currentTime - lastDisplayUpdateTime >= 60000) {
    lastDisplayUpdateTime = currentTime;
    displayLastUpdateTime();
  }
}

void handleRoot() {
  String html = "<html><head>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; background-color: #f4f4f9; margin: 0; padding: 0; display: flex; justify-content: center; align-items: center; height: 100vh; }";
  html += "form { background: #fff; padding: 20px; border-radius: 10px; box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1); width: 300px; }";
  html += "input[type='text'], input[type='password'], input[type='number'] { width: 100%; padding: 10px; margin: 10px 0; border: 1px solid #ccc; border-radius: 5px; }";
  html += "input[type='checkbox'] { margin-right: 10px; }";
  html += "label { display: block; margin-bottom: 5px; font-weight: bold; }";
  html += "button { width: 100%; padding: 10px; background: #007bff; color: white; border: none; border-radius: 5px; font-size: 16px; cursor: pointer; }";
  html += "button:hover { background: #0056b3; }";
  html += "</style>";
  html += "<script>";
  html += "function togglePassword() {";
  html += "  var passwordInput = document.getElementById('password');";
  html += "  if (passwordInput.type === 'password') {";
  html += "    passwordInput.type = 'text';";
  html += "  } else {";
  html += "    passwordInput.type = 'password';";
  html += "  }";
  html += "}";
  html += "</script>";
  html += "</head><body>";
  html += "<form action='/save' method='get'>";
  html += "<h2>ESP32 WiFi Setup</h2>";
  html += "<label for='ssid'>SSID:</label>";
  html += "<input type='text' id='ssid' name='ssid' placeholder='Enter WiFi SSID'>";
  html += "<label for='password'>Password:</label>";
  html += "<input type='password' id='password' name='password' placeholder='Enter WiFi Password'>";
  html += "<div><input type='checkbox' onclick='togglePassword()'> Show Password</div>";
  html += "<label for='apiKey'>API Key:</label>";
  html += "<input type='text' id='apiKey' name='apiKey' placeholder='Enter API Key'>";
  html += "<label for='fetchCycleTime'>Fetch Cycle (minutes):</label>";
  html += "<input type='number' id='fetchCycleTime' name='fetchCycleTime' placeholder='Enter Fetch Cycle Time'>";
  html += "<button type='submit'>Save</button>";
  html += "</form>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleSave() {
  ssid = server.arg("ssid");
  password = server.arg("password");
  apiKey = server.arg("apiKey");
  String cycleTimeString = server.arg("fetchCycleTime");

  if (ssid != "" && password != "" && apiKey != "") {
    // Save credentials to SPIFFS
    writeFile("/ssid.txt", ssid);
    writeFile("/password.txt", password);
    writeFile("/apiKey.txt", apiKey);

    if (cycleTimeString != "") {
      int fetchCycleInMinutes = cycleTimeString.toInt();
      fetchCycleTime = fetchCycleInMinutes * 60; // Convert minutes to seconds
      writeFile("/fetchCycleTime.txt", String(fetchCycleTime));
    }

    // Reboot and connect to Wi-Fi
    connectToWiFi();
    server.send(200, "text/html", "<html><body><h1>Credentials Saved. Rebooting...</h1></body></html>");
    delay(2000);
    ESP.restart();
  } else {
    server.send(200, "text/html", "<html><body><h1>Invalid credentials. Please try again.</h1></body></html>");
  }
}

void writeFile(const char * path, String data) {
  File file = SPIFFS.open(path, FILE_WRITE);
  if (file) {
    file.print(data);
    file.close();
    Serial.println("File written: " + String(path));
  } else {
    Serial.println("Failed to open file for writing: " + String(path));
  }
}

String readFile(const char * path) {
  String data = "";
  File file = SPIFFS.open(path);
  if (file) {
    data = file.readString();
    file.close();
  }
  return data;
}

void clearStoredCredentials() {
  writeFile("/ssid.txt", "");
  writeFile("/password.txt", "");
  writeFile("/apiKey.txt", "");
  writeFile("/fetchCycleTime.txt", "");
  Serial.println("Stored credentials cleared.");
}

void startConfigMode() {
  clearStoredCredentials();
  WiFi.disconnect(); // Ensure no connection attempts are ongoing
  WiFi.softAP("ESP32_Config", "12345678");
  Serial.println("Configuration mode started. Connect to the AP with your phone.");
  Serial.println("Access Point IP: 192.168.4.1");

  // Clear any existing server state and restart
  server.stop();
  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_GET, handleSave);
  server.begin();
  Serial.println("Web server started in configuration mode.");

  // Update display
  displayMessage("AP Mode", "Config", "Required");
}

void connectToWiFi() {
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.println("Connecting to WiFi");
  displayMessage("Connect", "to Wi-Fi", "...");

  unsigned long startMillis = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - startMillis > 30000) { // Timeout after 30 seconds
      Serial.println("Wi-Fi connection failed.");
      startConfigMode(); // Reopen configuration mode if connection fails
      return;
    }
  }

  Serial.println("Wi-Fi connected.");
  displayMessage("Connected", "Fetching", "data...");
  fetchPrices(); // Fetch prices after successful connection
}

void displayMessage(String line1, String line2, String line3) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.setCursor(40, 35);
  u8g2.print(line1);
  u8g2.setCursor(40, 45);
  u8g2.print(line2);
  u8g2.setCursor(40, 55);
  u8g2.print(line3);
  u8g2.sendBuffer();
}

void displayConnectionStatus() {
  if (ssid == "" || password == "" || apiKey == "") {
    displayMessage("Configure!", "enter", "Data...");
  } else {
    displayMessage("Found", "Data,", "Connecting...");
  }
}

void fetchPrices() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String completeUrl = String(apiUrl) + apiKey;
    http.begin(completeUrl);
    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      Serial.println(payload);

      // Parse JSON
      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc, payload);
      if (!error) {
        float eurRate = doc["rates"]["EUR"];
        float xauRate = doc["rates"]["XAU"];
        float btcRate = doc["rates"]["BTC"];

        float goldPricePerOunceInUsd = 1.0 / xauRate;
        float goldPricePerGramInUsd = goldPricePerOunceInUsd / 31.1035;
        goldPrice = goldPricePerGramInUsd * eurRate;

        float btcInUsd = 1.0 / btcRate;
        bitcoinPrice = btcInUsd * eurRate;

        Serial.print("Gold Price (EUR): ");
        Serial.println(goldPrice);
        Serial.print("Bitcoin Price (EUR): ");
        Serial.println(bitcoinPrice);
      } else {
        Serial.println("JSON Parsing failed!");
      }
    } else {
      Serial.printf("Error: %d\n", httpCode);
      startConfigMode(); // Reopen the config page if HTTP request fails
    }
    http.end();
  }
}

void displayGoldPrice() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.setCursor(40, 35);
  u8g2.print("Gold: ");
  u8g2.setCursor(40, 45);
  u8g2.print(goldPrice);
  u8g2.setCursor(40, 55);
  u8g2.print("EUR/gram");
  u8g2.sendBuffer();
}

void displayBitcoinPrice() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.setCursor(40, 35);
  u8g2.print("Bitcoin: ");
  u8g2.setCursor(40, 45);
  u8g2.print(bitcoinPrice);
  u8g2.setCursor(40, 55);
  u8g2.print("EUR/BTC");
  u8g2.sendBuffer();
}

void displayLastUpdateTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  time(&now);

  char dateString[32];
  char timeString[32];
  
  // Separate date and time formats
  strftime(dateString, sizeof(dateString), "%d-%m-%y", &timeinfo);
  strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);

  // Display date and time separately
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.setCursor(40, 35);
  u8g2.print("Updated:");
  u8g2.setCursor(40, 45);
  u8g2.print(dateString); // Display the date
  u8g2.setCursor(40, 55);
  u8g2.print(timeString); // Display the time
  u8g2.sendBuffer();
  
  delay(10000); // Keep the last update display for 10 seconds
}

