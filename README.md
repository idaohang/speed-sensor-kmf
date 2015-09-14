# speed-sensor-kmf
This is native C android code, that uses data from the smartphone accelerometer and applies linear kalman filter to it to try and get the speed of the smartphone. 


I wrote it for research of android native developement and for learning kalman filter concepts.
It is basically impossible for this to work good for an interval longer than 0.5 secnods(yes, half a second).


Usage:


Download, compile and run the executable on an android phone through a shell, you'll get debug output. 
You can change the output easily and see make graphics with gnuplot or other software.
