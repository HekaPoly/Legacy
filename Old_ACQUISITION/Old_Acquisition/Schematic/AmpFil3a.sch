EESchema Schematic File Version 4
LIBS:AmpFil3a-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L AmpFil3a-rescue:LM324 U13
U 3 1 5AAA2583
P 2700 2500
F 0 "U13" H 2700 2700 50  0000 L CNN
F 1 "LM324" H 2700 2300 50  0000 L CNN
F 2 "footprints.pretty.LM324DR-296-14597-1-ND:LM324DR" H 2650 2600 50  0001 C CNN
F 3 "" H 2750 2700 50  0001 C CNN
	3    2700 2500
	1    0    0    -1  
$EndComp
$Comp
L AmpFil3a-rescue:R R45
U 1 1 5AAA258B
P 1750 2400
F 0 "R45" V 1830 2400 50  0000 C CNN
F 1 "1k" V 1750 2400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 1680 2400 50  0001 C CNN
F 3 "" H 1750 2400 50  0001 C CNN
	1    1750 2400
	0    1    1    0   
$EndComp
$Comp
L AmpFil3a-rescue:R R46
U 1 1 5AAA2592
P 1750 2600
F 0 "R46" V 1830 2600 50  0000 C CNN
F 1 "1k" V 1750 2600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 1680 2600 50  0001 C CNN
F 3 "" H 1750 2600 50  0001 C CNN
	1    1750 2600
	0    1    1    0   
$EndComp
$Comp
L AmpFil3a-rescue:R R47
U 1 1 5AAA2599
P 2450 2000
F 0 "R47" V 2530 2000 50  0000 C CNN
F 1 "10k" V 2450 2000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2380 2000 50  0001 C CNN
F 3 "" H 2450 2000 50  0001 C CNN
	1    2450 2000
	0    1    1    0   
$EndComp
$Comp
L AmpFil3a-rescue:R R48
U 1 1 5AAA25A0
P 2450 3000
F 0 "R48" V 2530 3000 50  0000 C CNN
F 1 "10k" V 2450 3000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2380 3000 50  0001 C CNN
F 3 "" H 2450 3000 50  0001 C CNN
	1    2450 3000
	0    1    1    0   
$EndComp
$Comp
L AmpFil3a-rescue:GNDREF #PWR011
U 1 1 5AAA25A7
P 3200 2050
F 0 "#PWR011" H 3200 1800 50  0001 C CNN
F 1 "GNDREF" H 3200 1900 50  0000 C CNN
F 2 "" H 3200 2050 50  0001 C CNN
F 3 "" H 3200 2050 50  0001 C CNN
	1    3200 2050
	1    0    0    -1  
$EndComp
Text Notes 2150 1750 0    60   ~ 0
Gain = vO / (v2–v1) =  (R2/R1) = 10\nv2 : Voltage électrode +\nv1 : Voltage électrode -\nR2 = 10kohm\nR1 = 1kohm
$Comp
L AmpFil3a-rescue:GNDREF #PWR012
U 1 1 5AAA25AE
P 3900 2900
F 0 "#PWR012" H 3900 2650 50  0001 C CNN
F 1 "GNDREF" H 3900 2750 50  0000 C CNN
F 2 "" H 3900 2900 50  0001 C CNN
F 3 "" H 3900 2900 50  0001 C CNN
	1    3900 2900
	1    0    0    -1  
$EndComp
Text Label 3350 2400 0    60   ~ 0
v0
$Comp
L AmpFil3a-rescue:C C32
U 1 1 5AAA25B5
P 3700 2500
F 0 "C32" H 3725 2600 50  0000 L CNN
F 1 "22u" H 3725 2400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 3738 2350 50  0001 C CNN
F 3 "" H 3700 2500 50  0001 C CNN
	1    3700 2500
	0    1    1    0   
$EndComp
Text Notes 3500 3500 0    60   ~ 0
Filtre passe-haut passif\nordre 1\nw0 = 1 / RC\nf0 = 2 Hz
Text Notes 2150 1250 0    60   ~ 0
Amplification différentielle
$Comp
L AmpFil3a-rescue:INA128 U14
U 1 1 5AAA25C5
P 4750 2700
F 0 "U14" H 4900 2825 50  0000 L CNN
F 1 "INA128" H 4900 2575 50  0000 L CNN
F 2 "footprints.pretty.INA128PA:INA128PA" H 4850 2700 50  0001 C CNN
F 3 "" H 4850 2700 50  0001 C CNN
	1    4750 2700
	1    0    0    -1  
