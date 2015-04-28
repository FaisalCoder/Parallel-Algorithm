#include<iostream>
#include <time.h>
#include <stdlib.h>
#include<omp.h>
#include<ctime>
#include <sys/time.h>
#include<new>
#include<math.h>
using namespace std;

void prefixSum(int [],int);
void prefixSumV2(int [],int);
void print(int [],int);

int main()
{
    int n,i=0;
    cout << "Enter the number of elements in the array : " ;
    cin >> n;

    int* array = new int[n];
    srand(time(NULL));

    struct timeval start, end;
    gettimeofday(&start, NULL);

   #pragma omp parallel for
    for( int i=0; i<n ; i++)
    {
        array[i] = (i+1)%100;
    }

    //cout << "\nINPUT---------\n";
    //print(array,n);
    prefixSumV2(array,n);

    gettimeofday(&end, NULL);
    float delta = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    cout << "TOTAL TIME TAKEN : " << delta << endl;

    //cout << "\nOUTPUT---------\n";
    //print(array,n);
    cout << "Total sum : " << array[n-1] << endl;

    delete []array;

    return 0;
}

void prefixSum(int x[],int n)
{
    int* t = new int[n];
    int i;

    for (int j = 0; j < log(n); j++)
    {
     #pragma omp parallel private(i)
	{
	       #pragma omp for
		for (i = pow(2,j); i < n; i++)
		{
                    t[i] = x[i] + x[i-int(pow(2,j))];
		}

	       #pragma omp for
		for (i = pow(2,j); i < n; i++)
		{
		    x[i] = t[i];
		}
        }
    }
}

void prefixSumV2(int x[],int n)
{
    int* t = new int[n];
    int j,tid;

   #pragma omp parallel shared(n,x,t) private(j,tid) num_threads(n)
    {
        tid = omp_get_thread_num();
        //cout << "TID : " << tid << endl;
        for ( j = 1; j < n; j = 2*j)
        {
            if (tid >= j)
            t[tid] = x[tid] + x[tid - j];
            #pragma omp barrier

            x[tid] = t[tid];
            #pragma omp barrier
        }
    }
}

void print(int a[],int n)
{
    cout << "Total elements in the array : " << n << endl;
    cout << "Input Array :\n";

    for (int i=0; i<n; i++)
    {
        cout << "A[" << i << "] = " << a[i] << "\n";

    }

}


