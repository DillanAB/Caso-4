#ifndef _FUNCTIONS_
#define _FUNCTIONS_ 1

//Create the initial table of fixed quadrants. 40x40.
vector<TableRow> createTable(){
    vector<TableRow> table; // Array of rows
    TableRow newRow; //The row

    //To move in the image x,y
    int widthIncrease = QUADRANT; int heightIncrease = QUADRANT;
    int widthDecrease = 0; int heightDecrease = 0;
    int controlRow = 1;

    for( int indexQuadrant = 0; indexQuadrant < NUMBER_OF_QUADRANT; indexQuadrant++){
        //Create the rows with the desired size
        newRow.setInitialPosition(widthDecrease, widthIncrease);
        newRow.setFinalPosition(heightDecrease, heightIncrease);
        widthIncrease += QUADRANT; widthDecrease += QUADRANT; //Increase x
        table.push_back(newRow);
        
        //To control the rows when the row reaches the end
        if (controlRow == CONTROLROW && indexQuadrant != NUMBER_OF_QUADRANT ){
            controlRow = 1;
            widthDecrease = 0; widthIncrease = QUADRANT; //x returns to default values
            heightDecrease += QUADRANT; heightIncrease += QUADRANT; //For y to move y
        }
        controlRow ++;        
    }

    return table;
}

/*
This function consists of looking for the highest percentage among all the quadrants / rows of the table
If the highest percentage is 0, then it does a random to choose a quadrant.

It has as parameters: A vector of rows / quadrants and an index.
What it returns is the index of the table where the highest percentage is
*/

int findPercentage(vector<TableRow> pTable, int indexControl, int pNumOfNotVisited){
    int returnIndex;
    if (pNumOfNotVisited >= (int) (NUMBER_OF_QUADRANT/10)){ 
        returnIndex = rand()%NUMBER_OF_QUADRANT;
        return returnIndex;
    }

    float random = (float)(rand()) / ((float)(RAND_MAX/(1)));
    //float max = 0; 
    for(int indexTable = 0; indexTable < pTable.size(); indexTable++){
        if(pTable[indexTable].getPercentage() > random && indexTable != indexControl){
            //cout<<"Entra por porcentaje"<<endl;
            returnIndex = indexTable;
        }
        if(indexTable >= pTable.size()-1){
            return rand()%NUMBER_OF_QUADRANT;
        }
    }
    return returnIndex;
}

/*
What this function determines for me is if the color is in the range of the darkest grays
We define the range from 0 to 180 for darker grays greater than this will be a lighter gray
*/

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

    xPixelPosition = getXPosition + rand()% (getXFinalPosition - getXPosition);
    yPixelPosition = getYPosition + rand()% (getYFinalPosition - getYPosition);
    pixel.setPosition(xPixelPosition,yPixelPosition);

    return pixel;
}

// Sort a table from smallest to largest according to its amount of dark color
vector<TableRow> sorting( vector<TableRow> pList){
    for( int controlI = 0; controlI < pList.size(); controlI++ ){
        bool insert = false;
        for( int controlJ = 0; controlJ < controlI && !insert; controlJ++ ){
            if( pList[controlJ].getControlDarkGray() >= pList[controlI].getControlDarkGray() ){
                TableRow temp = pList[controlI];
                for( int controlK = controlI; controlK >= controlJ+1; controlK-- ){
                    pList[controlK] = pList[controlK-1];
                }
                pList[controlJ] = temp;
                insert = true;
            }
        }
    }
    return pList;
}


// Get the total of the population. Count only those that have the most predominant color

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

