#include<iostream>
#include<new>
#include<omp.h>
#include<cmath>
#include<algorithm>
#include <sys/time.h>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
using namespace std;
#define INF 99999
#define SIZE 10

void dijkstra(int [][SIZE], int, int [][SIZE], int,int[],int[],int []);

int main()
{
	int sizeOfNetwork = SIZE;
	int networkMatrix[SIZE][SIZE];
    int shortestDistance [SIZE] = {INF};        //setting the initial distance
    for(int i=0; i<SIZE; i++)
    {
        shortestDistance[i] = INF;
    }
    //cout << " spdist : " << i << " : " << shortestDistance[i] << endl;

	int distanceExchange[SIZE][SIZE];
	int shortestPathTree [SIZE] = {-1};
	
	int parentArray [SIZE] = {-1};    //-1 means undefined

	//Randomly create a strongly connected graph 
    int counterForGraph = 0;
	for( int i=0; i<sizeOfNetwork; i++)
	{
		for(int j=0; j<sizeOfNetwork; j++)
		{
			networkMatrix[i][j] = (i+j)%(sizeOfNetwork/2);        //assigning weight to edges
			//distanceExchange[i][j] = -1;        //-1 means empty initially
           
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

	dijkstra(networkMatrix, sizeOfNetwork,distanceExchange, root,shortestPathTree, shortestDistance,parentArray);

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
    	if (parentArray[i] != -1)
    	myfile << networkMatrix[parentArray[i]][i] << endl;
        //cout << "Writing code : " << networkMatrix[parentArray[i]][i] << endl;
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

void dijkstra(int networkMat[][SIZE], int n, int distMat[][SIZE], int rootNode,int spTree[],int spDist[], int parentArr[])
{
    int token = rootNode;
    bool traversed[SIZE];
    for( int i=0; i<SIZE; i++)
    {
        traversed[i] = false;
    }

	#pragma omp parallel shared(networkMat,n,distMat,rootNode,spTree,spDist,parentArr,traversed,token) num_threads(n)
	{
		int i = omp_get_thread_num();

        for(int k=0; k<SIZE-1; k++)         //total rounds
        {
            if( token == i)    //if current node has token
            {
                //cout << "-------------------------------\n";
                //cout << "Token : " << token << " at node id : " << i << endl;
                traversed[i] = true;
                for( int j=0; j<SIZE; j++)
                {
                    if(networkMat[i][j] > 0 && i != j && traversed[j] == false)      //all the neighbours
                    {
                        //Relaxation
                        //cout << "SPdist Neighbour of " << i << " : " << j << " is : " << spDist[j] << endl;
                        //cout << "Dist 1 : " << (spDist[i]+networkMat[i][j]) << " Dist 2: " << spDist[j] << endl;
                        spDist[j] = min(spDist[i]+networkMat[i][j],spDist[j]);
                        //cout << "Min : Shrtestdist Neighbour of " << i << " : " << j << " is : " << spDist[j] << endl;
                    }
                }    

                //take shortest distance neighbour           
                int temp = INF;
                
                for( int w=0; w<SIZE; w++)
                {
                    if ( temp > spDist[w] && w != i && traversed[w]== false)
                    {
                        temp = spDist[w];
                        token = w;
                    }
                }

                int min_nbDist = INF;
                int closest_nb = -1;
                for( int nb=0; nb<SIZE; nb++)
                {
                    if(traversed[nb]== true && networkMat[token][nb] > 0)
                    {
                        if ( min_nbDist > spDist[nb])
                        {
                            min_nbDist = spDist[nb];
                            closest_nb = nb;
                        }
                    }
                }

                parentArr[token] = closest_nb;
                sleep(1);

            }

            else
            {
                //pass
            }

            #pragma omp barrier
        }      
    }
}
