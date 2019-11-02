#ifndef FINDER_H
#define FINDER_H

////////////////////////////////////////////////////////////////////////////////
//
// FILE:        cluster_finder.h
// DESCRIPTION: contains class for k means cluster algorithm
// AUTHOR:      Dan Fabian
// DATE:        10/19/2019

#include "config.h"
#include <vector>
#include <valarray>
#include <fstream>
#include <iostream>
#include <cmath>
#include <random>

using std::vector;
using std::valarray;
using std::cout; using std::endl;

typedef valarray<double> ValD;

////////////////////////////////////////////////////////////////////////////////
//
// CLUSTER
class Cluster {
public:
	Cluster() : 
		clusters_(vector<vector<ValD>>(K)),
		means_(vector<ValD>(K, ValD(DIMENSIONS))) {}

	// methods
	void loadData      (string file = OUTPUT_FILE);
	void findClusters  ();
	void printMeans    () const;
	void printClusters () const;

private:
	// helper functions
	void   initMeans ();
	double distance  (const ValD& p1, const ValD& p2) const;
	ValD   center    (int num)                        const;

	vector<ValD>         data_;
	vector<vector<ValD>> clusters_; // vector of sets of points
	vector<ValD>         means_;
};

////////////////////////////////////////////////////////////////////////////////
//
// CLUSTER functions
////////////////////////////////////////
// loads all data from text file
void Cluster::loadData(string file)
{
	std::ifstream in(file);
	string num;
	while (!in.eof())
	{
		ValD point(DIMENSIONS);
		for (int i = 0; i < DIMENSIONS; ++i)
		{
			in >> num;
			point[i] = stod(num);
		}
		
		data_.push_back(point);
	}
}

////////////////////////////////////////
// finds clusters
void Cluster::findClusters()
{
	initMeans();

	// keep adjusting clusters until all means dont change by a set amount
	int maxShift;
	do {
		// reset all cluster sets
		clusters_ = vector<vector<ValD>>(K);

		// for all points, find closest mean/center
		for (int i = 0; i < data_.size(); ++i)
		{
			int clusterNum = 0;
			double minDist = distance(means_[0], data_[i]);
			for (int j = 1; j < K; ++j)
			{
				double dist = distance(means_[j], data_[i]);
				if (minDist > dist)
				{
					clusterNum = j; 
					minDist = dist;
				}
			}

			// insert point into that cluster
			clusters_[clusterNum].push_back(data_[i]);
		}

		// store prev means to compare
		vector<ValD> prevMeans = means_;

		// calc new means
		for (int i = 0; i < K; ++i)
			means_[i] = center(i);

		// now find the max shift in means from prev
		maxShift = distance(prevMeans[0], means_[0]);
		for (int i = 1; i < K; ++i)
		{
			double dist = distance(prevMeans[i], means_[i]);
			if (maxShift < dist)
				maxShift = dist;
		}

	} while (MAX_MEAN_SHIFT < maxShift);
}

////////////////////////////////////////
// prints only means
void Cluster::printMeans() const
{
	cout << "MEANS: " << endl << endl;

	for (int i = 0; i < K; ++i)
	{
		for (int j = 0; j < DIMENSIONS; ++j)
			cout << means_[i][j] << ' ';
		cout << endl;
	}
}

////////////////////////////////////////
// prints all data in clusters and mean of cluster
void Cluster::printClusters() const
{
	for (int i = 0; i < K; ++i)
	{
		cout << endl << endl << "CLUSTER " << i << endl
			<< "Mean: "; 
		for (int j = 0; j < DIMENSIONS; ++j)
			cout << means_[i][j] << ' ';
		cout << endl << endl;

		cout << "Points:" << endl;
		for (int a = 0; a < clusters_[i].size(); ++a)
		{
			for (int j = 0; j < DIMENSIONS; ++j)
				cout << clusters_[i][a][j] << ' ';
			cout << endl;
		}
	}
}

////////////////////////////////////////
// init cluster means
void Cluster::initMeans()
{
	// find min and max of all dims of data
	double min[DIMENSIONS], max[DIMENSIONS];
	for (int j = 0; j < DIMENSIONS; ++j)
		min[j] = max[j] = data_[0][j];

	for (int i = 1; i < data_.size(); ++i)
		for (int j = 0; j < DIMENSIONS; ++j)
		{
			if (data_[i][j] < min[j]) min[j] = data_[i][j];
			else if (data_[i][j] > max[j]) max[j] = data_[i][j];
		}

	// after min and max of all dims found, randomly select a point for all k
	std::default_random_engine generator;
	for (int j = 0; j < DIMENSIONS; ++j)
	{
		std::uniform_real_distribution<double> distribution(min[j], max[j]);

		for (int i = 0; i < K; ++i)
			means_[i][j] = distribution(generator);
	}

	cout << "INITIAL ";
	printMeans();
}

////////////////////////////////////////
// calculates euclidean distance between two points
double Cluster::distance(const ValD& p1, const ValD& p2) const
{
	// eq for euclidean distance: sqrt((x_1 - y_1)^2 + (x_2 - y_2)^2...)
	return sqrt(pow(p1 - p2, 2.0).sum());
}

////////////////////////////////////////
// calculates center of cluster #
ValD Cluster::center(int num) const
{
	ValD tmp(0.0, DIMENSIONS);
	double size = clusters_[num].size();
	for (int i = 0; i < size; ++i)
		tmp = tmp + clusters_[num][i];

	return tmp / size;
}


#endif FINDER_H
