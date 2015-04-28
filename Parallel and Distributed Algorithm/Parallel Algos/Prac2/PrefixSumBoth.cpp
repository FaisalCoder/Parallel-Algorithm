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
void prefixSum_parallelV2(int [],int);
void prefixSum_sequential(int [],int);
void print(int [],int);

int main()
{
    int n,i=0;
    cout << "Enter the number of elements in the array : " ;
    cin >> n;

    int* array1 = new int[n];
    int* array2 = new int[n];
    srand(time(NULL));

    struct timeval start, end;


   #pragma omp parallel for
    for( int i=0; i<n ; i++)
    {
        ///array1[i] = array2[i] = rand()%100;
        array1[i] = array2[i] = (i+1);

    }

    /**
    Parallel Implementation
    **/
    //print(array,n);
    cout << "\n-----PARALLEL-----\n";
    gettimeofday(&start, NULL);
//    prefixSum_parallel(array2,n);
    prefixSum_sequential(array2,n);
    gettimeofday(&end, NULL);

    float delta2 = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    cout << "TOTAL TIME TAKEN BY PARALLEL : " << delta2 << endl;
    cout << "Total sum : " << array2[n-1] << endl;
    //delete []array2;
    //cout << "\nOUTPUT---------\n";
   // print(array2,n);


    /**
    Sequential Implementation
    **/
    //print(array,n);
    struct timeval start1, end1;
    cout << "-----SEQUENTIAL------\n";
    gettimeofday(&start1, NULL);
   // prefixSum_sequential(array1,n);
    prefixSum_parallel(array1,n);
    gettimeofday(&end1, NULL);

    float delta = ((end1.tv_sec  - start1.tv_sec) * 1000000u + end1.tv_usec - start1.tv_usec) / 1.e6;
    cout << "TOTAL TIME TAKEN BY SEQUENTIAL : " << delta << endl;
    cout << "Total sum : " << array1[n-1] << endl;
    //print(array1,n);
   // delete []array1;

    return 0;
}

void prefixSum_parallel(int x[],int n)
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

void prefixSum_parallelV2(int x[],int n)
{
    int* t = new int[n];
    int i;

//    #pragma omp parallel private(i)
   #pragma omp parallel for
for (int i1=1; i1 < n; i1++){

  
    for (int j = 0; j < log(n); j++)
    {
     
	{    
               if (i1-pow(2,j) >= 0){
               x[i1] = x[i1] + x[i1-int(pow(2,j))];
	      /*// #pragma omp for
		for (i = pow(2,j); i < n; i++)
		{
                    t[i] = x[i] + x[i-int(pow(2,j))];
		}

	       //#pragma omp for
		for (i = pow(2,j); i < n; i++)
		{
		    x[i] = t[i];
		}*/
           }
        }
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


