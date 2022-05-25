#include <bits/stdc++.h>
#define TOTAL_PIXELS  300000.0 //Cantidad de pixeles que deseo buscar.Colocarle el .0
#define QUADRANT_QUANTITY 32400 //180 X 180
#define TOTAL_PIXELES_COPY 349920.0 //El 30% de la img
#define QUADRANT 180 //Longitud que tendrá cada cuadrante
#define CONTROLROW 6 //Cuadrantes x fila 
#define NUMBER_OF_QUADRANT 30 //6*5 restarle una para que no se sobrepase
#define RANGE 25
#define IMAGE_NAME "persona_2.jpg"
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include <iostream>
#include "geneticbase.h"
#include "lib/cromodistribution.h"
#include <vector>
#include "lib/individual.h"
#include "probabilistic.h"
#include <ctime>
#include <time.h>

using namespace std;

#define DISTRIBUTION_SIZE 3 //Distribucion de 3 elementos

double control = 0;

//Crea la tabla inicial. 180 x 180. Asignamos un pixel random en la posicion para que sirva de base 
//A la hora de comprar el color

vector<TableRow> createTable(){
    vector<TableRow> table;
    TableRow newRow;
    //Para moverse en la imagen
    int widthIncrease = QUADRANT; int heightIncrease = QUADRANT;
    int widthDecrease = 0; int heightDecrease = 0;
    int controlRow = 1; //Avisa cuando llega al final de una fila

    for( int indexQuadrant = 0; indexQuadrant < NUMBER_OF_QUADRANT; indexQuadrant++){
        //Creamos las filas con el tamaño deseado
        newRow.setInitialPosition(widthDecrease, widthIncrease);
        newRow.setFinalPosition(heightDecrease, heightIncrease);
        widthIncrease += QUADRANT; widthDecrease += QUADRANT; //x
        table.push_back(newRow);
        //Para el control de las filas
        if (controlRow == CONTROLROW && indexQuadrant != NUMBER_OF_QUADRANT ){
            controlRow = 1;
            widthDecrease = 0; widthIncrease = QUADRANT; //x vuelve a los valores predeterminados
            heightDecrease += QUADRANT; heightIncrease += QUADRANT; //Para que y se mueva
        }
        controlRow ++;        
    }

    return table;
}


//Colocamos una condicion en la funcion find indicando que si el prcentaje más alto es el 0, 
//haga un random de las 30 
int findPercentage(vector<TableRow> pTable, int indexControl){
    float max = 0;

    for(int indexTable = 0; indexTable < pTable.size(); indexTable++){
        if(pTable[indexTable].getPercentage() > max && indexTable != indexControl){
            max = indexTable;
        }
    }
    //Si el porcentaje es = 0, se elige cualquier cuadrante.
    if (pTable[max].getPercentage() == 0){ 
        max = rand()%NUMBER_OF_QUADRANT;
    }
    return max;
}

bool isInRange(int pRed, int pGreen, int pBlue, Pixel pPixel) {
    if (pRed >= pPixel.getColorRed() && pRed <= pPixel.getColorRed() + RANGE) {
        if (pGreen >= pPixel.getColorGreen() && pGreen <= pPixel.getColorGreen() + RANGE) {
            if (pBlue >= pPixel.getColorBlue() && pBlue <= pPixel.getColorBlue() + RANGE) {
                return true;
            }
        }
    }
    return false;
}

Pixel createNewPixel(TableRow pRow){
    Pixel pixel;
    int getXPosition, getXFinalPosition, getYPosition, getYFinalPosition;
    int xPixelPosition, yPixelPosition;

    getXPosition = pRow.getInitialPositionOne();
    getXFinalPosition = pRow.getInitialPositionTwo();
    getYPosition = pRow.getFinalPositionOne();
    getYFinalPosition = pRow.getFinalPositionTwo();

    //Creamos un pixel aleatorio para añadirlo a la lista de pixeles de la fila
    xPixelPosition = getXPosition + rand()% (getXFinalPosition - getXPosition);
    yPixelPosition = getYPosition + rand()% (getYFinalPosition - getYPosition);
    pixel.setPosition(xPixelPosition,yPixelPosition);
    return pixel;
}

