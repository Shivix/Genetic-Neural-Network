#ifndef CONNECT4_WITH_REINFORCEMENT_LEARNING_NEURON_HPP
#define CONNECT4_WITH_REINFORCEMENT_LEARNING_NEURON_HPP
#include <random>
#include <vector>

namespace GNN{
    static std::random_device randomDevice; // used to seed RNG engine. Will attempt to use hardware entropy if possible
    static std::mt19937 engine(randomDevice()); // Mersenne-twister engine used for random number generation
    static std::uniform_real_distribution<float> weightDistribution(-1, 1.0); // calling as static gives less overhead
    
    class neuron{
    public:
        explicit neuron(int connections){ // randomly sets all weights and adds the connections to a vector
            connectionVec.resize(connections);
            int inputIter = -1;
            for(auto&& i: connectionVec){
                i.weight = weightDistribution(engine);
                i.input = ++inputIter;
            }
        }
        neuron(const neuron& neuron) = default; // rule of zero could easily (and should) be followed here, but I decided to implement the move constructor and assignment myself
        neuron(neuron&& neuron) noexcept:
            value(std::move(neuron.value)),
            bias(std::move(neuron.bias)),
            connectionVec(std::exchange(this->connectionVec, neuron.connectionVec))
        {};
        ~neuron()= default;
        neuron& operator = (const neuron& neuron) = default;
        neuron& operator = (neuron&& neuron) noexcept{
            this->value = std::move(neuron.value);
            this->connectionVec = std::move(neuron.connectionVec);
            neuron.connectionVec.clear();
            this->bias = std::move(neuron.bias);
            return *this;}
    public:
        float value = 0.0f;
        struct connection{ // connections going IN to the neuron
            float weight;
            int input;
        };
        
        typedef float (*function)(float a);
        void activation(function activationFunc){ // can allow the library user to provide their own nonlinear function
            activationFunc(value);
        }
        [[nodiscard]] auto getConnectionVec() const{ // provides better encapsulation since connectionVec should never be edited outside of the class
            return connectionVec;
        }
        void sigmoid() { // runs when a neuron is fed from all previous neurons
            value = value / (1 + std::abs(value));
        }
        void tanh() const { // runs when a neuron is fed from all previous neurons
            std::tanh(value);
        }
        template <typename charT, typename traits>
        friend std::basic_ostream<charT,traits>& operator << (std::basic_ostream<charT,traits>& os, const connection& connection){// makes the content of a neuron easier to output.
            os << "Weight:" << connection.weight << "from:" << connection.input << "neuron\n";
            return os;
        }
        friend bool operator > (const neuron& neuron1, const neuron& neuron2){
            return neuron1.value > neuron2.value;
        }
        friend bool operator < (const neuron& neuron1, const neuron& neuron2){
            return neuron1.value < neuron2.value;
        }
    private:
        float bias = 0;
        std::vector<connection> connectionVec = {};
    };
}



#endif //CONNECT4_WITH_REINFORCEMENT_LEARNING_NEURON_HPP