$EndComp
$Comp
L AmpFil3a-rescue:R Rg4
U 1 1 5AAA25CC
P 4300 2700
F 0 "Rg4" V 4380 2700 50  0000 C CNN
F 1 "1k" V 4300 2700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4230 2700 50  0001 C CNN
F 3 "" H 4300 2700 50  0001 C CNN
	1    4300 2700
	1    0    0    -1  
$EndComp
Text Notes 4250 2000 0    60   ~ 0
Amplification d'instrumentation\nG = 1 + 50kohm/RG = 50,1\nRG = 1kohm
Text Notes 600  2200 0    60   ~ 0
Poser sur un même muscle \npour capter les potentiels \nd'action associés \nà un mouvement.
$Comp
L AmpFil3a-rescue:LM324 U13
U 4 1 5AAA25D5
P 7100 2600
F 0 "U13" H 7100 2800 50  0000 L CNN
F 1 "LM324" H 7100 2400 50  0000 L CNN
F 2 "footprints.pretty.LM324DR-296-14597-1-ND:LM324DR" H 7050 2700 50  0001 C CNN
F 3 "" H 7150 2800 50  0001 C CNN
	4    7100 2600
	1    0    0    -1  
$EndComp
$Comp
L AmpFil3a-rescue:C C33
U 1 1 5AAA25DC
P 5900 2700
F 0 "C33" H 5925 2800 50  0000 L CNN
F 1 "2,2µF" H 5925 2600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 5938 2550 50  0001 C CNN
F 3 "" H 5900 2700 50  0001 C CNN
	1    5900 2700
	0    1    1    0   
$EndComp
$Comp
L AmpFil3a-rescue:R Ri3
U 1 1 5AAA25E3
P 6400 2700
F 0 "Ri3" V 6480 2700 50  0000 C CNN
F 1 "3,61k" V 6400 2700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 6330 2700 50  0001 C CNN
F 3 "" H 6400 2700 50  0001 C CNN
	1    6400 2700
	0    1    1    0   
$EndComp
$Comp
L AmpFil3a-rescue:R Rf3
U 1 1 5AAA25EA
P 6950 3150
F 0 "Rf3" V 7030 3150 50  0000 C CNN
F 1 "4,53k" V 6950 3150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 6880 3150 50  0001 C CNN
F 3 "" H 6950 3150 50  0001 C CNN
	1    6950 3150
	0    1    1    0   
$EndComp
$Comp
L AmpFil3a-rescue:GNDREF #PWR013
U 1 1 5AAA25F1
P 6500 2350
F 0 "#PWR013" H 6500 2100 50  0001 C CNN
F 1 "GNDREF" H 6500 2200 50  0000 C CNN
F 2 "" H 6500 2350 50  0001 C CNN
F 3 "" H 6500 2350 50  0001 C CNN
	1    6500 2350
	1    0    0    -1  
$EndComp
Text Notes 6450 4100 0    60   ~ 0
Filtre passe-haut actif\nordre 1\ntopologie classique\nw1 = 1/RiC\nGain à hautes fréquences (w >>w0) \nG = Rf/Ri = 2/A
$Comp
L AmpFil3a-rescue:R R52
U 1 1 5AAA25FF
P 9100 2600
F 0 "R52" V 9180 2600 50  0000 C CNN
F 1 "3.57k" V 9100 2600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 9030 2600 50  0001 C CNN
F 3 "" H 9100 2600 50  0001 C CNN
	1    9100 2600
	0    1    1    0   
$EndComp
$Comp
L AmpFil3a-rescue:LM324 U16
U 3 1 5AAA2606
P 9750 2700
F 0 "U16" H 9750 2900 50  0000 L CNN
F 1 "LM324" H 9750 2500 50  0000 L CNN
F 2 "footprints.pretty.LM324DR-296-14597-1-ND:LM324DR" H 9700 2800 50  0001 C CNN
F 3 "" H 9800 2900 50  0001 C CNN
	3    9750 2700
	1    0    0    -1  
$EndComp
$Comp
L AmpFil3a-rescue:C C34
U 1 1 5AAA260D
P 8900 2150
F 0 "C34" H 8925 2250 50  0000 L CNN
F 1 "100nF" H 8925 2050 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 8938 2000 50  0001 C CNN
F 3 "" H 8900 2150 50  0001 C CNN
	1    8900 2150
	1    0    0    -1  
$EndComp
$Comp
L AmpFil3a-rescue:GNDREF #PWR014
U 1 1 5AAA2614
P 9200 2250
F 0 "#PWR014" H 9200 2000 50  0001 C CNN
F 1 "GNDREF" H 9200 2100 50  0000 C CNN
F 2 "" H 9200 2250 50  0001 C CNN
F 3 "" H 9200 2250 50  0001 C CNN
	1    9200 2250
	1    0    0    -1  
