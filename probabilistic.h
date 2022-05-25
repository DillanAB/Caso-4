#ifndef _DYNAMICTABLE_
#define _DYNAMICTABLE_ 

class Pixel {
    private:
        int position[2] = {0,0};
        int colorRGB[3] = {0,0,0};
    public:
        Pixel(){
        }
        void setPosition(int pInitialX, int pInitialY)
        {
            position[0] = pInitialX;
            position[1] = pInitialY;
        }
        void setColorRGB(int pRed, int pGreen, int pBlue)
        {
            colorRGB[0] = pRed;
            colorRGB[1] = pGreen;
            colorRGB[2] = pBlue;
        }
        int getPositionX()
        {
            return position[0];
        }
        int getPositionY()
        {
            return position[1];
        }
        int getColorRed()
        {
            return colorRGB[0];
        }
        int getColorGreen()
        {
            return colorRGB[1];
        }
        int getColorBlue()
        {
            return colorRGB[2];
        }
        void getInformation(){
            cout<<"__Pixel x y: "<<position[0]<<" "<<position[1]
            <<" color: "<<colorRGB[0]<<" "<<colorRGB[1]<<" "<<colorRGB[2]<<endl;
        }
};

class TableRow {
    private:

        int initialPosition[2] = {0,0};
        int finalPosition[2] = {0,0};
        int range[2] = {0,0};
        Pixel comparePixel;
        vector<Pixel>pixelVector;
        float percentage = 0.0;
        int currentAmount = 0; //Actual
        int actualAmount = 0; //Real

    public:

        TableRow(){};
        void setCurrentAmount(int pCurrent){
            currentAmount += pCurrent;
        }
        void setActualAmount(int pActual){
            actualAmount += pActual;
        }
        void setPercentage(float pPercentage ){
            percentage = pPercentage;
        }
        void setInitialPosition(int pInitialOne, int pInitialTwo){
            initialPosition[0] = pInitialOne;
            initialPosition[1] = pInitialTwo;
        }
        void setPixel(Pixel pPixel){
            comparePixel = pPixel;
        }
        void setFinalPosition(int pFinalOne, int pFinalTwo){
            finalPosition[0] = pFinalOne;
            finalPosition[1] = pFinalTwo;
        }
        void setRange(int pRangeOne, int pRangeTwo){
            range[0] = pRangeOne;
            range[1] = pRangeTwo;
        }
        void setPixelVector(Pixel pPixel){
            pixelVector.push_back(pPixel);
        }
        int getInitialPositionOne(){
            return initialPosition[0];
        }
        int getInitialPositionTwo(){
            return initialPosition[1];
        }
        int getFinalPositionOne(){
            return finalPosition[0];
        }
        int getFinalPositionTwo(){
            return finalPosition[1];
        }
        int getRangeOne(){
            return range[0];
        }
        int getRangeTwo(){
            return range[1];
        }
        vector<Pixel> getVector(){
            return pixelVector;
        }
        float getPercentage(){
            return percentage;
        }
        int getCurrentAmount(){
            return currentAmount;
        }
        int getActualAmount(){
            return actualAmount;
        }
        Pixel getPixel(){
            return comparePixel;
        }
        int getVectorSize(){
            return pixelVector.size();
        }

};

#endif