/*
Create the structure of the chromosome.

It has as parameters
A pRow: What is a row or quadrant
A pNum: Which indicates the shade of gray
a pMinValue that tells me the distribution of the nibble.
A pTotal: Which indicates the totality of the population

*/
cromodistribution createNewCromo(TableRow pRow, int pNum, int pMinValue, float pTotal){
    cromodistribution cromo;
    int shapeDensity = 0;
    Gray color;

    cromo.xMin = pRow.getInitialPositionOne();
    cromo.xMax = pRow.getInitialPositionTwo();
    cromo.yMin = pRow.getFinalPositionOne();
    cromo.yMax = pRow.getFinalPositionTwo();

    if(pNum == 1){
        cromo.quantity = pRow.getControlDarkGray();
        cromo.red = color.darkRed; cromo.green = color.darkGreen; cromo.blue = color.darkBlue; 
        //cout<<"RED: "<<cromo.red<<endl;
    }else{
        cromo.quantity = pRow.getControlLightGray();
        cromo.red = color.light; cromo.green = color.light; cromo.blue = color.light;
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
    int maxNibble = 0;

    for(int indexCromo = (NUMBER_OF_QUADRANT*CONTROL_PORCENTAGE_CROMO); indexCromo < NUMBER_OF_QUADRANT; indexCromo++){
        if(pTable[indexCromo].getControlDarkGray() == 0){
            continue;
        }

        if(pTable[indexCromo].getControlDarkGray() > pTable[indexCromo].getControlLightGray()){
            distribution = createNewCromo(pTable[indexCromo],1,maxNibble,totalPopulation);
        }else{
            distribution = createNewCromo(pTable[indexCromo],2,maxNibble,totalPopulation);
        }
        genetic.addDistribution(distribution);
        maxNibble = distribution.maxValue;
    }

    return genetic;
} 

/*
The function consists of creating 702 quadrants of 40 x 40.
When the function starts, all the quadrants have a percentage of 0, so it is
randomly choose the first.

Then the random is guided looking for the percentages of the highest quadrants.
The percentage of a quadrant is governed by matching the color of gray between medium and dark
(For example, the point is randomly selected and turned gray, if that gray is between the ranges of the
darker grays) If it matches, the percentage increases and if not, its percentage decreases. but anyway
anyway it counts. The percentage decreases considerably if that quadrant already
15% of the total number of pixels in that quadrant were visited.
*/
vector<TableRow> samplingFunction(){
    int numOfNotVisited = NUMBER_OF_QUADRANT;
    vector<TableRow> pTable = createTable(); //Create quadrants

    //Load and read the image
    int width, height, channels, indexControl;
    unsigned char *image = stbi_load(IMAGE_NAME, &width, &height, &channels, 0);
    const size_t RGB = 3;

    //Variables 
    int red, green, blue, actualAmount, currenyAmount;
    int rowHighestPercentage; 
    Pixel pixel;
    float percentage;
    
    for(int pixelIndex = 0; pixelIndex < TOTAL_PIXELS; pixelIndex++){

        rowHighestPercentage = findPercentage(pTable,indexControl, numOfNotVisited);
        indexControl = rowHighestPercentage; //So that it does not repeat, the same pixel in the next run
        if(pTable[indexControl].getVisited()==false){
            --numOfNotVisited;
            pTable[indexControl].setVisited(true);
        }

        //Create the pixel and add its color
        pixel = createNewPixel(pTable[rowHighestPercentage]);
        size_t index = RGB * (pixel.getPositionY() * width + pixel.getPositionX());
        red = static_cast<int>(image[index + 0]);
        green = static_cast<int>(image[index + 1]);
        blue = static_cast<int>(image[index + 2]);

        //Assign the current value and the real value of the quadrant to the variables
        currenyAmount = pTable[rowHighestPercentage].getCurrentAmount(); 
        actualAmount = pTable[rowHighestPercentage].getActualAmount(); 
        //Compare if the pixel color is among the darkest grays. If it is.
        //Increase percentage if not, decrease it
        if(isInRange(red,green,blue)){
            pTable[rowHighestPercentage].setControlDarkGray(1);
            if(actualAmount >= QUADRANT_QUANTITY*0.15){
                pTable[rowHighestPercentage].setCurrentAmount(-currenyAmount);
            }
            else{
                pTable[rowHighestPercentage].setCurrentAmount(1);
            }
        }else{
            pTable[rowHighestPercentage].setControlLightGray(1);
            pTable[rowHighestPercentage].setCurrentAmount(-1);
        }
        pTable[rowHighestPercentage].setActualAmount(1); //Increase real
        percentage = pTable[rowHighestPercentage].getCurrentAmount()/TOTAL_PIXELS; //ANTES ESTABA ENTRE TOTAL PIXELS.
        pTable[rowHighestPercentage].setPercentage(percentage);
        pTable[rowHighestPercentage].setPixelVector(pixel);
    }
    return pTable;
}


#endif