#pragma once
#include <mutex>
#include <vector>

#include "Math/Matrix.h"
#include "MnistLoader.h"

class MnistNetwork {
public:
  std::mutex network_mutex;
  float learning_rate;
  // stores a matrix for each layer except input
  std::vector<Matrix> layer_biases;

  // stores a matrix for each set of weights
  std::vector<Matrix> weights;

  // trains the model
  bool train(MnistLoader &dataLoader, int epochs);
  // Tests the model and returns accuracy rate
  float test(MnistLoader &dataLoader);

  // inputs the image and returns the calculated network
  //  (1 - 9)
  std::vector<Matrix> predict(Matrix flattened_image_T);

  void backpropagate(std::vector<Matrix> &brain, int correct_label);

  // each num in layer_data corresponds to # of neurons in that layer
  // {784, 10, 10} = 784 inputs, 10 neurons in hidden layer, 10 output
  MnistNetwork(std::vector<int> layer_data, float lr = 0.01f);

  void backpropagate_mse(std::vector<Matrix> &brain, int correct_label);
  std::vector<Matrix> predict_sigmoid(Matrix flattened_image_T);
};
