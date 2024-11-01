
# Orbit

 The Orbit Module find orbital elements from given 
- vector position and velocity,
- Apoasis & Periapsis.

The Orbit Module determines
- the Estimate time of Arrival (ETA) at a position 
- the position and velocity vectors after an interval of time, at a particular instant of time is given.

The Orbit Module find:

a. The type of trajectory (circular, rectilinear, elliptical, parabolic, hyperbolic).
b. Position and velocity vectors at impact or closest approach in the geocentric-equatorial coordinate system.
c. Time for object to go from its observed position to point of impact or closest approach.
d. The total change in true anomaly from the observed position to impact or point of closest approach.

Given the position and velocity vectors of a object at a particular instant of time, you are to determine the position and velocity vectors after an interval of time, $\Delta t$ . 
To solve this problem use the universal variable approach and refer closely to section 4.4 of the text for guidance. 
Use a Newtonian iteration to solve for the value of x corresponding to the given $\Delta t$. 

The Module use a special procedure to calculate the functions S(z) and C(z).

To determine when convergence has occurred, use

$\left | \frac{\Delta t - \Delta t_n}{\Delta t}  \right | \geq 10^{-7}$

where $\Delta t$ is the given time interval and it is compared to the calculated $\Delta t_n$. 

The Algorithm for the prediction need to

a. Put upper and lower bounds on z determined by the computer being used. When these bounds are violated you must reduce the stepsize in the Newton iteration such that you go in the correct direction but stay within bounds.
b. If $\Delta t$ is greater than the period (if it is an ellipse) the algorith should reduce the flight time an integral number of periods.
c. Also for an ellipse, x should never be greater than $2 \pi \sqrt{a}$
d. The sign of $\Delta t$ should always be the same as the sign of x.

On debugging mode, the values of x, $\Delta t$  (calculated), and dt/dx for each iteration are logs. 
If the algorithm do not converge in 50 iterations, a message is display/logs and go to the next data set. 

When the position & velocity is given, to dermine the orbital elements, the following step are follow
1. Determine the angular momentum vector ($\vec{h}$)
2.  Determine the node vector ($\vec{n}$)
3.  Determine the eccentricity vector ($\vec{e}$)
4.  Determine the inclination($i$)
5.  Determine the longitude of the ascending node ($\Omega$)
6.  Determine the argument of periapsis ($\omega$)
7.  Determine the semi-major axis ($a$)
8.  Determine the Semi-latus rectum ($p$)
9.  Determine the time of periapsis passage ($?$)
10. Determine the longitude of periapsis ($\Pi$)
11. Determine the true anomaly at epoch ($\nu_o$)
12. Determine the argument of latitude at epoch ($u_o$)
13. Determine the True longitude at epoch ($l_o$)
14. Determine the mean anomaly at epoch ($M_o$)

