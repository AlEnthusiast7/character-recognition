#include "MnistLoader.h"

MnistLoader::MnistLoader(std::string tdf1, std::string tlf1, std::string tdf2,
                         std::string tlf2)
    : training_images(tdf1, std::ios::binary),
      training_labels(tlf1, std::ios::binary),
      // ios::binary is needed for windows machines
      // to interpret binary data correctly
      test_images(tdf2, std::ios::binary), test_labels(tlf2, std::ios::binary),
      //
      test_images_read(0), training_images_read(0) {
  // check training labels magic number
  int magic_number = 0;
  training_labels.read((char *)&magic_number, sizeof(magic_number));
  magic_number = reverseInt(magic_number);

  if (magic_number != 2049) {
    throw std::runtime_error(
        "Magic number mismatch for training_labels, expecting 2049");
  }

  // read # of labels to move cursor
  int num_labels = 0;
  training_labels.read((char *)&num_labels, sizeof(num_labels));

  // check test labels magic number
  magic_number = 0;
  test_labels.read((char *)&magic_number, sizeof(magic_number));
  magic_number = reverseInt(magic_number);

  if (magic_number != 2049) {
    throw std::runtime_error(
        "Magic number mismatch for test_labels, expecting 2049");
  }

  // read # of labels to move cursor
  num_labels = 0;
  test_labels.read((char *)&num_labels, sizeof(num_labels));

  // check training images magic number
  magic_number = 0;
  training_images.read((char *)&magic_number, sizeof(magic_number));
  magic_number = reverseInt(magic_number);

  if (magic_number != 2051) {
    throw std::runtime_error(
        "Magic number mismatch for training_images, expecting 2051");
  }

  // get number of images
  number_of_images1 = 0;
  training_images.read((char *)&number_of_images1, sizeof(number_of_images1));
  number_of_images1 = reverseInt(number_of_images1);

  // get number of rows
  n_rows1 = 0;
  training_images.read((char *)&n_rows1, sizeof(n_rows1));
  n_rows1 = reverseInt(n_rows1);

  // get number of columns
  n_cols1 = 0;
  training_images.read((char *)&n_cols1, sizeof(n_cols1));
  n_cols1 = reverseInt(n_cols1);

  printf("\nTraining images: %i, rows: %i cols: %i", number_of_images1, n_rows1,
         n_cols1);

  // check test images magic number
  magic_number = 0;
  test_images.read((char *)&magic_number, sizeof(magic_number));
  magic_number = reverseInt(magic_number);

  if (magic_number != 2051) {
    throw std::runtime_error(
        "Magic number mismatch for test_images, expecting 2051");
  }

  // get number of images
  number_of_images2 = 0;
  test_images.read((char *)&number_of_images2, sizeof(number_of_images2));
  number_of_images2 = reverseInt(number_of_images2);

  // get number of rows
  n_rows2 = 0;
  test_images.read((char *)&n_rows2, sizeof(n_rows2));
  n_rows2 = reverseInt(n_rows2);

  // get number of columns
  n_cols2 = 0;
  test_images.read((char *)&n_cols2, sizeof(n_cols2));
  n_cols2 = reverseInt(n_cols2);

  printf("\nTest images: %i, rows: %i cols: %i\n", number_of_images2, n_rows2,
         n_cols2);
}

// reverses the order of bytes in i
// mnist database is in big endian
int MnistLoader::reverseInt(int i) {
  unsigned char b1, b2, b3, b4;

  // right shift to get byte at place X
  //  and operator with 1111 1111, to only get the right most bits
  b1 = i & 255;
  b2 = (i >> 8) & 255;
  b3 = (i >> 16) & 255;
  b4 = (i >> 24) & 255;
  return (b1 << 24) + (b2 << 16) + (b3 << 8) + b4;
}

// reads training/testing images and returns a matrix
Matrix MnistLoader::read_training_image() {
  ++training_images_read;
  return read_image(training_images, n_rows1, n_cols1);
}

Matrix MnistLoader::read_test_image() {
  ++test_images_read;
  return read_image(test_images, n_rows2, n_cols2);
}

// reads training/testing labels and returns int
int MnistLoader::read_training_label() { return read_label(training_labels); }

int MnistLoader::read_test_label() { return read_label(test_labels); }

// baseplate for reading training_images/test
Matrix MnistLoader::read_image(std::ifstream &images, int rows, int cols) {
  Matrix image(rows, cols);
  // char size is 1 byte
  unsigned char value = 0;

  for (int i = 0; i < rows * cols; i++) {
    value = 0;
    images.read((char *)&value, sizeof(value));
    // reverse int not needed as its a single byte

    image.data.at(i) = value;
  }

  // flatten to (1, 28^2)
  image = image.flatten();
  // transpose to (28^2, 1)
  image = image.transpose();
  // normalize
  image = image / 255.0f;

  return image;
}

// baseplate for reading training_labels/test
int MnistLoader::read_label(std::ifstream &labels) {
  unsigned char label = 0;
  labels.read((char *)&label, sizeof(label));
  // reverse int not needed as its a single byte

  return label;
}
