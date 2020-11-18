#include "NeuralNetwork.h"
#include <time.h>

NeuralNetwork::baseLayer::baseLayer(double* _prevNeurons, int _amountNeurons, int _prevAmountNeurons, double** _coefficients):
	prevNeurons(_prevNeurons), amountNeurons(_amountNeurons), prevAmountNeurons(_prevAmountNeurons), coefficients(_coefficients) {
	neurons = new double[amountNeurons];
}

void NeuralNetwork::baseLayer::setNewBaseLayer(double* _prevNeurons, int _amountNeurons, int _prevAmountNeurons, double** _coefficients) { // use only with new base layer without neurons
	prevNeurons = _prevNeurons;
	amountNeurons = _amountNeurons;
	prevAmountNeurons = _prevAmountNeurons;
	coefficients = _coefficients;
	neurons = new double[amountNeurons];
}
NeuralNetwork::baseLayer::~baseLayer() {
	delete[] neurons;
}

void NeuralNetwork::baseLayer::calculate(void (*activator)(double*, int)) {
	if (prevNeurons == nullptr) {
		return;
	}
	for (int i = 0; i < amountNeurons; ++i) {
		neurons[i] = 0;
	}
	for (int i = 0; i < prevAmountNeurons; ++i) {
		for (int j = 0; j < amountNeurons; ++j) {
			neurons[j] += prevNeurons[i] * coefficients[i][j];
		}
	}
	for (int j = 0; j < amountNeurons; ++j) {
		// last index for displacement neuron
		neurons[j] += coefficients[prevAmountNeurons][j];
	}
	activator(neurons, amountNeurons);
}

double* NeuralNetwork::baseLayer::getNeurons() {
	return neurons;
}

void NeuralNetwork::baseLayer::setNeurons(double* newNeurons) {
	neurons = newNeurons;
}

void NeuralNetwork::baseLayer::setPrevNeurons(double* newPrevNeurons) {
	prevNeurons = newPrevNeurons;
}

NeuralNetwork::NeuralNetwork(std::string _fileName, std::vector<std::vector<double>>& _input, std::vector<std::vector<double>>& _result, NeuralNeutworkData& _data) :
	fileName(_fileName), input(_input), result(_result), data(_data) {

	// coef = (levels - 1)(prev neurons + 1)(neurons) = (data.amountNeurons.size() - 1) (data.amountNeurons[i] + 1) (data.amountNeurons[i + 1])

	coefficients = new double** [data.amountNeurons.size() - 1];
	for (int i = 0; i < data.amountNeurons.size() - 1; ++i) {
		coefficients[i] = new double* [data.amountNeurons[i] + 1];
		for (int j = 0; j < data.amountNeurons[i] + 1; ++j) {
			coefficients[i][j] = new double[data.amountNeurons[i + 1]];
		}
	}

	std::fstream file(fileName);
	if (!file.fail()) {
		for (int i = 0; i < data.amountNeurons.size() - 1; ++i) {
			for (int j = 0; j < data.amountNeurons[i] + 1; ++j) {
				for (int k = 0; k < data.amountNeurons[i + 1]; ++k) {
					file >> coefficients[i][j][k];
				}
			}
		}
	}
	else {
		for (int i = 0; i < data.amountNeurons.size() - 1; ++i) {
			for (int j = 0; j < data.amountNeurons[i] + 1; ++j) {
				for (int k = 0; k < data.amountNeurons[i + 1]; ++k) {
					coefficients[i][j][k] = (rand() % 50 - 25) / 10.0;
				}
			}
		}
	}
	file.close();

	normaledInput = new double* [input.size()];
	for (int i = 0; i < input.size(); ++i) {
		normaledInput[i] = new double[input[i].size()];
		for (int j = 0; j < input[i].size(); ++j) {
			normaledInput[i][j] = input[i][j];
		}
		data.activator(normaledInput[i], input[i].size());
	}

	neuralNetwork = new baseLayer[data.amountNeurons.size() - 1];
	neuralNetwork[0].setNewBaseLayer(&normaledInput[0][0], data.amountNeurons[1], data.amountNeurons[0], coefficients[0]);
	for (int i = 1; i < data.amountNeurons.size() - 1; ++i) {
		neuralNetwork[i].setNewBaseLayer(neuralNetwork[i - 1].getNeurons(), data.amountNeurons[i + 1], data.amountNeurons[i], coefficients[i]);
	}
}

