#ifndef CONFIG_H
#define CONFIG_H

////////////////////////////////////////////////////////////////////////////////
//
// FILE:        config.h
// DESCRIPTION: contains parameters for neural network solution
// AUTHOR:      Dan Fabian
// DATE:        10/20/2019

#include <vector>

using std::vector;

////////////////////////////////////////////////////////////////////////////////
//
// TEST DATA PARAMETERS

const size_t INPUTS = 4;
const size_t OUTPUTS = 3;
const size_t TRAIN_DATA_SIZE = 100;
const size_t TEST_DATA_SIZE = 100;

////////////////////////////////////////////////////////////////////////////////
//
// NETWORK PARAMETERS

const vector<size_t> LAYERS_SIZES = { INPUTS, OUTPUTS };
const double STEP_CONSTANT = .12;
const double LAMBDA = 1;
const size_t TRAINING_EPOCHS = 20;

#endif CONFIG_H