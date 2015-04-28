#include<iostream>
#include<new>
#include<omp.h>
#include<cmath>
#include <sys/time.h>
#include <stdlib.h>
using namespace std;

void listRank(int *,int *, int );

int main()
{
	//TREE STORED AS AN ARRAY
	int tree[100] = {0};      //taking maximum nodes as 100, 0 means NULL
	int next[100*3] = {0};   //array for simulating pointer to next node
	/*
	For a node i, 
	2*i - left child
	2*(i+1) - right child
	*/
/*		
	tree[1] = 1;
	tree[2] = 2;
	tree[3] = 3;
	tree[4] = 4;
	tree[5] = 5;
	tree[7] = 7;
	tree[8] = 8;
	tree[9] = 9;
	tree[15] = 15;
	int lastNode = 15;
	*/

	tree[1] = 1;
	tree[2] = 2;
	tree[3] = 3;
	tree[6] = 6;
	tree[7] = 7;
	tree[12] = 12;
	tree[13] = 13;
	tree[15] = 15;
	tree[27] = 27;
	int lastNode = 27;

	int totalNodes = 0;
 
    srand(time(NULL));

    struct timeval start, end;
    gettimeofday(&start, NULL);

    //creating the linked list of the processors
    
    for(int i=1; i<lastNode+1; i++)
    {
    	if(tree[i] == 0){continue;}
    	//left child
    	if(tree[2*i] != 0)
    	{
    		next[3*i] = 3*(2*i);
    	}

    	else
    	{
    		next[3*i] = 3*i + 1;
    	}

    	//right child
    	if(tree[2*i + 1] != 0)
    	{
    		next[3*i+1] = 3*(2*i+1);
    	}

    	else
    	{
    		next[3*i+1] = 3*i + 2;
    	}

    	//parent
    	int parent = int(i/2);

    	if(parent == 0)
    	{
    		next[3*i+2] = -1;
    	}

    	else if(i % 2 == 0)
    	{
    		next[3*i+2] = 3*parent + 1;
    	}

    	else if(i % 2 != 0)
    	{
    		next[3*i+2] = 3*parent + 2;
    	}
     
    	totalNodes++;
    }
   

    int i = 3 ;
    totalNodes = totalNodes*3;         //3 processor per nide
    int * linkedlist = new int[totalNodes];
    int * distance = new int[totalNodes];
    int* mapprocessor2dist = new int[totalNodes];       //mapping processor nos to array index of distance
    int count = 0;
	while( i != 5)    //until reaches the last node
	{
		//linkedlist[count] = next[i];
		linkedlist[count] = count+1;

		if( i%3 == 0)       //processor A
		{
			distance[count] = 1;
		}

		else if( (i-1)%3 == 0)    //processor B
		{
			distance[count] = 0;
		}

		else if( (i-2)%3 == 0)  //processor C
		{
			distance[count] = -1;
		}
   
        mapprocessor2dist[count] = i;       // mapping count to processor nos
 		i = next[i];

		count++;		
	}
 
    linkedlist[count] = -1;    //next node is -1
	distance[count] = -1;       //for last processor C distance is -1
    mapprocessor2dist[count] = 5;         //C node of root
 
	int * reversedistance= new int[totalNodes];  
	
	//Reverse distance
	for(int i=0; i<totalNodes; i++)
	{
		reversedistance[totalNodes-i-1] = distance[i];
	}

	listRank(linkedlist,reversedistance,totalNodes);
    
    for(int i=0; i<totalNodes; i++)
	{
		distance[i] = reversedistance[totalNodes-i-1];
	}

	//mapping processor to node nos
	cout << "------ Depth of nodes of the tree ------\n";
	int* finalDepth = new int[lastNode];
    for(int k=0; k <lastNode; k++)
    {
    	finalDepth[k] = -10000;
    }

	for(int i=0; i<totalNodes; i++)
	{
		int processorNos = mapprocessor2dist[i];
		if ( (processorNos-2) % 3 == 0)
		{
			int nodeNos = (processorNos-2) / 3 ;
			finalDepth[nodeNos] = distance[i];
		}
		
	}

	for(int i=0; i<=lastNode; i++)
	{
		if( finalDepth[i] != -10000 )
		cout << "Node # "  << i << " Depth : " << finalDepth[i] << endl;
	}

	

    gettimeofday(&end, NULL);
    float delta = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    cout << "TOTAL TIME TAKEN : " << delta << " sec" << endl;

  return 0;

}
void listRank(int * list,int *dist, int n)
{
	int i=0;
	while(i < n)
	{
		i++; // moving to next node of the linked list
	}

    int backtrack=i-1;

	while(backtrack >=0 )
	{
		if( backtrack == n-1)
		{
			dist[backtrack] = 1;
		}

		else
		{
			dist[backtrack] = dist[backtrack+1] + dist[backtrack];
		}

		backtrack--;
		
	}

}
