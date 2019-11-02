////////////////////////////////////////////////////////////////////////////////
//
// FILE:        main.cpp
// DESCRIPTION: performs linear regression analysis on randomly generated data
// AUTHOR:      Dan Fabian
// DATE:        11/2/2019

#include "config.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <valarray>
#include <functional>

using std::cout; using std::endl;
using std::valarray;

typedef valarray<double> ValD;

void generateData(ValD& x, ValD& y);

int main()
{
	ValD x(AMOUNT), y(AMOUNT);
	generateData(x, y);

	// print out all data
	cout << "DATA:" << endl;
	for (int i = 0; i < AMOUNT; ++i)
		cout << "x: " << x[i] << "\ty: " << y[i] << endl;
	cout << endl;
	
	// linear regression y = a * x + b
	double xSum = x.sum(), ySum = y.sum(), xSquareSum = pow(x, 2.0).sum(), 
		xySum = (x * y).sum(), denom = AMOUNT * xSquareSum - pow(xSum, 2.0);

	double a = (AMOUNT * xySum - xSum * ySum) / denom;
	double b = (ySum * xSquareSum - xSum * xySum) / denom;

	cout << "Approximate Linear Equation: y = " << a << " * x + " << b << endl;
}

////////////////////////////////////////
// creates the random data for analysis
void generateData(ValD& x, ValD& y)
{
	// create generator
	std::default_random_engine generator;

	// generate random x vals
	std::uniform_real_distribution<double> xDist(X_RAND_MIN, X_RAND_MAX);
	auto xRand = std::bind(xDist, generator);
	std::generate(begin(x), end(x), xRand);

	// generate random errors
	ValD error(AMOUNT);
	std::normal_distribution<double> errorDist(ERROR_RAND_MEAN, ERROR_STDDEV);
	auto errorRand = std::bind(errorDist, generator);
	std::generate(begin(error), end(error), errorRand);

	// generate y vals
	y = SLOPE * x + Y_INTERCEPT + error;
}