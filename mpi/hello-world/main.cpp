//#include <stdio>
#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[])
{
    // TODO: say hello! in parallel
    //int rank,ntasks;

    MPI_Init(&argc,&argv);

    std::cout << "Hello world!\n";

    MPI_Finalize();
    return 0;

}