NeuralNetwork::~NeuralNetwork() {
	delete[] neuralNetwork;

	std::fstream file(fileName, std::ios::out);
	for (int i = 0; i < data.amountNeurons.size() - 1; ++i) {
		delete[] normaledInput[i];
		for (int j = 0; j < data.amountNeurons[i] + 1; ++j) {
			for (int k = 0; k < data.amountNeurons[i + 1]; ++k) {
				file << coefficients[i][j][k] << " ";
			}
			delete[] coefficients[i][j];
			file << "\n";
		}
		delete[] coefficients[i];
	}
	delete[] normaledInput;
	delete[] coefficients;
}

void NeuralNetwork::learn(int mode, int amount, double learnCoef) {
	switch (mode) {
	case 0:
		randomLearn(amount, learnCoef);
		break;
	case 1:
		backpropagationLearn(amount, learnCoef);
	}
}

void NeuralNetwork::calculateAll() {
	for (int i = 0; i < data.amountNeurons.size() - 1; ++i) {
		neuralNetwork[i].calculate(data.activator);
	}
}

void NeuralNetwork::sigmaAll(double* input, int amount) {
	for (int i = 0; i < amount; ++i) {
		input[i] = 1.0 / (1.0 + exp(-input[i]));
	}
}

void NeuralNetwork::simpleActivateAll(double* input, int amount) {
	for (int i = 0; i < amount; ++i) {
		if (input[i] >= 0) {
			input[i] = 1;
		}
		else {
			input[i] = 0;
		}
	}
}

void NeuralNetwork::simpleActivateAll1(double* input, int amount) {
	for (int i = 0; i < amount; ++i) {
		input[i] = (input[i] + 100.0) / 200.0;
	}
}

void NeuralNetwork::randomLearn(int amount, double learnCoef) {
	int numberLevel, numberNeuron, numberPrevNeuron;
	double* currentResults = new double[result[0].size()];
	double prevError = 0, error = 0, delta;

	for (int numberLearn = 0; numberLearn < amount; ++numberLearn) {

		//testing
		double errorSum = 0;
		//end testing

		for (int numberInput = 0; numberInput < input.size(); ++numberInput) {

			neuralNetwork[0].setPrevNeurons(normaledInput[numberInput]);
			calculateAll();

			currentResults = neuralNetwork[data.amountNeurons.size() - 2].getNeurons();
			prevError = 0;
			for (int i = 0; i < result[0].size(); ++i) {
				prevError += abs(result[numberInput][i] - currentResults[i]);
			}

			numberLevel = rand() % (data.amountNeurons.size() - 1);
			numberPrevNeuron = rand() % (data.amountNeurons[numberLevel] + 1);
			numberNeuron = rand() % (data.amountNeurons[numberLevel + 1]);
			if (rand() % 2) {
				delta = learnCoef;
			}
			else {
				delta = -learnCoef;
			}

			// tesing
			delta *= prevError / result[0].size();
			//end testing

			coefficients[numberLevel][numberPrevNeuron][numberNeuron] += delta;

			calculateAll();

			currentResults = neuralNetwork[data.amountNeurons.size() - 2].getNeurons();
			error = 0;
			for (int i = 0; i < result[0].size(); ++i) {
				error += abs(result[numberInput][i] - currentResults[i]);
			}
			if (error >= prevError) {
				coefficients[numberLevel][numberPrevNeuron][numberNeuron] -= delta;
			}
			//testing
			errorSum += prevError;
			//end testing 
		}
		//testing
		std::cout << errorSum << std::endl;
		//end testing
	}
	delete[] currentResults;
}

