## Pre-built firmware
* 2022-03-02T1317: _legacy_
* 2022-05-15_weatherstation-default_wind_3min: _All sensors equipped, except weight cells. Wind sampling period: 3 minutes_
* 2022-05-15_weatherstation-default_wind_5min: _All sensors equipped, except weight cells. Wind sampling period: 5 minutes_
* 2022-05-15_weatherstation-default_wind_15min: _All sensors equipped, except weight cells. Wind sampling period: 15 minutes_
* firstConfiguration: _Read ID/PAC of SigFox RF module_

## Flash firmware
**Windows**
1. Install Arduino IDE for Windows
2. Open Terminal here in builds folder
3. Connect Arduino MKR board to host computer via USB. Double-press the blue RESET button on the Arduino board. Orange LED is pulsating.
4. Issue: `C:\Users\test\Documents\ArduinoData\packages\arduino\tools\bossac\1.7.0-arduino3/bossac.exe -i -d -e -w -v -R ./BUILD/BUILDNAME.ino.bin`
5. Adjust the command above appropriate to your installation directory of the Arduino IDE / bossac

**Linux**
1. Install bossac (http://manpages.ubuntu.com/manpages/bionic/man1/bossac.1.html)
2. Open Terminal here in _builds_ folder
3. Connect Arduino MKR board to host computer via USB. Double-press the blue RESET button on the Arduino board. Orange LED is pulsating.
4. Issue: `bossac -i -d -e -w -v -R ./BUILD/BUILDNAME.ino.bin`