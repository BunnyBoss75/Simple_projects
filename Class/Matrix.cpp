#include "Matrix.h"

Matrix::Matrix(): epsilon(10e-8), height(0), width(0), matrix(nullptr) {}

Matrix::Matrix(const Matrix& other): epsilon(other.epsilon), height(other.height), width(other.width) {
	matrix = new double [height * width];
	memcpy(other.matrix, this->matrix, sizeof(double) * height * width);
}

Matrix::Matrix(Matrix&& other): epsilon(other.epsilon), height(other.height), width(other.width), matrix(other.matrix) {
	other.height = 0;
	other.width = 0;
	other.matrix = nullptr;
}

Matrix::Matrix(int _height, int _width): epsilon(10e-8), height(_height), width(_width) {
	matrix = new double [height * width];
}

Matrix::Matrix(int _height, int _width, double value): epsilon(10e-8), height(_height), width(_width) {
	matrix = new double[height * width]{value};
	fill_n(*matrix, height * width, value);
}

Matrix::~Matrix() {
	delete matrix;
}

Matrix& Matrix::operator=(const Matrix& rhs) {
	this->~Matrix();

	epsilon = rhs.epsilon;
	height = rhs.height;
	width = rhs.width;

	matrix = new double [height * width];
	memcpy(rhs.matrix, this->matrix, sizeof(double) * height * width);

	return *this;
}

Matrix& Matrix::operator=(Matrix&& rhs) {
	this->~Matrix();

	epsilon = rhs.epsilon;
	height = rhs.height;
	width = rhs.width;
	matrix = rhs.matrix;

	rhs.height = 0;
	rhs.width = 0;
	rhs.matrix = nullptr;

	return *this;
}

bool Matrix::operator==(const Matrix& rhs) {
	if (height == rhs.height && width == rhs.width) {
		for (int i = 0; i < height * width; ++i) {
			if (abs(this->matrix[i] - rhs.matrix[i]) >= epsilon) {
				return false;
			}
		}
		return true;
	}
	return false;
}

Matrix Matrix::operator+(const Matrix& rhs) {
	if (height != rhs.height || width != rhs.width) {
		throw "matrix dimensions are not equal";
	}
	Matrix result(height, width);
	for (int i = 0; i < height * width; ++i) {
		result.matrix[i] = this->matrix[i] + rhs.matrix[i];
	}
	return result;
}

Matrix Matrix::operator-(const Matrix& rhs) {
	if (height != rhs.height || width != rhs.width) {
		throw "matrix dimensions are not equal";
	}
	Matrix result(height, width);
	for (int i = 0; i < height * width; ++i) {
		result.matrix[i] = this->matrix[i] - rhs.matrix[i];
	}
	return result;
}

Matrix Matrix::operator*(const Matrix& rhs) {
	if (width != rhs.height) {
		throw "the number of columns of matrix 1 is not equal to the number of rows of matrix 2";
	}
	Matrix result(height, rhs.width, 0);

	// some optimization of matrix multiplication

	// for comfort 
	int M = result.height;
	int N = result.width;
	int P = this->width;

	for (int i = 0; i < M; ++i) {
		double* resultLine = result.matrix + i * N;
		for (int j = 0; j < P; ++j) {
			double thisValue = this->matrix[i * P + j];
			double* rhsLine = rhs.matrix + N * j;
			for (int k = 0; k < N; ++k) {
				resultLine[k] += thisValue * rhsLine[k];
			}
		}
	}
	return result;
}

Matrix Matrix::operator*(const double number) {
	Matrix result(*this);
	for (int i = 0; i < this->height * this->width; ++i) {
		result.matrix[i] *= number;
	}
	return result;
}

double* Matrix::operator[](const int _height) {
	return matrix + width * _height;
}

const double* Matrix::operator[](const int _height) const {
	return matrix + width * _height;
}

Matrix& Matrix::transpose() {
	swap(height, width);
	return *this;
}

Matrix Matrix::getTransposed() {
	Matrix result(*this);
	return result.transpose();
}

vector<double> Matrix::getVector() {
	if (height != 1) {
		throw "it's not a vector";
	}
	vector<double> result(width);
	memcpy(matrix, &result[0], sizeof(double) * width);
	return result;
}

vector<vector<double>> Matrix::getVectorOfVectors() {
	vector<vector<double>> result(height, vector<double>(width));
	for (int i = 0; i < height; ++i) {
		memcpy(matrix + i * width, &result[i][0], sizeof(double) * width);
	}
	return result;
}

double Matrix::getDeterminante() const{
	if (height != width) {
		throw "this isn't square matrix";
	}

	double result = 0;
	double temp; // using for intermediate multiplication 
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

double Matrix::getHeight() const {
	return height;
}

double Matrix::getWidth() const{
	return width;
}

void Matrix::setEpsilon(double newEpsilon) {
	epsilon = newEpsilon;
}

double Matrix::getEpsilon() const { 
	return epsilon;
}

void Matrix::printfMatrix() {
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			printf("%9.6f  ", matrix[i][j]);
		}
		printf("\n");
	}
}

Matrix Matrix::createUnitMatrix(int size) {
	Matrix result(size, size, 0);
	for (int i = 0; i < size; ++i) {
		result[i][i] = 1;
	}
	return result;
}