void NeuralNetwork::backpropagationLearn(int amount, double learnCoef) {

	int amountLevels = data.amountNeurons.size();
	double tempCoefSum;
	double* tempOut, *tempOut1;

	double error;

	double** errorNeuron = new double* [amountLevels - 1];
	for (int i = 0; i < amountLevels - 1; ++i) {
		errorNeuron[i] = new double[data.amountNeurons[i]];
	}

	for (int numberLearn = 0; numberLearn < amount; ++numberLearn) {
		int time = clock();
		error = 0;
		for (int numberInput = 0; numberInput < input.size(); ++numberInput) {

			neuralNetwork[0].setPrevNeurons(normaledInput[numberInput]);
			calculateAll();

			tempOut = neuralNetwork[amountLevels - 2].getNeurons();
			for (int i = 0; i < data.amountNeurons[amountLevels - 1]; ++i) {
				errorNeuron[amountLevels - 2][i] = result[numberInput][i] - tempOut[i];
				error += abs(errorNeuron[amountLevels - 2][i]);
			}

			for (int i = amountLevels - 3; i > -1; --i) {
				for (int k = 0; k < data.amountNeurons[i + 1]; ++k) {
					errorNeuron[i][k] = 0;
				}

				for (int j = 0; j < data.amountNeurons[i + 2]; ++j) {
					tempCoefSum = 0;
					for (int k = 0; k < data.amountNeurons[i + 1]; ++k) {
						tempCoefSum += abs(coefficients[i + 1][k][j]);
					}
					for (int k = 0; k < data.amountNeurons[i + 1]; ++k) {
						errorNeuron[i][k] += abs(coefficients[i + 1][k][j]) / tempCoefSum * errorNeuron[i + 1][j];
					}
				}
			}

			tempOut1 = neuralNetwork[0].getNeurons();
			for (int k = 0; k < data.amountNeurons[1]; ++k) {
				coefficients[0][data.amountNeurons[0]][k] += learnCoef * errorNeuron[0][k] * tempOut1[k] * (1 - tempOut1[k]);
				for (int j = 0; j < data.amountNeurons[0]; ++j) {
					coefficients[0][j][k] += learnCoef * errorNeuron[0][k] * tempOut1[k] * (1 - tempOut1[k]) * normaledInput[numberInput][j];
				}
			}

			for (int i = 1; i < amountLevels - 1; ++i) {
				tempOut = tempOut1;
				tempOut1 = neuralNetwork[i].getNeurons();
				for (int k = 0; k < data.amountNeurons[i + 1]; ++k) {
					coefficients[i][data.amountNeurons[i]][k] += learnCoef * errorNeuron[i][k] * tempOut1[k] * (1 - tempOut1[k]);
					for (int j = 0; j < data.amountNeurons[i]; ++j) {
						coefficients[i][j][k] += learnCoef * (errorNeuron[i][k]) * (tempOut1[k]) * (1 - tempOut1[k]) * (tempOut[j]);
					}
				}
			}
		}
		std::cout << error / input.size() << std::endl;
		std::cout << clock() - time << " " << numberLearn << std::endl;
	}

	for (int i = 0; i < amountLevels - 1; ++i) {
		delete[] errorNeuron[i];
	}
	delete[] errorNeuron;
}

std::vector<std::vector<double>> NeuralNetwork::getResultAll() {
	double* out;
	int amountLevels = data.amountNeurons.size();

	std::vector<std::vector<double>> resultOut(result.size(), std::vector<double> (result[0].size()));

	for (int numberInput = 0; numberInput < input.size(); ++numberInput) {

		neuralNetwork[0].setPrevNeurons(normaledInput[numberInput]);
		calculateAll();

		out = neuralNetwork[amountLevels - 2].getNeurons();
		for (int i = 0; i < data.amountNeurons[amountLevels - 1]; ++i) {
			resultOut[numberInput][i] = result[numberInput][i] - out[i];
		}
	}

	return resultOut;
}
