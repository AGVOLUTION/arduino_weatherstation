EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 3350 4250 1    50   ~ 0
AREF
Text Notes 3450 4250 1    50   ~ 0
DAC0/A0
Text Notes 3550 4250 1    50   ~ 0
A1
Text Notes 3650 4250 1    50   ~ 0
A2
Text Notes 3750 4250 1    50   ~ 0
A3
Text Notes 3850 4250 1    50   ~ 0
A4
Text Notes 3950 4250 1    50   ~ 0
A5
Text Notes 4050 4250 1    50   ~ 0
A6
Text Notes 4150 4250 1    50   ~ 0
0
Text Notes 4250 4250 1    50   ~ 0
1
Text Notes 4350 4250 1    50   ~ 0
2
Text Notes 4450 4250 1    50   ~ 0
3
Text Notes 4550 4250 1    50   ~ 0
4
Text Notes 4650 4250 1    50   ~ 0
5
Text Notes 3400 3800 1    50   ~ 0
5V
Text Notes 3500 3800 1    50   ~ 0
VIN
Text Notes 3600 3800 1    50   ~ 0
3.3V
Text Notes 3700 3800 1    50   ~ 0
GND
Text Notes 3800 3800 1    50   ~ 0
RESET
Text Notes 3900 3800 1    50   ~ 0
14 TX
Text Notes 4000 3800 1    50   ~ 0
13 RX
Text Notes 4100 3800 1    50   ~ 0
12 SCL
Text Notes 4200 3800 1    50   ~ 0
11 SDA
Text Notes 4300 3800 1    50   ~ 0
10 MISO
Text Notes 4400 3800 1    50   ~ 0
9 SCK
Text Notes 4500 3800 1    50   ~ 0
8 MOSI
Text Notes 4600 3800 1    50   ~ 0
7
Text Notes 4700 3800 1    50   ~ 0
6
Text Notes 4450 3200 1    50   ~ 0
SPI Bus used by SD card\nand Sigfox Radio Module
Text Notes 1850 4500 0    50   ~ 0
PWM Pins\n0,1,2,3,4,5,6,7,8,10,A3,A4\n\nAnalog Inputs\nA0 - A6 (8/10/12 bit)\n\nAnalog Outputs\nDAC0 (10 bit)\n\nExternal Interrupts\n0,1,4,5,6,7,8,A1,A2\n\nLED\n6
$Comp
L power:+3V3 #PWR0102
U 1 1 604A8739
P 3550 3050
F 0 "#PWR0102" H 3550 2900 50  0001 C CNN
F 1 "+3V3" H 3565 3223 50  0000 C CNN
F 2 "" H 3550 3050 50  0001 C CNN
F 3 "" H 3550 3050 50  0001 C CNN
	1    3550 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 604A93BC
P 3650 3200
F 0 "#PWR0103" H 3650 2950 50  0001 C CNN
F 1 "GND" H 3655 3027 50  0000 C CNN
F 2 "" H 3650 3200 50  0001 C CNN
F 3 "" H 3650 3200 50  0001 C CNN
	1    3650 3200
	-1   0    0    1   
$EndComp
Wire Wire Line
	3650 3200 3650 3300
Wire Wire Line
	3550 3050 3550 3300
Wire Wire Line
	3350 3050 3350 3300
Text Notes 7350 4700 0    50   ~ 0
Davis Aerocone
Text Notes 7350 3450 0    50   ~ 0
Davis Anemometer
Text Notes 8900 3100 0    50   ~ 0
black (Wind speed contact closure to ground)
Text Notes 8900 3000 0    50   ~ 0
red (Ground)
Text Notes 8900 2900 0    50   ~ 0
green (Wind direction pot wiper)
Text Notes 8900 2800 0    50   ~ 0
yellow (Pot supply voltage)
Text Notes 8900 4300 0    50   ~ 0
black (N.C.)
Text Notes 8900 4200 0    50   ~ 0
red (Signal 2)
Text Notes 8900 4100 0    50   ~ 0
green (Signal 1)
Text Notes 8900 4000 0    50   ~ 0
yellow (Signal 1)
Text Notes 8900 3900 0    50   ~ 0
Signal 1 + 2 connected when tipping spoon tilted
$Comp
L power:GND #PWR0106
U 1 1 604B2DF3
P 8350 3000
F 0 "#PWR0106" H 8350 2750 50  0001 C CNN
F 1 "GND" V 8355 2872 50  0000 R CNN
F 2 "" H 8350 3000 50  0001 C CNN
F 3 "" H 8350 3000 50  0001 C CNN
	1    8350 3000
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 604B3D5E
P 8400 4000
F 0 "#PWR0107" H 8400 3750 50  0001 C CNN
F 1 "GND" V 8405 3872 50  0000 R CNN
F 2 "" H 8400 4000 50  0001 C CNN
F 3 "" H 8400 4000 50  0001 C CNN
	1    8400 4000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8050 3000 8350 3000
Wire Wire Line
	8050 2800 8100 2800
Wire Wire Line
	8050 2900 8100 2900
Wire Wire Line
	8050 3100 8100 3100
Text GLabel 8100 2800 2    50   Input ~ 0
DAVIS_WIND_SUP
Text GLabel 8100 2900 2    50   Input ~ 0
DAVIS_WIND_DIR
Text GLabel 8100 3100 2    50   Input ~ 0
DAVIS_WIND_SPD
Wire Wire Line
	8050 4000 8250 4000
Wire Wire Line
	8050 4100 8250 4100
Wire Wire Line
	8250 4100 8250 4000
Connection ~ 8250 4000
Wire Wire Line
	8250 4000 8400 4000
