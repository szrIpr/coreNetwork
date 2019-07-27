#include <chrono>
#include <random>
#include "./NeuralNetwork.hpp"
#include "./Matrix.hpp"

int main() {
    auto start = std::chrono::system_clock::now();

    std::default_random_engine gen;
    std::uniform_int_distribution<int> dist(0, 3);

    std::vector<Matrix> inputs= {{{1.0f},{1.0f}},
                                {{1.0f}, {0.0f}},
                                {{0.0f}, {1.0f}},
                                {{0.0f}, {0.0f}}};

    std::vector<Matrix> targets={{{0.0f}}, {{1.0f}}, {{1.0f}}, {{0.0f}}};

    NeuralNetwork nn(2, {2}, 1);

    for(int i = 0; i < 5000; i++) {
        int rand = dist(gen);
        nn.backward(inputs[rand], targets[rand]);
    }

    std::cout << std::fixed;
    print(nn.forward(inputs[0]));
    print(nn.forward(inputs[1]));
    print(nn.forward(inputs[2]));
    print(nn.forward(inputs[3]));

    auto end = std::chrono::system_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "\n";

    return 0;
}