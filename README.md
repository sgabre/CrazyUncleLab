Intoduction
---------------

Table of Contents
---------------

[TOC]

Prerequisites
---------------

The Crazy Uncle Lab project described in this article is hosted on my [github](https://git-scm.com/) page. 

Tools used in this project can be found under the following URLs:

* [git](https://git-scm.com/)
* [Git Credential Manager](https://github.com/git-ecosystem/git-credential-manager)
* [CMake](https://cmake.org/)
* [Unity](https://www.throwtheswitch.org/unity/)
* [CMock](https://www.throwtheswitch.org/cmock)
* [Ceedling](https://www.throwtheswitch.org/ceedling)
* [Exception](https://www.throwtheswitch.org/cexception)
* Compilators
* [Doxygen](https://www.doxygen.nl)
* [GraphViz](https://graphviz.org/download/)
* [Message Sequence Chart](https://www.mcternan.me.uk/mscgen/)

The architecture in this project is base on the Appendix D of the Fundamentals of Astrodynamics - Bate, Mueller, and White

The structure of the sky project is as follows:

```
.
|- main
|  \- (entry point of the main application)
|- LSTime
|  \- (Software Component sources)
|- Site
|  \- (Software Component sources)
|- Track
|  \- (Software Component sources)
|- Predict
|  \- (Software Component sources)
|- Kepler
|  \- (Software Component sources)
|- Gauss
|  \- (Software Component sources)
|- Intercept
|  \- (Software Component sources)
|- test
|  \- (Unit-Tests sources)
```

The Site, Track, Predict, Kepler, Gauss, Intercept module contains a library of shared code. 

In this project it is both referenced by the main application as well as the unit-tests.

The external directory contains the sources from external projects or other git submodules. 

The main directory contains the production code and the test directory contains the source of the unit-tests.

[LSTime Module](https://github.com/sgabre/CrazyUncleLab/blob/main/SoftwareProduct/LSTime/documentations/LSTime.md)
-----------------

The LSTime Module compute the local @ Greenwich sidereal time 

The Longitude and the local Date & Time is given.

Site/[Track](https://github.com/sgabre/CrazyUncleLab/blob/main/SoftwareProduct/Track/documentation/Track.md) Module
-----------------

The Site/Track Module compute the geocentric-equatorial components of position and velocity of the radar site (RS and VS), and of the satellite (R and V).

The geographic location of a radar tracking site on the surface of the earth is known. 

Its geodetic latitude, longitude, and altitude above mean sea level are specified. 

Observations of a satellite are made by a radar at this site at a specified date and universal time. 

The radar determines $\rho$, $\dot{\rho}$, $A_z$ , $\dot{A_z}$, $E$ , $\dot{E}$ from its tracking and doppler capability.

Note:

The Track module compute also the Horizontal components (Azimuth, Elevation) when the Right Ascension and the declination of a start is given.

[Orbit Module](https://github.com/sgabre/CrazyUncleLab/blob/main/SoftwareProduct/Orbit/documentation/Orbit.md) 
-----------------

This Module group the Predict Module and Kepler Module from the project description from the the Fundamentals of Astrodynamics - Bate, Mueller, and White - Appendix D

The Orbit Module find orbital parameters from given 
- vector position and velocity,
- Apoasis & Periapsis.
- determines the Estimate time of Arrival (ETA) at a position 
- determines the position and velocity vectors after an interval of time.
- The position and velocity vectors of a satellite at a particular instant of time is given

The Predict Module find:

a. The type of trajectory (circular, rectilinear, elliptical, parabolic, hyperbolic).
b. Position and velocity vectors at impact or closest approach in the geocentric-equatorial coordinate system.
c. Time for object to go from its observed position to point of impact or closest approach.
d. The total change in true anomaly from the observed position to impact or point of closest approach.

Kepler Module
-----------------
The Kepler Module determines the position and velocity vectors after an interval of time 

The position and velocity vectors of a satellite at a particular instant of time is given

Gauss Module
-----------------

The Gauss Module determines the departure & arrival velocity base on the "short way" or "long way".

The departure position and arrival position at expected time for a conic orbit is given.

Intercept Module
-----------------

The Intercept Module computes the impulsive velocity change required for both intercept and intercept­ plus-rendezvous for various combinations of launch time and interceptor time-of-flight. 

Neglect the atmosphere and assume impulsive velocity change from the launch site and at the target. 

The radar tracking data on a target satellite, location of the tracking site, time of radar observation, and the location of an interceptor launch site is given. 







``` shell
function test
{
	echo "notice the blank line before this function?"
}
```
