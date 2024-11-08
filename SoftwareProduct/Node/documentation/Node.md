
# Node

This module is used to define the change velocity need for a manoeuvre  
-  Hohmann transfer
-  Bi-elliptic transfer
-  One-tangent Burn transfer
-  Apsis Change (Apoapsis or Periapsis)
-  Cirularization
-  Change Inclinasion
-  Resonant Orbit
-  Geosyncronious Equatorial transfer
-  Interplanetary Transfer
-  Moon Return
-  Ascent/Descent
-  Launch
-  Gravity Assist

## Hohmann transfer

1. Transfer Orbit Semi-Major Axis
   
$a_{tx} = \frac{R_o + R_t}{2}$

2. Velocity of the Origin Orbit @ the Origin Burn Point

$V_o = \sqrt{\frac{\mu}{R_o}}$

3. Velocity of the Transfer Orbit @ the Origin Burn Point

$V_{txo} = \sqrt{\mu \left(\frac{2}{R_B} -\frac{1}{a_{tx}}\right )}$

4. Velocity of the Target Orbit @ the Target Burn Point

$V_t = \sqrt{\frac{\mu}{R_t}}$

5. Velocity of the Transfer Orbit @ the Target Burn Point

$V_{txt} = \sqrt{\mu \left(\frac{2}{R_t} -\frac{1}{a_{tx}}\right )}$

6. Velocity Change @ the Origin Burn Point

$\Delta V_o = V_{txo} - V_o$

7. Velocity Change @ the Target Burn Point

$\Delta V_t = V_t - V_{txt}$

8. Total Velocity Change 

$\Delta V_T = \Delta V_o + \Delta V_t$

## One-tangent Burn transfer

1. Transfer Semi-Major Axis

$a_{tx} = \frac{R_o + R_{txa}}{2}$

2. Transfer eccentricity

$e_{tx} = 1 - \frac{R_o}{a_{tx}}$

3. Transfer semi-latus rectum

$p_{tx} = a (1-e_{tx}^2)$

4. True Anomaly @ Second Burn

$\cos{\nu} = \frac{p_{tx}-R_t}{e_{tx} \cdot R_t}$

5. Flight-path Angle @ Second Burn

$\tan{\phi} = \frac{e \sin{\nu}}{1 + e \cos{\nu}}$

2. Velocity of the Origin Orbit @ the Origin Burn Point

$V_o = \sqrt{\frac{\mu}{R_o}}$

3. Velocity of the Transfer Orbit @ the Origin Burn Point

$V_{txo} = \sqrt{\mu \left(\frac{2}{R_B} -\frac{1}{a_{tx}}\right )}$

6. Transfer velocity @ Target

$V_{txt} = \sqrt{\mu \left(\frac{1}{R_t} - \frac{2}{a_{tx}}\right )}$

7. Velocity @ Target

$V_t = \sqrt{\frac{\mu}{R_t}}$

8. Final velocity @ Second Burn

$\Delta V_t = \sqrt{V_{txt}^2+V_{t}^2-2\cdot V_{txt} \cdot V_{t} \cdot\cos{\phi}}$

6. Velocity Change @ the Origin Burn Point

$\Delta V_o = V_{txo} - V_o$

8. Total Velocity Change 

$\Delta V_T = \Delta V_o + \Delta V_t$

## Apsis Change (Apoapsis)

1. Origin Semi-Major Axis

$a_{o} = \frac{R_{ao} + R_{po}}{2}$

3. Transfer Semi-Major Axis

$a_{tx} = \frac{R_{ao} + R_{pt}}{2}$

4. Orbital Velocity @ the Origin Burn Point

$V_{o} = \sqrt{\mu \left(\frac{2}{R_{ao}} -\frac{1}{a_{o}}\right )}$

5. Transfer velocity @ the Origin Burn Point

$V_{txt} = \sqrt{\mu \left(\frac{1}{R_{ao}} - \frac{2}{a_{tx}}\right )}$

6. Velocity Change @ the Origin Burn Point

$\Delta V_o = V_{txo} - V_o$


## Apsis Change (Periapsis)

1. Origin Semi-Major Axis

$a_{o} = \frac{R_{ao} + R_{po}}{2}$

3. Transfer Semi-Major Axis

$a_{tx} = \frac{R_{at} + R_{po}}{2}$

4. Orbital Velocity @ the Origin Burn Point

$V_{o} = \sqrt{\mu \left(\frac{2}{R_{po}} -\frac{1}{a_{o}}\right )}$

5. Transfer velocity @ the Origin Burn Point

$V_{txt} = \sqrt{\mu \left(\frac{1}{R_{po}} - \frac{2}{a_{tx}}\right )}$

6. Velocity Change @ the Origin Burn Point

$\Delta V_o = V_{txo} - V_o$

## Ascent/Descent Burn (Draft)

Note:
- Launch Site impact the Sidereal rotation velocity
- Azimuth Launch impact the Total Change Velocity

1. Hohmann transfer

$\Delta V_o, \Delta V_t$ := Hohmann($R_B$,$R_t$)

1. Orbital Velocity a Surface 

$V_B = \sqrt{\frac{\mu}{R_B}}$

2. Sidereal rotation velocity

$V_{\omega_B} = \frac{2 \pi R_B}{T_\omega}$

8. Total Velocity Change 

$\Delta V_T = \Delta V_o + \Delta V_t + V_B - V_{\omega_B}$

## Balistic (Draft)

Given, Launch Site (Latitude (Lo), Longitude (No), Altitude), Target Site (Latitude, Longitude, Altitude), Max Altitue/Apoapsis

1. Pre-condition

a. Determine the distance orthodromique/loxodromie

b. Equation (6.4.6)

$\Delta N = N_t - N_o $

c. Equation (6.4.6)

$\cos{\Lambda} = \sin{L_o}\sin{L_t} + \cos{L_o}\cos{L_t} \cos{(\Delta N + \omega_B t_{\Lambda})}$

d. Equation (6.4.8)

$\cos{\beta} = \frac{\sin{L_t} - \sin{L_o} \cos{\Lambda}} {\cos{L_o}\sin{\Lambda}}$

2.  Velocity Change

3. 


