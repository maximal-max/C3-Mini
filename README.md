
# Wi-Fi Enabled Price Display for Gold & Bitcoin

This project displays real-time prices for Gold and Bitcoin in EUR using an ESP32 microcontroller. The data is fetched from the **MetalPriceAPI** and shown on an OLED screen. The device fetches prices at user-defined intervals and shows the most recent data. Configuration is done via a web interface, allowing easy entry of Wi-Fi credentials and API key.

## Features:
- **Real-time Gold & Bitcoin Prices in EUR**: Prices are fetched using the [MetalPriceAPI](https://metalpriceapi.com/).
- **OLED Display**: Shows Gold and Bitcoin prices alternately every 10 seconds.
- **Web Interface for Configuration**: The device starts in Access Point mode to allow users to configure Wi-Fi credentials, API key, and fetch cycle time through a web page.
- **Local Storage (SPIFFS)**: Stores credentials and settings on the ESP32, so they are retained even after a reboot.
- **User-defined Fetch Cycle**: Allows users to set the frequency (in minutes) for fetching price updates via the web interface.
- **Automatic Price Updates**: Prices are updated automatically according to the fetch cycle time.

## Components:
- **ESP32 Microcontroller**: Manages Wi-Fi connections, API requests, and display.
- **OLED Display (128x64)**: Displays Gold and Bitcoin prices alternately.
- **SPIFFS**: Local storage on ESP32 to save configuration settings.
- **MetalPriceAPI**: Provides real-time gold and Bitcoin price data.

## Hardware Requirements:
- ESP32 Development Board
- OLED Display (128x64, I2C compatible)
- Connecting Wires

## Libraries Used:
The following libraries are required to run the project. Install them using the Arduino Library Manager:
- **WiFi** by Arduino: Connects the ESP32 to Wi-Fi networks.
- **WebServer** by ESP32: Handles HTTP requests for the web interface.
- **SPIFFS** by Arduino: Local storage on ESP32.
- **HTTPClient** by Arduino: Handles HTTP requests for API calls.
- **ArduinoJson** by Benoît Blanchon: JSON parsing and serialization.
- **Wire** by Arduino: I2C communication with the OLED.
- **U8g2lib** by olikraus: Controls the OLED display.
- **Time** by Michael Margolis: Provides functions for handling time on the ESP32.

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
2. Open the Project in the Arduino IDE:
   - Launch Arduino IDE and go to **File > Open**.
   -Select the project folder you cloned and open the `main.ino` file.

3. Install Required Libraries:

   - 1. Go to **Sketch > Include Library > Manage Libraries**.
   - 2. Search for and install:
   - **WiFi** by Arduino
   - **WebServer** by ESP32
   - **HTTPClient** by Arduino
   - **ArduinoJson** by Benoît Blanchon
   - **Wire** by Arduino
   - **U8g2lib** by olikraus
   - **SPIFFS** by Arduino
   - **Time** by Michael Margolis

## Upload the Code to your ESP32:

1. Select the appropriate ESP32 board in **Tools > Board**.
2. Choose the correct port under **Tools > Port**.
3. Click the **Upload** button to flash the code to your ESP32.

## Web Interface Configuration:

### Initial Setup:
1. After uploading the code, the ESP32 will enter Access Point mode if no Wi-Fi credentials are stored.
2. Connect to the ESP32's Wi-Fi network named `ESP32_Config` from your phone or computer.

### Configure Wi-Fi and API Key:
1. Open a browser and go to `http://192.168.4.1`.
2. Enter your Wi-Fi SSID, password, API key, and fetch cycle time (in minutes) via the web interface.
3. Click **Save** to store your settings on the ESP32.
4. After saving, the ESP32 will attempt to connect to the specified Wi-Fi network.

### Reboot and Connect:
1. After the ESP32 reboots, it will attempt to connect to your Wi-Fi network.
2. If successful, it will start fetching and displaying real-time prices.

## Fetch Cycle Considerations:

- The **MetalPriceAPI's** free plan allows up to **100 requests per month**.
- For example, to fetch data once per day, set the cycle to **1440 minutes** (24 hours).
- Be mindful of the request limits to avoid reaching the API quota.
- If more frequent updates are needed, consider upgrading your API plan.

## Usage:

- The ESP32 will show:
  - **Gold Price** (EUR/gram) every 10 seconds.
  - **Bitcoin Price** (EUR/BTC) every 10 seconds.
  - **Last Update Time** is displayed for 10 seconds every minute.
  
- **Price Updates**: The device fetches the latest prices based on your set cycle.
- **OLED Display**: Gold and Bitcoin prices alternate every 10 seconds.
- **Offline Data**: If the device can't fetch new data, it will show the last known prices saved in SPIFFS.

## Troubleshooting:

### Wi-Fi Connection Issues:
- Ensure the SSID and password are correct.

### API Errors:
- Check the internet connection or verify the API key.

### SPIFFS Errors:
- Reformat the file system if needed.

## Tips:
- Use the **Serial Monitor** for debugging and log messages.
- Ensure the OLED display is wired correctly.
- If credentials don't save, check available space on the ESP32 file system.

## Future Enhancements:
- Add support for more cryptocurrencies or precious metals.
- Enable manual refresh of prices through the web interface.
- Allow dynamic adjustments to fetch cycle and display mode without rebooting.

## License:
This project is open-source and released under the MIT License.

## Acknowledgments:
- Thanks to **olikraus** for the **U8g2lib** library.
- Thanks to **Benoît Blanchon** for **ArduinoJson**.
- Thanks to **Michael Margolis** for the **Time** library.
- **MetalPriceAPI** for providing real-time price data – [MetalPriceAPI](https://metalpriceapi.com).
