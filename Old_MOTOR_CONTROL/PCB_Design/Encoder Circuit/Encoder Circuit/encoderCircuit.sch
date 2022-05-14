EESchema Schematic File Version 4
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 5500 4300 0    50   Input ~ 0
ENC_A
Text HLabel 5500 4200 0    50   Input ~ 0
ENC_!A
Text HLabel 8500 4400 2    50   Input ~ 0
ENC_B
Text HLabel 8500 4300 2    50   Input ~ 0
ENC_!B
Text HLabel 4300 1700 0    50   Input ~ 0
Vreg
Text HLabel 4300 2100 0    50   Input ~ 0
GND
$Comp
L MyLibrary:SN75175N U2
U 1 1 5C7C56D8
P 5900 4200
F 0 "U2" H 7000 4587 60  0000 C CNN
F 1 "SN75175N" H 7000 4481 60  0000 C CNN
F 2 "N16" H 7000 4440 60  0001 C CNN
F 3 "" H 5900 4200 60  0000 C CNN
	1    5900 4200
	1    0    0    -1  
$EndComp
$Comp
L MyLibrary:SN74LVC245AN U1
U 1 1 5C7C5A22
P 5900 1900
F 0 "U1" H 7000 2287 60  0000 C CNN
F 1 "SN74LVC245AN" H 7000 2181 60  0000 C CNN
F 2 "N20" H 7000 2140 60  0001 C CNN
F 3 "" H 5900 1900 60  0000 C CNN
	1    5900 1900
	1    0    0    -1  
$EndComp
Text HLabel 8600 1900 2    50   Input ~ 0
3.3V
Text Label 4600 1700 0    50   ~ 0
Vreg
Wire Wire Line
	4300 1700 4600 1700
Text Label 5400 4500 2    50   ~ 0
Vreg
Text Label 8500 4600 0    50   ~ 0
Vreg
Text Label 8500 4200 0    50   ~ 0
Vreg
Wire Wire Line
	8500 4200 8100 4200
Wire Wire Line
	8500 4600 8100 4600
Wire Wire Line
	5900 4500 5400 4500
Text Label 4600 2100 0    50   ~ 0
GND
Wire Wire Line
	4600 2100 4300 2100
Text Label 5400 4900 2    50   ~ 0
GND
Wire Wire Line
	5900 4900 5400 4900
Wire Wire Line
	5900 4200 5500 4200
Wire Wire Line
	8500 4300 8450 4300
Wire Wire Line
	8500 4400 8150 4400
$Comp
L Device:R R1
U 1 1 5C7C60CD
P 5750 3850
F 0 "R1" V 5543 3850 50  0000 C CNN
F 1 "120" V 5634 3850 50  0000 C CNN
F 2 "" V 5680 3850 50  0001 C CNN
F 3 "~" H 5750 3850 50  0001 C CNN
	1    5750 3850
	0    1    1    0   
$EndComp
Wire Wire Line
	5900 3850 5900 4200
Connection ~ 5900 4200
Wire Wire Line
	5500 4300 5600 4300
Wire Wire Line
	5600 3850 5600 4300
Connection ~ 5600 4300
Wire Wire Line
	5600 4300 5900 4300
$Comp
L Device:R R2
U 1 1 5C7C6CF9
P 8300 3850
F 0 "R2" V 8093 3850 50  0000 C CNN
F 1 "120" V 8184 3850 50  0000 C CNN
F 2 "" V 8230 3850 50  0001 C CNN
F 3 "~" H 8300 3850 50  0001 C CNN
	1    8300 3850
	0    1    1    0   
$EndComp
Wire Wire Line
	8150 3850 8150 4400
Connection ~ 8150 4400
Wire Wire Line
	8150 4400 8100 4400
Wire Wire Line
	8450 3850 8450 4300
Connection ~ 8450 4300
Wire Wire Line
	8450 4300 8100 4300
Wire Wire Line
	5900 4400 5000 4400
Wire Wire Line
	8100 4500 8950 4500
Wire Wire Line
	5000 4400 5000 2000
Wire Wire Line
	5000 2000 5900 2000
Wire Wire Line
	8950 4500 8950 3100
Wire Wire Line
	8950 3100 5500 3100
Wire Wire Line
	5500 3100 5500 2100
Wire Wire Line
	5500 2100 5900 2100
Text HLabel 8600 2100 2    50   Input ~ 0
A
Text HLabel 8600 2200 2    50   Input ~ 0
B
Wire Wire Line
	8600 2100 8100 2100
Wire Wire Line
	8100 2200 8600 2200
Text Label 5500 1900 2    50   ~ 0
Vreg
Wire Wire Line
	5500 1900 5900 1900
Text Label 8400 2000 0    50   ~ 0
GND
Wire Wire Line
	8100 2000 8400 2000
NoConn ~ 5900 4600
NoConn ~ 5900 4700
NoConn ~ 5900 4800
NoConn ~ 8100 4900
NoConn ~ 8100 4800
NoConn ~ 8100 4700
NoConn ~ 8100 2300
NoConn ~ 8100 2400
NoConn ~ 8100 2500
NoConn ~ 8100 2600
NoConn ~ 8100 2700
NoConn ~ 8100 2800
NoConn ~ 5900 2800
NoConn ~ 5900 2700
NoConn ~ 5900 2600
NoConn ~ 5900 2500
NoConn ~ 5900 2400
NoConn ~ 5900 2300
NoConn ~ 5900 2200
Wire Wire Line
	8100 1900 8600 1900
$EndSCHEMATC
