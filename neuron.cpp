#include <cmath>
#include "neuron.hpp"


neuron::neuron(int connections){

    static std::uniform_real_distribution<float> randomDistribution(0, 1.0); // negative weights?
    connectionVec.resize(connections);
    for(auto &&i: connectionVec){
        i.weight = randomDistribution(engine);
    }
}

neuron::~neuron()= default;

void neuron::sigmoid() { // runs when a neuron is fed from all previous neurons

    value = value / (1 + (std::abs(value)));

}

void neuron::activation(){

    
    
    
    
    
}
