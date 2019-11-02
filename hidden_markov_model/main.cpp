////////////////////////////////////////////////////////////////////////////////
//
// FILE:        main.cpp
// DESCRIPTION: finds most probable state sequence given an HMM and observation sequence
// AUTHOR:      Dan Fabian
// DATE:        10/19/2019

#include "hmm.h"
#include "config.h"

int main()
{
	HMM model;
	model.print();

	// Viterbi algorithm
	double probs[NUM_OF_STATES][NUM_OF_OBSERVATIONS];
	double prev[NUM_OF_STATES][NUM_OF_OBSERVATIONS];
	for (int i = 0; i < NUM_OF_STATES; ++i)
	{
		probs[i][0] = model.initialProbs_[i] * model.emissions_[i][OBSERVATIONS[0]];
		prev[i][0] = 0;
	}

	for (int i = 1; i < NUM_OF_OBSERVATIONS; ++i)
		for (int j = 0; j < NUM_OF_STATES; ++j)
		{
			probs[j][i] = probs[0][i - 1] * model.transitions_[0][j] * model.emissions_[j][OBSERVATIONS[i]];
			prev[j][i] = 0;
			for (int k = 1; k < NUM_OF_STATES; ++k)
			{
				double probability = probs[k][i - 1] * model.transitions_[k][j] * model.emissions_[j][OBSERVATIONS[i]];
				if (probs[j][i] < probability)
				{
					probs[j][i] = probability;
					prev[j][i] = k;
				}
			}
		}

	int maxProb[NUM_OF_OBSERVATIONS], last = NUM_OF_OBSERVATIONS - 1;
	maxProb[last] = 0;
	double max = probs[0][last];
	for (int k = 1; k < NUM_OF_STATES; ++k)
		if (max < probs[k][last])
		{
			maxProb[last] = k;
			max = probs[k][last];
		}

	for (int i = last; i >= 1; --i)
		maxProb[i - 1] = prev[maxProb[i]][i];
	
	// print out result
	cout << "MOST LIKELY STATE SEQUENCE: ";
	for (int i = 0; i < NUM_OF_OBSERVATIONS; ++i)
		cout << maxProb[i] << ' ';
	cout << endl;
}