Text GLabel 8100 4200 2    50   Input ~ 0
DAVIS_RAIN
Wire Wire Line
	8050 4200 8100 4200
Text GLabel 3650 4600 3    50   Input ~ 0
DAVIS_WIND_SPD
Text GLabel 3450 4600 3    50   Input ~ 0
DAVIS_WIND_DIR
Text GLabel 3250 3900 0    50   Input ~ 0
DAVIS_WIND_SUP
Text GLabel 4150 4600 3    50   Input ~ 0
DAVIS_RAIN
Wire Wire Line
	4150 4500 4150 4600
Text GLabel 4550 4600 3    50   Input ~ 0
SD_CS1
Text GLabel 4650 3200 1    50   Input ~ 0
LED
Wire Wire Line
	4550 4500 4550 4600
Wire Wire Line
	4650 3300 4650 3200
Text Notes 6800 1700 0    50   ~ 0
Add Sensor Tube sockets to connect our own soil sensor and rain gauge
Text Notes 6850 6750 0    50   ~ 0
Watermark sensor requires AC excitation.\nDC time within square signal shall not exceed 50ms total. Analog measurement shall be taken within 100us.\nWhat hardware connector for watermark?
Text GLabel 4350 4600 3    50   Input ~ 0
WM_EXC1
Text GLabel 4650 4600 3    50   Input ~ 0
WM_EXC2
Text GLabel 4050 4600 3    50   Input ~ 0
WM_RESISTANCE
Wire Wire Line
	4050 4500 4050 4600
Wire Wire Line
	4350 4500 4350 4600
Wire Wire Line
	4650 4500 4650 4600
$Comp
L Device:R R3
U 1 1 604BC6CA
P 7450 5800
F 0 "R3" H 7520 5846 50  0000 L CNN
F 1 "7.87k" H 7520 5755 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7380 5800 50  0001 C CNN
F 3 "~" H 7450 5800 50  0001 C CNN
	1    7450 5800
	1    0    0    -1  
$EndComp
Text GLabel 1700 6400 0    50   Input ~ 0
WM_RESISTANCE
Text GLabel 7450 5150 1    50   Input ~ 0
WM_EXC2
Text GLabel 7450 6100 0    50   Input ~ 0
WM_EXC1
Wire Wire Line
	7450 6100 7450 5950
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J7
U 1 1 604C00AD
P 7050 1400
F 0 "J7" H 7100 1717 50  0000 C CNN
F 1 "Conn_02x03_Odd_Even" H 7100 1626 50  0000 C CNN
F 2 "Connector_IDC:IDC-Header_2x03_P2.54mm_Vertical" H 7050 1400 50  0001 C CNN
F 3 "~" H 7050 1400 50  0001 C CNN
	1    7050 1400
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0108
U 1 1 604C0B50
P 6650 1300
F 0 "#PWR0108" H 6650 1150 50  0001 C CNN
F 1 "+3V3" V 6665 1428 50  0000 L CNN
F 2 "" H 6650 1300 50  0001 C CNN
F 3 "" H 6650 1300 50  0001 C CNN
	1    6650 1300
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0109
U 1 1 604C0F08
P 7500 1300
F 0 "#PWR0109" H 7500 1050 50  0001 C CNN
F 1 "GND" V 7505 1172 50  0000 R CNN
F 2 "" H 7500 1300 50  0001 C CNN
F 3 "" H 7500 1300 50  0001 C CNN
	1    7500 1300
	0    -1   -1   0   
$EndComp
Text GLabel 3750 4600 3    50   Input ~ 0
AGV_MOISTURE_EN
Text GLabel 4050 3200 1    50   Input ~ 0
SCL
Text GLabel 4150 3200 1    50   Input ~ 0
SDA
Wire Wire Line
	4050 3200 4050 3300
Wire Wire Line
	4150 3200 4150 3300
Wire Wire Line
	3750 4500 3750 4600
Text GLabel 6650 1400 0    50   Input ~ 0
SCL
Text GLabel 7500 1400 2    50   Input ~ 0
SDA
Text GLabel 6650 1500 0    50   Input ~ 0
AGV_RAINREED
Text GLabel 7500 1500 2    50   Input ~ 0
AGV_MOISTURE_EN
Wire Wire Line
	6850 1300 6650 1300
Wire Wire Line
	6850 1400 6650 1400
Wire Wire Line
	6850 1500 6650 1500
Wire Wire Line
	7350 1300 7500 1300
Wire Wire Line
	7350 1400 7500 1400
Wire Wire Line
	7350 1500 7500 1500
Text Notes 6350 3300 0    50   ~ 0
Zusätzlich Lötpads
Text Notes 6350 4400 0    50   ~ 0
Zusätzlich Lötpads
$Comp
L Connector:Conn_01x02_Male J5
U 1 1 604A5CD1
P 6300 4100
F 0 "J5" H 6408 4281 50  0000 C CNN
F 1 "Conn_01x02_Male" H 6408 4190 50  0000 C CNN
F 2 "AGV1k-700:Phoenix_19_84_96_3_1x02_P3.50mm_Vertical" H 6300 4100 50  0001 C CNN
F 3 "~" H 6300 4100 50  0001 C CNN
	1    6300 4100
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x03_Male J11
U 1 1 604A9375
P 8500 5650
F 0 "J11" H 8350 5450 50  0000 C CNN
F 1 "Conn_01x03_Male" H 8550 5350 50  0000 C CNN
F 2 "AGV1k-700:Phoenix_19_84_97_6_1x03_P3.50mm_Vertical" H 8500 5650 50  0001 C CNN
F 3 "~" H 8500 5650 50  0001 C CNN
	1    8500 5650
	1    0    0    1   
