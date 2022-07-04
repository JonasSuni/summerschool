#include <cstdio>
#include <hip/hip_runtime.h>

/* Blocksize divisible by the warp size */
#define BLOCKSIZE 64

/* Example struct to practise copying structs with pointers to device memory */
typedef struct
{
  float *x;
  int *idx;
  int size;
} Example;

/* GPU kernel definition */
__global__ void hipKernel(Example* const d_ex)
{
  const int thread = blockIdx.x * blockDim.x + threadIdx.x;
  if (thread < d_ex->size)
  {
    printf("x[%d]: %.2f, idx[%d]:%d/%d \n", thread, d_ex->x[thread], thread, d_ex->idx[thread], d_ex->size - 1);
  }
}

/* Run on host */
void runHost()
{
  // Allocate host struct
  Example *ex;
  ex = (Example*)malloc(sizeof(Example));
  ex->size = 10;

  // Allocate host struct members
  ex->x = (float*)malloc(ex->size * sizeof(float));
  ex->idx = (int*)malloc(ex->size * sizeof(int));

  // Initialize host struct members
  for(int i = 0; i < ex->size; i++)
  {
    ex->x[i] = (float)i;
    ex->idx[i] = i;
  }

  // Print struct values from host
  printf("\nHost:\n");
  for(int i = 0; i < ex->size; i++)
  {
    printf("x[%d]: %.2f, idx[%d]:%d/%d \n", i, ex->x[i], i, ex->idx[i], ex->size - 1);
  }

  // Free host struct
  free(ex->x);
  free(ex->idx);
  free(ex);
}

/* Run on device using Unified Memory */
void runDeviceUnifiedMem()
{
  //#error Allocate struct using Unified Memory
  Example *ex;
  hipMallocManaged((void**) &ex,sizeof(Example));
  ex->size = 10;

  //#error Allocate struct members using Unified 
  hipMallocManaged((void**)&ex->x,ex->size*sizeof(float));
  hipMallocManaged((void**)&ex->idx,ex->size*sizeof(int));


  // Initialize struct from host
  for(int i = 0; i < ex->size; i++)
  {
    ex->x[i] = (float)i;
    ex->idx[i] = i;
  }

  //#error Print struct values from device by calling hipKernel()
  printf("\nDevice (UnifiedMem):\n");
  hipLaunchKernelGGL(hipKernel,dim3(16),dim3(BLOCKSIZE),0,0,ex);

  //#error Free struct
  hipFree(ex->x);
  hipFree(ex->idx);
  hipFree(ex);
}

/* Create the device struct (needed for explicit memory management) */
Example* createDeviceExample(Example *ex)
{

  Example* d_ex;
  
  //#error Allocate device struct
  hipMalloc((void**)&d_ex,sizeof(Example));

  float *d_x;
  int *d_idx;

  //#error Allocate device struct members
  hipMalloc((void**)&d_x,ex->size*sizeof(float));
  hipMalloc((void**)&d_idx,ex->size*sizeof(int));

  //#error Copy arrays pointed by the struct members from host to device
  hipMemcpy(d_x,(ex->x),ex->size*sizeof(float),hipMemcpyHostToDevice);
  hipMemcpy(d_idx,(ex->idx),ex->size*sizeof(int),hipMemcpyHostToDevice);

  //#error Copy struct members from host to device
  hipMemcpy(&(d_ex->size),&(ex->size),sizeof(int),hipMemcpyHostToDevice);
  hipMemcpy(&(d_ex->x),&d_x,sizeof(float*),hipMemcpyHostToDevice);
  hipMemcpy(&(d_ex->idx),&d_idx,sizeof(int*),hipMemcpyHostToDevice);

  //#error Return device struct

  return d_ex;
}

/* Free the device struct (needed for explicit memory management) */
void freeDeviceExample(Example *d_ex)
{
  float *d_x;
  int *d_idx;

  //#error Copy struct members (pointers) from device to host
  printf("Copying arrays from device to host\n");
  hipMemcpy(&d_x,&(d_ex->x),sizeof(float*),hipMemcpyDeviceToHost);
  hipMemcpy(&d_idx,&(d_ex->idx),sizeof(int*),hipMemcpyDeviceToHost);

  //#error Free device struct members
  printf("Freeing arrays on device\n");
  hipFree(d_x);
  hipFree(d_idx);

  //#error Free device struct
  printf("Freeing device struct members and struct\n");
  hipFree(d_ex->x);
  hipFree(d_ex->idx);
  hipFree(d_ex);
}

/* Run on device using Explicit memory management */
void runDeviceExplicitMem()
{
  //#error Allocate host struct
  Example* ex;

  ex = (Example*)malloc(sizeof(Example));
  ex->size = 10;

  // Allocate host struct members
  ex->x = (float*)malloc(ex->size * sizeof(float));
  ex->idx = (int*)malloc(ex->size * sizeof(int));

  // Initialize host struct
  for(int i = 0; i < ex->size; i++)
  {
    ex->x[i] = (float)i;
    ex->idx[i] = i;
  }

  // Allocate device struct and copy values from host to device
  Example *d_ex = createDeviceExample(ex);

  //#error Print struct values from device by calling hipKernel()
  printf("\nDevice (ExplicitMem):\n");
  hipLaunchKernelGGL(hipKernel,dim3(16),dim3(BLOCKSIZE),0,0,d_ex);
  hipDeviceSynchronize();

  // Free device struct
  freeDeviceExample(d_ex);

  //#error Free host struct
  free(ex->x);
  free(ex->idx);
  free(ex);

}

/* The main function */
int main(int argc, char* argv[])
{
  runHost();
  runDeviceUnifiedMem();
  runDeviceExplicitMem();
}
