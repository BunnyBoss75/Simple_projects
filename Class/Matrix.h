#pragma once

#include <iostream>
#include <vector>
using namespace std;

template <typename T>
class Matrix {
public:
	Matrix();
	Matrix(const Matrix& other);
	Matrix(Matrix&& other);
	
	Matrix(int _heigth, int _width);
	Matrix(int _height, int _width, const T& value);

	~Matrix();

	Matrix<T>& operator=(const Matrix<T>& rhs);
	Matrix<T>& operator=(Matrix<T>&& rhs);

	Matrix<T> operator+(const Matrix<T>& rhs) const;
	Matrix<T> operator-(const Matrix<T>& rhs) const;
	Matrix<T> operator*(const Matrix<T>& rhs) const;

	Matrix<T> operator*(const T& value) const;

	T* operator[](int _height);
	const T* operator[](int _height) const;

	Matrix& transpose();
	Matrix getTransposed() const;
	vector<T> getVector() const;
	vector<vector<T>> getVectorOfVectors() const;
	T getDeterminante() const;

	int getHeight() const;
	int getWidth() const;

	void printfMatrix() const;
	
	static Matrix<T> createUnitMatrix(int size);
private:
	int height;
	int width;
	T* matrix;
};