$EndComp
$Comp
L Connector:Conn_01x04_Male J4
U 1 1 604B056E
P 6300 2900
F 0 "J4" H 6272 2782 50  0000 R CNN
F 1 "Conn_01x04_Male" H 6272 2873 50  0000 R CNN
F 2 "AGV1k-700:Phoenix_19_84_98_9_1x04_P3.50mm_Vertical" H 6300 2900 50  0001 C CNN
F 3 "~" H 6300 2900 50  0001 C CNN
	1    6300 2900
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male J13
U 1 1 604B1C3A
P 10300 6000
F 0 "J13" H 10408 6281 50  0000 C CNN
F 1 "Conn_01x04_Male" H 10408 6190 50  0000 C CNN
F 2 "AGV1k-700:Phoenix_19_84_98_9_1x04_P3.50mm_Vertical" H 10300 6000 50  0001 C CNN
F 3 "~" H 10300 6000 50  0001 C CNN
	1    10300 6000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 604B2CC0
P 10950 6000
F 0 "#PWR0110" H 10950 5750 50  0001 C CNN
F 1 "GND" V 10955 5872 50  0000 R CNN
F 2 "" H 10950 6000 50  0001 C CNN
F 3 "" H 10950 6000 50  0001 C CNN
	1    10950 6000
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR0111
U 1 1 604B3026
P 10650 5900
F 0 "#PWR0111" H 10650 5750 50  0001 C CNN
F 1 "+3V3" V 10665 6028 50  0000 L CNN
F 2 "" H 10650 5900 50  0001 C CNN
F 3 "" H 10650 5900 50  0001 C CNN
	1    10650 5900
	0    1    1    0   
$EndComp
Text GLabel 10650 6200 2    50   Input ~ 0
SDA
Text GLabel 10650 6100 2    50   Input ~ 0
SCL
Wire Wire Line
	10650 5900 10500 5900
Wire Wire Line
	10950 6000 10500 6000
Wire Wire Line
	10650 6100 10500 6100
Wire Wire Line
	10650 6200 10500 6200
Text Notes 10250 6350 0    50   ~ 0
BME280 or SHT30
Text GLabel 6650 2800 2    50   Input ~ 0
DAVIS_WIND_SUP
Text GLabel 6650 2900 2    50   Input ~ 0
DAVIS_WIND_DIR
$Comp
L power:GND #PWR0112
U 1 1 604BB171
P 6800 3000
F 0 "#PWR0112" H 6800 2750 50  0001 C CNN
F 1 "GND" V 6805 2872 50  0000 R CNN
F 2 "" H 6800 3000 50  0001 C CNN
F 3 "" H 6800 3000 50  0001 C CNN
	1    6800 3000
	0    -1   -1   0   
$EndComp
Text GLabel 6650 3100 2    50   Input ~ 0
DAVIS_WIND_SPD
Text GLabel 6600 4200 2    50   Input ~ 0
DAVIS_RAIN
$Comp
L power:GND #PWR0113
U 1 1 604C0659
P 6900 4100
F 0 "#PWR0113" H 6900 3850 50  0001 C CNN
F 1 "GND" V 6905 3972 50  0000 R CNN
F 2 "" H 6900 4100 50  0001 C CNN
F 3 "" H 6900 4100 50  0001 C CNN
	1    6900 4100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6900 4100 6500 4100
Wire Wire Line
	6600 4200 6500 4200
Text Notes 8350 5850 0    50   ~ 0
DS18B20
$Comp
L power:GND #PWR0114
U 1 1 604C4842
P 8950 5550
F 0 "#PWR0114" H 8950 5300 50  0001 C CNN
F 1 "GND" V 8955 5422 50  0000 R CNN
F 2 "" H 8950 5550 50  0001 C CNN
F 3 "" H 8950 5550 50  0001 C CNN
	1    8950 5550
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR0115
U 1 1 604C4AE1
P 8850 5650
F 0 "#PWR0115" H 8850 5500 50  0001 C CNN
F 1 "+3V3" V 8865 5778 50  0000 L CNN
F 2 "" H 8850 5650 50  0001 C CNN
F 3 "" H 8850 5650 50  0001 C CNN
	1    8850 5650
	0    1    1    0   
$EndComp
Text GLabel 8850 5750 2    50   Input ~ 0
DS18B20_1W
Text GLabel 4550 3200 1    50   Input ~ 0
DS18B20_1W
$Comp
L power:GND #PWR0116
U 1 1 604C77F8
P 9150 5000
F 0 "#PWR0116" H 9150 4750 50  0001 C CNN
F 1 "GND" V 9155 4872 50  0000 R CNN
F 2 "" H 9150 5000 50  0001 C CNN
F 3 "" H 9150 5000 50  0001 C CNN
	1    9150 5000
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R4
U 1 1 604CAFAF
P 8800 5900
F 0 "R4" H 8870 5946 50  0000 L CNN
F 1 "10k" H 8870 5855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8730 5900 50  0001 C CNN
F 3 "~" H 8800 5900 50  0001 C CNN
	1    8800 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	8800 5750 8850 5750
$Comp
L power:+3V3 #PWR0117
U 1 1 604CB5C0
P 8900 6050
F 0 "#PWR0117" H 8900 5900 50  0001 C CNN
F 1 "+3V3" V 8915 6178 50  0000 L CNN
F 2 "" H 8900 6050 50  0001 C CNN
F 3 "" H 8900 6050 50  0001 C CNN
	1    8900 6050
	0    1    1    0   
$EndComp
Wire Wire Line
	8800 6050 8900 6050
