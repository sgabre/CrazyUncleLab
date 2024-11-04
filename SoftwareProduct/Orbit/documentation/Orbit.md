
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

## Orbit from state vectors 

When the position & velocity is given, to dermine the orbital elements, the following step are follow

1. Determine the angular momentum vector ($\vec{h}$)

$\vec{h} = \vec{R} \times \vec{V}$

2.  Determine the node vector ($\vec{n}$)

$\vec{n} = \vec{K} \times \vec{h} = - h_J \vec{I} + h_I \vec{J} $

3.  Determine the eccentricity vector ($\vec{e}$)

$\vec{e} = \frac{1}{\mu} \left ( \left ( v^2 - \frac{\mu}{r}\right ) \vec{r} - \left ( \vec{r} \cdot \vec{v} \right )\vec{v} \right )$

4.  Determine the inclination($i$)

 $\cos{i} = \frac{h_K}{|\vec{h}|}$

5.  Determine the longitude of the ascending node ($\Omega$)

$\cos{\Omega} = \frac{n_I}{|\vec{n}|}$

6.  Determine the argument of periapsis ($\omega$)

$\cos{\omega} = \frac{ \vec{n} \cdot \vec{e}}{|\vec{n}||\vec{e}|}$



7. Specific orbital energy

$\epsilon = -\frac{1}{2}\frac{\mu^2}{h^2}\left( 1-e^2\right)$

7.  Determine the semi-major axis ($a$)

$a = -\frac{\mu}{2\epsilon}$

8.  Determine the Semi-latus rectum ($p$)

$p = a (1 - e^2)$

9.  Determine the time of periapsis passage ($?$)



10. Determine the longitude of periapsis ($\Pi$)

$\Pi = \Omega + \omega$
 
12. Determine the true anomaly at epoch ($\nu_o$)

$\cos{\nu} = \frac{\vec{e}\cdot\vec{R}}{|\vec{e}| \cdot|\vec{R}|}$

if $\vec{R} \cdot \vec{V} < 0$ then replace $\nu = 2π − \nu$

13. Determine the argument of latitude at epoch ($U_o$)

$\cos{(U_o)} = \frac{\vec{n}\cdot\vec{R}}{|\vec{n}| \cdot|\vec{R}|}$

if $R_z < 0$ then replace $U_o = 2 \pi − U_o$

14. Determine the True longitude at epoch ($l_o$)

$\cos{(l_o)} = \frac{R_x}{|\vec{R}|}$

if $V_x > 0$ then replace $l_o = 2 \pi − l_o$

15. Determine the mean anomaly at epoch ($M_o$)


## Orbit from the 3 positions  vector

1. Check if it is Coplanar vector

$\vec{R_1} \cdot \vec{R_2} \times \vec{R_3} = 0$

2. Determine the Denominator Vector ($\vec{D}$)

$\vec{D} = (\vec{R_1} \times \vec{R_2} ) + (\vec{R_2}$

3. Determine the Numerator Vector ($\vec{N}$)

$\vec{N} = R_3(\vec{R_1} \times \vec{R_2} ) + R_1(\vec{R_2} \times \vec{R_3}) + R_2( \vec{R_3} \times \vec{R_1})$

4. Determine the S Vector ($\vec{S}$)

$\vec{S} = (R_2 - R_3)\vec{R_1}  +(R_3 - R_1)\vec{R_2} +(R_1 - R_2) \vec{R_3}$

5. Check if $D \neq 0$, $N \neq 0$ and $\vec{D} \cdot \vec{N} > 0$

6.  ?

$p = \frac{N}{D}$

7.  ?

$e = \frac{S}{D}$

9.  ?

$T = 2\pi \sqrt{\frac{a^3}{\mu}}$

10.  Determine the Q Vector ($\vec{Q}$)

$\vec{Q} = \frac{\vec{S}}{S} = - 0.963\vec{J} - 0.270\vec{K}$

11.  Determine the W Vector ($\vec{W}$)

$\vec{W} = \frac{\vec{N}}{N} $

12.  Determine the P Vector ($\vec{P}$)

$\vec{P} = \vec{Q} \times \vec{W} $

13.  Determine the B Vector ($\vec{B}$)

$\vec{B} = \vec{D} \times \vec{R_2} $

14. Determine the L Vector ($L$)

$L = \frac{1}{\sqrt{DN}}$

15. Determine the Velocity Vector($V_2$)

 $\vec{V_2} = \frac{L}{R_2} \vec{B} + L \vec{S} $


## Orbit from the 2 positions vector
