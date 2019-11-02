#ifndef CONFIG_H
#define CONFIG_H

////////////////////////////////////////////////////////////////////////////////
//
// FILE:        config.h
// DESCRIPTION: contains parameters for Hidden Markov Model solution
// AUTHOR:      Dan Fabian
// DATE:        10/19/2019

////////////////////////////////////////////////////////////////////////////////
//
// HMM parameters

const int NUM_OF_STATES = 3;
const int NUM_OF_EMISSIONS = 5;
const double MIN_RAND = 0;
const double MAX_RAND = 10;

////////////////////////////////////////////////////////////////////////////////
//
// TESTING parameters

const int NUM_OF_OBSERVATIONS = 5;
const int OBSERVATIONS[NUM_OF_OBSERVATIONS] = { 1, 1, 1, 1, 1 };

#endif CONFIG_H