$Comp
L Connector:Conn_01x02_Male J6
U 1 1 604D8CA0
P 3050 6100
F 0 "J6" H 3000 6000 50  0000 C CNN
F 1 "Conn_01x02_Male" H 3100 6050 50  0000 C CNN
F 2 "AGV1k-700:Phoenix_19_84_96_3_1x02_P3.50mm_Vertical" H 3050 6100 50  0001 C CNN
F 3 "~" H 3050 6100 50  0001 C CNN
	1    3050 6100
	0    1    1    0   
$EndComp
Wire Wire Line
	4550 3200 4550 3300
Text Notes 11050 6200 0    50   ~ 0
3V3 I2C Bus
Text Notes 3900 2900 1    50   ~ 0
3V3 UART
Text Notes 10050 1150 0    50   ~ 0
3V3 UART
Text Notes 7950 1450 0    50   ~ 0
3V3 I2C Bus
$Comp
L 4xxx:4051 U1
U 1 1 6050C33B
P 2300 7000
F 0 "U1" H 2300 8081 50  0000 C CNN
F 1 "4051" H 2300 7990 50  0000 C CNN
F 2 "AGV1k-700:SO-16_3.9x9.9mm_P1.27mm" H 2300 7000 50  0001 C CNN
F 3 "http://www.intersil.com/content/dam/Intersil/documents/cd40/cd4051bms-52bms-53bms.pdf" H 2300 7000 50  0001 C CNN
	1    2300 7000
	-1   0    0    -1  
$EndComp
$Comp
L 4xxx:4051 U2
U 1 1 6050DF23
P 4050 7000
F 0 "U2" H 4594 7046 50  0000 L CNN
F 1 "4051" H 4594 6955 50  0000 L CNN
F 2 "AGV1k-700:SO-16_3.9x9.9mm_P1.27mm" H 4050 7000 50  0001 C CNN
F 3 "http://www.intersil.com/content/dam/Intersil/documents/cd40/cd4051bms-52bms-53bms.pdf" H 4050 7000 50  0001 C CNN
	1    4050 7000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 7400 3200 7400
Wire Wire Line
	2800 7500 3300 7500
Wire Wire Line
	2800 7600 3000 7600
$Comp
L power:GND #PWR0119
U 1 1 60525846
P 4250 7900
F 0 "#PWR0119" H 4250 7650 50  0001 C CNN
F 1 "GND" V 4255 7772 50  0000 R CNN
F 2 "" H 4250 7900 50  0001 C CNN
F 3 "" H 4250 7900 50  0001 C CNN
	1    4250 7900
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0120
U 1 1 60525F77
P 2400 7900
F 0 "#PWR0120" H 2400 7650 50  0001 C CNN
F 1 "GND" V 2405 7772 50  0000 R CNN
F 2 "" H 2400 7900 50  0001 C CNN
F 3 "" H 2400 7900 50  0001 C CNN
	1    2400 7900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2200 7900 2300 7900
Wire Wire Line
	2300 7900 2400 7900
Connection ~ 2300 7900
Wire Wire Line
	4050 7900 4150 7900
Wire Wire Line
	4150 7900 4250 7900
Connection ~ 4150 7900
$Comp
L power:GND #PWR0121
U 1 1 6052F33E
P 3000 7700
F 0 "#PWR0121" H 3000 7450 50  0001 C CNN
F 1 "GND" V 3005 7572 50  0000 R CNN
F 2 "" H 3000 7700 50  0001 C CNN
F 3 "" H 3000 7700 50  0001 C CNN
	1    3000 7700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 7300 3100 7300
Text GLabel 7350 5550 0    50   Input ~ 0
WM_RESISTANCE
Wire Wire Line
	7450 5550 7450 5650
Wire Wire Line
	7450 5150 7450 5400
Wire Wire Line
	1700 6400 1800 6400
Text GLabel 4650 6400 2    50   Input ~ 0
WM_EXC2
Wire Wire Line
	4550 6400 4650 6400
Text Notes 9850 2550 0    50   ~ 0
GPS module cut GND
Text GLabel 4150 6100 2    50   Input ~ 0
WM_SWITCH_SUP
Text GLabel 2100 6100 0    50   Input ~ 0
WM_SWITCH_SUP
Text GLabel 3200 7650 3    50   Input ~ 0
WM_A
Text GLabel 3300 7650 3    50   Input ~ 0
WM_B
Wire Wire Line
	3100 7300 3100 7600
Connection ~ 3100 7300
Connection ~ 3100 7600
Wire Wire Line
	3100 7600 3550 7600
Wire Wire Line
	3200 7400 3200 7650
Connection ~ 3200 7400
Wire Wire Line
	3200 7400 3550 7400
Wire Wire Line
	3300 7500 3300 7650
Connection ~ 3300 7500
Wire Wire Line
	3300 7500 3550 7500
Text GLabel 3950 4600 3    50   Input ~ 0
WM_A
Text GLabel 4250 4600 3    50   Input ~ 0
WM_B
Text GLabel 3850 4600 3    50   Input ~ 0
WM_SWITCH_SUP
Text GLabel 9750 1850 0    50   Input ~ 0
GPS_SUP
$Comp
L power:GND #PWR0122
U 1 1 60586BAD
P 10200 2100
F 0 "#PWR0122" H 10200 1850 50  0001 C CNN
F 1 "GND" V 10205 1972 50  0000 R CNN
F 2 "" H 10200 2100 50  0001 C CNN
F 3 "" H 10200 2100 50  0001 C CNN
	1    10200 2100
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0123
U 1 1 60593058
P 10500 1450
F 0 "#PWR0123" H 10500 1300 50  0001 C CNN
F 1 "+3V3" V 10500 1600 50  0000 L CNN
F 2 "" H 10500 1450 50  0001 C CNN
F 3 "" H 10500 1450 50  0001 C CNN
	1    10500 1450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9750 1850 9900 1850
