## Wie viel hat es kürzlich auf meinem Feld tatsächlich geregnet?
_What was the last on-site rain amount on my farm?_
## Ist die Temperatur im Pflanzenbestand die selbe, wie die der nächsten Wetterstation?
_Does the temperature of the public weatherstation and inside the plants match?_

## Wir helfen dir, kostengünstig und simpel eine eigene Wetterstation in Betrieb zu nehmen.
Wir stellen dir die Anbindung ans Internet, Datenauswertung und -visualisierung kostenlos und unkompliziert zur Verfügung.

We help you to install your own low-cost weatherstation. Connectivity and data storage, evaluation and -visualization is provided by us.

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

## Hardware Installation
### Solar Panel
1. Cut the DC jack.
![solar_1](https://user-images.githubusercontent.com/13632429/174666261-f9aac4dd-9976-445b-85a4-fafdd1a673b0.JPEG)
2. Strip the two wires and twist the ends.
![solar_2](https://user-images.githubusercontent.com/13632429/174666271-c05c64cd-63a2-4e83-9168-605042f8c1cb.JPEG)
![solar_3](https://user-images.githubusercontent.com/13632429/174666278-9f59a4af-5318-47fc-9806-c758342422ec.JPEG)
![solar_4](https://user-images.githubusercontent.com/13632429/174666282-2a1e8d7c-002a-4fbd-b8ca-f97cc6d9c8b2.JPEG)
3. Install the JST connector at the respective clamp. Please take care of the polarity ( + red, - black ). Also, replace the screw next to the GPS module area by a hex distance bolt. You can use Loctite to fix the screws.
![solar_5](https://user-images.githubusercontent.com/13632429/174666292-81c7bda0-93b7-40a6-bcc7-d9b1b6b8ddc8.JPEG)
4. Conduct the solar wire through one of the housing cable mounts. Bend the copper wires 180°, insert them on the Solar Charger Module at PWR IN (+ red, - blue) and tighten the screws. To improve the durability, make sure, that the screws of the clamp not only fasten the thin copper wires, but also the plastic moulding of the cables. Mount the Solar Charger Module on the hex nut and connect the JST cable to the socket. The status LEDs should be on, when the module is placed in the sun.
![solar_6](https://user-images.githubusercontent.com/13632429/174666316-f755650c-a8c0-45f8-86b6-fbd02ad276c8.JPEG)

### Anemometer (Davis)
1. Cut off the RJ11 connector.
![wind_1](https://user-images.githubusercontent.com/13632429/174666321-c976e302-d07c-4214-ad5a-18a19d154ce9.JPEG)
2. Use the 4-pin clamp in the PCB corner, labelled with 'Davis Anemometer'.
![wind_2](https://user-images.githubusercontent.com/13632429/174666324-2c699038-8b19-429f-ba02-a3979dbad79f.JPEG)
3. Strip the wire and drill the ends. Insert the wires into the clamp while pressing down the bolt, using a screwdriver or a similar tool. Please take care of the wire colors (Y: yellow, G: green, R: red, B: black).
![wind_3](https://user-images.githubusercontent.com/13632429/174666335-d5b52da1-f7c4-4947-a3a5-2190b0af1439.JPEG)
![wind_4](https://user-images.githubusercontent.com/13632429/174666338-fe48e8a7-19b4-461e-83ac-c569f308afe0.JPEG)


## ToDo
* If BME76 and BME77 looses initialization (power cut) -> Detect and Re-Init ? Which registers need to be written?
* Integration of LoRa, mioty, Nb-IoT boards

