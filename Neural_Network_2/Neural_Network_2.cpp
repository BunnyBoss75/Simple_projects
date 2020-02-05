#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
using namespace std;
class NeuralNetwork {
	private:
		int amount_of_level, amount_of_neurons_per_level, amount;
		double Sigma(double x) {
			return 1 / (1 + exp(-x));
		}
		double F_result(double** Coefficients, double* Data) {
			bool survived = Data[0];
			double** Neurons = new double* [amount_of_level];
			for (int i = 0; i < amount_of_level; i++) {
				Neurons[i] = new double[amount_of_neurons_per_level];
				for (int j = 0; j < amount_of_neurons_per_level; j++) {
					Neurons[i][j] = 0;
				}
			}
			for (int j = 0; j < amount_of_neurons_per_level; j++) {
				Neurons[0][j] = Data[j + 1];
				Neurons[0][j] = Sigma(Neurons[0][j]);
			}
			for (int i = 1; i < amount_of_level; i++) {
				for (int j = 0; j < amount_of_neurons_per_level; j++) {
					for (int k = 0; k < amount_of_neurons_per_level; k++) {
						Neurons[i][j] += Neurons[i - 1][k] * Coefficients[(i - 1) * amount_of_neurons_per_level + j][k];
					}
					Neurons[i][j] = Sigma(Neurons[i][j]);
				}
			}
			double result = 0;
			for (int i = 0; i < amount_of_neurons_per_level; i++) {
				result += Neurons[amount_of_level - 1][i] * Coefficients[(amount_of_level - 1) * amount_of_neurons_per_level][i];
			}
			result = Sigma(result);
			for (int i = 0; i < amount_of_level; i++) {
				delete Neurons[i];
			}
			delete[] Neurons;
			if (survived) {
				return result * 100;
			}
			else {
				return 100 - (result * 100);
			}
		}
		double** Read_coefficients() {
			double** Coefficients = new double* [(amount_of_level - 1) * amount_of_neurons_per_level + 1];
			for (int i = 0; i < (amount_of_level - 1) * amount_of_neurons_per_level + 1; i++) {
				Coefficients[i] = new double[amount_of_neurons_per_level];
			}
			ifstream IFcoefficients("coefficient.txt");
			for (int i = 0; i < (amount_of_level - 1) * amount_of_neurons_per_level + 1; i++) {
				for (int j = 0; j < amount_of_neurons_per_level; j++) {
					IFcoefficients >> Coefficients[i][j];
				}
			}
			IFcoefficients.close();
			return Coefficients;
		}
		double** Read_Train() {
			ifstream IFtrain("train.txt");
			char** Train = new char* [amount];
			for (int i = 0; i < amount; i++) {
				Train[i] = new char[1000];
				IFtrain >> Train[i];
			}
			string** Train1 = new string * [amount];
			for (int i = 0; i < amount; i++) {
				Train1[i] = new string[13];
			}
			for (int i = 0; i < amount; i++) {
				Train1[i][0] = strtok(Train[i], ",");
				for (int j = 1; j < 13; j++)
					Train1[i][j] = strtok(NULL, ",");
			}
			double** Data = new double* [amount];
			for (int i = 0; i < amount; i++)
				Data[i] = new double[11];
			for (int i = 0; i < amount; i++) {
				Data[i][0] = atof(Train1[i][1].c_str()); // Survived
				if (Train1[i][2] == "$")
					Data[i][1] = 0;
				else
					Data[i][1] = atof(Train1[i][2].c_str()); // Pclass
				Train1[i][3] += Train1[i][4];
				const char* temporality = Train1[i][3].c_str();
				Data[i][2] = 0;
				for (int j = 0; j < strlen(temporality); j++)
					Data[i][2] += (int)temporality[j];
				Data[i][2] *= 0.01; // Name
				if (Train1[i][5] == "male")
					Data[i][3] = 1;
				else
					Data[i][3] = 2; // Sex
				if (Train1[i][6] == "$")
					Data[i][4] = 0;
				else
					Data[i][4] = atof(Train1[i][6].c_str()); // Age
				if (Train1[i][7] == "$")
					Data[i][5] = 0;
				else
					Data[i][5] = atof(Train1[i][7].c_str()); // SibSp
				if (Train1[i][8] == "$")
					Data[i][6] = 0;
				else
					Data[i][6] = atof(Train1[i][8].c_str()); // Parch
				Data[i][7] = 0;
				if (Train1[i][9] != "$") {
					temporality = Train1[i][9].c_str();
					for (int j = 0; j < strlen(temporality); j++)
						Data[i][7] += (int)temporality[j];
					Data[i][7] *= 0.01; // Ticket
				}
				if (Train1[i][10] == "$")
					Data[i][8] = 0;
				else
					Data[i][8] = atof(Train1[i][10].c_str()) * 0.1; // Fare
				Data[i][9] = 0;
				if (Train1[i][11] != "$") {
					temporality = Train1[i][11].c_str();
					for (int j = 0; j < strlen(temporality); j++)
						Data[i][9] += (int)temporality[j];
					Data[i][9] *= 0.01; // Cabin
				}
				if (Train1[i][12] == "$")
					Data[i][10] = 0;
				else if (Train1[i][12] == "S")
					Data[i][10] = 1;
				else if (Train1[i][12] == "C")
					Data[i][10] = 2;
				else
					Data[i][10] = 3; // Embarked
			}
			IFtrain.close();
			return Data;
		}
	public:
		void Create_Coefficients() {
			srand(time(NULL));
			ofstream OFcoefficients("coefficient.txt");
			for (int i = 0; i < (amount_of_level - 1) * amount_of_neurons_per_level + 1; i++) {
				for (int j = 0; j < amount_of_neurons_per_level; j++) {
					OFcoefficients << ((rand() % 20) - 10) / 10.0 << " ";
				}
				OFcoefficients << endl;
			}
			OFcoefficients.close();
		}
		NeuralNetwork (int value_amount_of_level, int value_amount_of_neurons_per_level, int value_amount){
			amount_of_level = value_amount_of_level;
			amount_of_neurons_per_level = value_amount_of_neurons_per_level;
			amount = value_amount;
		}
		void Learn_1(int amount_of_iteration, double coefficient_of_delta) {
			double** Coefficients = Read_coefficients();
			double** Data = Read_Train();
			double delta = 0;
			int amount_of_coefficients = ((amount_of_level - 1) * amount_of_neurons_per_level + 1) * amount_of_neurons_per_level;
			for (int i = 0; i < amount_of_iteration; i++) {
				double result = 0;
				for (int j = 0; j < amount; j++) {
					result += F_result(Coefficients, Data[j]);
				}
				result = result / amount;
				cout << "result is " << result << endl;
				delta = coefficient_of_delta * (100 - result);
				bool up = false;
				for (int j = 0; j < amount; j++) {
					int best_k = 0, best_x = 0;
					double best_result = 0;
					for (int k = 0; k < amount_of_coefficients; k++) {
						for (int x = -1; x < 2; x += 2) {
							Coefficients[(k - (k % 10)) / 10][k % 10] += delta * x;
							double result_now = F_result(Coefficients, Data[j]);
							Coefficients[(k - (k % 10)) / 10][k % 10] -= delta * x;
							if (result_now > best_result) {
								best_result = result_now;
								best_k = k;
								best_x = x;
								up = true;
							}
						}
					}
					if (up) {
						Coefficients[(best_k - (best_k % 10)) / 10][best_k % 10] += delta * best_x;
					}
					else {
						cout << "up is failed" << endl;
					}
				}
			}
			double last_result = 0;
			for (int j = 0; j < amount; j++) {
				last_result += F_result(Coefficients, Data[j]);
			}
			last_result = last_result / amount;
			cout << "last result is " << last_result << endl;
			ofstream OFcoefficients("coefficient.txt");
			for (int i = 0; i < ((amount_of_level - 1) * amount_of_neurons_per_level + 1); i++) {
				for (int j = 0; j < amount_of_neurons_per_level; j++) {
					OFcoefficients << Coefficients[i][j] << " ";
				}
				OFcoefficients << endl;
			}
			OFcoefficients.close();
		}
		void Get_result(int number) {
			double** Coefficients = Read_coefficients();
			double** Data = Read_Train();
			cout << F_result(Coefficients, Data[number - 1]) << endl;
		}
		void Learn_2(int amount_of_iteration, double coefficient_of_delta) {
			double** Coefficients = Read_coefficients();
			double** Data = Read_Train();
			double** Error_per_neuron = new double* [amount_of_level - 1];
			double** New_coefficients = new double* [(amount_of_level - 1) * amount_of_neurons_per_level + 1];
			for (int i = 0; i < amount_of_level - 1; i++) {
				Error_per_neuron[i] = new double[amount_of_neurons_per_level];
				for (int j = 0; j < amount_of_neurons_per_level; j++) {
					Error_per_neuron[i][j] = 0;
				}
			}
			for (int i = 0; i < (amount_of_level - 1) * amount_of_neurons_per_level + 1; ++i) {
				New_coefficients[i] = new double[amount_of_neurons_per_level];
			}
			double delta = coefficient_of_delta, error, whole_coef;
			int amount_of_coefficients = ((amount_of_level - 1) * amount_of_neurons_per_level + 1) * amount_of_neurons_per_level; //without last level
			for (int z = 0; z < amount_of_iteration; ++z) {
				for (int i = 0; i < amount; i++) {
					//cout << F_result(Coefficients, Data[i]) << " ";
					error = 100 - F_result(Coefficients, Data[i]);
					whole_coef = 0;
					for (int j = 0; j < amount_of_neurons_per_level; j++) {
						whole_coef += abs(Coefficients[(amount_of_level - 1) * amount_of_neurons_per_level][j]);
					}
					for (int j = 0; j < amount_of_neurons_per_level; j++) {
						Error_per_neuron[amount_of_level - 2][j] = Coefficients[(amount_of_level - 1) * amount_of_neurons_per_level][j] * error / whole_coef;
						Coefficients[(amount_of_level - 1) * amount_of_neurons_per_level][j] += Coefficients[(amount_of_level - 1) * amount_of_neurons_per_level][j] * error * delta / whole_coef;
						if (F_result(Coefficients, Data[i]) + error > 100) {
							New_coefficients[(amount_of_level - 1) * amount_of_neurons_per_level][j] = Coefficients[(amount_of_level - 1) * amount_of_neurons_per_level][j] + Coefficients[(amount_of_level - 1) * amount_of_neurons_per_level][j] * error * delta / whole_coef;
						}
						else {
							New_coefficients[(amount_of_level - 1) * amount_of_neurons_per_level][j] = Coefficients[(amount_of_level - 1) * amount_of_neurons_per_level][j] - Coefficients[(amount_of_level - 1) * amount_of_neurons_per_level][j] * error * delta / whole_coef;
						}
						Coefficients[(amount_of_level - 1) * amount_of_neurons_per_level][j] -= Coefficients[(amount_of_level - 1) * amount_of_neurons_per_level][j] * error * delta / whole_coef;
					}
					for (int j = amount_of_level - 3; j > -1; j--) {
						for (int k = 0; k < amount_of_neurons_per_level; k++) {
							whole_coef = 0;
							for (int x = 0; x < amount_of_neurons_per_level; x++) {
								whole_coef += abs(Coefficients[(j + 1) * 10 + k][x]);
							}
							for (int x = 0; x < amount_of_neurons_per_level; x++) {
								Error_per_neuron[j][k] += Coefficients[(j + 1) * 10 + k][x] * Error_per_neuron[j + 1][x] / whole_coef;
								Coefficients[j * 10 + k][x] += Coefficients[j * 10 + k][x] * Error_per_neuron[j + 1][x] * delta / whole_coef;
								if (F_result(Coefficients, Data[i]) + error > 100) {
									New_coefficients[(j + 1) * 10 + k][x] = Coefficients[(j + 1) * 10 + k][x] + Coefficients[(j + 1) * 10 + k][x] * Error_per_neuron[j + 1][x] * delta / whole_coef;
								}
								else {
									New_coefficients[(j + 1) * 10 + k][x] = Coefficients[(j + 1) * 10 + k][x] - Coefficients[(j + 1) * 10 + k][x] * Error_per_neuron[j + 1][x] * delta / whole_coef;
								}
								Coefficients[(j + 1) * 10 + k][x] -= Coefficients[(j + 1) * 10 + k][x] * Error_per_neuron[j + 1][x] * delta / whole_coef;
							}
						}
					}
					for (int k = 0; k < amount_of_neurons_per_level; k++) {
						whole_coef = 0;
						for (int x = 0; x < amount_of_neurons_per_level; x++) {
							whole_coef += abs(Coefficients[k][x]);
						}
						for (int x = 0; x < amount_of_neurons_per_level; x++) {
							Coefficients[k][x] += Coefficients[k][x] * Error_per_neuron[0][x] * delta / whole_coef;
							if (F_result(Coefficients, Data[i]) + error > 100) {
								New_coefficients[k][x] = Coefficients[k][x] + Coefficients[k][x] * Error_per_neuron[0][x] * delta / whole_coef;
							}
							else {
								New_coefficients[k][x] = Coefficients[k][x] - Coefficients[k][x] * Error_per_neuron[0][x] * delta / whole_coef;
							}
							Coefficients[k][x] -= Coefficients[k][x] * Error_per_neuron[0][x] * delta / whole_coef;
						}
					}
					cout << error << endl;
					for (int ii = 0; ii < amount_of_level - 1; ++ii) {
						for (int iii = 0; iii < amount_of_neurons_per_level; ++iii) {
							cout << Error_per_neuron[ii][iii] << " ";
						}
						cout << endl;
					}
					for (int j = 0; j < (amount_of_level - 1) * amount_of_neurons_per_level + 1; ++j) {
						for (int k = 0; k < amount_of_neurons_per_level; ++k) {
							Coefficients[j][k] = New_coefficients[j][k];
						}
					}
					//cout << F_result(Coefficients, Data[i]) << "\n";
				}
			}
			ofstream OFcoefficients("coefficient.txt");
			for (int i = 0; i < ((amount_of_level - 1) * amount_of_neurons_per_level + 1); i++) {
				for (int j = 0; j < amount_of_neurons_per_level; j++) {
					OFcoefficients << Coefficients[i][j] << " ";
				}
				OFcoefficients << endl;
			}
			OFcoefficients.close();
		}
};
int main() {
	NeuralNetwork A1(6, 10, 1);
	A1.Create_Coefficients();
	A1.Learn_2(1, 0.1);
}