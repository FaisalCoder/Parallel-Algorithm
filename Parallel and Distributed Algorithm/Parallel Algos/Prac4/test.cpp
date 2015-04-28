#include<iostream>
#include<new>
#include<cmath>
#include <sys/time.h>
#include <stdlib.h>
using namespace std;

//Size of matrices
const int l = 2;
const int m = 3;
const int n = 3;


void multiply(int [l][m] ,int [m][n],int[l][n] );


int main()
{
	cout << "Size of matrix 1 : " << l << " x " << m <<  endl ;
	cout << "Size of matrix 2 : " << m << " x " << n <<  endl ;

    //matrix 1
    cout << "------ Matrix 1 -----> \n";
	int matrix1[l][m];
	for(int i=0; i<l; i++)
	{
		 
		for( int k=0; k<m; k++)
		{
			matrix1[i][k] = rand()%10;
			cout << "Matrix[" << i << "][" << k << "] = " << matrix1[i][k]  << endl;
		}
		cout << endl;
	}
   
    //matrix 2
    cout << "------ Matrix 2 -----> \n";
	int matrix2[m][n];
	for(int i=0; i<m; i++)
	{
		for( int k=0; k<n; k++)
		{
			matrix2[i][k] = rand()%10;
			cout << "Matrix[" << i << "][" << k << "] = " << matrix2[i][k]  << endl;
		}
		cout << endl;
	}


	//output matrix
	int result[l][n] ;	
	
	struct timeval start, end;
    gettimeofday(&start, NULL);

	multiply(matrix1,matrix2,result);

	gettimeofday(&end, NULL);
    float delta = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
   
	cout << "------ Result -----> \n";
	for(int i=0; i<l; i++)
	{
		for( int k=0; k<n; k++)
		{
			cout << "Matrix[" << i << "][" << k << "] = " << result[i][k]  << endl;
		}
		cout << endl;
	}

	cout << "TOTAL TIME TAKEN : " << delta << " sec" << endl;

	return 0;
}

void multiply(int matrix1[l][m], int matrix2[m][n],int result[l][n])
{

	for(int i=0; i<l; i++)
	{
		for(int j=0; j<n; j++)
		{
			result[i][j] = 0;
			for( int k=0; k<m; k++)
			{
				result[i][j] += matrix1[i][k]*matrix2[k][j];
			}
		}
	}

}


