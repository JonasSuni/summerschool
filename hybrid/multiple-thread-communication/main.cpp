#include <cstdio>
#include <omp.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int mpi_rank, ntasks;
    int provided, required=MPI_THREAD_MULTIPLE;

    MPI_Init_thread( &argc , &argv , required , &provided);
    MPI_Comm_rank( MPI_COMM_WORLD , &mpi_rank);
    MPI_Comm_size( MPI_COMM_WORLD , &ntasks);

    #pragma omp parallel shared(mpi_rank,ntasks)
    {
        int omp_rank = omp_get_thread_num();
        int recvbuf[ntasks] = {-1};
        int sendbuf[ntasks];
        for (int i=0;i<ntasks;i++) {
            sendbuf[i] = omp_rank;
        }

        if (mpi_rank == 0) {
            MPI_Send( &sendbuf , ntasks , MPI_INT , 1 , omp_rank , MPI_COMM_WORLD);
            MPI_Send( &sendbuf , ntasks , MPI_INT , 2 , omp_rank , MPI_COMM_WORLD);
        } else {
            MPI_Recv( &recvbuf , ntasks , MPI_INT , 0 , omp_rank , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
            printf("My MPI rank is %d, my OMP rank is %d, and I received data from OMP rank %d\n",mpi_rank,omp_rank,recvbuf[0]);
        }
    }

    MPI_Finalize();
    return 0;
}