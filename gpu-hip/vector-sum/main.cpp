#include <cstdio>
#include <cmath>
#include <hip/hip_runtime.h>

// Data structure for storing decomposition information
struct Decomp {
    int len;    // length of the array for the current device
    int start;  // start index for the array on the current device
};


/* HIP kernel for the addition of two vectors, i.e. C = A + B */
__global__ void vector_add(double *C, const double *A, const double *B, int N)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    // Do not try to access past the allocated memory
    if (idx < N) {
        C[idx] = A[idx] + B[idx];
    }
}


int main(int argc, char *argv[])
{
    const int ThreadsInBlock = 128;
    double *dA[2], *dB[2], *dC[2];
    double *hA, *hB, *hC;
    int devicecount;
    int N = 100;
    hipEvent_t start, stop;
    hipStream_t strm[2];
    Decomp dec[2];

    // TODO: Check that we have two HIP devices available
    hipGetDeviceCount(&devicecount);
    if (devicecount != 2) {
        printf("Device count is not 2!");
        return 1;
    }

    // Create timing events
    hipSetDevice(0);
    hipEventCreate(&start);
    hipEventCreate(&stop);

    // Allocate host memory
    // TODO: Allocate enough pinned host memory for hA, hB, and hC
    //       to store N doubles each
    hipHostMalloc((void**)&hA,N*sizeof(double));
    hipHostMalloc((void**)&hB,N*sizeof(double));
    hipHostMalloc((void**)&hC,N*sizeof(double));

    // Initialize host memory
    for(int i = 0; i < N; ++i) {
        hA[i] = 1.0;
        hB[i] = 2.0;
    }

    // Decomposition of data for each stream
    dec[0].len   = N / 2;
    dec[0].start = 0;
    dec[1].len   = N - N / 2;
    dec[1].start = dec[0].len;

    // Allocate memory for the devices and per device streams
    for (int i = 0; i < 2; ++i) {
        // TODO: Allocate enough device memory for dA[i], dB[i], dC[i]
        //       to store dec[i].len doubles
        // TODO: Create a stream for each device
        // hipSetDevice(i);

        hipMalloc((void**)&(dA[i]),dec[i].len*sizeof(double));
        hipMalloc((void**)&(dB[i]),dec[i].len*sizeof(double));
        hipMalloc((void**)&(dC[i]),dec[i].len*sizeof(double));

        hipStreamCreate(&strm[i]);
    }

    // Start timing
    hipSetDevice(0);
    hipEventRecord(start);

    /* Copy each decomposed part of the vectors from host to device memory
       and execute a kernel for each part.
       Note: one needs to use streams and asynchronous calls! Without this
       the execution is serialized because the memory copies block the
       execution of the host process. */
    for (int i = 0; i < 2; ++i) {
        // TODO: Set active device
        // TODO: Copy data from host to device asynchronously (hA[dec[i].start] -> dA[i], hB[dec[i].start] -> dB[i])
        // TODO: Launch 'vector_add()' kernel to calculate dC = dA + dB
        // TODO: Copy data from device to host (dC[i] -> hC[dec[0].start])
        hipSetDevice(i);
        hipMemcpyAsync(&dA[i],&hA[dec[i].start],dec[i].len*sizeof(double),hipMemcpyHostToDevice,strm[i]);
        hipMemcpyAsync(&dC[i],&hB[dec[i].start],dec[i].len*sizeof(double),hipMemcpyHostToDevice,strm[i]);
        hipLaunchKernelGGL(vector_add,dim3(10),dim3(ThreadsInBlock),0,strm[i],dC[i],dA[i],dB[i],dec[i].len);
        hipMemcpyAsync(&hC[dec[0].start],&dC[i],dec[i].len*sizeof(double),hipMemcpyDeviceToHost,strm[i]);
    }

    // Synchronize and destroy the streams
    for (int i = 0; i < 2; ++i) {
        // TODO: Add synchronization calls and destroy streams
        hipStreamSynchronize(strm[i]);
        hipStreamDestroy(strm[i]);
    }

    // Stop timing
    // TODO: Add here the timing event stop calls
    hipEventRecord(stop);

    // Free device memory
    for (int i = 0; i < 2; ++i) {
        // TODO: Deallocate device memory
        hipFree(dA[i]);
        hipFree(dB[i]);
        hipFree(dC[i]);
    }

    // Check results
    int errorsum = 0;
    for (int i = 0; i < N; i++) {
        errorsum += hC[i] - 3.0;
    }
    printf("Error sum = %i\n", errorsum);

    // Calculate the elapsed time
    float gputime;
    hipSetDevice(0);
    hipEventElapsedTime(&gputime, start, stop);
    printf("Time elapsed: %f\n", gputime / 1000.);

    // Deallocate host memory
    hipHostFree((void*)hA);
    hipHostFree((void*)hB);
    hipHostFree((void*)hC);

    return 0;
}