Wire Wire Line
	10200 2050 10200 2100
Text GLabel 10450 1250 0    50   Input ~ 0
GPS_TX
Text GLabel 10450 1350 0    50   Input ~ 0
GPS_RX
Wire Wire Line
	10550 1250 10450 1250
Wire Wire Line
	10550 1350 10450 1350
Text GLabel 3950 3200 1    50   Input ~ 0
GPS_TX
Wire Wire Line
	4050 6100 4150 6100
$Comp
L Connector:Conn_01x02_Male J8
U 1 1 605C7B7D
P 3200 6100
F 0 "J8" H 3150 6000 50  0000 C CNN
F 1 "Conn_01x02_Male" H 3250 6050 50  0000 C CNN
F 2 "AGV1k-700:Phoenix_19_84_96_3_1x02_P3.50mm_Vertical" H 3200 6100 50  0001 C CNN
F 3 "~" H 3200 6100 50  0001 C CNN
	1    3200 6100
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J9
U 1 1 605C7EE5
P 3350 6100
F 0 "J9" H 3300 6000 50  0000 C CNN
F 1 "Conn_01x02_Male" H 3400 6050 50  0000 C CNN
F 2 "AGV1k-700:Phoenix_19_84_96_3_1x02_P3.50mm_Vertical" H 3350 6100 50  0001 C CNN
F 3 "~" H 3350 6100 50  0001 C CNN
	1    3350 6100
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J10
U 1 1 605C828F
P 3500 6100
F 0 "J10" H 3450 6000 50  0000 C CNN
F 1 "Conn_01x02_Male" H 3550 6050 50  0000 C CNN
F 2 "AGV1k-700:Phoenix_19_84_96_3_1x02_P3.50mm_Vertical" H 3500 6100 50  0001 C CNN
F 3 "~" H 3500 6100 50  0001 C CNN
	1    3500 6100
	0    1    1    0   
$EndComp
Wire Wire Line
	2800 6400 2950 6400
Wire Wire Line
	2950 6400 2950 6300
Wire Wire Line
	3050 6300 3050 6400
Wire Wire Line
	3050 6400 3550 6400
Wire Wire Line
	2800 6500 3100 6500
Wire Wire Line
	3100 6500 3100 6300
Wire Wire Line
	2800 6600 3250 6600
Wire Wire Line
	3250 6600 3250 6300
Wire Wire Line
	2800 6700 3400 6700
Wire Wire Line
	3400 6700 3400 6300
Wire Wire Line
	3200 6300 3200 6500
Wire Wire Line
	3200 6500 3550 6500
Wire Wire Line
	3350 6300 3350 6600
Wire Wire Line
	3350 6600 3550 6600
Wire Wire Line
	3500 6300 3500 6700
Wire Wire Line
	3500 6700 3550 6700
Text GLabel 4450 4600 3    50   Input ~ 0
GPS_SUP
$Comp
L Connector:Conn_01x04_Male J14
U 1 1 60639821
P 10750 1350
F 0 "J14" H 10722 1324 50  0000 R CNN
F 1 "Conn_01x04_Male" H 10722 1233 50  0000 R CNN
F 2 "AGV1k-700:NS-Tech_Grove_1x04_P2mm_Vertical" H 10750 1350 50  0001 C CNN
F 3 "~" H 10750 1350 50  0001 C CNN
	1    10750 1350
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10500 1450 10550 1450
Wire Wire Line
	10200 1650 10200 1550
Wire Wire Line
	10200 1550 10550 1550
$Sheet
S 5800 6850 800  600 
U 60A8DE14
F0 "DMS Messverstärker" 50
F1 "DMSAmp.sch" 50
F2 "WEIGHT_1" I L 5800 6950 50 
F3 "WEIGHT_2" I L 5800 7050 50 
F4 "+3V3" I L 5800 7150 50 
F5 "GND" I L 5800 7250 50 
F6 "WEIGHT_EN" I L 5800 7350 50 
$EndSheet
$Comp
L Connector:RJ14 J16
U 1 1 60B70E5B
P 7650 4200
F 0 "J16" H 7707 4767 50  0000 C CNN
F 1 "RJ14" H 7707 4676 50  0000 C CNN
F 2 "AGV1k-700:RJ14_Wuerth_615004138421_Horizontal" V 7650 4225 50  0001 C CNN
F 3 "~" V 7650 4225 50  0001 C CNN
	1    7650 4200
	1    0    0    -1  
$EndComp
$Comp
L Connector:RJ14 J15
U 1 1 60B71F14
P 7650 3000
F 0 "J15" H 7707 3567 50  0000 C CNN
F 1 "RJ14" H 7707 3476 50  0000 C CNN
F 2 "AGV1k-700:RJ14_Wuerth_615004138421_Horizontal" V 7650 3025 50  0001 C CNN
F 3 "~" V 7650 3025 50  0001 C CNN
	1    7650 3000
	1    0    0    -1  
$EndComp
Text GLabel 3750 3200 1    50   Input ~ 0
RESET
Wire Wire Line
	3750 3200 3750 3300
Text GLabel 5700 6950 0    50   Input ~ 0
WEIGHT_1
Text GLabel 5700 7050 0    50   Input ~ 0
WEIGHT_2
Text GLabel 5700 7350 0    50   Input ~ 0
WEIGHT_EN
Wire Wire Line
	5700 6950 5800 6950
