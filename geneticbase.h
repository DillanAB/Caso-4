#ifndef _GENETICBASE_
#define _GENETICBASE_ 1
#define AREA 200 
#define PERCENTAGE 0.40
#define MUTATION_RATIO 5
#include <vector>
#include "lib/cromodistribution.h"
#include "lib/individual.h"

using namespace std;

struct Pivot{
    int quantity = 10;
    int pivot[10] = {540,100,540,250,540,700,200,400,800,400};
};

class GeneticBase {
    private:
        vector<cromodistribution> representation; //Cromo distribution es un struc localizado en lib
        vector<individual*> *population;
        vector<individual*> *fitnessPopulation;
        vector<individual*> *unfitnessPopulation;
        int populationQuantity;
        int targetGenerations;

        void evaluateFitness() {
            fitnessPopulation->clear();
            unfitnessPopulation->clear();
            
            //Se recorre la población.
            //cout<<"Los que tengo: "<<endl; 
            for(int i=0;i<population->size(); i++) {
                population->at(i)->setFitnessValue(fitness(population->at(i)));
            }
            sortingPopulation(population); // Sort from smallest to largest

            //Keep the highest 60%
            unsigned int limitRange = (population->size()* PERCENTAGE);
            for(int i= limitRange ;i < population->size(); i++) {
                fitnessPopulation->push_back(population->at(i));
            }
        }

        void sortingPopulation(vector<individual*> *pList){
            for( int controlI = 0; controlI < pList->size(); controlI++ ){
            bool insert = false;
                for( int controlJ = 0; controlJ < controlI && !insert; controlJ++ ){
                    if( pList->at(controlJ)->getFitnessValue() >= pList->at(controlI)->getFitnessValue()){
                    individual *temp = pList->at(controlI);
                    for( int controlK = controlI; controlK >= controlJ+1; controlK-- ){
                        pList->at(controlK) = pList->at(controlK-1);
                    }
                    pList->at(controlJ) = temp;
                    insert = true;
                    }
                }
            }
        }
        //return pList;

        //Selecciona los que están más cerca de la respuesta
        float fitness(individual *pIndividual) {
            Pivot points;
            int xMin = pIndividual->getInitialPoints().at(0);
            int yMin = pIndividual->getInitialPoints().at(1);
            float distance = 0; float result = 0;
            for(int indexControl = 0; indexControl < points.quantity; indexControl += 2){
                distance += 1/((sqrt(pow(points.pivot[indexControl]-xMin,2) + pow(points.pivot[indexControl+1]-yMin,2)))/(AREA*AREA)); 
            }
            return distance;
        }

        void reproduce(int pAmountOfChildrens) {

            population->clear();
            for(int i=0; i<pAmountOfChildrens; i++) {
                int parent_a_index = rand()%fitnessPopulation->size(); 
                individual* parent_a = fitnessPopulation->at(parent_a_index);

                int parent_b_index = rand()%fitnessPopulation->size();
                individual* parent_b = fitnessPopulation->at(parent_b_index);
                
                population->push_back(cross(parent_a, parent_b));
            }
        }
        // this operation will depends on: nibble size, fixed o variable point
        individual* cross(individual *pParent_a, individual *pParent_b) {

            int cut_position = (rand() % (NIBBLE_SIZE-MIN_GENOTYPE_SIZE_BY_PARENT*2)) + MIN_GENOTYPE_SIZE_BY_PARENT;

            unsigned int mask_a = CROMO_MAX_VALUE - 1;
            mask_a <<= cut_position;

            unsigned int mask_b = CROMO_MAX_VALUE - 1; 
            mask_b >>= NIBBLE_SIZE - cut_position;

            unsigned int kid = (pParent_a->getCromosoma() & mask_a) | (pParent_b->getCromosoma() & mask_b);

            individual *children = new individual(kid);
            cromodistribution distribution = findDistribution((int)children->getCromosoma());
            children->setDistribution(distribution);
            children->setPointsFromDistri();

            //Mutation
            float mutationChance = (rand() % 100);
            if(mutationChance<MUTATION_RATIO){
               int bitToMutate = rand() % (NIBBLE_SIZE - 1);
               unsigned int cromo = children->getCromosoma();
               unsigned int mask = 1 << bitToMutate;
               unsigned int cromoMutated = ((cromo & ~mask) | (1 << bitToMutate));
               children->setCromosome(cromoMutated);
               cromodistribution distribution = findDistribution((int)children->getCromosoma());
            }else{
                cromodistribution distribution = findDistribution((int)children->getCromosoma());
                children->setDistribution(distribution);
            }
            children->setPointsFromDistri();

            return children;
        }

    public:
        GeneticBase() {
            this->population = new vector<individual*>();
            this->fitnessPopulation = new vector<individual*>();
            this->unfitnessPopulation = new vector<individual*>();
            this->populationQuantity = 0;
            this->targetGenerations = 20;
        }

        void addDistribution(cromodistribution pDistribution) {
            representation.push_back(pDistribution);
        }

        cromodistribution findDistribution(int pValue){
            
            cromodistribution distribution;
            for(int i = 0; i < representation.size();i++){
                if(pValue > representation[i].minValue && pValue <= representation[i].maxValue){
                    distribution = representation[i];
                }
            }
            return distribution;
        }

        void initPopulation(int pAmountOfIndividuals) {
            population->clear();
            cromodistribution distribution; 

            for(int i=0; i<pAmountOfIndividuals; i++) {
                individual* p = new individual((unsigned int) rand()%CROMO_MAX_VALUE);
                distribution = findDistribution((int)p->getCromosoma());
                p->setDistribution(distribution);
                p->setPointsFromDistri();
                population->push_back(p);
            }
        }
        
        void produceGenerations(int ptargetGenerations, int pChildrensPerGenerations) {
            for(int i=0; i<ptargetGenerations; i++) {
                cout<<"Inicia una gen"<<endl;
                evaluateFitness(); 
                reproduce(pChildrensPerGenerations); 
            }
        }

        vector<individual*> getPopulation() {
            return *this->population;
        }

        vector<individual*> getFitnessPopulation() {
            return *this->fitnessPopulation;
        }

        int getRepresentationSize(){
            return representation.size();
        }

        vector<cromodistribution> getRepresentation(){
            return this->representation;
        }
};

#endif