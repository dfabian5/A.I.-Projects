#ifndef NETWORK_H
#define NETWORK_H

////////////////////////////////////////////////////////////////////////////////
//
// FILE:        network.h
// DESCRIPTION: contains Network class and implementation, uses valarrays
// AUTHOR:      Dan Fabian
// DATE:        10/20/2019

#include "network_utility.h"
#include <algorithm>
#include <functional>
#include <iostream>

using std::cout; using std::endl;

////////////////////////////////////////////////////////////////////////////////
//
// NETWORK
class Network {
public:
	// constructor
	Network(vector<size_t> layerSizes, double stepConst, double lambda);

	// methods
	void   train     (const valarray<ValD>& Xdata, const ValD& Ydata, const size_t& epochs); // trains the whole network
	double test      (const valarray<ValD>& Xdata, const ValD& Ydata, const size_t& epochs); // tests the network and returns a decimal of correct answers / total
	void   dropout   (size_t layer, size_t toDrop);                                          // randomly chooses toDrop amount of neurons to dropout in layer
	void   setLambda (double lambda) { lambda_ = lambda; }
	void   setStep   (double step)   { stepConstant_ = step; }
	void   print     () const;

private:
	// helper functions
	void backPropagation    (const ValD& alpha, const ValD& Yvalue); // uses backprop to adjust weights and biases
	ValD forwardPropagation (const ValD& inputs);                    // returns a valarray of output layer activations

	vector<Layer> layers_;
	vector<ValD>  z_; // need to store z values after each forward prop to be used in back prop alg
	double        stepConstant_;
	double        lambda_;
	size_t        trainingSetSize_;
};

////////////////////////////////////////////////////////////////////////////////
//
// NETWORK functions
////////////////////////////////////////
// constructor
Network::Network(vector<size_t> layerSizes, double stepConst, double lambda) :
	layers_(vector<Layer>(layerSizes.size())),
	z_(vector<ValD>(layerSizes.size())),
	stepConstant_(stepConst),
	lambda_(lambda),
	trainingSetSize_(0)
{
	// init layers, start from 1 since 0 is the input layer which doesn't have weights or biases
	layers_[0].size_ = layerSizes[0];
	for (size_t i = 1; i != layers_.size(); ++i)
		layers_[i] = Layer(layerSizes[i - 1], layerSizes[i]);
}

////////////////////////////////////////
// forward propagation, returns a valarray of output layer activations
ValD Network::forwardPropagation(const ValD& inputs)
{
	// alpha is the activation from the previous neuron
	// input layer
	ValD alpha = inputs;

	// also store inputs in z_[0] for use in backprop function
	z_[0] = inputs;

	// begin progatating forward, layer 0 is the input layer so start at layer 1
	for (size_t l = 1; l != layers_.size(); ++l)
	{
		ValD z(layers_[l].size_);
		for (size_t j = 0; j != layers_[l].size_; ++j) // finding z for the j-th neuron in the l-th layer
			z[j] = (layers_[l].weights_[j] * alpha).sum() + layers_[l].biases_[j];

		// save z values
		z_[l] = z;

		// get activations
		alpha = sigmoid(z);
	}

	return alpha;
}

////////////////////////////////////////
// back propagation algorithm to adjust weights and biases in each layer
void Network::backPropagation(const ValD& alpha, const ValD& Yvalue)
{
	// init vector of deltas for each layer
	const size_t L = layers_.size() - 1; // final layer
	valarray<ValD> delta(L + 1);

	// begin with delta in the output layer
	delta[L] = Yvalue * (1.0 - alpha) - alpha * (1.0 - Yvalue);

	// now propagate backward to find deltas
	for (size_t l = L - 1; l > 0; --l)
	{
		ValD deltaSum(0.0, layers_[l].size_);
		for (size_t k = 0; k != layers_[l + 1].size_; ++k)
			deltaSum += layers_[l + 1].weights_[k] * delta[l + 1][k] * sigmoidPrime(z_[l]);

		delta[l] = deltaSum;
	}

	// adjust weights and biases
	for (size_t l = 1; l != layers_.size(); ++l)
	{
		layers_[l].biases_ += stepConstant_ * delta[l];
		ValD activation = sigmoid(z_[l - 1]);

		for (size_t j = 0; j != layers_[l].size_; ++j)
		{
			double deltaAndRatio = stepConstant_ * delta[l][j];
			double regularization = lambda_ / trainingSetSize_;

			if (l - 1 != 0)
				layers_[l].weights_[j] += deltaAndRatio * activation + regularization * layers_[l].weights_[j];
			else
				layers_[l].weights_[j] += deltaAndRatio * z_[l - 1] + regularization * layers_[l].weights_[j]; // doesn't need sigmoid since its the raw input
		}
	}
}