$EndComp
$Comp
L AmpFil3a-rescue:R R53
U 1 1 5AAA261A
P 9900 3200
F 0 "R53" V 9980 3200 50  0000 C CNN
F 1 "2.05k" V 9900 3200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 9830 3200 50  0001 C CNN
F 3 "" H 9900 3200 50  0001 C CNN
	1    9900 3200
	0    1    1    0   
$EndComp
$Comp
L AmpFil3a-rescue:GNDREF #PWR015
U 1 1 5AAA2628
P 8550 3350
F 0 "#PWR015" H 8550 3100 50  0001 C CNN
F 1 "GNDREF" H 8550 3200 50  0000 C CNN
F 2 "" H 8550 3350 50  0001 C CNN
F 3 "" H 8550 3350 50  0001 C CNN
	1    8550 3350
	1    0    0    -1  
$EndComp
Text Notes 8700 4150 0    60   ~ 0
Filtre passe-bas actif\nordre 2\ntopologie Sallen-Key\nw2 = 1/RC\nQ = 1 / (3-A) = 0,707\nGain = A
Text Notes 7850 1450 0    60   ~ 0
Filtrage de la bande passante\nf1 = 20 Hz\nf2 = 500 Hz
Wire Wire Line
	2150 3000 2300 3000
Connection ~ 2150 2600
Wire Wire Line
	2150 2600 2150 3000
Connection ~ 3200 2500
Wire Wire Line
	2600 2000 3200 2000
Connection ~ 2150 2400
Wire Wire Line
	2150 2000 2300 2000
Wire Wire Line
	2150 2400 2150 2000
Wire Wire Line
	1900 2600 2150 2600
Wire Wire Line
	1900 2400 2150 2400
Wire Wire Line
	1250 2600 1600 2600
Wire Wire Line
	1250 2400 1600 2400
Wire Wire Line
	3200 3000 2600 3000
Wire Wire Line
	3200 3000 3200 2500
Wire Wire Line
	3200 2000 3200 2050
Wire Wire Line
	3000 2500 3200 2500
Wire Wire Line
	3850 2500 4000 2500
Wire Wire Line
	3900 2900 4000 2900
Connection ~ 4000 2500
Connection ~ 4000 2900
Wire Wire Line
	4300 2550 4450 2550
Wire Wire Line
	4450 2550 4450 2600
Wire Wire Line
	4300 2850 4450 2850
Wire Wire Line
	4450 2850 4450 2800
Wire Wire Line
	5150 2700 5450 2700
Wire Wire Line
	6050 2700 6250 2700
Wire Wire Line
	6550 2700 6650 2700
Wire Wire Line
	6650 2700 6650 3150
Wire Wire Line
	6650 3150 6800 3150
Connection ~ 6650 2700
Wire Wire Line
	7550 3150 7100 3150
Wire Wire Line
	7550 2600 7550 3150
Wire Wire Line
	7400 2600 7550 2600
Connection ~ 7550 2600
Wire Wire Line
	6800 2500 6700 2500
Wire Wire Line
	6700 2500 6700 2300
Wire Wire Line
	6700 2300 6500 2300
Wire Wire Line
	6500 2300 6500 2350
Wire Wire Line
	8850 2600 8900 2600
Wire Wire Line
	9250 2600 9350 2600
Wire Wire Line
	8900 2600 8900 2300
Connection ~ 8900 2600
Wire Wire Line
	8900 2000 8900 1800
Wire Wire Line
	8900 1800 10250 1800
Wire Wire Line
	10250 1800 10250 2700
Connection ~ 10250 2700
Wire Wire Line
	9350 2600 9350 2450
Wire Wire Line
	9350 2100 9200 2100
Wire Wire Line
	9200 2100 9200 2250
Connection ~ 9350 2600
Wire Wire Line
	9450 2800 9250 2800
Wire Wire Line
	9250 2800 9250 3200
Wire Wire Line
	9050 3200 9250 3200
Connection ~ 9250 3200
Wire Wire Line
	8750 3200 8550 3200
Wire Wire Line
	8550 3200 8550 3350
Wire Wire Line
	10250 3200 10050 3200
Wire Wire Line
	10050 2700 10250 2700
Connection ~ 5450 2700
Wire Wire Line
	5450 2700 5450 3150
Wire Wire Line
	5450 3150 5500 3150
Text GLabel 2600 2800 2    60   Output ~ 0
-3.3V
Text GLabel 2600 2200 2    60   Output ~ 0
+3.3V
Text GLabel 4750 3250 2    60   Output ~ 0
-3.3V
Text GLabel 4750 2400 2    60   Output ~ 0
+3.3V
Wire Wire Line
	4750 3000 4750 3250
