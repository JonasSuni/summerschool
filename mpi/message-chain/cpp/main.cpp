#include <cstdio>
#include <vector>
#include <mpi.h>

void print_ordered(double t);

int main(int argc, char *argv[])
{
    int i, myid, ntasks,nrecv;
    constexpr int size = 10000000;
    std::vector<int> message(size);
    std::vector<int> receiveBuffer(size);
    MPI_Status status;
    MPI_Comm comm1d;
    int dims[1];
    int period[1] = {0};

    double t0, t1;

    int source, destination;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    // Initialize message
    for (i = 0; i < size; i++) {
        message[i] = myid;
    }

    // MPI_Dims_create( ntasks , 2 , dims);
    // dims[0] = 4;
    // dims[1] = 4;
    dims[0] = ntasks;
    MPI_Cart_create( MPI_COMM_WORLD , 1 , ntasks , dims , 0 , &comm1d);

    // TODO: set source and destination ranks 
    // Treat boundaries with MPI_PROC_NULL

        destination = myid + 1;
        source = myid - 1;
        MPI_Cart_shift( comm1d , 1 , 1 , &source , &destination);


        // if (myid == 0) {
        //     source = MPI_PROC_NULL;
        // } else if (myid == ntasks-1) {
        //     destination = MPI_PROC_NULL;
        // }

    // end TODO

    // Start measuring the time spent in communication
    MPI_Barrier(MPI_COMM_WORLD);
    t0 = MPI_Wtime();

    // TODO: Send messages 

    MPI_Sendrecv( message.data() , size , MPI_INT , destination , destination , receiveBuffer.data() , size , MPI_INT , source , myid , comm2d , &status);

    //MPI_Send(message.data(),size,MPI_INT,destination,myid+1,MPI_COMM_WORLD);
    if (true) {
        printf("Sender: %d. Sent elements: %d. Tag: %d. Receiver: %d\n",
           myid, size, myid + 1, destination);
    }
    

    // TODO: Receive messages

    //MPI_Recv(receiveBuffer.data(),size,MPI_INT,source,myid,MPI_COMM_WORLD,&status);
    if (true) {
        MPI_Get_count( &status , MPI_INT , &nrecv);
        printf("Receiver: %d. first element %d. amount received %d\n",
           myid, receiveBuffer[0], nrecv);
    }

    // Finalize measuring the time and print it out
    t1 = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    fflush(stdout);

    print_ordered(t1 - t0);

    MPI_Finalize();
    return 0;
}

void print_ordered(double t)
{
    int i, rank, ntasks;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    if (rank == 0) {
        printf("Time elapsed in rank %2d: %6.3f\n", rank, t);
        for (i = 1; i < ntasks; i++) {
            MPI_Recv(&t, 1, MPI_DOUBLE, i, 11, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Time elapsed in rank %2d: %6.3f\n", i, t);
        }
    } else {
        MPI_Send(&t, 1, MPI_DOUBLE, 0, 11, MPI_COMM_WORLD);
    }
}
