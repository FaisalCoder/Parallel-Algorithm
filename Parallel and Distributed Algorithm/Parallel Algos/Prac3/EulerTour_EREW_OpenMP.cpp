#include<iostream>
#include<new>
#include<omp.h>
#include<cmath>
#include <sys/time.h>
#include <stdlib.h>
using namespace std;

void print_next(int*,int); 
void print_dist(int*,int); 
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

    //Display nodes along with their processor nos
    /*
	for(int i=0; i<lastNode+1;i++)
	{
		if(tree[i] == 0){continue;}
		cout << "Node : " << i << " \n";
    	cout << "A : " << 3*i << endl;
    	cout << "B : " <<  3*i+1 << endl;
    	cout << "C : " << 3*i+2 << endl;
	}
	*/
    srand(time(NULL));

    struct timeval start, end;
    gettimeofday(&start, NULL);

    //creating the linked list of the processors
   #pragma omp parallel for
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

    	/*else if(parent == 1)
    	{
    		next[3*i+2] = 5;
    	}*/

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
   
   //     cout << " i : " << i << " next : " << next[i] << endl;
        mapprocessor2dist[count] = i;       // mapping count to processor nos
 		i = next[i];

		count++;		
	}
 
	//cout << "Outside count : " << count << endl;
    linkedlist[count] = -1;    //next node is -1
	distance[count] = -1;       //for last processor C distance is -1
    mapprocessor2dist[count] = 5;         //C node of root

/*	cout << "------ Mapped list ------\n";
	for(int i=0; i<totalNodes; i++)
	{
		cout << " Node # " << i << " : " << mapprocessor2dist[i] << endl;
	}
*/
	int * reversedistance= new int[totalNodes];  
	
	//Reverse distance
	for(int i=0; i<totalNodes; i++)
	{
		reversedistance[totalNodes-i-1] = distance[i];
	}

/*
	cout << "------ Linked list ------\n";
	print_next(linkedlist,totalNodes);
	cout << "------ Original distance ------\n";
	print_dist(distance,totalNodes);
*/
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
			//cout << "NODENOS : " << nodeNos << " VALUE  : " << distance[i] << endl;
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


  //  cout << "------ Distance list ------\n";
	//print_dist(distance,totalNodes);

/* */

  return 0;

}

void listRank(int * list,int *dist, int n)
{
	int* temp1 = new int[n];
	int* temp2 = new int[n];
	
	#pragma omp parallel  shared(list,dist,n,temp1,temp2) num_threads(n)
	{
		int i = omp_get_thread_num();
	//	cout << "THREAD : " << i << endl;
        
        for( int k=0; k<n; k++)
        {      	
        	if( list[i] == -1)          //bypassing when list[i] == -1
        	{
        		//pass
        	}

        	else
        	{
        		temp1[i] = dist[i] + dist[list[i]];
			    temp2[i] = list[list[i]];
        	}
        	
			#pragma omp barrier
	        
	        if( list[i] == -1)        //bypassing when list[i] == -1
	        { 
	           //pass
	        }  

	        else
	        {
	        	dist[i] = temp1[i];
			    list[i] = temp2[i];
	        } 
	        

			#pragma omp barrier

        }
	}
}

void print_next(int * list, int n)
{
	for( int i=0; i<n; i++)
	{
		cout << "Next # " << i << " : " << list[i] << endl;
	}
}

void print_dist(int * list, int n)
{
	for( int i=0; i<n; i++)
	{
		cout << "Distance # " << i << " : " << list[i] << endl;
	}
}