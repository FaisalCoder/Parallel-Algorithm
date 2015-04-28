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
	int n;
	cout << "Enter number of elements in the linked list : ";
	cin >> n;
	int* next = new int[n];          //Simulating a linked list using an array
	int*d = new int[n];

	srand(time(NULL));

    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    //initialising the linked list
   #pragma omp parallel for shared(next)
	for(int i = 0; i < n; i++)
	{
		if( i == n-1)
		{
			next[i] = -1;       //-1 is null
		}

		else
		{
			next[i] = i+1;
		}
		
	}
    
    cout << "------ Linked list ------\n";
	print_next(next,n);
	listRank(next,d,n);
    cout << "------ List Rank ------\n";
	print_dist(d,n);

	gettimeofday(&end, NULL);
    float delta = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    cout << "TOTAL TIME TAKEN : " << delta << " sec" << endl;

	return 0;
}

void listRank(int * list,int *dist, int n)
{
  //#pragma omp parallel shared(list,dist,n) num_threads(n)
	int* temp1 = new int[n];
	int* temp2 = new int[n];

  #pragma omp parallel for
	for( int i=0; i<n; i++)
	{
		if (list[i] == -1)
		{
			dist[i] = 0;
		}

		else
		{
			dist[i] = 1;

		}
	}

	#pragma omp barrier
   
	
	#pragma omp parallel  shared(list,dist,n,temp1,temp2) num_threads(n)
	//for( int i=0; i<n; i++)
	{
		int i = omp_get_thread_num();
        
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