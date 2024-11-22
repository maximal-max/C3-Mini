
Wi-Fi Enabled Price Display for Gold & Bitcoin

This project displays real-time prices for Gold and Bitcoin in EUR using an ESP32 microcontroller. 
The data is fetched from the MetalPriceAPI and displayed on an OLED screen. 
The device also synchronizes the current time via an NTP (Network Time Protocol) server. 
Prices are updated every 8 hours, and stored locally on the ESP32 for offline use.

Features:
- Real-time Gold & Bitcoin Prices in EUR using the MetalPriceAPI.
- OLED Display: Prices and last update time are shown alternately every 10 seconds.
- Time Synchronization: Keeps track of the current time using an NTP server.
- Local Storage: Stores the latest prices using SPIFFS (SPI Flash File System) for offline access.
- Wi-Fi Connectivity: Connects to your local Wi-Fi network to fetch data.

Components:
- ESP32 Microcontroller: Handles the Wi-Fi connection, API calls, and display logic.
- OLED Display (128x64): Shows Gold and Bitcoin prices alternately.
- SPIFFS: Used for local storage of the last known prices.
- MetalPriceAPI: The API service for fetching real-time price data.

Hardware Requirements:
- ESP32 Development Board
- OLED Display (128x64, I2C compatible)
- Connecting Wires

Libraries Used:
The following libraries are required to run the project. Install them using the Arduino Library Manager:
+------------------+----------------------------------------+---------------------+
| Library          | Purpose                                 | Publisher          |
+------------------+----------------------------------------+---------------------+
| WiFi             | Connects the ESP32 to Wi-Fi networks    | Arduino            |
| HTTPClient       | Handles HTTP requests for API calls     | Arduino            |
| ArduinoJson      | JSON parsing and serialization          | Benoît Blanchon    |
| Wire             | I2C communication with the OLED         | Arduino            |
| U8g2lib          | Controls the OLED display               | olikraus           |
| SPIFFS           | Local storage on ESP32                  | Arduino            |
| WiFiUdp          | UDP communication for NTP client        | Arduino            |
| NTPClient        | Fetches the current time via NTP        | Fabrice Weinberg   |
+------------------+----------------------------------------+---------------------+

Setup Instructions:

Hardware Setup:
1. Connect the OLED display to the ESP32 using I2C pins:
- SDA to GPIO 8
- SCL to GPIO 10
2. Ensure the ESP32 has access to your local Wi-Fi.
  
Software Setup:
1. Clone the Project:
git clone [Link](https://github.com/maximal-max/C3-mini_0.66_OLED_current_Gold_and_Bitcoin_prices.git)
2. Open the Project in the Arduino IDE.
3. Install Required Libraries via the Arduino Library Manager
4. Configure Wi-Fi and API:
   
   - Open the main.ino file.
   - Modify the following lines with your Wi-Fi SSID, password, and MetalPriceAPI key:

     const char* ssid = "YourWiFiSSID";
     const char* password = "YourWiFiPassword";
     const char* apiKey = "YourAPIKey";
     
6. Upload the Code to your ESP32.
   
Usage:
1. Power on the ESP32.
2. The device will attempt to connect to your specified Wi-Fi.
3. If successful, it will fetch the initial Gold and Bitcoin prices from the MetalPriceAPI.
4. The OLED display will:
   - Show the Gold price for 10 seconds.
   - Show the Bitcoin price for the next 10 seconds.
   - Display the last update time for 10 seconds.
5. The device will update prices every 8 hours automatically.
6. If Wi-Fi is disconnected, it will display an error message and use the last stored prices.

Troubleshooting:
1. Common Errors:
- "Storage Error": Failed to initialize SPIFFS. Ensure that SPIFFS is properly mounted.
- "Wi-Fi Disconnected": The device failed to connect to Wi-Fi. Check your SSID and password.
- "JSON Error": Failed to parse the API response. Verify that the API key is correct.
- "HTTP Error": Network error while trying to reach the API. Check your internet connection.
2. Tips:
- Check the Serial Monitor for debugging information.
- Ensure the OLED display connections are correct if there's no output.
- Update your API key if the service isn't responding.
  
Future Enhancements:
- Add additional cryptocurrency prices.
- Implement an interface to change settings via a web page.
- Add a button to manually refresh the price display.
- Integrate other precious metals like silver or platinum.
  
License:
This project is open-source and available under the MIT License.

Acknowledgments:
Special thanks to olikraus for the U8g2lib library for OLED displays.
Thanks to Benoît Blanchon for the ArduinoJson library.
Time synchronization powered by NTPClient by Fabrice Weinberg.
