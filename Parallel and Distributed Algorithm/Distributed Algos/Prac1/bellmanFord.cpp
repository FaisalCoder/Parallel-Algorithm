#include<iostream>
#include<new>
#include<omp.h>
#include<cmath>
#include <sys/time.h>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
using namespace std;
#define INF 99999
#define SIZE 10

void bellmanFord(int [][SIZE], int, int [][SIZE], int,int[],int[],int []);

int main()
{
	int sizeOfNetwork = SIZE;
	int networkMatrix[SIZE][SIZE];
	int distanceExchange[SIZE][SIZE];
	int shortestPathTree [SIZE] = {-1};
	int shortestDistance [SIZE] = {INF};        //setting the initial distance
	int parentArray [SIZE] = {-1};    //-1 means undefined

	//Randomly create a strongly connected graph 
	int counterForGraph = 0;
	for( int i=0; i<sizeOfNetwork; i++)
	{
		for(int j=0; j<sizeOfNetwork; j++)
		{
			networkMatrix[i][j] = (i+j)%(sizeOfNetwork/2);        //assigning weight to edges
			distanceExchange[i][j] = -1;        //-1 means empty initially

			if( i == j)
			{
				networkMatrix[i][j] = 0;
			}

			if (networkMatrix[i][j] > 0)
            counterForGraph++;

		}
	}

	//selecting the root node
	int root = rand() % sizeOfNetwork;
	//int root = 0;
	shortestDistance[root] = 0;
	cout << "Root of the tree is Node # " << root << endl;

	//writing the network in a file for visualisation
	ofstream myfile;
    myfile.open ("network.txt");
    myfile << "--size " << SIZE << " " << root << endl;
    myfile << "--edges " << counterForGraph << endl;
    for(int i=0; i<SIZE; i++)
    {
    	for( int j=0; j<SIZE; j++)
    	{
    		if (networkMatrix[i][j] > 0)
    	    myfile << i << "," << j << endl;
    	}
    }

    //writing the weight of edges in parent array
    myfile << "--weight " << counterForGraph << endl;
    for(int i=0; i<SIZE; i++)
    {
    	for( int j=0; j<SIZE; j++)
    	{
    		if (networkMatrix[i][j] > 0)
    	    myfile << networkMatrix[i][j] << endl;
    	}
    }
    
    myfile.close();

	struct timeval start, end;
    gettimeofday(&start, NULL);

	bellmanFord(networkMatrix, sizeOfNetwork,distanceExchange, root,shortestPathTree, shortestDistance,parentArray);

	gettimeofday(&end, NULL);
    float delta = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
 
	//write parent and shortest distance in a file
	//ofstream myfile;
    myfile.open ("output.txt");
    myfile << "--size " << SIZE << " " << root << endl;
    myfile << "--edges " << SIZE << endl;
    for(int i=0; i<SIZE; i++)
    {
    	if (parentArray[i] != -1)
    	myfile << parentArray[i] << "," << i << endl;

    }

    //writing the weight of edges in parent array
    myfile << "--weight " << SIZE << endl;
    for(int i=0; i<SIZE; i++)
    {
    	if (networkMatrix[parentArray[i]][i] > 0)
    	myfile << networkMatrix[parentArray[i]][i] << endl;
    }
    
    myfile.close();


	//print or write the result in a file and print its graph using python
	 
	int iterator = root;
	int count = 0;
	cout << "\n---------------- Network--------------\n";
	for(int i=0; i<SIZE; i++)
    {
    	cout << "--------From Node # " << i << " -------\n";
    	for(int j=0; j<SIZE; j++)
    	{
    		if ( networkMatrix[i][j] > 0)
    		{
    			cout << "To Node # " << j << " : " << networkMatrix[i][j] << endl;
    		}
    	}

    }

	cout << "\n---------------- Shortest Path Tree Edges--------------\n";
	for(int i=0; i<SIZE; i++)
    {
    	cout << parentArray[i] << " --> " << i << endl;

    }

    cout << "\n---------------- Shortest Path Nodes--------------\n";
	for(int i=0; i<SIZE; i++)
    {
    	cout << "Node # " << i << " : " << shortestDistance[i] << endl;

    }

    cout << "Root : " << root << endl;
    cout << "TOTAL TIME TAKEN : " << delta << " sec" << endl;
	 
	
	return 0;
}

void bellmanFord(int networkMat[][SIZE], int n, int distMat[][SIZE], int rootNode,int spTree[],int spDist[], int parentArr[])
{
	#pragma omp parallel shared(networkMat,n,distMat,rootNode,spTree,spDist,parentArr) num_threads(n)
	{
		int i = omp_get_thread_num();
		int dist = INF;
		if ( i == rootNode)
		{
			dist = 0;
		}
		int min = dist;

		for( int k=0; k< (n-1); k++)          //total rounds of the algo
        {      	
        	
        	//Step 1 : Send Distance to neighbours
        	for( int p=0; p < SIZE; p++)
        	{
        		if ( networkMat[i][p] > 0 && p != i)     //if their is a edge
        		{
        			distMat[p][i] = dist;
        			//cout << "node #" << i << " dist sento  : " << p << " : " << dist << endl; 
        		}
        	}     

        	//add a sleep to mimic the communication delay  
        	sleep(1);	

			#pragma omp barrier
	       
	        //Step 2 : Relaxation 
	        for( int p=0; p < SIZE; p++)
        	{
        		if ( distMat[i][p] >= 0 && p != i && distMat[i][p] < INF)     //if their is a weight
        		{
        			//cout << "node #" << i << " dist recvfrom : " << p << " : " << distMat[i][p] << endl; 
        			int tmp = distMat[i][p] + networkMat[i][p];
        			if( min > tmp )
        			{
        				min = tmp;
        				parentArr[i] = p;
        				//cout << "node #" << i << " parent : " << p << endl;
        			}
        		}

        	} 	  

        	dist = min;      

			#pragma omp barrier
        }

        spDist[i] = min;
	}

	//create the shortest path tree
}