Wire Wire Line
	5700 7050 5800 7050
Wire Wire Line
	5700 7350 5800 7350
$Comp
L power:GND #PWR0125
U 1 1 60BF0470
P 5650 7250
F 0 "#PWR0125" H 5650 7000 50  0001 C CNN
F 1 "GND" V 5655 7122 50  0000 R CNN
F 2 "" H 5650 7250 50  0001 C CNN
F 3 "" H 5650 7250 50  0001 C CNN
	1    5650 7250
	0    1    1    0   
$EndComp
Wire Wire Line
	5650 7250 5800 7250
$Comp
L power:+3V3 #PWR0126
U 1 1 60BF6492
P 5650 7150
F 0 "#PWR0126" H 5650 7000 50  0001 C CNN
F 1 "+3V3" V 5665 7278 50  0000 L CNN
F 2 "" H 5650 7150 50  0001 C CNN
F 3 "" H 5650 7150 50  0001 C CNN
	1    5650 7150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5650 7150 5800 7150
Text GLabel 4350 3900 2    50   Input ~ 0
WEIGHT_2
Text GLabel 3250 3800 0    50   Input ~ 0
WEIGHT_1
Wire Wire Line
	3250 3800 3750 3800
Wire Wire Line
	3750 3800 3750 3900
Connection ~ 3750 4500
$Sheet
S 1350 1300 500  500 
U 60AA82D1
F0 "Battery Management" 50
F1 "Battery.sch" 50
F2 "+3V3" I R 1850 1400 50 
F3 "GND" I R 1850 1500 50 
F4 "VBAT+_SENS" I R 1850 1600 50 
$EndSheet
Text GLabel 1950 1600 2    50   Input ~ 0
VBAT+_SENS
Wire Wire Line
	1850 1600 1950 1600
Text Notes 4200 2900 1    50   ~ 0
3V3 I2C Bus\nI2C Pull-Ups optional (Arduino has internal Pull-Ups)
Text Notes 400  -500 0    50   ~ 0
ToDo Layout:\n- Billo Arduino aus China oder uC direkt auf PCB?
Text Notes 2350 2400 0    50   ~ 0
Mutual Exclusion\n- CLIMAVI, WEIGHT_1 (Prio)\n- DAVIS WIND, WEIGHT_1 (Prio)\n- Watermark, WEIGHT 1+2 (Prio)
Text Notes 550  2950 0    50   ~ 0
Module List\n- Davis Anemometer (Wind)\n- Niederschlag (Davis oder Pronamic)\n- GPS (Grove)\n- CLIMAVI\n- BME 0x76 und 0x77\n- SHT30 0x44\n- Watermark (up to four)\n- DS18B20 (up to one)\n- Weight (up to two)\n- SD
Wire Wire Line
	3950 3200 3950 3300
Text Notes 12000 1700 0    50   ~ 0
In v2.0.0:\nNormally, Serial1 on RX/TX pins (13/14) uses: 13/RX/PB23/SERCOM5/PAD03 and 14/TX/PB22/SERCOM5/PAD02\nGPS requires an RX on A1: 16/A1/PB02/SERCOM5/PAD00 configure as RX on SAM-D\n---> Fetter Dusel ;-) ==>> Läuft!!! :-))))))\n\nAb v2.0.1:\nGPS RX on pin 13 -> no reconfiguration required\n\nChanges from V2.1.0 to V2.2.0\n Moved SMD components to top side for manufacturability.
Wire Wire Line
	2100 6100 2300 6100
Wire Wire Line
	6500 2800 6650 2800
Wire Wire Line
	6500 2900 6650 2900
Wire Wire Line
	6500 3000 6800 3000
Wire Wire Line
	6500 3100 6650 3100
Wire Wire Line
	8700 5550 8950 5550
Wire Wire Line
	8700 5650 8850 5650
Wire Wire Line
	8700 5750 8800 5750
Connection ~ 8800 5750
Text GLabel 3550 4600 3    50   Input ~ 0
PRONAMIC_RAIN
Wire Wire Line
	3550 4500 3550 4600
Text GLabel 8950 5100 2    50   Input ~ 0
PRONAMIC_RAIN
$Comp
L Connector:Conn_01x02_Male J1
U 1 1 6149F721
P 8500 5000
F 0 "J1" H 8608 5181 50  0000 C CNN
F 1 "Conn_01x02_Male" H 8608 5090 50  0000 C CNN
F 2 "AGV1k-700:Phoenix_19_84_96_3_1x02_P3.50mm_Vertical" H 8500 5000 50  0001 C CNN
F 3 "~" H 8500 5000 50  0001 C CNN
	1    8500 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 5000 9150 5000
Wire Wire Line
	8700 5100 8950 5100
$Comp
L Device:C C?
U 1 1 61550BF1
P 1100 7200
AR Path="/60A8DE14/61550BF1" Ref="C?"  Part="1" 
AR Path="/61550BF1" Ref="C1"  Part="1" 
F 0 "C1" H 1215 7246 50  0000 L CNN
F 1 "100nF" H 1215 7155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1138 7050 50  0001 C CNN
F 3 "~" H 1100 7200 50  0001 C CNN
	1    1100 7200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 61550BF7
P 1600 7200
AR Path="/60A8DE14/61550BF7" Ref="C?"  Part="1" 
AR Path="/61550BF7" Ref="C14"  Part="1" 
F 0 "C14" H 1715 7246 50  0000 L CNN
F 1 "100nF" H 1715 7155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1638 7050 50  0001 C CNN
F 3 "~" H 1600 7200 50  0001 C CNN
	1    1600 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 7050 1600 7050
