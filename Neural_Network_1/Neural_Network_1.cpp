// A - array with data, other variables in one letter - counters
#include <fstream>
#include <iostream>
#include <ctime>
#include <string>
using namespace std;
double F_result(double** Coefficients, double** Data, int amount);
double Sigma(double x) {
	return 1 / (1 + exp(-x));
}
int main() {
	int i = 0, j = 0;
	int best_coefficients = 0, best_i = 0, best_j = 0;
	double best_result = 0;
	double coef = 0.001;
	int amount = 2, check1 = 0;
	int count_1 = 0;
	double previous, now;
	char* context;
	ifstream train("train.txt");
	char** Train = new char*[amount];
	for (i = 0; i < amount; i++) {
		Train[i] = new char[1000];
		train >> Train[i];
	}
	string** Train1 = new string*[amount];
	for (i = 0; i < amount; i++)
		Train1[i] = new string[13];
	for (i = 0; i < amount; i++) {
		Train1[i][0] = strtok_s(Train[i], ",", &context);
		for (j = 1; j < 13; j++)
			Train1[i][j] = strtok_s(NULL, ",", &context);
	}
	double** A = new double* [amount];
	for (i = 0; i < amount; i++)
		A[i] = new double[11];
	for (i = 0; i < amount; i++) {
		A[i][0] = atof(Train1[i][1].c_str()); // Survived
		if (Train1[i][2] == "$")
			A[i][1] = 0;
		else
			A[i][1] = atof(Train1[i][2].c_str()); // Pclass
		Train1[i][3] += Train1[i][4];
		const char* temporality = Train1[i][3].c_str();
		A[i][2] = 0;
		for (j = 0; j < strlen(temporality); j++)
			A[i][2] += (int)temporality[j];
		A[i][2] *= 0.01; // Name
		if (Train1[i][5] == "male")
			A[i][3] = 1;
		else
			A[i][3] = 2; // Sex
		if (Train1[i][6] == "$")
			A[i][4] = 0;
		else
			A[i][4] = atof(Train1[i][6].c_str()); // Age
		if (Train1[i][7] == "$")
			A[i][5] = 0;
		else
			A[i][5] = atof(Train1[i][7].c_str()); // SibSp
		if (Train1[i][8] == "$")
			A[i][6] = 0;
		else
			A[i][6] = atof(Train1[i][8].c_str()); // Parch
		A[i][7] = 0;
		if (Train1[i][9] != "$") {
			temporality = Train1[i][9].c_str();
			for (j = 0; j < strlen(temporality); j++)
				A[i][7] += (int)temporality[j];
			A[i][7] *= 0.01; // Ticket
		}
		if (Train1[i][10] == "$")
			A[i][8] = 0;
		else
			A[i][8] = atof(Train1[i][10].c_str()) * 0.1; // Fare
		A[i][9] = 0;
		if (Train1[i][11] != "$") {
			temporality = Train1[i][11].c_str();
			for (j = 0; j < strlen(temporality); j++)
				A[i][9] += (int)temporality[j];
			A[i][9] *= 0.01; // Cabin
		}
		if (Train1[i][12] == "$")
			A[i][10] = 0;
		else if (Train1[i][12] == "S")
			A[i][10] = 1;
		else if (Train1[i][12] == "C")
			A[i][10] = 2;
		else
			A[i][10] = 3; // Embarked
	}
	train.close();
	ifstream IFcoefficients("coefficient.txt");
	double** Coefficients = new double* [51];
	for (i = 0; i < 51; i++) {
		Coefficients[i] = new double[10];
		for (j = 0; j < 10; j++)
			IFcoefficients >> Coefficients[i][j];
	}
	IFcoefficients.close();
	previous = F_result(Coefficients, A, amount);
	best_result = previous;
	while (1) {
		for (i = 0; i < 510; i++) {
			for (j = -1; j < 2; j += 2) {
				Coefficients[(i - (i % 10)) / 10][i % 10] += coef * (double)j * (100.0 - previous);
				if (Coefficients[(i - (i % 10)) / 10][i % 10] < 100 && Coefficients[(i - (i % 10)) / 10][i % 10] > -100) {
					now = F_result(Coefficients, A, amount);
					if (now > best_result) {
						best_i = i;
						best_j = j;
						best_result = now;
					}
				}
				Coefficients[(i - (i % 10)) / 10][i % 10] -= coef * (double)j * (100.0 - previous);
			}
		}
		if (best_result > previous) {
			Coefficients[(best_i - (best_i % 10)) / 10][best_i % 10] += coef * (double)best_j * (100.0 - previous);
			count_1++;
			if (count_1 % 1 == 0)
				cout << "result is: " << best_result << " up is: " << best_result - previous << " delta is " << coef * (double)best_j * (100.0 - previous) <<" coef is: " << best_i << " count_ is: " << count_1 << "\n";
			if (count_1 == 1000)
				break;
			previous = best_result;
			continue;
		}
		else {
			cout << "It is the end\n";
			break;
		}
	}
	ofstream OFcoefficients("coefficient.txt");
	for (i = 0; i < 51; i++) {
		for (j = 0; j < 10; j++)
			OFcoefficients << Coefficients[i][j] << " ";
		OFcoefficients << "\n";
	}
	OFcoefficients.close();
	system("pause");
}
double F_result(double** Coefficients, double** Data, int amount) {
	int i = 0, j = 0, x = 0, k = 0;
	double result = 0, result1 = 0;
	double** A = new double* [6];
	for (i = 0; i < 6; i++)
		A[i] = new double[10];
	for (x = 0; x < amount; x++) {
		bool survived = Data[x][0];
		for (i = 0; i < 11; i++)
			A[0][i] = Data[x][i + 1];
		for (i = 1; i < 6; i++)
			for (j = 0; j < 10; j++) {
				A[i][j] = 0;
				for (k = 0; k < 10; k++)
					A[i][j] += A[i - 1][k] * Coefficients[(i - 1) * 10 + j][k];
				A[i][j] *= 0.1;
			}
		result1 = 0;
		for (i = 0; i < 10; i++)
			result1 += A[5][i] * Coefficients[50][i];
		result1 = 100 * Sigma(result1);
	    //cout << result1 << "    " << survived << "\n";
		if (survived)
			result += result1;
		else
			result += (100 - result1);
	}
	for (i = 0; i < 6; i++) {
		delete A[i];
	}
	delete[] A;
	return result/amount;
}