////////////////////////////////////////
// trains the whole network
void Network::train(const valarray<ValD>& Xdata, const ValD& Ydata, const size_t& epochs)
{
	// set up random generator
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, Xdata.size() - 1);
	auto rand = std::bind(distribution, generator);

	trainingSetSize_ = Xdata.size();
	for (size_t ep = 0; ep < epochs; ++ep)
	{
		size_t index = rand(); // select a random piece of data to train with
		ValD alpha = forwardPropagation(Xdata[index]);
		ValD ans(0.0, alpha.size());
		ans[Ydata[index]] = 1.0; // set correct answer

		backPropagation(alpha, ans); // adjust weights
	}
}

////////////////////////////////////////
// tests the network and returns a decimal of correct answers / total
double Network::test(const valarray<ValD>& Xdata, const ValD& Ydata, const size_t& epochs)
{
	size_t success = 0;
	for (size_t i = 0; i != epochs; ++i)
	{
		ValD alpha = forwardPropagation(Xdata[i]);

		// find argmax
		size_t argmax = 0;
		for (size_t j = 1; j != alpha.size(); ++j)
			if (alpha[argmax] < alpha[j])
				argmax = j;

		if (argmax == Ydata[i])
			++success;
	}

	return double(success) / double(epochs);
}

////////////////////////////////////////
// randomly chooses toDrop amount of neurons to dropout in layer
void Network::dropout(size_t layer, size_t toDrop)
{
	if (layer == 0 || layer >= layers_.size() - 1) // can't dropout in the input layer or output layer
		return;

	// select neurons to keep
	vector<int> neuronsToDrop(toDrop, -1); // list of neuron indicies to drop
	size_t i = 0;
	while (i < neuronsToDrop.size())
	{
		int possibleIndex = rand() % layers_[layer].size_; // select random index
		if (std::find(neuronsToDrop.begin(), neuronsToDrop.end(), possibleIndex) == neuronsToDrop.end()) // make sure the index isn't repeated
		{
			neuronsToDrop[i] = possibleIndex;
			++i;
		}
	}

	// changing layer
	valarray<ValD> newWeights(ValD(layers_[layer - 1].size_), layers_[layer].size_ - toDrop);
	ValD newBiases(layers_[layer].size_ - toDrop);
	for (size_t j = 0, k = 0; j < layers_[layer].size_; ++j)
		if (std::find(neuronsToDrop.begin(), neuronsToDrop.end(), j) == neuronsToDrop.end()) // if index j isnt to be dropped
		{
			newWeights[k] = layers_[layer].weights_[j];
			newBiases[k] = layers_[layer].biases_[j];
			++k;
		}

	layers_[layer].weights_ = newWeights;
	layers_[layer].biases_ = newBiases;
	layers_[layer].size_ = layers_[layer].size_ - toDrop;

	// changing next layer
	newWeights = valarray<ValD>(ValD(layers_[layer].size_), layers_[layer + 1].size_);
	for (size_t j = 0; j < newWeights.size(); ++j)
		for (size_t k = 0, p = 0; k < newWeights[j].size() + toDrop; ++k)
			if (std::find(neuronsToDrop.begin(), neuronsToDrop.end(), k) == neuronsToDrop.end()) // if index k isnt to be dropped
			{
				newWeights[j][p] = layers_[layer + 1].weights_[j][k];
				++p;
			}

	layers_[layer + 1].weights_ = newWeights;
}

////////////////////////////////////////
// prints out all parts of the network
void Network::print() const
{
	// output layer sizes first seperated by a space
	cout << "LAYER SIZES: " << endl;
	for (size_t i = 0; i < layers_.size(); ++i)
		cout << layers_[i].size_ << ' ';
	cout << endl << endl;

	// now output weights
	cout << "WEIGHTS: " << endl;
	for (size_t i = 1; i < layers_.size(); ++i)
		for (size_t j = 0; j < layers_[i].size_; ++j)
		{
			for (size_t k = 0; k < layers_[i].weights_[j].size(); ++k)
				cout << layers_[i].weights_[j][k] << ' ';
			cout << endl;
		}
	cout << endl;

	// output biases
	cout << "BIASES: " << endl;
	for (size_t i = 1; i < layers_.size(); ++i)
	{
		for (size_t j = 0; j < layers_[i].size_; ++j)
			cout << layers_[i].biases_[j] << ' ';
		cout << endl;
	}
	cout << endl;
}

#endif // NETWORK_H

