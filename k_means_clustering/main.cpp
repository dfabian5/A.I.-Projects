////////////////////////////////////////////////////////////////////////////////
//
// FILE:        main.cpp
// DESCRIPTION: runs k means clustering on a randomly generated set of data
// AUTHOR:      Dan Fabian
// DATE:        10/19/2019

#include "cluster_generator.h"
#include "cluster_finder.h"
#include "config.h"

int main()
{
	// create test data file
	std::ofstream out(OUTPUT_FILE);
	
	// create clusters
	for (int i = 0; i < TEST_CLUSTERS; ++i)
		testCluster(out, MIN[i], MAX[i]);

	// find clusters
	Cluster cluster;
	cluster.loadData();
	cluster.findClusters();

	// print
	cout << endl;
	//cluster.printClusters();
	cluster.printMeans();
}