Connection ~ 1100 7050
Wire Wire Line
	1100 7400 1100 7350
Wire Wire Line
	1100 7350 1600 7350
Connection ~ 1100 7350
$Comp
L power:GND #PWR?
U 1 1 61550C09
P 1100 7400
AR Path="/60A8DE14/61550C09" Ref="#PWR?"  Part="1" 
AR Path="/61550C09" Ref="#PWR0118"  Part="1" 
F 0 "#PWR0118" H 1100 7150 50  0001 C CNN
F 1 "GND" H 1250 7250 50  0000 C CNN
F 2 "" H 1100 7400 50  0001 C CNN
F 3 "" H 1100 7400 50  0001 C CNN
	1    1100 7400
	1    0    0    -1  
$EndComp
Text GLabel 1200 6900 2    50   Input ~ 0
WM_SWITCH_SUP
Wire Wire Line
	1200 6900 1100 6900
Wire Wire Line
	1100 6900 1100 7050
$Comp
L AGV1k-700:DMN65D8LDW Q1
U 1 1 6156BB51
P 10200 1850
F 0 "Q1" H 10304 1896 50  0000 L CNN
F 1 "DMN65D8LDW" H 10304 1805 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-363_SC-70-6" H 10200 1850 50  0001 C CNN
F 3 "" H 10200 1850 50  0001 C CNN
	1    10200 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 5550 7450 5550
Wire Wire Line
	3100 7300 3550 7300
Wire Wire Line
	3000 7700 3000 7600
Connection ~ 3000 7600
Wire Wire Line
	3000 7600 3100 7600
$Comp
L Connector:Conn_01x14_Female J3
U 1 1 60491D37
P 4050 3500
F 0 "J3" V 4050 4300 50  0000 C CNN
F 1 "Conn_01x14_Female" V 4150 4600 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x14_P2.54mm_Vertical" H 4050 3500 50  0001 C CNN
F 3 "~" H 4050 3500 50  0001 C CNN
	1    4050 3500
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x14_Female J2
U 1 1 604A0170
P 3950 4300
F 0 "J2" V 4150 3400 50  0000 C CNN
F 1 "Conn_01x14_Female" V 4050 3100 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x14_P2.54mm_Vertical" H 3950 4300 50  0001 C CNN
F 3 "~" H 3950 4300 50  0001 C CNN
	1    3950 4300
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:Fiducial FID1
U 1 1 616426D0
P 1250 4950
F 0 "FID1" H 1335 4996 50  0000 L CNN
F 1 "Fiducial" H 1335 4905 50  0000 L CNN
F 2 "Fiducial:Fiducial_1mm_Mask3mm" H 1250 4950 50  0001 C CNN
F 3 "~" H 1250 4950 50  0001 C CNN
	1    1250 4950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:Fiducial FID2
U 1 1 61642E84
P 1250 5150
F 0 "FID2" H 1335 5196 50  0000 L CNN
F 1 "Fiducial" H 1335 5105 50  0000 L CNN
F 2 "Fiducial:Fiducial_1mm_Mask3mm" H 1250 5150 50  0001 C CNN
F 3 "~" H 1250 5150 50  0001 C CNN
	1    1250 5150
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:Fiducial FID3
U 1 1 6164425D
P 1250 5350
F 0 "FID3" H 1335 5396 50  0000 L CNN
F 1 "Fiducial" H 1335 5305 50  0000 L CNN
F 2 "Fiducial:Fiducial_1mm_Mask3mm" H 1250 5350 50  0001 C CNN
F 3 "~" H 1250 5350 50  0001 C CNN
	1    1250 5350
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:Fiducial FID4
U 1 1 61644263
P 1250 5550
F 0 "FID4" H 1335 5596 50  0000 L CNN
F 1 "Fiducial" H 1335 5505 50  0000 L CNN
F 2 "Fiducial:Fiducial_1mm_Mask3mm" H 1250 5550 50  0001 C CNN
F 3 "~" H 1250 5550 50  0001 C CNN
	1    1250 5550
	1    0    0    -1  
$EndComp
$Comp
L Sensor:BME280 U9
U 1 1 615CFE71
P 12800 4000
F 0 "U9" H 12371 4046 50  0000 R CNN
F 1 "BME280" H 12371 3955 50  0000 R CNN
F 2 "Package_LGA:Bosch_LGA-8_2.5x2.5mm_P0.65mm_ClockwisePinNumbering" H 14300 3550 50  0001 C CNN
F 3 "https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280-DS002.pdf" H 12800 3800 50  0001 C CNN
	1    12800 4000
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0124
U 1 1 615D4057
P 12700 3150
F 0 "#PWR0124" H 12700 3000 50  0001 C CNN
F 1 "+3V3" V 12715 3278 50  0000 L CNN
F 2 "" H 12700 3150 50  0001 C CNN
F 3 "" H 12700 3150 50  0001 C CNN
	1    12700 3150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0151
U 1 1 615D44E9
P 12700 4750
F 0 "#PWR0151" H 12700 4500 50  0001 C CNN
F 1 "GND" V 12705 4622 50  0000 R CNN
F 2 "" H 12700 4750 50  0001 C CNN
F 3 "" H 12700 4750 50  0001 C CNN
	1    12700 4750
	1    0    0    -1  
$EndComp
Text GLabel 13550 3900 2    50   Input ~ 0
SCL
Text GLabel 13550 4100 2    50   Input ~ 0
SDA
$Comp
L power:GND #PWR0154
U 1 1 615D589E
P 12900 4750
F 0 "#PWR0154" H 12900 4500 50  0001 C CNN
F 1 "GND" V 12905 4622 50  0000 R CNN
F 2 "" H 12900 4750 50  0001 C CNN
F 3 "" H 12900 4750 50  0001 C CNN
	1    12900 4750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0155