/*
La función consiste en crear 30 cuadrantes de 180 x 180.
Cuando inicia la funcion todos los cuadrantes inician con un porcentaje de 0, por lo que se 
elige al azar el primero.

Luego se guia el random buscando los porcentajes de los cuadrantes más altos.
El porcentaje de un cuadrante se rige por hacer match con el color del primer pixel que se eligió al inicio
Si hace match el porcentaje aumenta y si no, se disminuye su porcentaje. Pero el pixel igual se añade
a la lista de pixeles de esa fila. El porcetaje se disminuye considerablemente si ese cuadrante ya
se visitó un 40% del total de los pixeles de ese cuadrante. Le da oportunidad de revisar otros cuadrantes.

El problema es que dura demasiado en revisar el 40% de la imagen. Ver si se puede mejorar la búsqueda
del mejor porcentaje de los cuadrante.
*/

void samplingFunction(vector<TableRow> pTable){
    int width, height, channels, indexControl;
    unsigned char *image = stbi_load(IMAGE_NAME, &width, &height, &channels, 0);
    const size_t RGB = 3;

    int red, green, blue, actualAmount, currenyAmount;
    int rowHighestPercentage; 
    Pixel pixel;
    float percentage;
    
    for(int pixelIndex = 0; pixelIndex < TOTAL_PIXELS; pixelIndex++){

        rowHighestPercentage = findPercentage(pTable,indexControl);
        indexControl = rowHighestPercentage;
        //Creamos pixel y añadimos su color
        pixel = createNewPixel(pTable[rowHighestPercentage]);
        size_t index = RGB * (pixel.getPositionY() * width + pixel.getPositionX());
        red = static_cast<int>(image[index + 0]);
        green = static_cast<int>(image[index + 1]);
        blue = static_cast<int>(image[index + 2]);
        pixel.setColorRGB(red, green, blue);
        //cout<<"iNDEX: "<<rowHighestPercentage<<endl;
        currenyAmount = pTable[rowHighestPercentage].getCurrentAmount();
        actualAmount = pTable[rowHighestPercentage].getActualAmount();
        if(pTable[rowHighestPercentage].getPercentage() == 0){
            pTable[rowHighestPercentage].setCurrentAmount(1);
            pTable[rowHighestPercentage].setPixel(pixel); //Añade el pixel que servira para comparar su color
        }else{
            //Hacemos comparación de color
            if(isInRange(red,green,blue,pTable[rowHighestPercentage].getPixel())){
                if(actualAmount >= QUADRANT_QUANTITY*0.40){
                    pTable[rowHighestPercentage].setCurrentAmount(-currenyAmount);
                }
                else{
                    pTable[rowHighestPercentage].setCurrentAmount(1);
                }
            }else{
                pTable[rowHighestPercentage].setCurrentAmount(-1);
            }
        }
        pTable[rowHighestPercentage].setActualAmount(1);
        percentage = pTable[rowHighestPercentage].getCurrentAmount()/TOTAL_PIXELS;
        pTable[rowHighestPercentage].setPercentage(percentage);
        pTable[rowHighestPercentage].setPixelVector(pixel);
    }

    for(int j = 0; j< pTable.size();j++){
        cout<<"Cuadrante: "<<j<<" Canti de pixeles: "<<pTable[j].getVectorSize()<<endl;
    }
}

int main() {


samplingFunction(createTable());



    // // class creation
    // GeneticBase genetic;

    // // 1. Chromosomatic representation
    // cromodistribution distribution[DISTRIBUTION_SIZE] = {
    //     {"Ojos Azules", 0, 99, 0.00, 0.39, 400, 1000},
    //     //va de 0 a 99(los bits que llegan hasta el 256), 40% son 400 de 1000 (cantidad de individuos)
    //     {"Ojos Negros", 100, 199, 0.39, 0.78, 400, 1000},
    //     {"Ojos Verdes", 200, 255, 0.78, 1.00, 200, 1000}
    // };


    // for(int i=0; i<DISTRIBUTION_SIZE; i++) {
    //     genetic.addDistribution(&distribution[i]);
    // }


    // // 2. Initial population of individuals
    // //Que inicie con una población de 10
    // //La poblacion que se tiene actual se limpia por completo
    // //Otros detalles
    // genetic.initPopulation(10);

    // cout << "initial population" << endl;
    // vector<individual*> result = genetic.getPopulation();
    // for(int i=0; i<result.size(); i++) {
    //     cout << result.at(i)->getCromosoma() << endl;
    // }

    // // 3. measure the fitness of the population and reproduce it until reach the target generations
    // //Aquí lo que pasa es:
    // //Cantidad de generaciones y cuántos hijos quiero por generación
    // //Sigue explicacion con produceGenerations
    // genetic.produceGenerations(1, 40);

    // // check final population
    // cout << "final population" << endl;
    // result = genetic.getPopulation();
    // for(int i=0; i<result.size(); i++) {
    //     cout << result.at(i)->getCromosoma() << endl;
    // }

}