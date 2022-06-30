#include <cstdio>
#include <omp.h>

int main(int argc, int * argv[])
{
    int mpi_rank,omp_rank, ntasks;
    int provided, required=MPI_THREAD_MULTIPLE;

    MPI_Init_thread( &argc , &argv , required , &provided);
    MPI_Comm_rank( MPI_COMM_WORLD , &rank);
    MPI_Comm_size( MPI_COMM_WORLD , &ntasks);

    #pragma omp parallel private(omp_rank)
    {
        omp_rank = omp_get_thread_num();
        int recvbuf[ntasks] = {-1};
        int sendbuf[ntasks] = {omp_rank};

        if (rank == 0) {
            MPI_Send( &sendbuf , ntasks , MPI_INT , 1 , omp_rank , MPI_COMM_WORLD);
            MPI_Send( &sendbuf , ntasks , MPI_INT , 2 , omp_rank , MPI_COMM_WORLD);
        } else {
            MPI_Recv( &recvbuf , ntasks , MPI_INT , 0 , omp_rank , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
            printf("My MPI rank is %d, my OMP rank is %d, and I received data from OMP rank %d\n",rank,omp_rank,recvbuf[0]);
        }
    }

    MPI_Finalize();
    return 0;
}