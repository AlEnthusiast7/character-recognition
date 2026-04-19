#include "MnistNetwork.h"
#include <cmath>
#include <iostream>

float sigmoid(float val) { return 1.0f / (1.0f + exp(-val)); }

float sigmoid_derivative(float sigmoid_output) {
  return sigmoid_output * (1 - sigmoid_output);
}

float relu(float val) { return (val > 0.0f) ? val : 0.0f; }
float relu_derivative(float val) { return (val > 0.0f) ? 1.0f : 0.0f; }

Matrix softmax(Matrix x) {
  Matrix result(x.rows, 1);

  float sum = 0.0f;
  for (int i = 0; i < x.rows; ++i) {
    result(i, 0) = exp(x(i, 0));
    sum += result(i, 0);
  }
  result = result / sum;

  return result;
}

MnistNetwork::MnistNetwork(std::vector<int> layer_data, float lr)
    : learning_rate(lr) {
  // skip the input layer
  for (int i = 1; i < layer_data.size(); ++i) {
    int prev_nodes = layer_data[i - 1];
    int current_nodes = layer_data[i];

    // create and randomize weights
    Matrix weight(current_nodes, prev_nodes);
    weight.He_randomize((float)layer_data[i - 1]);
    weights.push_back(weight);

    // create and randomize biases
    Matrix bias(current_nodes, 1);
    bias.zero();
    layer_biases.push_back(bias);
  }
}

bool MnistNetwork::train(MnistLoader &dataLoader, int epochs) {
  // lock so no other predictions can be made until training is done
  network_mutex.lock();

  int count = 0;
  std::cout << "Starting training now..." << std::endl;
  for (int i = 0; i < epochs; ++i) {
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
    std::cout << "\nEpochs finished: " << i << std::endl;
    float accuracy = test(dataLoader);
    std::cout << "Accuracy: " << accuracy << std::endl;
    // reset dataloader
    dataLoader.verify();
  }
  std::cout << "\nFinished training" << std::endl;

  network_mutex.unlock();
  return true;
}

float MnistNetwork::test(MnistLoader &dataLoader) {
  int count = 0;
  int correct = 0;
  std::cout << "Testing the model..." << std::endl;
  while (dataLoader.test_images_read < dataLoader.number_of_images2) {
    // get image in matrix format (1, 28*28)
    Matrix image = dataLoader.read_test_image();

    std::vector<Matrix> brain = predict(image);
    int Y = dataLoader.read_test_label();

    int prediction = 0;
    for (int i = 0; i < brain.back().rows; ++i)
      if (brain.back()(i, 0) > brain.back()(prediction, 0))
        prediction = i;

    if (prediction == Y)
      ++correct;
    ++count;
  }

  return (float)correct / count;
}

void MnistNetwork::backpropagate_mse(std::vector<Matrix> &brain,
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

    // ALTER WEIGHTS AFTER current_errors
    weights[i] = weights[i] - (weight_deltas * learning_rate);
  }
}

void MnistNetwork::backpropagate(std::vector<Matrix> &brain,
                                 int correct_label) {
  // holds dL/dZ for the current neurons
  Matrix current_errors(brain.back().rows, 1);

  // SOFTMAX + CROSS ENTROPY DERIVATIVE
  for (int i = 0; i < brain.back().rows; ++i) {
    float a = brain.back()(i, 0); // current neuron
    float y = (i == correct_label) ? 1.0f : 0.0f;
    current_errors(i, 0) = a - y;
  }

  for (int i = weights.size() - 1; i >= 0; --i) {
    Matrix A = brain[(i + 1) * 2];
    Matrix A_prev = brain[i * 2];

    // relu is not applied to the output layer
    if (i != weights.size() - 1) {
      // dC/dA * dA/dZ = dC/dA * A(1-A)
      // A = RELU
      for (int r = 0; r < current_errors.rows; r++) {
        current_errors(r, 0) = current_errors(r, 0) * relu_derivative(A(r, 0));
      }
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

    // ALTER WEIGHTS AFTER current_errors
    weights[i] = weights[i] - (weight_deltas * learning_rate);
  }
}

std::vector<Matrix> MnistNetwork::predict_sigmoid(Matrix flattened_image_T) {
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

    // don't activate the last layer
    if (i != layer_biases.size() - 1)
      next_layer = next_layer.apply(relu);
    // append each layer of neuron
    brain.push_back(next_layer);
  }

  Matrix output = brain.back();
  brain.back() = softmax(output);

  return brain;
}
