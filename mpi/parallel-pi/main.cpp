#include <cstdio>
#include <cmath>
#include <mpi.h>

constexpr int n = 840;

int main(int argc, char** argv)
{
  
  int ntasks,rank;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&ntasks);
  double sendbuf,recvbuf;

  if (rank == 0) {
    printf("Computing approximation to pi with N=%d\n", n);
  }

  int istart = rank*n/ntasks+1;
  int istop = rank*n/ntasks+n/ntasks;

  double pi = 0.0;
  for (int i=istart; i <= istop; i++) {
    double x = (i - 0.5) / n;
    pi += 1.0 / (1.0 + x*x);
  }

  if (rank == 0) {
    for (int i=1;i<ntasks;i++) {
      MPI_Recv(&recvbuf,1,MPI_DOUBLE,MPI_ANY_SOURCE,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      pi += recvbuf;
    }
    
    pi *= 4.0 / n;
    printf("Approximate pi=%18.16f (exact pi=%10.8f)\n", pi, M_PI);
  } else {
    sendbuf=pi;
    MPI_Send(&sendbuf,1,MPI_DOUBLE,0,1,MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;

}
   

