#ifndef _CROMODISTRIBUTION_
#define _CROMODISTRIBUTION_ 1

#define CROMO_MAX_VALUE 256
#define NIBBLE_SIZE 8 //8 bits 8 a la 8 

using namespace std;

struct cromodistribution {
    string name;
    int minValue;
    int maxValue;
    float minProbability;
    float maxProbability;
    int quantity; //Cantidad que sostengo
    int totalPopulation; //Para dividirlo entre canti
};


#endif
