# Agvolution Open-Source Weatherstation
Embedded software and hardware design for Arduino MKR-board based Weatherstation (Atmel SAMD processors).

## Frequently Asked Questions (FAQ)
1. How can I build my own Weatherstation? _We have published a KiCad schematic, PCB-layout, Bill of Materials, and the embedded firmware - including pre-built binaries. You can either buy a complete Weatherstation or a DIY-kit (assembled PCB) from Agvolution GmbH or start from scratch by ordering and assembling all the components (PCB, electronic components, sensors, housing, etc.) on your own. Do not hesitate to contact us! (contact@agvolution.com)_
2. How can I customize the configuration (e.g. packet transmit, wind sampling, position periods, etc.)? _You can make adjustments to the transmit and sampling periods in Inc/commission/commission.h and then rebuild the project. We also provide pre-built binaries with different configurations (check the builds folder)._
3. Which sensors can I connect? _Please check the list below (supported sensors). You can simply integrated new sensors by adjusting the firmware. Let us know about your changes, we can then merge your pull-request (reach out to l.kamm@agvolution.com)._
4. What is the battery life time of my station? _We have provided an energy-benchmark table in the energy-benchmark folder. On the one hand, it contains energy measurements on the real system. On the other hand, it contains a parametric energy consumption simulation. You can use it to estimate the battery life time depending on your system configuration._
5. Can I connect a solar module? _Yes, you can. We will provide further information soon._
6. Which LPWANs are available? _The first version was built on SigFox. We can easily integrate other boards (Nb-IoT, LoRa, Bluetooth, WiFi, ...). Feel free to contact us._
7. How can I access the data? _There exist two methods: 1) You can register your Arduino MKRFox1200 board in the SigFox backend using the ID and PAC programmed to the RF chip. You have then access to the raw payloads. 2) You can register your Arduino MKRFox1200 board with Agvolution or buy a pre-programmed and pre-registered board at us. You have then access to the climate data (human-readable) via a Smartphone App (iOS, Android) [Farmalyzer], IoT-platform in the browser (charts, position, etc.) [https://iot.agvolution.com] and via a data API (GraphQL). Data storage, payload decoding, visualization - we do it for you! You can manage users and share your data with your partners on our platform. You can also make use of our application software services (recommendations, models, etc.). Reach out to us (contact@agvolution.com)._


## Supported sensors
At the moment, we support:
* Sensirion SHT3x (I2C-addresses 0x44 and 0x45)
* Bosch BME280 (I2C-addresses 0x76 and 0x77)
* DS18B20 (arbitrary address, automatic address scan, selecting the first available sensor on the 1-wire bus)
* Watermark soil matrix potential (up to 4x)
* GNSS satellite positioning module
* Rain Gauge (up to 2 passive, tipping spoon reed contact sensors)
* Wind direction (analog poti) and speed (passive contact pulse)
* Weight Cell (we have included a temperature-stable, low-noise instrumentation amplifier. It can be used to read weight cells (German: Dehnungsmessstreifen, DMS). Used for bee hive weight monitoring, ...)

## Custom configuration
* Edit Inc/commission/commission.h and build the project.
* Alternatively, you can use the pre-built binaries in the builds-folder

## Build the project
**Required Arduino libraries:**
* Arduino Low Power (by Arduino LLC), v1.2.2
* Arduino SigFox for MKRFox1200 (by Arduino LLC), v1.0.4
* RTCZero (by Arduino), v1.6.0
* SD (by Arduino), v1.2.4
* BlueDot BME280 Library (by BlueDot), v1.0.9
* DS18B20 (by Mathias Munk Hansen), v1.0.0
* FlashStorage (by Arduino), v1.0.0
* OneWire (by Paul Stoffregen), v2.3.5
* SHT31 (by Rob Tillaart), v.0.3.4

**Required Arduino boards:**
* Arduino SAMD Boards (32-bits ARM Cortex M0+) (by Arduino), v1.8.11

**Instructions**
1. Clone the project
2. Open arduino_weatherstation.ino in Arduino IDE. Install the required library and board packages.
3. Build + Flash.
4. Done!

