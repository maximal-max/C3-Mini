#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>      // Include the Wire library for I2C communication
#include <U8g2lib.h>   // Include the U8g2 library for OLED display

#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 64 

//------- Replace the following! ------
const char* ssid = " ";        // Your Wi-Fi SSID
const char* password = " ";    // Your Wi-Fi password
const char* apiKey = " ";      // Your API key
const char* apiUrl = "https://api.metalpriceapi.com/v1/latest?apikey=";
//------- Replace the following! ------

// Setup U8g2 library with D1 (SCL) and D2 (SDA) pins for SSD1306 OLED
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 10, /* data=*/ 8, /* reset=*/ U8X8_PIN_NONE);

// Variables for holding prices
float goldPrice = 0.0;
float bitcoinPrice = 0.0;

// Timing variables
unsigned long lastUpdateTime = 0;
unsigned long lastDisplaySwitch = 0;
bool displayGold = true; // Toggle to show gold or Bitcoin

void setup() {

  Serial.begin(9600); // Initialize serial communication
  u8g2.begin();       // Initialize OLED display

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wi-Fi connected");

  // Fetch initial gold and Bitcoin prices
  fetchPrices();
}

void fetchPrices() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String completeUrl = String(apiUrl) + "?api_key=" + apiKey; // Complete URL with API key
    http.begin(completeUrl);
    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      Serial.println(payload);

      // Parse JSON
      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc, payload);
      if (!error) {
        // Extract relevant values
        float eurRate = doc["rates"]["EUR"];      // Get the EUR exchange rate
        float xauRate = doc["rates"]["XAU"];      // Gold rate in USD
        float btcRate = doc["rates"]["BTC"];      // Bitcoin rate in USD

        // Convert USD prices to EUR
        float goldPricePerOunceInUsd = 1.0 / xauRate;  // Gold price per ounce in USD
        float goldPricePerGramInUsd = goldPricePerOunceInUsd / 31.1035; // Convert to USD/gram
        goldPrice = goldPricePerGramInUsd * eurRate;  // Convert Gold price to EUR/gram
        
        float btcInUsd = 1.0 / btcRate;           // 1 Bitcoin in USD
        bitcoinPrice = btcInUsd * eurRate;        // Convert Bitcoin price to EUR
        

        Serial.print("Gold Price (EUR): ");
        Serial.println(goldPrice);
        Serial.print("Bitcoin Price (EUR): ");
        Serial.println(bitcoinPrice);
      } else {
        Serial.println("JSON Parsing failed!");
      }
    } else {
      Serial.printf("Error: %d\n", httpCode);
    }
    http.end();
  }
}

void displayGoldPrice() {
  
  u8g2.clearBuffer(); // Clear the display buffer
  u8g2.setFont(u8g2_font_6x12_tf); // Choose a font
  u8g2.setCursor(40, 35);  // Set cursor position
  u8g2.print("Gold: "); 
  u8g2.setCursor(40, 45);  // Set cursor position
  u8g2.print(goldPrice); 
  u8g2.setCursor(40, 55);
  u8g2.print("EUR/gram");
  u8g2.sendBuffer(); // Send the display buffer to the OLED
}

void displayBitcoinPrice() {

  u8g2.clearBuffer(); // Clear the display buffer
  u8g2.setFont(u8g2_font_6x12_tf); // Choose a font
  u8g2.setCursor(40, 35);  // Set cursor position
  u8g2.print("Bitcoin: "); 
  u8g2.setCursor(40, 45);  // Set cursor position
  u8g2.print(bitcoinPrice); 
  u8g2.setCursor(40, 55);
  u8g2.print("EUR/BTC");
  u8g2.sendBuffer(); // Send the display buffer to the OLED
}

void loop() {
  unsigned long currentTime = millis();

  // Update prices every 8 hours
  if (currentTime - lastUpdateTime >= 8 * 60 * 60 * 1000) {
    lastUpdateTime = currentTime;
    fetchPrices();
  }

  // Switch display every 10 seconds
  if (currentTime - lastDisplaySwitch >= 10000) {
    lastDisplaySwitch = currentTime;
    if (displayGold) {
      displayGoldPrice();
    } else {
      displayBitcoinPrice();
    }
    displayGold = !displayGold; // Toggle between gold and Bitcoin
  }
}
