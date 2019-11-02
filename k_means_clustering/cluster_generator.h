#ifndef GENERATOR_H
#define GENERATOR_H

////////////////////////////////////////////////////////////////////////////////
//
// FILE:        cluster_generator.h
// DESCRIPTION: contains functions for generating test data for k means clustering
// AUTHOR:      Dan Fabian
// DATE:        10/19/2019

#include "config.h"
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

using std::endl;
using std::vector;

void testCluster(std::ofstream& out, const double min[], const double max[])
{
	// create distributions for all dimensions
	std::default_random_engine generator;
	vector<std::uniform_real_distribution<double>> distributions;
	for (int j = 0; j < DIMENSIONS; ++j)
		distributions.push_back(std::uniform_real_distribution<double>(min[j], max[j]));

	for (int i = 0; i < AMOUNT; ++i)
	{
		for (int j = 0; j < DIMENSIONS; ++j)
			out << distributions[j](generator) << ' ';

		out << endl;
	}
}

#endif GENERATOR_H