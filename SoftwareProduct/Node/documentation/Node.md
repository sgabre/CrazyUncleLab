
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





