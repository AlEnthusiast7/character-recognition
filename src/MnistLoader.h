#pragma once
#include <fstream>
#include <string>

#include "Math/Matrix.h"

class MnistLoader {
public:
  // data needed for reading training data
  int number_of_images1, n_rows1, n_cols1;
  // keep track of images read
  int training_images_read;

  // training data files
  std::ifstream training_images;
  std::ifstream training_labels;

  // data for reading test data
  int number_of_images2, n_rows2, n_cols2;
  int test_images_read;

  // testing data files
  std::ifstream test_images;
  std::ifstream test_labels;

  // to reverse bytes
  // big endian -> little endian
  int reverseInt(int i);
  MnistLoader(std::string training_data_filepath,
              std::string training_labels_filepath,
              std::string test_data_filepath, std::string test_labels_filepath);

  // get next image/label
  Matrix read_training_image();
  int read_training_label();

  // get next test image/label
  Matrix read_test_image();
  int read_test_label();

  // baseplates for reading images/labels
  Matrix read_image(std::ifstream &mode, int r, int c);
  int read_label(std::ifstream &mode);
};
