#include<iostream>
#include <time.h>
#include <stdlib.h>
#include<ctime>
#include <sys/time.h>
#include<new>
using namespace std;

void prefixSum(int [],int);
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

    for( int i=0; i<n ; i++)
    {
        array[i] = (i+1)%100;
    }

    //cout << "\nINPUT---------\n";
    //print(array,n);
    prefixSum(array,n);
    
    gettimeofday(&end, NULL);
    float delta = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    cout << "TOTAL TIME TAKEN : " << delta << endl;

    cout << "\nOUTPUT---------\n";
    //print(array,n);
    cout << "Total sum : " << array[n-1] << endl;

    delete []array;

    return 0;
}

void prefixSum(int x[],int n)
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


