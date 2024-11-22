
# Wi-Fi Enabled Price Display for Gold & Bitcoin

This project displays real-time prices for Gold and Bitcoin in EUR using an ESP32 microcontroller. The data is fetched from the MetalPriceAPI and shown on an OLED screen. The device also synchronizes the current time via an NTP (Network Time Protocol) server. Prices are updated every 8 hours, and the most recent data is stored locally on the ESP32 for offline use.

## Features:
- **Real-time Gold & Bitcoin Prices in EUR**: Prices fetched using the MetalPriceAPI.
- **OLED Display**: Displays Gold and Bitcoin prices alternately every 10 seconds.
- **Time Synchronization**: Syncs the current time using an NTP server, displaying the last update time.
- **Local Storage**: Stores the latest prices in SPIFFS for offline access.
- **Wi-Fi Connectivity**: Connects to a local Wi-Fi network to fetch price data.

## Components:
- **ESP32 Microcontroller**: Manages Wi-Fi connections, API requests, and display.
- **OLED Display (128x64)**: Shows Gold and Bitcoin prices alternately.
- **SPIFFS**: Uses the SPI Flash File System for storing the latest known prices.
- **MetalPriceAPI**: Provides real-time gold and Bitcoin price data.

## Hardware Requirements:
- ESP32 Development Board
- OLED Display (128x64, I2C compatible)
- Connecting Wires

## Libraries Used:
The following libraries are required to run the project. Install them using the Arduino Library Manager:
- **WiFi** by Arduino: Connects the ESP32 to Wi-Fi networks
- **HTTPClient** by Arduino: Handles HTTP requests for API calls
- **ArduinoJson** by Benoît Blanchon: JSON parsing and serialization
- **Wire** by Arduino: I2C communication with the OLED
- **U8g2lib** by olikraus: Controls the OLED display
- **SPIFFS** by Arduino: Local storage on ESP32
- **WiFiUdp** by Arduino: UDP communication for NTP client
- **NTPClient** by Fabrice Weinberg: Fetches the current time via NTP

## Setup Instructions:

### Hardware Setup:
1. **Connect the OLED display to the ESP32 using I2C pins**:
   - SDA to GPIO 8
   - SCL to GPIO 10
2. Ensure the ESP32 has access to your local Wi-Fi.

### Software Setup:
1. Clone the Project:
   ```bash
   git clone [Link](https://github.com/maximal-max/C3-mini_0.66_OLED_current_Gold_and_Bitcoin_prices.git)

2. **Open the Project in the Arduino IDE**.
   - Launch the Arduino IDE and go to `File > Open...`
   - Select the project folder you just cloned and open the `main.ino` file.

3. **Install Required Libraries via the Arduino Library Manager**:
   - Go to `Sketch > Include Library > Manage Libraries...`
   - Search for and install the following libraries:
     - **WiFi** by Arduino
     - **HTTPClient** by Arduino
     - **ArduinoJson** by Benoît Blanchon
     - **Wire** by Arduino
     - **U8g2lib** by olikraus
     - **SPIFFS** by Arduino
     - **WiFiUdp** by Arduino
     - **NTPClient** by Fabrice Weinberg

3. **Configure Wi-Fi and API**:
   - Open the `main.ino` file.
   - Modify the following lines with your Wi-Fi SSID, password, and MetalPriceAPI key:
     ```cpp
     const char* ssid = "YourWiFiSSID";    
     const char* password = "YourWiFiPassword";
     const char* apiKey = "YourAPIKey";
     ```
   - **API Key Setup**: To get your API key, you need to sign up for an account at [MetalPriceAPI](https://metalpriceapi.com/). After creating an account, you will receive an API key that you can use in your project.


5. **Upload the Code to your ESP32**:
   - Select your ESP32 board from `Tools > Board > ESP32 Dev Module` (or another appropriate board).
   - Choose the correct port from `Tools > Port`.
   - Click the upload button in the Arduino IDE to upload the code to your ESP32.

## Usage:
1. **Power on the ESP32**.
2. The device will attempt to connect to your specified Wi-Fi network.
3. Once connected, it will fetch the initial Gold and Bitcoin prices from the MetalPriceAPI.
4. The OLED display will:
   - Show the **Gold price** for 10 seconds.
   - Show the **Bitcoin price** for the next 10 seconds.
   - Display the **last update time** for 10 seconds.
5. The device will update prices every 8 hours automatically.
6. If Wi-Fi is disconnected, the device will display an error message and use the last stored prices from SPIFFS.

## Troubleshooting:
1. **Common Errors**:
   - **"Storage Error"**: Failed to initialize SPIFFS. Ensure that SPIFFS is properly mounted.
   - **"Wi-Fi Disconnected"**: The device failed to connect to Wi-Fi. Check your SSID and password.
   - **"JSON Error"**: Failed to parse the API response. Verify that the API key is correct.
   - **"HTTP Error"**: Network error while trying to reach the API. Check your internet connection.

2. **Tips**:
   - Check the **Serial Monitor** for debugging information.
   - Ensure the **OLED display connections** are correct if there's no output.
   - **Update your API key** if the service isn't responding.

## Future Enhancements:
- Add support for additional cryptocurrency prices.
- Implement a web interface for changing settings.
- Add a button to manually refresh the price display.
- Integrate other precious metals like silver or platinum.

## License:
This project is open-source and available under the **MIT License**.

## Acknowledgments:
- Special thanks to **olikraus** for the **U8g2lib** library for OLED displays.
- Thanks to **Benoît Blanchon** for the **ArduinoJson** library.
- Time synchronization powered by **NTPClient** by **Fabrice Weinberg**.
