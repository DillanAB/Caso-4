#ifndef _GENETICBASE_
#define _GENETICBASE_ 1
#define AREA 200
//#define PERCENTAGE 0.60
#define PERCENTAGE 0.40
#define MUTATION_RATIO 7
#include <vector>
#include "lib/cromodistribution.h"
#include "lib/individual.h"

using namespace std;

class GeneticBase {
    private:
        vector<cromodistribution> representation; //Cromo distribution es un struc localizado en lib
        vector<individual*> *population;
        vector<individual*> *fitnessPopulation;
        vector<individual*> *unfitnessPopulation;
        int populationQuantity;
        int targetGenerations;

        //Le pone una nota, una calificacion
        void evaluateFitness() {
            fitnessPopulation->clear();
            unfitnessPopulation->clear();
            
            //Se recorre la población.
            cout<<"Los que tengo: "<<endl; 
            for(int i=0;i<population->size(); i++) {
               population->at(i)->setFitnessValue(fitness(population->at(i)));
               cout<<"Fitness: "<<population->at(i)->getFitnessValue()<<endl;
            }
            sortingPopulation(population); //Ordena de menor a mayor
            cout<<"Los que quedan: "<<endl;
            //Nos quedamos con el 40% más altos
            unsigned int limitRange = (population->size()* PERCENTAGE);
            for(int i=0 ;i < limitRange; i++) {
                fitnessPopulation->push_back(population->at(i));
                cout<<"Fitness: "<<population->at(i)->getFitnessValue()<<endl;
            }
        }

        void sortingPopulation(vector<individual*> *pList){
            for( int i = 0; i < pList->size(); i++ ){
            bool insert = false;
                for( int j = 0; j < i && !insert; j++ ){
                    if( pList->at(j)->getFitnessValue() >= pList->at(i)->getFitnessValue()){
                    individual *temp = pList->at(i);
                    // Mover todos los elementos desde "j" hasta "i-1" una pos. a la derecha
                    for( int k = i; k >= j+1; k-- ){
                        pList->at(k) = pList->at(k-1);
                    }
                    // Reemplazamos el elemento: colocamos el elemento "i" en la posición de "j"
                    pList->at(j) = temp;
                    insert = true;
                }
            }
        }
        //return pList;
    }
        //Selecciona los que están más cerca de la respuesta
        float fitness(individual *pIndividual) {
            int pointsOfReference[10] = {540,100,540,250,540,700,200,400,800,400};
            int xMin = pIndividual->getDistribution().xMin;
            int yMin = pIndividual->getDistribution().yMin;
            //cout<<"LOS X: "<<xMin<< " y: "<<yMin<<endl;
            float distance = 0;
            int nex = 0;
            for(int indexControl = 0; indexControl < 9; indexControl += 2){
                distance += 1/((sqrt(pow(pointsOfReference[indexControl]-xMin,2) + pow(pointsOfReference[indexControl+1]-yMin,2)))/AREA); 
            }
            return distance;
        }

        void reproduce(int pAmountOfChildrens) {
            // previous population will be cleared, full saved, partial saved depending on the problem
            //Toda la poblacion se limpia, se va a mantener o se limpia
            //Aquí se limpia pero depende del problema. Se limpia para que 
            //La funcion corra
            population->clear();

            for(int i=0; i<pAmountOfChildrens; i++) {
                // select any two fitness parents
                //Se elige a dos pabres y se dejan en individual paren a o b
                //No importa que se reproduzcan los mismos padres
                int parent_a_index = rand()%fitnessPopulation->size(); 
                individual* parent_a = fitnessPopulation->at(parent_a_index);

                int parent_b_index = rand()%fitnessPopulation->size();
                individual* parent_b = fitnessPopulation->at(parent_b_index);

                population->push_back(cross(parent_a, parent_b)); //Explicando cross misma clase abajo
            }
        }

        individual* cross(individual *pParent_a, individual *pParent_b) {
            // this operation will depends on: nibble size, fixed o variable point
            //Depende del tamaño del nibble y si el punto de corte es fixed o variable
            // Está en individuo MIN_GENOTYPE_SIZE_BY_PARENT 2 Aqui lo que dice es que me dejo al menos 2 bits

            //Aqui se explica en pizzara 11000100 
            int cut_position = (rand() % (NIBBLE_SIZE-MIN_GENOTYPE_SIZE_BY_PARENT*2)) + MIN_GENOTYPE_SIZE_BY_PARENT;

            unsigned int mask_a = CROMO_MAX_VALUE - 1; // 255 -> 11111111
            mask_a <<= cut_position;
            //Lo corro la cantidad a la izquierda

            unsigned int mask_b = CROMO_MAX_VALUE - 1; // 255 -> 11111111
            mask_b >>= NIBBLE_SIZE - cut_position;
            //Corre para la derecha

            unsigned int kid = (pParent_a->getCromosoma() & mask_a) | (pParent_b->getCromosoma() & mask_b);

            individual *children = new individual(kid);
            cromodistribution distribution = findDistribution((int)children->getCromosoma());
            children->setDistribution(distribution);

            //Hace falta la mutación. Agregar el % de mutación y la mutación
            float mutationChance = (rand() % 100);
            if(mutationChance<MUTATION_RATIO){
                //rand()
            }
            return children;
        }

    public:
        GeneticBase() {
            this->population = new vector<individual*>();
            this->fitnessPopulation = new vector<individual*>();
            this->unfitnessPopulation = new vector<individual*>();
            //this->representation; 
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
                population->push_back(p);
            }
        }
        
        //Generaciones que quiero y cantidad de hijos por esa generación
        void produceGenerations(int ptargetGenerations, int pChildrensPerGenerations) {
            for(int i=0; i<ptargetGenerations; i++) {
                cout<<"INICIA GEN"<<endl;
                //Qué hace evaluateFitness
                evaluateFitness(); //Ya queda en una lista los que están fitness y los que no
                reproduce(pChildrensPerGenerations); //Se le dice que reproduzca esa cantidad de "niños"
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