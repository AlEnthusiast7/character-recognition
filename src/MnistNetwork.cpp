#include "MnistNetwork.h"
#include <cmath>
#include <iostream>

float sigmoid(float val) { return 1.0f / (1.0f + exp(-val)); }

float sigmoid_derivative(float sigmoid_output) {
  return sigmoid_output * (1 - sigmoid_output);
}
MnistNetwork::MnistNetwork(std::vector<int> layer_data, float lr)
    : learning_rate(lr) {
  // skip the input layer
  for (int i = 1; i < layer_data.size(); ++i) {
    int prev_nodes = layer_data[i - 1];
    int current_nodes = layer_data[i];

    // create and randomize weights
    Matrix weight(current_nodes, prev_nodes);
    weight.randomize();
    weights.push_back(weight);

    // create and randomize biases
    Matrix bias(current_nodes, 1);
    bias.randomize();
    layer_biases.push_back(bias);
  }
}

bool MnistNetwork::train(MnistLoader &dataLoader) {
  // lock so no other predictions can be made until training is done
  network_mutex.lock();

  int count = 0;
  std::cout << "Starting training now..." << std::endl;
  while (dataLoader.training_images_read < dataLoader.number_of_images1) {
    // get image in matrix format (1, 28*28)
    Matrix image = dataLoader.read_training_image();

    // gets all of the calculated neurons aka the brain
    std::vector<Matrix> brain = predict(image);
    // get the correct output for that image
    int Y = dataLoader.read_training_label();
    backpropagate(brain, Y);

    ++count;
    std::cout << "\33[2K\rImages trained on: " << count << std::flush;
  }
  std::cout << "\nFinished training" << std::endl;

  network_mutex.unlock();
  return true;
}

float MnistNetwork::test(MnistLoader &dataLoader) {
  int count = 0;
  while (dataLoader.test_images_read < dataLoader.number_of_images2) {
    // get image in matrix format (1, 28*28)
    Matrix image = dataLoader.read_test_image();

    std::vector<Matrix> brain = predict(image);
    int Y = dataLoader.read_test_label();
    backpropagate(brain, Y);
    ++count;
  }
  return 0.0f;
}

void MnistNetwork::backpropagate(std::vector<Matrix> &brain,
                                 int correct_label) {
  // C = 0.5(A-Y)**2
  // holds dC/dZ for the current neurons
  Matrix current_errors(brain.back().rows, 1);

  for (int i = 0; i < brain.back().rows; ++i) {
    float current_n = brain.back()(i, 0);
    float target = (i == correct_label) ? 1.0f : 0.0f;
    current_errors(i, 0) = (current_n - target);
  }

  for (int i = weights.size() - 1; i >= 0; --i) {
    Matrix A = brain[(i + 1) * 2];
    Matrix A_prev = brain[i * 2];

    // dC/dA * dA/dZ = dC/dA * A(1-A)
    for (int r = 0; r < current_errors.rows; r++) {
      current_errors(r, 0) = current_errors(r, 0) * A(r, 0) * (1 - A(r, 0));
    }

    // dC/dZ * dZ/dW = dC/dZ * A_prev
    Matrix A_prev_T = A_prev.transpose();
    Matrix weight_deltas = Matrix::dot(current_errors, A_prev_T);

    // biases are a constant so - dC/dZ
    layer_biases[i] = layer_biases[i] - (current_errors * learning_rate);

    //
    if (i > 0) {
      Matrix weights_i_T = weights[i].transpose();
      current_errors = Matrix::dot(weights_i_T, current_errors);
    }

    weights[i] = weights[i] - (weight_deltas * learning_rate);
  }
}

std::vector<Matrix> MnistNetwork::predict(Matrix flattened_image_T) {
  // set up the return vector
  std::vector<Matrix> brain;
  brain.reserve((2 * weights.size()) + 1);

  Matrix next_layer = flattened_image_T / 255.0f;
  // add the first set of inputs
  brain.push_back(next_layer);

  for (int i = 0; i < layer_biases.size(); ++i) {
    brain.push_back(weights[i]);
    next_layer = Matrix::dot(weights[i], next_layer) + layer_biases[i];

    next_layer = next_layer.apply(sigmoid);
    // append each layer of neuron
    brain.push_back(next_layer);
  }

  return brain;
}
