#pragma once
#include <initializer_list>
#include <vector>
#include <random>
#include <chrono>
#include "./Matrix.hpp"

float randomize(float x) {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    std::default_random_engine gen(std::chrono::duration_cast<std::chrono::nanoseconds>(now).count());
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    return dist(gen);
}

float sigmoid(float x) { return 1 / (1 + exp(-x)); }
float derivative(float x) { return x * (1.0f - x); }

class NeuralNetwork {
private:
    std::vector<Matrix> weights;
    std::vector<Matrix> biases;

public:
    float learningRate = 0.1f;

    NeuralNetwork(const int numOfInputs, std::initializer_list<const int> numOfHiddens, const int numOfOutputs)
    {
        weights.reserve(numOfHiddens.size()+1);
        biases.reserve(numOfHiddens.size()+1);

        weights.emplace_back(*numOfHiddens.begin(), numOfInputs);
        biases.emplace_back(*numOfHiddens.begin(), 1);

        for(auto it = numOfHiddens.begin()+1; it != numOfHiddens.end(); it++) {
            weights.emplace_back(*it, *(it-1));
            biases.emplace_back(*it, 1);
        }

        weights.emplace_back(numOfOutputs, *(numOfHiddens.end()-1));
        biases.emplace_back(numOfOutputs, 1);

        for(auto& weight : weights) weight.map(randomize);
        for(auto& bias : biases) bias.map(randomize);
    }

    Matrix forward(Matrix& inputs) {
        Matrix outputs = weights[0] * inputs;
        outputs += biases[0];
        outputs.map(sigmoid);

        for(int i = 1; i < weights.size(); i++) {
            outputs = weights[i] * outputs;
            outputs += biases[i];
            outputs.map(sigmoid);
        }
        return outputs;
    }

    void backward(Matrix& inputs, Matrix& targets) {
    }
};