#include<iostream>
#include<new>
#include<omp.h>
#include<ctime>
#include <sys/time.h>
using namespace std;

void seive_parallel(int,int []);
void seive_sequential(int,int []);
void print(int,int []);


int main()
{
    int n = 100000;
    
    // current date/time based on current system   
    //const clock_t st = clock()/CLOCKS_PER_SEC; 
    
    struct timeval start, end;
    

    int* prime = new int[n-1];
  #pragma omp parallel for
    for(int i=0; i <= n-2; i++)
    {
        prime[i] = i+2;
      //  cout << "prime[" << i << "] = " << prime[i] << "\t";
    }

    cout << "-----PARALLEL------\n" ;
    gettimeofday(&start, NULL);
    seive_parallel(n,prime);   
    gettimeofday(&end, NULL);
    float delta = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    cout << "TOTAL TIME TAKEN PARALLEL: " << delta << endl;

    #pragma omp parallel for
    for(int i=0; i <= n-2; i++)
    {
        prime[i] = i+2;
      //  cout << "prime[" << i << "] = " << prime[i] << "\t";
    }

    cout << "-----SEQUENTIAL------\n" ;
    gettimeofday(&start, NULL);
    seive_sequential(n,prime);   
    gettimeofday(&end, NULL);
    float delta2 = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    cout << "TOTAL TIME TAKEN SEQUENTIAL: " << delta2 << endl;

    return 0;
}

void seive_parallel(int n, int prime[])
{
    int k = 2;
    while(k*k <= n)
    {
        //Marking the multiples of k
     #pragma omp parallel for
        for(int i=0;i <= n-2; i++)
        {
            if ( prime[i] >= k*k && prime[i]%k == 0)
            {
                prime[i] = -1;
            }
        }

        //Finding the next k
        for( int i=0; i<= n-2; i++)
        {
            if ( prime[i] > k && prime[i] != -1)
            {
                k = prime[i];
                break;
            }
        }
    }
}

void seive_sequential(int n, int prime[])
{
    int k = 2;
    while(k*k <= n)
    {
        //Marking the multiples of k
        for(int i=0;i <= n-2; i++)
        {
            if ( prime[i] >= k*k && prime[i]%k == 0)
            {
                prime[i] = -1;
            }
        }

        //Finding the next k
        for( int i=0; i<= n-2; i++)
        {
            if ( prime[i] > k && prime[i] != -1)
            {
                k = prime[i];
                break;
            }
        }
    }
}

void print(int n, int a[])
{
    cout << "\nList of prime numbers:\n" ;
 #pragma omp parallel for
    for( int i=0; i<= n-2; i++)
    {
        if(a[i] != -1)
        {
//            cout << a[i] << "\t : " << omp_get_thread_num() << "\n";
              cout << a[i] << "\t";
        }
    }
   cout << endl;
}

