#define TOTAL_PIXELS  30000.0 //Cantidad de pixeles que deseo buscar.Colocarle el .0
#define QUADRANT_QUANTITY 1600 //40 x 40
#define QUADRANT 40 //Longitud que tendrá cada cuadrante
#define CONTROLROW 27 //Cuadrantes x fila 
#define NUMBER_OF_QUADRANT 702 //27*26 restarle una para que no se sobrepase
#define RANGE 25 //Rango de color que acepta
#define IMAGE_NAME "persona_3.jpg"
#define CONTROL_DARK_GRAY 180
#define SAMPLERATE 0.15

#define CONTROL_PORCENTAGE_CROMO 0.50


#define STB_IMAGE_IMPLEMENTATION

#include "main.hpp"

using namespace std;

#define DISTRIBUTION_SIZE 702 //Distribucion de 3 elementos

double control = 0;

//Crea la tabla inicial. 40 x 40. Asignamos un pixel random en la posicion para que sirva de base 
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

//Esta función lo que me determina es si el color está en el rango de los grises más oscuros
//Definimos el el rango de 0 a 180 para los grises más oscuros mayor a esto será un gris más claro
bool isInRange (int pRed, int pGreen, int pBlue) {
    int control = (0.2299 * pRed)+(0.587 * pGreen) + (0.114 * pBlue);  
    if(control <= CONTROL_DARK_GRAY){
        return true;
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

    //Creamos un pixel aleatorio para añadirlo a la pList de pixeles de la fila
    xPixelPosition = getXPosition + rand()% (getXFinalPosition - getXPosition);
    yPixelPosition = getYPosition + rand()% (getYFinalPosition - getYPosition);
    pixel.setPosition(xPixelPosition,yPixelPosition);
    return pixel;
}

vector<TableRow> sorting( vector<TableRow> pList){
    for( int i = 0; i < pList.size(); i++ ){
        bool insert = false;
        for( int j = 0; j < i && !insert; j++ ){
            if( pList[j].getControlDarkGray() >= pList[i].getControlDarkGray() ){
                TableRow temp = pList[i];
                // Mover todos los elementos desde "j" hasta "i-1" una pos. a la derecha
                for( int k = i; k >= j+1; k-- ){
                    pList[k] = pList[k-1];
                }
                // Reemplazamos el elemento: colocamos el elemento "i" en la posición de "j"
                pList[j] = temp;
                insert = true;
            }
        }
    }
    return pList;
}

float getTotalPopulation(vector<TableRow> pTable){
    float total = 0;
    for(int indexTable = (NUMBER_OF_QUADRANT * CONTROL_PORCENTAGE_CROMO); indexTable < NUMBER_OF_QUADRANT; indexTable++){
        if(pTable[indexTable].getControlDarkGray() > pTable[indexTable].getControlLightGray()){
            total += pTable[indexTable].getControlDarkGray();
        }else{
            total += pTable[indexTable].getControlLightGray();
        }
    }
    return total;
}

//Crear la estructura del cromosoma
cromodistribution createNewCromo(TableRow pRow, int pNum, int pMinValue, float pTotal){
    cromodistribution cromo;
    int shapeDensity = 0;
    cromo.xMin = pRow.getInitialPositionOne();
    cromo.xMax = pRow.getInitialPositionTwo();
    cromo.yMin = pRow.getFinalPositionOne();
    cromo.yMax = pRow.getFinalPositionTwo();

    if(pNum == 1){
        cromo.quantity = pRow.getControlDarkGray();
        cromo.red = 89; cromo.green = 93; cromo.blue = 100; //Tratar de cambiar
    }else{
        cromo.quantity = pRow.getControlLightGray();
        cromo.red = 205; cromo.green = 205; cromo.blue = 205; //Tratar de cambiar
    }
    shapeDensity = cromo.quantity / (QUADRANT_QUANTITY * SAMPLERATE);
    cromo.shape = shapeDensity < 0.60 ? "line" : "dot";
    cromo.maxProbability = (cromo.quantity+0.0)/ pTotal;
    cromo.minValue = pMinValue ;
    cromo.maxValue = (pMinValue +1)+ (CROMO_MAX_VALUE * cromo.maxProbability);
    return cromo;
}

//Añade la distribucion en GeneticBase
GeneticBase addCromodistribution(vector<TableRow> pTable){
    pTable = sorting(pTable);
    float totalPopulation= getTotalPopulation(pTable);
    GeneticBase genetic;
    cromodistribution distribution;
    //cromodistribution distribution_;
    //vector<cromodistribution> prueba; //Eliminar despues****

    int maxNibble = 0;
    for(int indexCromo = (NUMBER_OF_QUADRANT*CONTROL_PORCENTAGE_CROMO); indexCromo < NUMBER_OF_QUADRANT; indexCromo++){
        if(pTable[indexCromo].getControlDarkGray() > pTable[indexCromo].getControlLightGray()){
            distribution = createNewCromo(pTable[indexCromo],1,maxNibble,totalPopulation);
        }else{
            distribution = createNewCromo(pTable[indexCromo],2,maxNibble,totalPopulation);
        }
        //Primer color
        genetic.addDistribution(distribution); //*******************&***************************
        maxNibble = distribution.maxValue;

        // //Segundo color
        // distribution_ = createNewCromo(pTable[indexCromo],2,maxNibble,totalPopulation);
        // genetic.addDistribution(&distribution_);
        // maxNibble = distribution_.maxValue;

        //Solo prueba, quitar luego
        //prueba.push_back(distribution);
        //prueba.push_back(distribution_);
    }
    //Solo para imprimir  //Se sobrepasa del nibble--falta corregir
    // for(int i = 0; i < prueba.size();i++){
    //     cout<<"Cromosoma cantidad: "<< prueba[i].quantity
    //     <<" Rango del nibble: "<<prueba[i].minValue<<" "<<prueba[i].maxValue<<endl; 
    // }
    return genetic;
} 

/*
La función consiste en crear 702 cuadrantes de 180 x 180.
Cuando inicia la funcion todos los cuadrantes inician con un porcentaje de 0, por lo que se 
elige al azar el primero.

Luego se guia el random buscando los porcentajes de los cuadrantes más altos.
El porcentaje de un cuadrante se rige por hacer match con el color de gris entre medio y oscuro 
(Por ejemplo, se selecciona el punto al azar y se pasa a gris, si ese gris está entre los rangos de los 
grises más oscuros) Si hace match el porcentaje aumenta y si no, se disminuye su porcentaje. Pero de igual
menera lo cuenta. El porcetaje se disminuye considerablemente si ese cuadrante ya
se visitó un 15% del total de los pixeles de ese cuadrante. Le da oportunidad de revisar otros cuadrantes.

El problema es que dura demasiado en revisar el 40% de la imagen. Ver si se puede mejorar la búsqueda
del mejor porcentaje de los cuadrantes.
*/

vector<TableRow> samplingFunction(){

    vector<TableRow> pTable = (createTable());
    int width, height, channels, indexControl;
    unsigned char *image = stbi_load(IMAGE_NAME, &width, &height, &channels, 0);
    const size_t RGB = 3;

    int red, green, blue, actualAmount, currenyAmount;
    int rowHighestPercentage; 
    Pixel pixel;
    float percentage;
    
    for(int pixelIndex = 0; pixelIndex < TOTAL_PIXELS; pixelIndex++){

        rowHighestPercentage = findPercentage(pTable,indexControl);
        indexControl = rowHighestPercentage; //Para que no se repita, el mismo pixel en el próximo
        //Creamos pixel y añadimos su color
        pixel = createNewPixel(pTable[rowHighestPercentage]);
        size_t index = RGB * (pixel.getPositionY() * width + pixel.getPositionX());
        red = static_cast<int>(image[index + 0]);
        green = static_cast<int>(image[index + 1]);
        blue = static_cast<int>(image[index + 2]);
        //pixel.setColorRGB(red, green, blue);
        //cout<<"iNDEX: "<<rowHighestPercentage<<endl;
        currenyAmount = pTable[rowHighestPercentage].getCurrentAmount(); //Cantidad actual
        actualAmount = pTable[rowHighestPercentage].getActualAmount(); //Cantidad real
    
        //Comparamos si el color del pixel está entre los grises más oscuros. Si lo está.
        //Aumenta porcentaje si no, lo disminuye
        if(isInRange(red,green,blue)){
            pTable[rowHighestPercentage].setControlDarkGray(1);
            if(actualAmount >= QUADRANT_QUANTITY*0.15){
                pTable[rowHighestPercentage].setCurrentAmount(-currenyAmount); //Bajamos el porcentaje
            }
            else{
                pTable[rowHighestPercentage].setCurrentAmount(1);
            }
        }else{
            pTable[rowHighestPercentage].setControlLightGray(1);
            pTable[rowHighestPercentage].setCurrentAmount(-1); //Disminuimos el porcentaje
        }

        pTable[rowHighestPercentage].setActualAmount(1);
        percentage = pTable[rowHighestPercentage].getCurrentAmount()/TOTAL_PIXELS;
        pTable[rowHighestPercentage].setPercentage(percentage);
        pTable[rowHighestPercentage].setPixelVector(pixel);
    }
    // for(int j = 0; j< pTable.size();j++){
    //     cout<<"Cuadrante: "<<j<<" Canti de gris oscuro: "<<pTable[j].getControlDarkGray()
    //     <<" gris claro: "<<pTable[j].getControlLightGray()<<endl;
    // }

    return pTable;
}



int main() {
    srand((unsigned char) time(0));
    GeneticBase genetic;
    genetic = addCromodistribution(samplingFunction()); //Ya se añadió la distribución
    genetic.initPopulation(1000);
    genetic.produceGenerations(5,1000); //(pGeneraciones, pCantHijos)

    vector<individual*> generation = genetic.getPopulation();
    //vector<individual*> generation = genetic.getFitnessPopulation();
    for (int i = 0; i < generation.size(); i++){
        generation.at(i)->getDistribution().printInfo(); 
    }
    socketclient client;
    client.init();
    client.clear();
    client.drawGeneration(&generation);
    client.closeConnection();
    



    //cout<<genetic.getPopulation()[0]->getCromosoma()<<endl;
    //cout<<genetic.getPopulation().size()<<endl;
    
    /*
    client.paintLine(70, 80, 50, 100, 300, 100, 400, 600);
    client.paintDot(200, 0, 15, 200, 500, 600, 15);
    client.paintDot(220, 150, 15, 200, 600, 600, 20);
	client.paintLine(200, 1, 1, 50, 50, 500, 20, 350);*/

    //DISTRIBUTION_SIZE = 10;
    // int red, green, blue;
    // int width, height, channels, indexControl;
    // unsigned char *image = stbi_load(IMAGE_NAME, &width, &height, &channels, 0);
    // const size_t RGB = 3;

    //     size_t index = RGB * (600 * width + 540);
    //     red = static_cast<int>(image[index + 0]);
    //     green = static_cast<int>(image[index + 1]);
    //     blue = static_cast<int>(image[index + 2]);
    //     cout<<"Color: "<<red<<" "<<green<<" "<<blue<<endl;

    //     cout<<"Rango de gris: "<<(0.299* red)+(0.587*green) +(0.114*blue)<<endl;
    
    

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