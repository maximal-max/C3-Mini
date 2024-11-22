#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <SPIFFS.h>       // For storing data locally
#include <WiFiUdp.h>      // For NTP Client
#include <NTPClient.h>    // NTP Client to get the current time

//------- Replace the following! ------
const char* ssid = " ";        // Your Wi-Fi SSID
const char* password = " ";    // Your Wi-Fi password
const char* apiKey = " ";      // Your API key
const char* apiUrl = "https://api.metalpriceapi.com/v1/latest?apikey=";
//------- Replace the following! ------


// OLED Display Configuration
#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 64 

// OLED Display (I2C Configuration)
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 10, /* data=*/ 8, /* reset=*/ U8X8_PIN_NONE);

// Variables to hold gold and Bitcoin prices
float goldPrice = 0.0;
float bitcoinPrice = 0.0;

// NTP Client for time synchronization
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000); // UTC+1 (3600 sec offset), 60 sec update

// Timing variables
unsigned long lastUpdateTime = 0;             // Last time the prices were updated
unsigned long lastDisplaySwitch = 0;          // Last time the price display switched
unsigned long lastUpdateDisplayTime = 0;      // Last time the "Last Update" was shown

bool displayGold = true;                      // Toggle between showing gold or Bitcoin
bool showLastUpdateMessage = false;           // Flag to control showing the last update message
String lastUpdateTimeString;                  // String to store last update time

void setup() {
  Serial.begin(9600);
  u8g2.begin();

  // Initialize SPIFFS for local data storage
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed. Formatting...");
    if (!SPIFFS.format()) {
      Serial.println("SPIFFS Format Failed!");
      displayErrorMessage("Storage Error");
      return;
    } else {
      Serial.println("SPIFFS Formatted. Trying to mount again...");
      if (!SPIFFS.begin()) {
        Serial.println("SPIFFS Mount Failed after formatting.");
        displayErrorMessage("Storage Error");
        return;
      }
    }
  } else {
    Serial.println("SPIFFS Mounted Successfully.");
  }

  // Connect to Wi-Fi
  connectToWiFi();

  // Initialize NTP client for time synchronization
  timeClient.begin();
  timeClient.update();

  // Load stored prices from SPIFFS if available
  loadStoredPrices();

  // Create a FreeRTOS task for fetching prices periodically
  xTaskCreate(
    fetchPricesTask,   // Task function
    "Price Fetch Task",// Name of the task
    4096,              // Stack size (in bytes)
    NULL,              // Parameter passed to the task
    1,                 // Priority of the task (1 is lowest priority)
    NULL               // Task handle
  );
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Wi-Fi connected");
}

void fetchPricesTask(void *parameter) {
  while (true) {
    if (WiFi.status() == WL_CONNECTED) {
      fetchPrices();
    } else {
      Serial.println("Wi-Fi Disconnected. Retrying...");
      connectToWiFi();
    }

    // Use FreeRTOS delay for non-blocking wait (8 hours)
    vTaskDelay(8 * 60 * 60 * 1000 / portTICK_PERIOD_MS); // 8 hours
  }
}

void fetchPrices() {
  HTTPClient http;
  String completeUrl = String(apiUrl) + "?api_key=" + apiKey;
  int maxRetries = 3;
  int attempts = 0;
  int httpCode = 0;

  while (attempts < maxRetries && WiFi.status() == WL_CONNECTED) {
    http.begin(completeUrl);
    httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      Serial.println(payload);

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

        timeClient.update();
        lastUpdateTimeString = timeClient.getFormattedTime();

        storePrices();
        Serial.println("Prices updated successfully.");
        http.end();
        return;
      } else {
        Serial.println("JSON Parsing failed!");
        displayErrorMessage("JSON Error");
      }
    } else {
      Serial.printf("HTTP Error: %d\n", httpCode);
      displayErrorMessage("HTTP Error");
    }

    http.end();
    attempts++;
    vTaskDelay(2000 / portTICK_PERIOD_MS); // Wait 2 seconds before retrying
  }

  loadStoredPrices();
  displayErrorMessage("Failed to update; using stored data");
}


void displayGoldPrice() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.setCursor(40, 35);
  u8g2.print("Gold:");
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
  u8g2.print("Bitcoin:");
  u8g2.setCursor(40, 45);
  u8g2.print(bitcoinPrice);
  u8g2.setCursor(40, 55);
  u8g2.print("EUR/BTC");
  u8g2.sendBuffer();
}

void displayUpdateTime() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.setCursor(40, 35);
  u8g2.print("Last UPD:");
  u8g2.setCursor(40, 45);
  u8g2.print("Today:");
  u8g2.setCursor(40, 55);
  u8g2.print(lastUpdateTimeString);
  u8g2.sendBuffer();
}

void displayErrorMessage(const char* message) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);

  // Display "Error:" in the first line
  u8g2.setCursor(40, 35);
  u8g2.print("Error:");

  // Display the message in the second line, truncated if necessary
  u8g2.setCursor(40, 45);
  String shortMessage = String(message);
  if (shortMessage.length() > 12) {
    shortMessage = shortMessage.substring(0, 12);  // Truncate to 12 characters
  }
  u8g2.print(shortMessage);

  u8g2.sendBuffer();
}

void storePrices() {
  File file = SPIFFS.open("/prices.txt", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    displayErrorMessage("Storage Error");
    return;
  }
  
  file.print(goldPrice);
  file.print(",");
  file.println(bitcoinPrice);
  file.close();
  Serial.println("Prices stored successfully.");
}

void loadStoredPrices() {
  File file = SPIFFS.open("/prices.txt", FILE_READ);
  if (!file) {
    Serial.println("No stored data found");
    return;
  }
  
  String data = file.readString();
  file.close();
  
  int separatorIndex = data.indexOf(',');
  if (separatorIndex != -1) {
    goldPrice = data.substring(0, separatorIndex).toFloat();
    bitcoinPrice = data.substring(separatorIndex + 1).toFloat();
    
    Serial.println("Loaded prices from storage:");
    Serial.print("Gold: ");
    Serial.println(goldPrice);
    Serial.print("Bitcoin: ");
    Serial.println(bitcoinPrice);
  }
}

void loop() {
  unsigned long currentTime = millis();

  // Every 60 seconds, set the flag to show the "Last Update" message
  if (currentTime - lastUpdateDisplayTime >= 60000) {
    lastUpdateDisplayTime = currentTime;
    showLastUpdateMessage = true;
  }

  // Manage the price display (gold/bitcoin) every 10 seconds
  if (currentTime - lastDisplaySwitch >= 10000) {
    lastDisplaySwitch = currentTime;

    // Display gold or bitcoin based on the toggle
    if (displayGold) {
      displayGoldPrice();
    } else {
      displayBitcoinPrice();
    }

    displayGold = !displayGold; // Toggle display mode
  }

  // Check if we need to show the "Last Update" message
  if (showLastUpdateMessage) {
    displayUpdateTime();  // Show the last update time on the display
    delay(10000);         // Keep it visible for 10 seconds
    showLastUpdateMessage = false;  // Reset the flag
  }
}
