#include <cstdio>
#include <cmath>

constexpr int n = 840;

int main(int argc, char** argv)
{
  
  int ntasks,rank;

  printf("Computing approximation to pi with N=%d\n", n);

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&ntasks);
  double sendbuf,recvbuf;

  int istart = rank*n/2+1;
  int istop = rank*n/2+n/2;

  double pi = 0.0;
  for (int i=istart; i <= istop; i++) {
    double x = (i - 0.5) / n;
    pi += 1.0 / (1.0 + x*x);
  }

  if (rank == 0) {
    MPI_Recv(&recvbuf,1,MPI_DOUBLE,1,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE)
    pi += recvbuf;
    pi *= 4.0 / n;
    printf("Approximate pi=%18.16f (exact pi=%10.8f)\n", pi, M_PI);
  } else {
    sendbuf=pi;
    MPI_Send(&sendbuf,MPI_DOUBLE,0,1,MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;

}
   

