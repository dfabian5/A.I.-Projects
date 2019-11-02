#ifndef CONFIG_H
#define CONFIG_H

////////////////////////////////////////////////////////////////////////////////
//
// FILE:        config.h
// DESCRIPTION: contains all parameters for k means clustering solution
// AUTHOR:      Dan Fabian
// DATE:        10/19/2019

#include <string>

using std::string;

////////////////////////////////////////////////////////////////////////////////
//
// TEST DATA PARAMETERS

const string OUTPUT_FILE = "testData.txt";
const int TEST_CLUSTERS = 2;
const int AMOUNT = 30; // data generated per test cluster
const int DIMENSIONS = 2;
const double MIN[TEST_CLUSTERS][DIMENSIONS] = { {0,3}, {3,3} };
const double MAX[TEST_CLUSTERS][DIMENSIONS] = { {2,5}, {5,5} };

////////////////////////////////////////////////////////////////////////////////
//
// K MEANS PARAMETERS

const int K = 2; // num of clusters to find
const double MAX_MEAN_SHIFT = .5; // keep adjusting means until they shift within this amount

#endif CONFIG_H