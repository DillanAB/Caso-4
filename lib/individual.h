#ifndef _INDIVIDUAL_
#define _INDIVIDUAL_ 1

#define MIN_GENOTYPE_SIZE_BY_PARENT 4

class individual {
    private: 
        // any important related info
        // decide the size of your nibble for your problem
        float fitnessValue;
        unsigned int cromosoma;
        int xInitial, yInitial, xFinal, yFinal;
        cromodistribution distribution;
    public:
        individual(unsigned int pValue) {
            this->cromosoma = pValue;
        }

        unsigned int getCromosoma() {
            return this->cromosoma;
        }

        void setFitnessValue(float pValue) {
            this->fitnessValue = pValue;
        }

        float getFitnessValue() {
            return this->fitnessValue;
        }

        void setDistribution(cromodistribution pDistribution){
            this->distribution = pDistribution;
        }

        void setCromosome(unsigned int pCromosome){
            this->cromosoma = pCromosome;
        }

        void setPointsFromDistri(){
            this->xInitial = rand() % (distribution.xMax - distribution.xMin) + distribution.xMin;
			this->yInitial = rand() % (distribution.yMax - distribution.yMin) + distribution.yMin;
            this->xFinal = rand() % (distribution.xMax - xInitial) + xInitial;
			this->yFinal = rand() % (distribution.yMax - yInitial) + yInitial;
        }

        void setPoints(int pXInitial, int pYInitial, int pXFinal, int pYFinal){
            this->xInitial = pXInitial;
			this->yInitial = pYInitial;
            this->xFinal = pXFinal;
			this->yFinal = pYFinal;
        }

        cromodistribution getDistribution(){
            return this->distribution;
        }
        
        vector<int> getInitialPoints(){
            vector<int> initialPoints = {xInitial, yInitial};
            return initialPoints;
        }

         vector<int> getFinalPoints(){
            vector<int> finalPoints = {xFinal, yFinal};
            return finalPoints;
        }
};


#endif