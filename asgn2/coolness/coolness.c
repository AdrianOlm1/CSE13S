#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Declared coolnessMath function to be defined later
void coolnessMath (double temp, double windSpeed);

int main(int argc, char **argv[]) {
    if(argc>3) {
        printf("Usage: ./coolness [temperature] [wind speed]\n");
        return 1;
    }
    
    // Checks for 0 arguments
    if(argc==1) {

        // Prints the header of the output
        printf("Temp\tWind\tCoolness\n");

        // Goes up variable temp from -10 to 40 in increments of 10
        for(int temp=-10;temp<=40; temp+=10 ) {
            // Goes up variable wind from 5 to 15 in increments of 5
            for(int wind=5;wind<=15; wind+=5) {
                coolnessMath(temp,wind);
                printf("\n");
            }
            if(temp!=40)
                printf("\n");

        }
        return 0;
    }

    // Intializes the first argument as temp2
    double temp2 = atof((const char*)argv[1]);

    // Checks for acceptable input values of temp
    if((temp2<-99)||(temp2>50)) {
        printf("Error: Coolness. Acceptable input values are -99<=T<=50 and 0.5<=V.\n");
        return 1;
    }

    // Checks if there is 1 argument
    if(argc==2) {

        // Prints the header of the output
        printf("Temp\tWind\tCoolness\n");

        // Goes up variable wind from 5 to 15 in increments of 5
        for(int wind=5;wind<=15;wind+=5) {
            coolnessMath(temp2,wind);

            if(wind!=15) {
                printf("\n");
            } 
        }
        printf("\n");
        return 0;
    }

    // Initalizes second argument as windSpeed2
    double windSpeed2 = atof((const char*)argv[2]);

    // Checks for acceptable input values of windSpeed
    if(windSpeed2<.5) {
        printf("Error: Coolness. Acceptable input values are -99<=T<=50 and 0.5<=V.\n");
        return 1;
    }

    // Checks for two arguments
    if(argc==3) {

        // Prints the header of the output
        printf("Temp\tWind\tCoolness\n");

        // Initalizes first argument as temp 2 and second argument as windSpeed2
        double temp2 = atof((const char*)argv[1]);
        double windSpeed2 = atof((const char*)argv[2]);

        coolnessMath(temp2,windSpeed2);
        printf("\n");
    }

    return 0;
}

// Defines coolnessMath function
void coolnessMath(double temp, double windSpeed) {
    double coolness;
    
    // Computes formula given and puts it in the double variable coolness
    double exp= pow(windSpeed,0.16);
    coolness= 35.74 + (.6215*temp) - (35.75*(exp)) + ((.4275*temp)*(exp));
    
    // Prints the temperature, windspeed, and coolness, with margin sizes of 8, 1 decimal place, and to the left
    printf("%.1f\t%.1f\t%.1f", temp, windSpeed, coolness);

}
