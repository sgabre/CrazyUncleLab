
# Gauss

## P-iteration Method

($\vec{V_1}$, $\vec{V_2}$) := P-iteration Function ($\vec{R_1}$, $\vec{R_2}$, $\Delta t$  )

1. Evaluate the constant $k$, $l$, $m$ from $\vec{R_1}$, $\vec{R_2}$ and $\Delta \nu$ using equation 5.4.4
2. Determine the limits on the value $p$ by evaluating $p_i$ and $p_{ii}$ from equation 5.4.14 and 5.4.15
3. Pick up a trial value of $p$  within the appropriate limits
4. Using the trial value of $p$, solve for $a$ from equation 5.4.8 The conic type orbit will be know from the value of $a$
5. Solve $f$, $g$  and $\dot{f}$ from equation 5.2.3, 5.2.4 and 5.3.5
6. Solve $\Delta E$  or $\Delta F$ , as appropriate, using equation 5.4.9 and 5.4.10 or 5.4.11
7. Solve $t$ from equation 5.4.12 or 5.4.13 and compare it with the desired time-of-flight.
8. Adjust the trial value of $p$ using one of the iteration methods discussed above until the desired time-of-flight is obtained.
9. Evaluate $\dot{g}$ from equation 5.2.6 and then solve $\vec{V_1}$ and $\vec{V_2}$ using equations 5 .2.7 and 5.2.2.


The p-iteration method converges in all cases except when $\vec{R_1}$ and $\vec{R_2}$ are collinear. Its main disadvantage is that separate equations are used for the ellipse and hyperbola. This defect may be overcome by using the universal variables X and z introduced in Chapter 4 and discussed earlier in this chapter.

## Universal Variable Method


## Original Gauss Method


1. Compute the constants, $s$ and $w$, from $\vec{R_1}$, $\vec{R_2}$ and $\Delta \nu$ using equations (5.6-2) and (5.6-3).
2. Assume y = 1 and compute $X$ from equation (5.6-13).
3. Determine $X$ from equation (5.6-15) and use it to compute a better approximation to $y$ from equation (5.6-14).  Repeat this cycle until $y$ converges to a solution.
4. The type of conic orbit is determined at this point, the orbit being an ellipse, parabola, or hyperbola according to whether $X$ is positive, zero, or negative. Depending on the type of conic, determine $\Delta E$ or $\Delta F$ from equation (5.6-10) or (5.6-1 1).
5. Determine $p$ from equation (5.4-3)
6. Evaluate $f$, $g$,  $\dot{f}$ and $\dot{g}$ from equations (5.2-3), (5.2-4), (5.2-5) and (5.2-6).
7. Solve for $\vec{V_1}$ and $\vec{V_2}$ from equations (5.2-7) and (5.2-2).

 The method outlined above is perhaps the most accurate and rapid technique known for solving the Gauss problem when is less than 90°; the iteration to determine y fails to converge shortly beyond this point .
