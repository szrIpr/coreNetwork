#pragma once
#include <initializer_list>
#include <vector>
#include <random>
#include <chrono>
#include "./Matrix.hpp"

float sigmoid(float x) { return 1.0f / (1.0f + exp(-x)); }
float d_sigmoid(float x) { return x * (1.0f - x); }
float relu(float x) { if(x < 0) return 0.0f; else return x; }
float d_relu(float x) { if(x < 0) return 0.0f; else return 1.0f; }

class NeuralNetwork {
private:
    std::vector<Matrix> weights;
    std::vector<Matrix> biases;

    std::normal_distribution<float> dist;
    std::default_random_engine gen;

public:
    float learningRate = 0.1f;

    NeuralNetwork(const int numOfInputs, std::initializer_list<const int> numOfHiddens, const int numOfOutputs)
    : dist(0.0f, 2.0f), gen(std::random_device()()) {
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

        for(auto& weight : weights)
            weight.map([this](float x){ return dist(gen); });
    }

    Matrix forward(Matrix& inputs)
    {
        Matrix output = dot(weights[0], inputs);
        output += biases[0];
        output.map(relu);

        for(int i = 1, max = weights.size()-1; i != max; i++) {
            output = dot(weights[i], output);
            output += biases[i];
            output.map(relu);
        }
        output = dot(weights.back(), output);
        output += biases.back();
        output.map(sigmoid);

        return output;
    }

    void backward(Matrix& input, Matrix& target) {
        std::vector<Matrix> acts;
        acts.reserve(weights.size()+1);

        acts.push_back(input);

        for(int i = 0, max = weights.size()-1; i < max; i++) {
            acts.push_back(dot(weights[i], acts[i]));
            acts.back() += biases[i];
            acts.back().map(relu);
        }
        acts.push_back(dot(weights.back(), acts.back()));
        acts.back() += biases.back();
        acts.back().map(sigmoid);

        std::vector<Matrix> errors;
        errors.reserve(weights.size());

        errors.push_back(target - acts.back());
        errors.back() *= map(acts.back(), d_sigmoid);
        errors.back() *= learningRate;

        for(int i = weights.size()-1; i >= 1; i--) {
            errors.push_back(dot(transpose(weights[i]), errors.back()));
            errors.back() *= map(acts[i], d_relu);
            errors.back() *= learningRate;
        }

        for(int i = 0, max = weights.size(); i < max; i++) {
            weights[i] += dot(errors[max-i-1], transpose(acts[i]));
            biases[i] += errors[max-i-1];
        }
    }
};