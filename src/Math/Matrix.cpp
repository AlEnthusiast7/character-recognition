#include "Matrix.h"
#include <cmath>
#include <iostream>
#include <random>

Matrix::Matrix(int r, int c) : rows(r), cols(c) { data.resize(r * c, 0.0f); }

float &Matrix::operator()(int r, int c) { return data.at((r * cols) + c); }

Matrix Matrix::flatten() {
  Matrix result(1, rows * cols);
  result.data = data;

  return result;
}

const float &Matrix::operator()(int r, int c) const {
  return data.at((r * cols) + c);
}

Matrix Matrix::dot(const Matrix &a, const Matrix &b) {
  assert(a.cols == b.rows && "Dimension mismatch in dot product");

  // transpose first so memory is accessed sequentially
  Matrix b_t = b.transpose();

  Matrix result(a.rows, b.cols);

  for (int i = 0; i < a.rows; i++) {
    for (int j = 0; j < b_t.rows; j++) {
      float sum = 0.0f;
      for (int k = 0; k < a.cols; k++) {
        sum += a(i, k) * b_t(j, k);
      }
      result(i, j) = sum;
    }
  }

  return result;
}

Matrix Matrix::operator+(const Matrix &other) const {
  assert(rows == other.rows && cols == other.cols);
  Matrix result(rows, cols);

  for (size_t i = 0; i < data.size(); i++) {
    result.data[i] = data[i] + other.data[i];
  }
  return result;
}

Matrix Matrix::operator-(const Matrix &other) const {
  assert(rows == other.rows && cols == other.cols);
  Matrix result(rows, cols);

  for (size_t i = 0; i < data.size(); i++) {
    result.data[i] = data[i] - other.data[i];
  }
  return result;
}

Matrix Matrix::operator*(const Matrix &other) const {
  assert(rows == other.rows && cols == other.cols);
  Matrix result(rows, cols);

  for (size_t i = 0; i < data.size(); i++) {
    result.data[i] = data[i] * other.data[i];
  }
  return result;
}

Matrix Matrix::operator*(float scalar) const {
  Matrix result(rows, cols);

  for (size_t i = 0; i < data.size(); i++) {
    result.data[i] = data[i] * scalar;
  }
  return result;
}

Matrix Matrix::operator/(float scalar) const {
  Matrix result(rows, cols);

  for (size_t i = 0; i < data.size(); i++) {
    result.data[i] = data[i] / scalar;
  }
  return result;
}

Matrix Matrix::transpose() const {
  Matrix result(cols, rows);

  for (size_t i = 0; i < data.size(); i++) {
    int r = (float)i / (float)cols;
    int c = i % cols;
    result(c, r) = (*this)(r, c);
  }

  return result;
}

Matrix Matrix::apply(std::function<float(float)> func) const {
  Matrix result(rows, cols);
  for (size_t i = 0; i < data.size(); i++) {
    result.data[i] = func(data[i]);
  }

  return result;
}

void Matrix::randomize() {
  // seed to start
  // randome device is slow, need to use an alg
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);
  for (auto &i : data) {
    i = dist(gen);
  }
}

void Matrix::He_randomize(float n) {
  // seed to start
  // randome device is slow, need to use an alg
  std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<float> dist(0.0f, std::sqrt(2.0f / n));
  for (auto &i : data) {
    i = dist(gen);
  }
}

void Matrix::zero() {
  // set all values to 0
  for (auto &i : data) {
    i = 0.0f;
  }
}

void Matrix::print() const {
  for (int i = 0; i < rows; i++) {
    std::cout << "| ";
    for (int j = 0; j < cols; j++) {
      std::cout << (*this)(i, j) << " ";
    }
    std::cout << "|" << std::endl;
  }
}
