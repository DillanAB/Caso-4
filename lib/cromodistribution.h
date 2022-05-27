#ifndef _CROMODISTRIBUTION_
#define _CROMODISTRIBUTION_ 1

#define CROMO_MAX_VALUE 65536
#define NIBBLE_SIZE 16 //bits
#include <iostream>
using namespace std;

struct cromodistribution {
    int xMin, xMax;
    int yMin, yMax;
    int minValue; //Nibble
    int maxValue; //Nibble
    int red, green, blue;
    string shape;
    float maxProbability; //Porcentaje segun cantidad
    int quantity; //Cantidad que sostengo
    //int * totalPopulation; //Para dividirlo entre canti

    void printInfo(){
        cout<<"xMin = "<<xMin<<" | yMin = "<<yMin<<" | ";
        cout<<"minValue = "<<minValue<<" | maxValue = "<<maxValue<<" |"<<endl;
    }
};


#endif
