#include<iostream>
#include <time.h>
#include <stdlib.h>
#include<omp.h>
#include<ctime>
#include <sys/time.h>
#include<new>
#include<math.h>
using namespace std;

void prefixSum_parallel(int [],int);
void prefixSum_sequential(int [],int);
void print(int [],int);

int main()
{
    int n,i=0;
    cout << "Enter the number of elements in the array : " ;
    cin >> n;

    int* array = new int[n];
    srand(time(NULL));

    struct timeval start, end;


   #pragma omp parallel for
    for( int i=0; i<n ; i++)
    {
        array[i] = rand()%100;
    }


    /**
    Sequential Implementation
    **/
    //print(array,n);
    cout << "-----SEQUENTIAL------\n";
    gettimeofday(&start, NULL);
    prefixSum_sequential(array,n);
    gettimeofday(&end, NULL);

    float delta = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    cout << "TOTAL TIME TAKEN BY SEQUENTIAL : " << delta << endl;
    cout << "Total sum : " << array[n-1] << endl;

    /**
    Parallel Implementation
    **/
    //print(array,n);
    cout << "\n-----PARALLEL-----\n";
    gettimeofday(&start, NULL);
    prefixSum_parallel(array,n);
    gettimeofday(&end, NULL);

    float delta2 = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    cout << "TOTAL TIME TAKEN BY PARALLEL : " << delta2 << endl;
    cout << "Total sum : " << array[n-1] << endl;

    //cout << "\nOUTPUT---------\n";
    //print(array,n);


    delete []array;

    return 0;
}

void prefixSum_parallel(int x[],int n)
{
    
   int* t = new int[n];
    int j,tid;

   #pragma omp parallel for
    for (int j=0; j<=n-1; j++)
    {
        if (j>=1)
        {
            x[j] = x[j]*100;
        }
    }
}

void prefixSum_sequential(int x[],int n)
{
    for (int j=0; j<=n-1; j++)
    {
        if (j>=1)
        {
            x[j] = x[j] + x[j-1];
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


