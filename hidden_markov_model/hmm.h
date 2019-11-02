#ifndef HMM_H
#define HMM_H

////////////////////////////////////////////////////////////////////////////////
//
// FILE:        hmm.h
// DESCRIPTION: contains HMM class
// AUTHOR:      Dan Fabian
// DATE:        10/19/2019

#include "config.h"
#include <random>
#include <functional>
#include <iostream>

using std::cout; using std::endl;

////////////////////////////////////////////////////////////////////////////////
//
// HMM
struct HMM {
	HMM();

	// method
	void print() const;

	// initialProbs_[i] = prob of starting in state i
	double initialProbs_[NUM_OF_STATES];

	// transitions[i][j] = prob of transitioning from state i to state j
	double transitions_[NUM_OF_STATES][NUM_OF_STATES];  

	// emissions_[i][j] = prob of observing j from state i
	double emissions_[NUM_OF_STATES][NUM_OF_EMISSIONS];

};

////////////////////////////////////////////////////////////////////////////////
//
// HMM functions
////////////////////////////////////////
// inits HMM with random transition, emission, and inital probabilites
HMM::HMM()
{
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(MIN_RAND, MAX_RAND);
	auto rand = std::bind(distribution, generator);

	// init initialProbs
	double total = 0;
	for (int i = 0; i < NUM_OF_STATES; ++i) total += initialProbs_[i] = rand();
	for (int i = 0; i < NUM_OF_STATES; ++i) initialProbs_[i] /= total;

	// init transitions
	for (int i = 0; i < NUM_OF_STATES; ++i)
	{
		total = 0;
		for (int j = 0; j < NUM_OF_STATES; ++j) total += transitions_[i][j] = rand();
		for (int j = 0; j < NUM_OF_STATES; ++j) transitions_[i][j] /= total;
	}

	// init emissions
	for (int i = 0; i < NUM_OF_STATES; ++i)
	{
		total = 0;
		for (int j = 0; j < NUM_OF_EMISSIONS; ++j) total += emissions_[i][j] = rand();
		for (int j = 0; j < NUM_OF_EMISSIONS; ++j) emissions_[i][j] /= total;
	}
}

////////////////////////////////////////
// print HMM
void HMM::print() const
{
	cout << "INITIAL PROBABILITES:" << endl;
	for (int i = 0; i < NUM_OF_STATES; ++i)
		cout << "state " << i << ": " << initialProbs_[i] << endl;
	cout << endl;

	cout << "TRANSITION PROBABILITIES:" << endl;
	for (int i = 0; i < NUM_OF_STATES; ++i)
	{
		for (int j = 0; j < NUM_OF_STATES; ++j)
			cout << transitions_[i][j] << ' ';
		cout << endl;
	}
	cout << endl;

	cout << "EMISSION PROBABILITIES:" << endl;
	for (int i = 0; i < NUM_OF_STATES; ++i)
	{
		for (int j = 0; j < NUM_OF_EMISSIONS; ++j)
			cout << emissions_[i][j] << ' ';
		cout << endl;
	}
	cout << endl;
}

#endif HMM_H