U 1 1 615D5ABB
P 13550 3700
F 0 "#PWR0155" H 13550 3450 50  0001 C CNN
F 1 "GND" V 13555 3572 50  0000 R CNN
F 2 "" H 13550 3700 50  0001 C CNN
F 3 "" H 13550 3700 50  0001 C CNN
	1    13550 3700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	13400 3700 13550 3700
Wire Wire Line
	12700 4600 12700 4750
Wire Wire Line
	12900 4600 12900 4750
$Comp
L power:+3V3 #PWR0156
U 1 1 615E5D97
P 12900 3150
F 0 "#PWR0156" H 12900 3000 50  0001 C CNN
F 1 "+3V3" V 12915 3278 50  0000 L CNN
F 2 "" H 12900 3150 50  0001 C CNN
F 3 "" H 12900 3150 50  0001 C CNN
	1    12900 3150
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0158
U 1 1 615E6092
P 13550 4300
F 0 "#PWR0158" H 13550 4150 50  0001 C CNN
F 1 "+3V3" V 13565 4428 50  0000 L CNN
F 2 "" H 13550 4300 50  0001 C CNN
F 3 "" H 13550 4300 50  0001 C CNN
	1    13550 4300
	0    1    1    0   
$EndComp
Wire Wire Line
	13400 4300 13550 4300
Wire Wire Line
	12900 3150 12900 3400
Wire Wire Line
	12700 3150 12700 3400
$Comp
L Device:C C?
U 1 1 615F6FE9
P 14450 4000
AR Path="/60A8DE14/615F6FE9" Ref="C?"  Part="1" 
AR Path="/615F6FE9" Ref="C17"  Part="1" 
F 0 "C17" H 14565 4046 50  0000 L CNN
F 1 "100nF" H 14565 3955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 14488 3850 50  0001 C CNN
F 3 "~" H 14450 4000 50  0001 C CNN
	1    14450 4000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0159
U 1 1 615F7CFD
P 14450 4250
F 0 "#PWR0159" H 14450 4000 50  0001 C CNN
F 1 "GND" V 14455 4122 50  0000 R CNN
F 2 "" H 14450 4250 50  0001 C CNN
F 3 "" H 14450 4250 50  0001 C CNN
	1    14450 4250
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0160
U 1 1 615F81E5
P 14450 3750
F 0 "#PWR0160" H 14450 3600 50  0001 C CNN
F 1 "+3V3" V 14465 3878 50  0000 L CNN
F 2 "" H 14450 3750 50  0001 C CNN
F 3 "" H 14450 3750 50  0001 C CNN
	1    14450 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	14450 3750 14450 3850
Wire Wire Line
	14450 4150 14450 4250
Wire Wire Line
	13400 3900 13550 3900
Wire Wire Line
	13400 4100 13550 4100
$Comp
L Connector:Conn_01x04_Male J12
U 1 1 617832DF
P 10300 5150
F 0 "J12" H 10408 5431 50  0000 C CNN
F 1 "Conn_01x04_Male" H 10408 5340 50  0000 C CNN
F 2 "AGV1k-700:Phoenix_19_84_98_9_1x04_P3.50mm_Vertical" H 10300 5150 50  0001 C CNN
F 3 "~" H 10300 5150 50  0001 C CNN
	1    10300 5150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0161
U 1 1 617832E5
P 10950 5150
F 0 "#PWR0161" H 10950 4900 50  0001 C CNN
F 1 "GND" V 10955 5022 50  0000 R CNN
F 2 "" H 10950 5150 50  0001 C CNN
F 3 "" H 10950 5150 50  0001 C CNN
	1    10950 5150
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR0162
U 1 1 617832EB
P 10650 5050
F 0 "#PWR0162" H 10650 4900 50  0001 C CNN
F 1 "+3V3" V 10665 5178 50  0000 L CNN
F 2 "" H 10650 5050 50  0001 C CNN
F 3 "" H 10650 5050 50  0001 C CNN
	1    10650 5050
	0    1    1    0   
$EndComp
Text GLabel 10650 5350 2    50   Input ~ 0
SDA
Text GLabel 10650 5250 2    50   Input ~ 0
SCL
Wire Wire Line
	10650 5050 10500 5050
Wire Wire Line
	10950 5150 10500 5150
Wire Wire Line
	10650 5250 10500 5250
Wire Wire Line
	10650 5350 10500 5350
Text Notes 10250 5500 0    50   ~ 0
BME280 or SHT30
Text Notes 11050 5350 0    50   ~ 0
3V3 I2C Bus
Wire Wire Line
	3450 4500 3450 4600
Wire Wire Line
	3650 4500 3650 4600
Wire Wire Line
	4450 4500 4450 4600
Wire Wire Line
	3950 4500 3950 4600
Wire Wire Line
	4250 4500 4250 4600
Wire Wire Line
	3250 3900 3750 3900
Connection ~ 3750 3900
Wire Wire Line
	3750 3900 3750 4500
Wire Wire Line
	3850 4500 3850 4600
Text GLabel 4400 4000 2    50   Input ~ 0
WEIGHT_EN
Connection ~ 4050 4500
Wire Wire Line
	4050 3900 4350 3900
Wire Wire Line
	4050 3900 4050 4500
Wire Wire Line
	4400 4000 4350 4000
Wire Wire Line
	4350 4000 4350 4500
Connection ~ 4350 4500
$EndSCHEMATC
