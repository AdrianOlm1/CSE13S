Descripton: This program calculates the coolness of something, it can be used with 0 arguments, 1 arguments(temperature), and 2 arguments (temperature windSpeed). The way it works is there's a function for the given equation and 3 if statments for each of the conditons being how many arguments are there. With 0 arguments, the temperature uses values from -10 to 40 on a scale of 10, and windSpeed uses values of 5 to 15 on a scale of 5. With 1 arugment, always, being temperature, it outputs values of coolness using the same temperature but windSpeed once again scales from 5 to 15 in increments of 5. With 2 arguments, the program runs as expected, just outputting the coolness with the given values. The make file is a set of commands that the bash executes. The bash script starts the make file with updates the executable that is made from the coolness.c code given.

Usage: To run type "bash -v testing.sh >& testing.out", input the numbers you want in testing.sh. The values will then print in testing.out. To use through console, type ./coolness in the directory