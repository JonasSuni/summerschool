//#include <stdio>
#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[])
{
    // TODO: say hello! in parallel
    int rank,ntasks;
    char name;
    int namelen;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if (rank==0) {
        std::cout << "Hello world from rank " << rank << ", total processes = " << ntasks << "\n";
    } else {
        std::cout << "Hello world from rank " << rank << "\n";
    }

    MPI_Get_processor_name(&name,&namelen);

    std::cout << "Processor name: " << name << "\n";

    MPI_Finalize();
    return 0;

}
