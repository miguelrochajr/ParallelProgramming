#include <stdio.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <omp.h>
#include <time.h>

/*
#define NEURONS // Number of neurons in hidden layer
*/
#define OUTPUTS	1
#define INPUTS  2
#define SAMPLES 1

void Matrix_mult(float *A, float *B, float *res,int M, int L, int N);
void Random_assign(float *Matrix, int rows, int cols);
void Activ_func(float *in,float *out,int row, int col);
void Usage();
//testando XOR com diferentes quantidades de NEURONS
int main(int argc, char *argv[])
{
	if (argc < 2) Usage();

	int NEURONS;
	double start, finish;
	NEURONS = strtol(argv[1], NULL, 10);

	/*  Input vector */
	float X[INPUTS+1][SAMPLES] = {{0},{0}};

	/* Hidden layer */
	float Wx[NEURONS][INPUTS+1];// Hidden layer weights matrix
	float IDF1[NEURONS][SAMPLES];// Induced Local field first layer
	float ATV1[NEURONS][SAMPLES];// Activated induced local fields

	/* Output Layer */
	float Wy[OUTPUTS][NEURONS+1];// Output layer weights matrix
	float IDF2[OUTPUTS][SAMPLES];// Induced Local field second layer
	float ATV2[OUTPUTS][SAMPLES];// Activated induced local fields

	Random_assign(Wx[0],NEURONS,INPUTS+1);
	Random_assign(Wy[0],OUTPUTS, NEURONS+1);

	start = omp_get_wtime();

	Matrix_mult(Wx[0],X[0],IDF1[0],NEURONS,INPUTS+1,SAMPLES);
	Activ_func(IDF1[0],ATV1[0],NEURONS,SAMPLES);

	Matrix_mult(Wy[0],ATV1[0],IDF2[0],OUTPUTS,NEURONS+1,SAMPLES);
	Activ_func(IDF2[0],ATV2[0],OUTPUTS,SAMPLES);

	finish = omp_get_wtime();

	printf("For %d neurons, the time taken was: %lf seconds \n", NEURONS, finish-start);

	#ifdef DEBUG
		int i,j;
		for(i = 0;i< OUTPUTS;i++)
		{
			for(j=0; j < SAMPLES;j++)
			{
				std::cout << std::setprecision(2) << std::fixed << ATV2[i][j] << "\t";
			}
			std::cout << "\n";
		}
	#endif

	return 0;
}

void Usage()
{
	std::cout << "Some arguments are missing." << std::endl;
	std::cout << "Usage: ./feed <NEURONS>" << std::endl;
	exit(0);
}

void Matrix_mult(float *A, float *B, float *res, int M, int L, int N)
{
	int i, j, k;
	double result = 0;
	for (i = 0; i < M; i++){
			for (j = 0; j < N; j++) {
					result = 0;
					for (k = 0; k < L; k++) {
							 result += (*(A + i*L + k))*(*(B + k*N + j));
					}
					*(res+ j +i*N) = (float)result; /*REMINDER: result is a Double variable*/
			}
	}
}

void Activ_func(float *in,float *out,int row, int col)
{
	int i,j;
	double result;
	for(i=0;i < row;i++)
	{
		for(j = 0;j< col;j++)
		{
			 result = 1.0/(1.0 + exp(-(*(in + i*col+j))));
		}
		*(out + i*col+j) = (float)result; /* REMINDER: result is a DOUBLE */
	}
}

void Random_assign(float *Matrix, int rows, int cols)
{
	srand(time(NULL)); // seed for random. time(NULL) is from actual time
	int i,j;
	for(i=0; i<rows; i++)
	{
		for(j=0; j<cols; j++)
		{
			*Matrix++ = (float)rand()/(float)RAND_MAX;
		}
	}
}
