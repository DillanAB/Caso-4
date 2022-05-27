#ifndef _INDIVIDUAL_
#define _INDIVIDUAL_ 1

#define MIN_GENOTYPE_SIZE_BY_PARENT 4

class individual {
    private: 
        // any important related info
        // decide the size of your nibble for your problem
        float fitnessValue;
        unsigned int cromosoma;
        int xPosition, yPosition;
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

        cromodistribution getDistribution(){
            return this->distribution;
        }
};


#endif