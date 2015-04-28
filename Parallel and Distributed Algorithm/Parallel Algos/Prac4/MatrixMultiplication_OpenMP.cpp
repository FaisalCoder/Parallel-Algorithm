#include<iostream>
#include<new>
#include<omp.h>
#include<cmath>
#include <sys/time.h>
#include <stdlib.h>
using namespace std;

//Size of matrices
/*
const int l = 100;
const int m = 100;
const int n = 100;
*/
const int l = 3;
const int m = 3;
const int n = 3;

struct matrix_entry
{
	int A;  //from matrix1
	int B;  //from matrix2

};

//openmp implementation of matrix multiplication
void multiply(matrix_entry [l][n],int[l][n] );


int main()
{
	cout << "Size of matrix 1 : " << l << " x " << m <<  endl ;
	cout << "Size of matrix 2 : " << m << " x " << n <<  endl ;

    //matrix 1
    cout << "------ Matrix 1 -----> \n";
	int matrix1[l][m];
	#pragma omp parallel for
	for(int i=0; i<l; i++)
	{
		 
		for( int k=0; k<m; k++)
		{
			matrix1[i][k] = (i+k)%10;
			cout << "Matrix[" << i << "][" << k << "] = " << matrix1[i][k]  << endl;
		}
		cout << endl;
	}
   
    //matrix 2
    cout << "------ Matrix 2 -----> \n";
	int matrix2[m][n];
	#pragma omp parallel for
	for(int i=0; i<m; i++)
	{
		for( int k=0; k<n; k++)
		{
			matrix2[i][k] = (i+k)%10;
			cout << "Matrix[" << i << "][" << k << "] = " << matrix2[i][k]  << endl;
		}
		cout << endl;
	}


	//Intial matrix of algo
	matrix_entry temp[l][n];
	for( int i=0; i<l; i++)
	{
		for(int j=0; j<n; j++)
		{
			temp[i][j].A = matrix1[i][j];
			temp[i][j].B = matrix2[i][j];
		}
	}
    
    /*
	cout << "------ Intial matrix -----> \n";
	for(int i=0; i<l; i++)
	{
		for( int k=0; k<n; k++)
		{
			cout << "Matrix[" << i << "][" << k << "] : " << temp[i][k].A << " , " << temp[i][k].B  << "\n";
		}
		cout << endl;
	}
	*/

	//Creating the staggered matrix
	matrix_entry staggered[l][n];
    //Shifting rows and columns
	for( int i=0; i<l; i++)
	{
		for(int j=0; j<n; j++)
		{
			staggered[i][j].A = temp[i][(j+i)%n].A ;
			staggered[i][j].B = temp[(i+j)%l][j].B ;
		}
	}

	cout << "------ Staggered matrix -----> \n";
	for(int i=0; i<l; i++)
	{
		for( int k=0; k<n; k++)
		{
			cout << "Matrix[" << i << "][" << k << "] : " << staggered[i][k].A << " , " << staggered[i][k].B  << "\n";
		}
		cout << endl;
	} 

	//output matrix
	int result[l][n];	
	
	struct timeval start, end;
    gettimeofday(&start, NULL);

	multiply(staggered,result);

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

void multiply(matrix_entry staggered[l][n],int result[l][n])
{
	int nos_processors = l*n;
  #pragma omp parallel shared(result,staggered) num_threads(nos_processors)
	{
		int processor_id = omp_get_thread_num();
		int row = processor_id/l;
		int col = processor_id%n;
		
		int tmp = 0;

		for( int iter_count=0; iter_count<m; iter_count++)
		{
			tmp += staggered[row][(col+iter_count)%n].A*staggered[(row+iter_count)%l][col].B;
		}

		result[row][col] = tmp;
	}

}



