#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <time.h>
class NeuralNeutworkData {
public:
	std::vector<int> amountNeurons;
	void (*activator)(double*, int);
};
class NeuralNetwork {
	class baseLayer {
	// owner only the neurons
	public:
		baseLayer(double* _prevNeurons, int _amountNeurons, int _prevAmountNeurons, double** _coefficients);
		baseLayer() {}
		void setNewBaseLayer(double* _prevNeurons, int _amountNeurons, int _prevAmountNeurons, double** _coefficients);

		~baseLayer();

		void calculate(void (* activator)(double*, int));
		double* getNeurons();
		void setNeurons(double* newNeurons);
		void setPrevNeurons(double* newPrevNeurons);
	private:
		double* prevNeurons;
		int amountNeurons;
		int prevAmountNeurons;
		// ( prevAmountNeurons + 1 (displacement neuron) ) * ( amountNeurons )
		double** coefficients;
		double* neurons;
	};
public:
	NeuralNetwork(std::string _fileName, std::vector<std::vector<double>>& _input, std::vector<std::vector<double>>& _result, NeuralNeutworkData& _data);
	~NeuralNetwork();
	void learn(int mode, int amount, double learnCoef);

	static void sigmaAll(double* input, int amount);
	static void simpleActivateAll(double* input, int amount);
	static void simpleActivateAll1(double* input, int amount);
	std::vector<std::vector<double>> getResultAll();
private:
	void calculateAll();
	void randomLearn(int amount, double learnCoef);
	void backpropagationLearn(int amount, double learnCoef);
	void getResult();

	baseLayer* neuralNetwork;
	std::string fileName;
	double*** coefficients;
	double** normaledInput;
	std::vector<std::vector<double>>& input;
	std::vector<std::vector<double>>& result;
	NeuralNeutworkData& data;
};