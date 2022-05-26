#ifndef _GENETICBASE_
#define _GENETICBASE_ 1
#include <vector>
#include "lib/cromodistribution.h"
#include "lib/individual.h"

using namespace std;

class GeneticBase {
    private:
        vector<cromodistribution*> *representation; //Cromo distribution es un struc localizado en lib
        vector<individual*> *population;
        vector<individual*> *fitnessPopulation;
        vector<individual*> *unfitnessPopulation;
        int populationQuantity;
        int targetGenerations;

        //Le pone una nota, una calificacion
        void evaluateFitness() {
            fitnessPopulation->clear();
            unfitnessPopulation->clear();

            for(int i=0;i<population->size(); i++) {
                //Population at tiene individuos y cada individuo 
                //Tiene un fitness
               population->at(i)->setFitnessValue(fitness(population->at(i))); //Se le setea el fitness

                //Aquí aparece un 50, pero no puede ser. Nunca debe ser un absoluto
                //Hay que cambiar este valor.
                if (population->at(i)->getFitnessValue()>50) {  // fitness criteria of selection never will be an absolute value always is relative to the population
                    fitnessPopulation->push_back(population->at(i));
                } else {
                    unfitnessPopulation->push_back(population->at(i));
                }
            }
        }

        float fitness(individual *pIndividual) {
            //Aqui se debe pensar más
            //No puede ser un randon nada màs
            //Esta funcion tiene la responsabilidad de guiar la poblacion
            //pues es la que selecciona los mejores dado que esos
            //están cada vez más cerca de la respuesta
            return rand()%100;
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

            unsigned char mask_a = CROMO_MAX_VALUE - 1; // 255 -> 11111111
            mask_a <<= cut_position;
            //Lo corro la cantidad a la izquierda

            unsigned char mask_b = CROMO_MAX_VALUE - 1; // 255 -> 11111111
            mask_b >>= NIBBLE_SIZE - cut_position;
            //Corre para la derecha

            unsigned char kid = (pParent_a->getCromosoma() & mask_a) | (pParent_b->getCromosoma() & mask_b);

            individual *children = new individual(kid);

            //Hace falta la mutación. Agregar el % de mutación y la mutación
            return children;
        }

    public:
        GeneticBase() {
            this->population = new vector<individual*>();
            this->fitnessPopulation = new vector<individual*>();
            this->unfitnessPopulation = new vector<individual*>();
            this->representation = new vector<cromodistribution*>(); 
            this->populationQuantity = 0;
            this->targetGenerations = 20;
        }

        void addDistribution(cromodistribution* pDistribution) {
            representation->push_back(pDistribution);
        }

        void initPopulation(int pAmountOfIndividuals) {
            population->clear();
            //Aquí se debe ajustar
            //Se está asumiendo nibble de 8 bits unsigned char
            //Si mi individuo es de otro tamaño se debe cambiar
            //En individuo. El cromososa es de tipo unsigned char
            for(int i=0; i<pAmountOfIndividuals; i++) {
                individual* p = new individual((unsigned char) rand()%CROMO_MAX_VALUE);
                //El cromo max value es hasta el 256, se crea un random de ese numero
                //Por ejemplo si cae en 90 queda en ojos azules
                //Si cae en 240 sería ojos verdes 
                population->push_back(p);
                //Ese individuo se mete en la población actual.
                
            }
        }
        
        //Generaciones que quiero y cantidad de hijos por esa generación
        void produceGenerations(int ptargetGenerations, int pChildrensPerGenerations) {
            for(int i=0; i<ptargetGenerations; i++) {
                //Qué hace evaluateFitness
                evaluateFitness(); //Ya queda en una lista los que están fitness y los que no
                reproduce(pChildrensPerGenerations); //Se le dice que reproduzca esa catidad de "niños"
            }
        }

        vector<individual*> getPopulation() {
            return *this->population;
        }

        int getRepresentationSize(){
            return representation->size();
        }
};

#endif