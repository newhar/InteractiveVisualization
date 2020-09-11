#include "cuda_runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

struct vec3
{
	float x,y,z;	
};

__global__ void vectorAdd(vec3 *c, vec3 *a, vec3 *b)
{
	int tid = threadIdx.x;
	/*	  1-1. write vector addition function						*/
	c->x = a->x + b->x;
	c->y = a->y + b->y;
	c->z = a->z + b->z;
	
}


int main( void )
{
	/*	  2-1. Check whether a proper device is mounted 			*/
	cudaError_t cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess)
	{
		cout << "cudaSetDevice failed! Do you have a CUDA-capable GPU installed?" << endl;
	}
	
	/*	  2-2. Declare Host and Device pointer variables			*/
	/*    2-3. Allocate Host memory									*/
	struct vec3 *a= (vec3*)malloc(sizeof(vec3));
	struct vec3 *b= (vec3*)malloc(sizeof(vec3));
	struct vec3 *c= (vec3*)malloc(sizeof(vec3));
	struct vec3* dev_a;
	struct vec3* dev_b;
	struct vec3* dev_c;
	
	/*    2-4. Allocate Device memory								*/
	/*    2-5. Check that memory is allocated well on Device		*/
	cudaStatus = cudaMalloc((void**)&dev_a, sizeof(vec3));
	if(cudaStatus != cudaSuccess) {
		printf("cudaMalloc failed!");
	}
	cudaStatus = cudaMalloc((void**)&dev_b, sizeof(vec3));
	if(cudaStatus != cudaSuccess) {
		printf("cudaMalloc failed!");
	}
	cudaStatus = cudaMalloc((void**)&dev_c, sizeof(vec3));
	if(cudaStatus != cudaSuccess) {
		printf("cudaMalloc failed!");
	}
	
	/*    2-6. Setup Input values to host array						*/
	a->x = 1;
	a->y = 2;
	a->z = 3;
	b->x = 10;
	b->y = 20;
	b->z = 30;
	
	/*    2-7. Copy memory for Input array from Host to Device		*/
	cudaStatus = cudaMemcpy(dev_a, a, sizeof(vec3), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
	}
	cudaStatus = cudaMemcpy(dev_b, b, sizeof(vec3), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
	}
	
	/*	  2-8. Call Kernel Function with <<<1, 1>>>					*/
	vectorAdd<<<1,1>>>(dev_c, dev_a, dev_b);

	/*    2-9. Copy memory for Result from Device to Host			*/
	cudaStatus = cudaMemcpy(c, dev_c, sizeof(vec3), cudaMemcpyDeviceToHost);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
	}
	
	/*    2-10. Print Results	*/
	printf("(x:%0.f, y:%0.f, z:%0.f) + ", a->x, b->y, b->z);
	printf("(x:%0.f, y:%0.f, z:%0.f)\n", b->x, b->y, b->z);
	printf("= (x:%0.f, y:%0.f, z:%0.f)\n", c->x, c->y, c->z);
	/*    2-11. Release Host and Device memory						*/
	cudaFree(dev_a); cudaFree(dev_b); cudaFree(dev_c);
	
	return 0;

}

// nvcc [fileName] -o [outName]