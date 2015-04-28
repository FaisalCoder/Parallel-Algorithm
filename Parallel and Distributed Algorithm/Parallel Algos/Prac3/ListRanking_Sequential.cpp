#include<iostream>
#include<new>
#include <sys/time.h>
#include <stdlib.h>
using namespace std;

void print(int*,int);      //print contents of the linked list
void listRank(int*,int*,int);

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
	for(int i = 0; i < n; i++)
	{
		if( i == n-1)
		{
			next[i] = -1;       //-1 is null
		}

		else
		{
			next[i] = i;
		}
		
	}

    cout << "------ Linked list ------\n";
	print(next,n);
	listRank(next,d,n);
    cout << "\n------ List Rank ------\n";
	print(d,n);

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
			dist[backtrack] = 0;
		}

		else
		{
			dist[backtrack] = dist[backtrack+1] + 1;
		}

		backtrack--;
		
	}

}

void print(int * list, int n)
{
	for( int i=0; i<n; i++)
	{
		cout << "element # " << i << " : " << list[i] << "\n";
	}
}