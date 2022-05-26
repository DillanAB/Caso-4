#ifndef _CROMODISTRIBUTION_
#define _CROMODISTRIBUTION_ 1

#define CROMO_MAX_VALUE 65536
#define NIBBLE_SIZE 16 //bits

using namespace std;

struct cromodistribution {
    int xMin, xMax;
    int yMin, yMax;
    float minValue; //Nibble
    float maxValue; //Nibble
    int red, green, blue;
    string shape;
    float maxProbability; //Prcentaje segun cantidad
    int quantity; //Cantidad que sostengo
    //int * totalPopulation; //Para dividirlo entre canti
};


#endif
