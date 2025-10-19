Intoduction
---------------

To calculate Greenwich sidereal time and local sidereal time based on geographic longitude and date/time. 

The equations provided give a mathematical framework for these calculations, and the usage example demonstrates how to use this information in practice. 


# Greenwich sidereal time

$$ \theta_{g} = \theta_{go} +  \theta_{c} \cdot 2 \pi \cdot D$$

* $\theta_{g}$, is the Greenwich sidereal time, in radian
* $\theta_{go}$,is the Greenwich sidereal time the 1 january 1968, equal to 1.74046342 
* $\theta_{c}$, is days of mean sidereal time constant, equal to 1.0027379093
* $D$, is the number of days which have elapsed since 1 January 1968.

# Local Sidereal Time 

$$ \theta = \theta_{g} +  \lambda $$

* $\theta_{g}$, is the Greenwich sidereal time, in radian
* $\theta$, is the Local sidereal time, in radian
* $\lambda $, is the geographic longitude, in radian

# Usage

Demonstrates how to use this information in a command-line tool (**LSTime**):

```

$>LSTime --LNG  <ValueInRadians> --date <DD/MM/YYYY> --time <HH:MM:SS>
	Local Sidereal Time: 0.940583 @ LNG: 0 @ 22/04/2024 14:31:10
	
```
Here, it's calculating the local sidereal time for a specific geographic longitude (0 radians) and date/time (22nd April 2024, 14:31:10).
