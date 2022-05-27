
#define STB_IMAGE_IMPLEMENTATION

#include "main.hpp"
#include "functions.hpp"

using namespace std;


int main() {
    srand((unsigned char) time(0));
    GeneticBase genetic;
    genetic = addCromodistribution(samplingFunction()); 
    genetic.initPopulation(1000);
    genetic.produceGenerations(10,1000); //(pGeneraciones, pCantHijos)

    vector<individual*> generation = genetic.getPopulation();
    //vector<individual*> generation = genetic.getFitnessPopulation();
    /*for (int i = 0; i < generation.size(); i++){
        generation.at(i)->getDistribution().printInfo(); 
    }*/
    socketclient client;
    client.init();
    client.clear();
    cout<<"Inicia a pintar"<<endl;
    client.drawGeneration(&generation);
    client.closeConnection();
    
    //cout<<genetic.getPopulation()[0]->getCromosoma()<<endl;
    //cout<<genetic.getPopulation().size()<<endl;
    
    /*
    client.paintLine(70, 80, 50, 100, 300, 100, 400, 600);
    client.paintDot(200, 0, 15, 200, 500, 600, 15);
    client.paintDot(220, 150, 15, 200, 600, 600, 20);
	client.paintLine(200, 1, 1, 50, 50, 500, 20, 350);*/
}