#define BLOCK_DIM	4
#define TILE_DIM	BLOCK_DIM


#include <stdio.h>
#include <stdlib.h>
#include<time.h>


void PrintMatrixToText(int* mtxArray, int height, int width, const char* fileName);
// Matrix Mult Kernel
__global__ void matrixMult( int* A, int* B, int* C, const int n, const int m, const int k, const int numPhase)
{
	__shared__ int Ads[TILE_DIM][TILE_DIM];
	__shared__ int Bds[TILE_DIM][TILE_DIM];
	/*Todo: Write C = A*B */
	int bx = blockIdx.x;  int by = blockIdx.y;
	int tx = threadIdx.x; int ty = threadIdx.y;

	int Row = by * blockDim.y + ty;
	int Col = bx * blockDim.x + tx;

	int Pvalue = 0;

	// Loop over the A and B tiles required to compute the C element
	for ( int ph=0 ; ph<numPhase ; ++ph )
	{
		// Collaborative loading of A and B tiles into shared memory
		if(  Row < n && ph*TILE_DIM + tx < m )	Ads[ty][tx] = A[ Row*m + ph*TILE_DIM + tx  ];
		else	Ads[ty][tx] = 0;
		
		if(  ph*TILE_DIM + ty < m && Col < k )	Bds[ty][tx] = B[ (ph*TILE_DIM + ty)*k + Col ];
		else	Bds[ty][tx] = 0;
		
		__syncthreads();

		if( Col < k && Row < n )
		{
			for ( int i=0 ; i<TILE_DIM ; ++i )
			{
				Pvalue += (Ads[ ty ][ i ] * Bds[ i ][ tx ]);//인덱스 입력
			}
			__syncthreads();
		}
	}

	if( Col < k && Row < n )	C[Row*k + Col] = Pvalue;
	
}

int main(){
	int *h_a, *h_b, *h_c;	// Host Variables
	int *d_a, *d_b, *d_c;	// Device Variables
	int n,m,k;
	
	
	// Receive Parameters
	/*Todo:Write Receive Parameters through console commands*/
	printf("Input the Value of n : ");
	scanf("%d",&n);
	printf("Input the Value of m : ");
	scanf("%d",&m);
	printf("Input the Value of k : ");
	scanf("%d",&k);	
	
	
	// Alloc & Initialize Host Input Matrices	
	h_a = (int*)malloc(n*m*sizeof(int));
	h_b = (int*)malloc(m*k*sizeof(int));
	h_c = (int*)malloc(n*k*sizeof(int));
	
	srand(time(NULL));
	
	/*Todo: Initialize A and B with Random Varialbles Ranged (-10, 10)*/
	for(int i = 0 ; i < n*m ; i++)
	{
		h_a[i] = rand() % 21 - 10;
	}
	for(int i = 0 ; i < m*k ; i++)
	{
		h_b[i] = rand() % 21 - 10;
	}
	
	// Allocate Device Memory
	cudaMalloc((void **) &d_a, n*m*sizeof(int));
	cudaMalloc((void **) &d_b, m*k*sizeof(int));
	cudaMalloc((void **) &d_c, n*k*sizeof(int));

	// Copy Input Matrices to Device Memory
	cudaMemcpy(d_a, h_a, n*m*sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, h_b, m*k*sizeof(int), cudaMemcpyHostToDevice);

	
	// Set Grid/Block Dimensions
	
	int dimxG = (int)ceil((float)k/(float)BLOCK_DIM);
	int dimyG = (int)ceil((float)n/(float)BLOCK_DIM);
	dim3 dimGrid( dimxG , dimyG);
	dim3 dimBlock(BLOCK_DIM,BLOCK_DIM);

	//페이징
	int numPhase = (int)ceil((float)m/(float)BLOCK_DIM);
		
	// Launch Kernel
	matrixMult<<<dimGrid ,dimBlock >>>(d_a, d_b, d_c, n, m, k, numPhase);

	// Copy Result to Host
	cudaMemcpy( h_c, d_c, n*k*sizeof(int) , cudaMemcpyDeviceToHost );
	
	// Free Device Memory
	cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);
	
	
	// Print Results
	PrintMatrixToText(h_a,n,m,"matrixA.txt");
	PrintMatrixToText(h_b,m,k,"matrixB.txt");
	PrintMatrixToText(h_c,n,k,"matrixC.txt");
	
	// Free Host Memory
	free(h_a); free(h_b); free(h_c);
	
	return 0;
}

// Utilities
void PrintMatrixToText(int* mtxArray, int height, int width, const char* fileName){
	FILE *f = fopen(fileName, "w");
	for ( int i=0 ; i<height ; i++ )
	{
		for ( int j=0 ; j<width ; j++ )
		{
			fprintf(f, "%d\t" , mtxArray[i*width+j] );
		}
		fprintf(f,"\n" );
	}	
	
	fclose(f);
}