Text GLabel 7000 2900 2    60   Output ~ 0
-3.3V
Text GLabel 7000 2300 2    60   Output ~ 0
+3.3V
Text GLabel 9650 3000 2    60   Output ~ 0
-3.3V
Text GLabel 9650 2400 2    60   Output ~ 0
+3.3V
Text HLabel 10400 2700 2    60   Input ~ 0
Mvt3a
$Comp
L AmpFil3a-rescue:Audio-Jack-3 J15
U 1 1 5AC31300
P 1000 1550
F 0 "J15" H 950 1725 50  0000 C CNN
F 1 "Audio-Jack-3" H 1100 1480 50  0000 C CNN
F 2 "Connectors:PJ31060-I_3.5mm_Jack" H 1250 1650 50  0001 C CNN
F 3 "" H 1250 1650 50  0001 C CNN
	1    1000 1550
	1    0    0    -1  
$EndComp
$Comp
L AmpFil3a-rescue:Conn_01x03 J14
U 1 1 5AC31307
P 850 1000
F 0 "J14" H 850 1200 50  0000 C CNN
F 1 "Conn_01x03" H 850 800 50  0000 C CNN
F 2 "Connectors:PINHEAD1-3" H 850 1000 50  0001 C CNN
F 3 "" H 850 1000 50  0001 C CNN
	1    850  1000
	-1   0    0    1   
$EndComp
Wire Wire Line
	800  1650 800  1700
Wire Wire Line
	800  1700 1350 1700
Text Label 1350 1700 0    60   ~ 0
Électrode(+)
Wire Wire Line
	1200 1450 1350 1450
Text Label 1350 1450 0    60   ~ 0
Électrode(-)
Wire Wire Line
	1200 1550 1350 1550
Text Label 1350 1550 0    60   ~ 0
Électrode(ref)
Text Label 1050 900  0    60   ~ 0
Électrode(+)
Text Label 1050 1100 0    60   ~ 0
Électrode(ref)
Text Label 1050 1000 0    60   ~ 0
Électrode(-)
Text Label 1250 2400 2    60   ~ 0
Électrode(+)
Text Label 1250 2600 2    60   ~ 0
Électrode(-)
Text Label 4850 3000 0    60   ~ 0
Électrode(ref)
Text HLabel 5500 3150 2    60   Input ~ 0
SignalAmplifié3a
Wire Wire Line
	4000 2900 4000 2850
Wire Wire Line
	4000 2500 4000 2550
$Comp
L AmpFil3a-rescue:R R49
U 1 1 5AC3635A
P 4000 2700
F 0 "R49" V 4080 2700 50  0000 C CNN
F 1 "3,6k" V 4000 2700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3930 2700 50  0001 C CNN
F 3 "" H 4000 2700 50  0001 C CNN
	1    4000 2700
	-1   0    0    1   
$EndComp
$Comp
L AmpFil3a-rescue:R R50
U 1 1 5AC369AC
P 8700 2600
F 0 "R50" V 8780 2600 50  0000 C CNN
F 1 "3,57k" V 8700 2600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 8630 2600 50  0001 C CNN
F 3 "" H 8700 2600 50  0001 C CNN
	1    8700 2600
	0    1    1    0   
$EndComp
$Comp
L AmpFil3a-rescue:R R51
U 1 1 5AC36A1A
P 8900 3200
F 0 "R51" V 8980 3200 50  0000 C CNN
F 1 "3,57k" V 8900 3200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 8830 3200 50  0001 C CNN
F 3 "" H 8900 3200 50  0001 C CNN
	1    8900 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	2150 2600 2400 2600
Wire Wire Line
	3200 2500 3550 2500
Wire Wire Line
	2150 2400 2400 2400
Wire Wire Line
	4000 2500 4450 2500
Wire Wire Line
	4000 2900 4450 2900
Wire Wire Line
	6650 2700 6800 2700
Wire Wire Line
	7550 2600 8550 2600
Wire Wire Line
	8900 2600 8950 2600
Wire Wire Line
	10250 2700 10250 3200
Wire Wire Line
	10250 2700 10400 2700
Wire Wire Line
	9350 2600 9450 2600
Wire Wire Line
	9250 3200 9750 3200
Wire Wire Line
	5450 2700 5750 2700
$Comp
L AmpFil3a-rescue:C C35
U 1 1 5BF615AA
P 9350 2300
F 0 "C35" H 9465 2346 50  0000 L CNN
F 1 "100nf" H 9465 2255 50  0000 L CNN
F 2 "" H 9388 2150 50  0001 C CNN
F 3 "" H 9350 2300 50  0001 C CNN
	1    9350 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 2150 9350 2100
$EndSCHEMATC
