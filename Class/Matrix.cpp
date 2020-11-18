#include "Matrix.h"

template <typename T>
Matrix<T>::Matrix(): height(0), width(0), matrix(nullptr) {}

template <typename T>
Matrix<T>::Matrix(const Matrix& other): height(other.height), width(other.width), matrix(new T[height * width]) {
	std::memcpy(other.matrix, this->matrix, sizeof(T) * height * width);
}

template <typename T>
Matrix<T>::Matrix(Matrix&& other): height(other.height), width(other.width), matrix(other.matrix) {
	other.height = 0;
	other.width = 0;
	other.matrix = nullptr;
}

template <typename T>
Matrix<T>::Matrix(int _height, int _width): height(_height), width(_width), matrix(new T[height * width]()) {}

template <typename T>
Matrix<T>::Matrix(int _height, int _width, const T& value): height(_height), width(_width), matrix(new T[height*width]) {
	std::fill_n(matrix, height * width, value);
}

template <typename T>
Matrix<T>::~Matrix() {
	delete matrix;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& rhs) {
	this->~Matrix();

	height = rhs.height;
	width = rhs.width;

	matrix = new T[height * width];
	std::memcpy(this->matrix, rhs.matrix, sizeof(T) * height * width);

	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& rhs) {
	this->~Matrix();

	height = rhs.height;
	width = rhs.width;
	matrix = rhs.matrix;

	rhs.height = 0;
	rhs.width = 0;
	rhs.matrix = nullptr;

	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& rhs) const {
	if (height != rhs.height || width != rhs.width) {
		throw "matrix dimensions are not equal";
	}

	Matrix<T> result(height, width);
	for (int i = 0; i < height * width; ++i) {
		result.matrix[i] = this->matrix[i] + rhs.matrix[i];
	}
	return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& rhs) const {
	if (height != rhs.height || width != rhs.width) {
		throw "matrix dimensions are not equal";
	}

	Matrix<T> result(height, width);
	for (int i = 0; i < height * width; ++i) {
		result.matrix[i] = this->matrix[i] - rhs.matrix[i];
	}
	return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& rhs) const {
	if (width != rhs.height) {
		throw "the number of columns of matrix 1 is not equal to the number of rows of matrix 2";
	}
	Matrix<T> result(height, rhs.width, 0);

	// some optimization of matrix multiplication

	// for comfort 
	int M = result.height;
	int N = result.width;
	int P = this->width;

	for (int i = 0; i < M; ++i) {
		T* resultLine = result.matrix + i * N;
		for (int j = 0; j < P; ++j) {
			T thisValue = this->matrix[i * P + j];
			T* rhsLine = rhs.matrix + N * j;
			for (int k = 0; k < N; ++k) {
				resultLine[k] += thisValue * rhsLine[k];
			}
		}
	}
	return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const T& value) const{
	Matrix result(*this);
	for (int i = 0; i < height * width; ++i) {
		result.matrix[i] *= value;
	}
	return result;
}

template <typename T>
T* Matrix<T>::operator[](int _height) {
	return matrix + width * _height;
}

template <typename T>
const T* Matrix<T>::operator[]( int _height) const {
	return matrix + width * _height;
}

template <typename T>
Matrix<T>& Matrix<T>::transpose() {
	swap(height, width);
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::getTransposed() const {
	Matrix result(*this);
	return result.transpose();
}

template <typename T>
vector<T> Matrix<T>::getVector() const {
	if (height != 1) {
		throw "it's not a vector";
	}

	vector<T> result(width);
	std::memcpy(&result[0], matrix, sizeof(T) * width);
	return result;
}

template <typename T>
vector<vector<T>> Matrix<T>::getVectorOfVectors() const {
	vector<vector<T>> result(height, vector<T>(width));
	for (int i = 0; i < height; ++i) {
		std::memcpy(&result[i][0], matrix + i * width, sizeof(T) * width);
	}
	return result;
}

template <typename T>
T Matrix<T>::getDeterminante() const {
	if (height != width) {
		throw "this isn't square matrix";
	}

	T result = T();
	T temp; // using for intermediate multiplication 
	int* transposition = new int[height];
	for (int i = 0; i < height; ++i) {
		transposition[i] = i;
	}

	while (true) {
		temp = 1;
		for (int i = 0; i < height; ++i) {
			temp *= matrix[i * width + transposition[i]];
		}
		for (int i = 0; i < height - 1; ++i) {  // sign
			for (int x = i + 1; x < height; ++x) {
				if (transposition[i] > transposition[x]) {
					temp *= -1;
				}
			}
		}
		result += temp;

		// next transposition
		int j = height - 2;
		while (j != -1 && transposition[j] > transposition[j + 1]) { 
			--j;
		}
		if (j == -1) {
			break;
		}
		int k = height - 1;
		while (transposition[j] > transposition[k]) {
			--k;
		}
		swap(transposition[j], transposition[k]);
		int l = j + 1, r = height - 1;
		while (l < r) {
			swap(transposition[l++], transposition[r--]);
		}
	}

	delete[] transposition;
	return result;
}

template <typename T>
int Matrix<T>::getHeight() const {
	return height;
}

template <typename T>
int Matrix<T>::getWidth() const{
	return width;
}

template <typename T>
void Matrix<T>::printfMatrix() const {
	try {
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				printf("%9.6f  ", matrix[i * width + j]);
			}
			printf("\n");
		}
	}
	catch (exception e) {
		throw "can not be printed default";
	}
}

template <typename T>
Matrix<T> Matrix<T>::createUnitMatrix(int size) {
	Matrix<T> result(size, size, 0);
	for (int i = 0; i < size; ++i) {
		result[i][i] = 1;
	}
	return result;
}
