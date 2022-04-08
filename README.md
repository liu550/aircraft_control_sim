# aircraft_control_sim

 The project is developed for the purpose of acquainting myself with basic aircraft control knowledge.
 
 ## Notes
 Embarrassing typos in the plots below......(Should be "roll")
 
 ## Functionalities
 
 The project consists of the following parts:
 1. An arithmetic expression parser that converts mathematical expressions into tree structures and stores them in a variables lookup table;
 2. The representation of a state model with RK4 integration method that calculates the dynamics of a given state model;
 3. An implementation of algebraic Riccati equation solver that optimizes the stability of a linear state model.

## Demonstration

### Overview
The Jupyter Notebook file `plot.ipynb` inside the python folder showcases the following lateral/directional-mode state evolutions implemented with the program:
1. Rolling mode stability;
2. Spiral mode stability;
3. Dutch roll stability;
4. Trimming of the aircraft undergoing a unit perturbation in sideslip, with different penalty parameters for the control vector.

The configuration and dimensionless aerodynamic coefficients data are of Boeing 747, obtained from the [aircraft handling qualities data reported by NASA](https://ntrs.nasa.gov/citations/19730003312). The configuration used is power approach configuration.

The following compare the results calculated by the program versus the results used by Professor Caughey in his [lecture notes on aircraft control](https://courses.cit.cornell.edu/mae5070/Caughey_2011_04.pdf):

### Lateral/directional modes
#### Results
<p float="left">
 <img src="https://github.com/liu550/aircraft_control_sim/blob/main/assets/images/ss-rolling-mode.png" width="400" height="300">
 <img src="https://github.com/liu550/aircraft_control_sim/blob/main/assets/images/ss-spiral-mode.png" width="400" height="300">
 <img src="https://github.com/liu550/aircraft_control_sim/blob/main/assets/images/ss-dutch-roll.png" width="400" height="300">
</p>

#### Professor Caughey's reference results
<img src="https://github.com/liu550/aircraft_control_sim/blob/main/assets/images/ss-caughey-lat:dir-modes.png">

### Trim
#### Results
<p float="left">
 <img src="https://github.com/liu550/aircraft_control_sim/blob/main/assets/images/ss-trim-c_u=0.00001.png" width="400" height="300">
 <img src="https://github.com/liu550/aircraft_control_sim/blob/main/assets/images/ss-trim-c_u=0.1.png" width="400" height="300">
 <img src="https://github.com/liu550/aircraft_control_sim/blob/main/assets/images/ss-trim-c_u=2.png" width="400" height="300">
</p>

#### Professor Caughey's reference results
<img src="https://github.com/liu550/aircraft_control_sim/blob/main/assets/images/ss-caughey-lat:dir-trim.png">

### Notes
The state and control vector explodes when setting penalty parameter (c_u) to 0.5. Likely a problem of the Riccati solver.

### Summary
The differences between the results produced by the program and professor Caughey's results possibly come from different values of dimensional derivatives and different control vectors found by the optimizers. Despite the differences, the program is able to demonstrate (1) the characteristics of the free responses of the aircraft in the three lateral/directional modes and (2) that the pertubation is more heavily damped when using a higher penalty parameter for the control vector during trimming.

## Future work
1. Improve the software architecture, mainly by making `Variable` class a derived class of `MathExpressionElement`;
2. Implement Simplex algorithm to trim non-linear models.
3. Increase the complexities of the state and control matrices and benchmark the results against the references provided by NASA.
