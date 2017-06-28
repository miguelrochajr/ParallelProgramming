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

//TODO:
//  - processar dados de arquivo CSV para preencher os pesos das camadas.
//  - modelo XOR que funcione para diferentes quantidades de neuronios
void random(float *m1, float *m2, int row1, int col1, int row2, int col2);
void produtomatricial(float *m1, float *m2, float *mprod, int row, int ig, int col);
void funcaoAtivacao(float *in,float *out,int row, int col);
void Usage();
//testando XOR com diferentes quantidades de NEURONS
int main(int argc, char *argv[])
{
	if (argc < 2) {
		Usage();
		return -1;
	}

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

	random(Wx[0],Wy[0],NEURONS,INPUTS+1, OUTPUTS, NEURONS+1); //creates random weights for each input due to each neuron


	start = omp_get_wtime();
	produtomatricial(Wx[0],X[0],IDF1[0],NEURONS,INPUTS+1,SAMPLES);// multiplies each input to each weight assigned to them
	/* pass the products of inputs by weights through
	 * the activation function of the first hidden layer*/
	funcaoAtivacao(IDF1[0],ATV1[0],NEURONS,SAMPLES);
	/*Multiplies the output matrix from the activation function by
	 * the output layer weight's*/
	produtomatricial(Wy[0],ATV1[0],IDF2[0],OUTPUTS,NEURONS+1,SAMPLES);
	/* Passes the local induced field (last activated weights*inputs) through the
	 * activation function of the output layer*/
	funcaoAtivacao(IDF2[0],ATV2[0],OUTPUTS,SAMPLES);
	/* Thus we get the result from the neural network.*/
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
}

void produtomatricial(float *m1, float *m2, float *mprod, int row, int ig, int col)
{
	int i,j,k;
	for(i=0; i<row; i++)
	{
		for(j=0; j<col; j++)
		{
			*mprod = 0;
			for(k=0; k<ig; k++)
			{
				*mprod += (*(m1 + i*ig + k))*(*(m2 + k*col + j));
			}
			mprod++;
		}
	}
}
void funcaoAtivacao(float *in,float *out,int row, int col)
{
	int i,j;
	for(i=0;i < row;i++)
	{
		for(j = 0;j< col;j++)
		{
			out[i*col+j] = 1.0/(1.0 + exp(-(in[i*col+j])));
		}
	}
}
void random(float *m1, float *m2, int row1, int col1, int row2, int col2)
{
	/*		*/
	srand(time(NULL)); // seed for random. time(NULL) is from actual time
	int i,j;
	for(i=0; i<row1; i++)
	{
		for(j=0; j<col1; j++)
		{
			*m1++ = (rand()%10)/10.0; //initializes Wx
		}
	}
	for(i=0; i<row2; i++)
	{
		for(j=0; j<col2; j++)
		{
			*m2++ = (rand()%11)/10.0; // Initializes Wy
		}
	}
}
