
#pragma once
#include <cassert>
#include <cmath>
#include <functional>
#include <vector>

class Matrix {
public:
  int rows;
  int cols;
  std::vector<float> data;

  Matrix(int r, int c);

  float &operator()(int r, int c);
  // accessing for read only
  const float &operator()(int r, int c) const;

  static Matrix dot(const Matrix &a, const Matrix &b);

  Matrix operator+(const Matrix &other) const;
  Matrix operator-(const Matrix &other) const;

  Matrix operator*(const Matrix &other) const;
  // scalar multiplication
  Matrix operator*(float scalar) const;
  Matrix operator/(float scalar) const;

  Matrix transpose() const;

  // left float is output and right float is input
  Matrix apply(std::function<float(float)> func) const;

  // randomize the values in the matrix
  void randomize();

  // flattens the matrix into shape (1, rows*cols)
  Matrix flatten();

  // zero out all values
  void zero();

  // for debugging
  void print() const;
};
