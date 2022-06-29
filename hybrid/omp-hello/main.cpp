#include <cstdio>
#include <omp.h>

int main()
{
    printf("Hello world!\n");
    

#pragma omp parallel
    {
        printf("X\n");
        printf("Num threads: %d\n",omp_get_num_threads());
        printf("Thread number: %d\n",omp_get_thread_num());
    }
    return 0;
}
