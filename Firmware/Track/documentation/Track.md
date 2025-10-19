
# Track

From a Celestial Coordinate object (e.g Start), the Track module compute the Horizontal coordinate (Azimuth & Elevation).

The Track Module compute the geocentric-equatorial components of position and velocity of the satellite (R and V).

The geographic location of a radar tracking site on the surface of the earth is known.

Its geodetic latitude, longitude, and altitude above mean sea level are specified.

Observations of a satellite are made by a radar at this site at a specified date and universal time.

The radar determines $\rho$, $\dot{\rho}$, $A_z$ , $\dot{A_z}$, $E$ , $\dot{E}$ from its tracking and doppler capability.

Note:

The Track module compute also the Horizontal components (Azimuth, Elevation) when the Right Ascension and the declination of a start is given.

# Usage

Tracker 
```shell
Track --type start --asc <**ValueInRadians**> --dec <**ValueInRadians**>

Track --type radar --range <**Value**> --elev <**ValueInRadians**> --azm <**Value**> --rrate <**ValueInMeterPerSeconds**> --erate <ValueInRadianPerSeconds**> --arate <ValueInRadianPerSeconds**>

Track --ifile <file.> 
```
# Start Tracking

## Local hour angle 

The local hour angle (LHA) of an object in the observer's sky is

$$ H = \theta - \alpha$$

where,

&Theta; , is the Local Sidereal Time, in radians

&alpha; , is is the object's Right ascension, in radians


## Elevation

The elevation is an angle between -90° et +90°.
The elevation is computed with the following formula: 

$$ E = \arcsin{\left(\sin{(\delta)} \cdot \sin{(\phi)} - \cos{(\delta)}\cdot \cos{(\phi)}\cdot \cos{(H)}\right )} $$

where,

* $E$, is the elevation angle, in radians
* $\phi$, is the geographic latitude of the observation site, in radians
* $\delta$, is the celestial Déclinaison of the celesital object site, in radians
* $H$, is the local hours angle, in radians

  
## Azimuth

The azimutis an angle between 0° et 360°.

$$ \alpha = \arccos{\left ( \frac{\sin{(\delta)}-\sin{(\phi)}\cdot\sin{(h)}}{\cos{(\phi)} \cdot\cos{(E)}}\right )} $$


$$ \sin{(\alpha)} =  \frac{\cos{(\delta)}\cdot\sin{(H)}}{\cos{(E)}} $$

If $\sin{(\alpha)} <=0$<br>
Then  $azimuth = -azimuth$

* $\phi$, is the azimuth angle, in radians
* $\phi$, is the geographic latitude of the observation site, in radians
* $\delta$, is the celestial Déclinaison of the celesital object site, in radians
* $H$, is the local hours angle, in radians
* $E$, is the elevation angle, in radians

# Radar Tracking

## Topocentric Position Vector
The position relative to the radar site is

$P_S = - \rho \cdot \cos{(E)}\cdot \cos{(\alpha)}$

$P_E = \rho \cdot \cos{(E)}\cdot \sin{(\alpha)}$

$P_E = \rho \cdot \sin{(E)}$

## Topocentric Velocity Vector

The velocity relative to the radar site is 

$V_S = \dot{\rho} \cdot \cos{(E)}\cdot \cos{(\alpha)}+\rho \cdot \dot{E} \cdot  \sin{(E)} \cdot \cos{(\alpha)}$

$V_E =$

$V_E =$


# Transformation Matrix

$D_{11} = \sin{(\lambda)}\cdot \cos{(\theta)}$

$D_{12} = \sin{(\Theta)}$

$D_{13} = \cos{(\lambda)}\cdot \cos{(\theta)}$
	
$D_{21} = \sin{(\lambda)}\cdot \sin{(\theta)}$

$D_{22} = \cos{(\Theta)}$

$D_{23} = \cos{(\lambda)}\cdot \sin{(\theta)}$

$D_{31} = -\cos{(\lambda)}$

$D_{32} = 0$

$D_{33} = \sin{(\lambda)}$

## Position Conversion from Topocentric to geocentric

$P_I = D_{11} \cdot P_S + D_{12} \cdot P_E +  D_{13} \cdot P_Z$

$P_J = D_{21} \cdot P_S + D_{22} \cdot P_E +  D_{23} \cdot P_Z$

$P_K = D_{31} \cdot P_S + D_{32} \cdot P_E +  D_{33} \cdot P_Z$

## Velocity Conversion from Topocentric to geocentric
	
$V_I = D_{11} \cdot V_S + D_{12} \cdot V_E +  D_{13} \cdot V_Z$

$V_J = D_{21} \cdot V_S + D_{22} \cdot V_E +  D_{23} \cdot V_Z$

$V_K = D_{31} \cdot V_S + D_{32} \cdot V_E +  D_{33} \cdot V_Z$

	