////////////////////////////////////////////////////////////////////////////////
//
// FILE:        main.cpp
// DESCRIPTION: trains a neural network to output its input mod (output layer size)
// AUTHOR:      Dan Fabian
// DATE:        10/20/2019

#include "config.h"
#include "network.h"

int main()
{
	// set up network
	Network net(LAYERS_SIZES, STEP_CONSTANT, LAMBDA);

	// set up random generator
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, INPUTS - 1);
	auto rand = std::bind(distribution, generator);

	// set up training data
	valarray<ValD> Xtrain(ValD(0.0, INPUTS), TRAIN_DATA_SIZE);
	ValD Ytrain(TRAIN_DATA_SIZE);
	for (size_t i = 0; i != Xtrain.size(); ++i)
	{
		int ans = rand();
		Xtrain[i][ans] = 1;
		Ytrain[i] = ans % OUTPUTS;
	}

	// set up test data
	valarray<ValD> Xtest(ValD(0.0, INPUTS), TEST_DATA_SIZE);
	ValD Ytest(0.0, TEST_DATA_SIZE);
	for (size_t i = 0; i != Xtest.size(); ++i)
	{
		int ans = rand();
		Xtest[i][ans] = 1;
		Ytest[i] = ans % OUTPUTS;
	}

	// test and output results before training
	cout << "Percent of success BEFORE training: " 
		<< net.test(Xtest, Ytest, TEST_DATA_SIZE) * 100.0 << '%' << endl << endl;

	// output network before training
	cout << "Initial weights and biases: " << endl;
	net.print();

	// train network
	net.train(Xtrain, Ytrain, TRAINING_EPOCHS);

	// test and output results after training
	cout << "Percent of success AFTER training: " 
		<< net.test(Xtest, Ytest, TEST_DATA_SIZE) * 100.0 << '%' << endl << endl;

	// output network
	cout << "New weights and biases after training: " << endl;
	net.print();
}
