#include<iostream>
#include<new>
#include<omp.h>
#include<cmath>
#include <sys/time.h>
#include <stdlib.h>
using namespace std;

void print(int*,int); 
void sum(int*,int); 
void listRank(int *,int *, int );
void listRank_V2(int *,int *, int );

int main()
{	
	int n;
	cout << "Enter number of elements in the linked list : ";
	cin >> n;
	int* next = new int[n];          //Simulating a linked list using an array
	int*d = new int[n];
    /*
    int count = 0;
    #pragma omp parallel num_threads(n)         //threads specified here are what are executed finally
    {
    	cout << " E is the second vowel\n";
    	next[omp_get_thread_num()] = omp_get_thread_num();
    	//next[omp_get_thread_num()] = count;
    	count += 1;
    }
    cout << "COUNT :  " << count << endl;
    cout << "OUTPUT : \n";
    print(next,n);
*/
    
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
/*
	cout << "NEXT ARRAY : \n";
    print(next,n);


    
	#pragma omp parallel num_threads(n)
	{
		d[omp_get_thread_num()] = omp_get_thread_num()*10;
	}

    
	cout << "OUTPUT 3 : \n";
    print(next,n);

    cout << "OUTPUT 4: \n";
    sum(d,n);
    print(d,n);
	*/

    
    cout << "------ Linked list ------\n";
	print(next,n);
	listRank(next,d,n);
    cout << "------ List Rank ------\n";
	print(d,n);
	/*/*
    cout << "linked list\n";
    print(next,n);
	sum(next,n);
	cout << "Sum list\n";
	print(next,n);
	*/

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
        	if( list[i] == -1)
        	{
        		temp1[i] = 0;
			    temp2[i] = 0;

        	}

        	else
        	{
        		temp1[i] = dist[i] + dist[list[i]];
			    temp2[i] = list[list[i]];
        	}
        	
			//cout << "THREAD : " << i << endl;
			#pragma omp barrier
	        
	        if( list[i] == -1) { temp1[i] = dist[i];temp2[i] = list[i];};
	        dist[i] = temp1[i];
			list[i] = temp2[i];

			#pragma omp barrier

        }
	}
}

void listRank_V2(int * list,int *dist, int n)
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


	
	#pragma omp parallel  shared(list,dist,n,temp1,temp2) num_threads(n)
	//for( int i=0; i<n; i++)
	{
		int i = omp_get_thread_num();

		/*

		while(true)
		{
			if( list[i] == -1) break;

			dist[i] = dist[i] + dist[list[i]];
			list[i] = list[list[i]];
		}
		*/

		#pragma omp sections
		{
			for( int k=0; k< n; k++)
			{
				if( list[i] == -1) break;

			//	#pragma omp section 
				{
					temp1[i] = dist[i] + dist[list[i]];
				    temp2[i] = list[list[i]];
				}
				//#pragma omp barrier
				//dist[i] = temp1[i];
				//#pragma omp barrier
				
				
				//#pragma omp barrier
				//list[i] = temp3;
			   
			  // #pragma omp section
				{					
					dist[i] = temp1[i];
				    list[i] = temp2[i];
				}		
			}
	    }
	}
}

// working fine
void sum( int * list, int n)
{
	int tid,max_threads,curr_threads;

  #pragma omp parallel shared(list) num_threads(n)
	{
		int t = omp_get_thread_num();
		//list[omp_get_thread_num()] = omp_get_thread_num()*10;	
		int c = 0;
		list[t] = t;
        while(c < n){
         list[t] = list[t] + 10;
         c++;		
        }
		
	}
}

void print(int * list, int n)
{
	for( int i=0; i<n; i++)
	{
		cout << "element # " << i << " : " << list[i] << endl;
	}
}