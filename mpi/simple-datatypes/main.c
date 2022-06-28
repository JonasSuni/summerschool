#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank;
    int array[8][8];
    //TODO: Declare a variable storing the MPI datatype
    MPI_Datatype subtype;

    

    int i, j;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Initialize arrays
    if (rank == 0) {
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                array[i][j] = (i + 1) * 10 + j + 1;
            }
        }
    } else {
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                array[i][j] = 0;
            }
        }
    }

    if (rank == 0) {
        printf("Data in rank 0\n");
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                printf("%3d", array[i][j]);
            }
            printf("\n");
        }
    }

    //TODO: Create datatype 
    
    // const int displ[2] = {0,1};
    // const int sizes[2] = {8,8};
    // const int subsizes[2] = {8,1};
    // MPI_Type_create_subarray( 2 , sizes , subsizes , displ , MPI_ORDER_C , MPI_INT , &subtype);

    // const int blocklengths[4] = {1,2,3,4};
    // const int arrdispl[4] = {0,8*2+1,8*4+2,8*6+3};
    // MPI_Type_indexed( 4 , blocklengths , arrdispl , MPI_INT , &subtype);

    const int displ[2] = {2,2};
    const int sizes[2] = {8,8};
    const int subsizes[2] = {4,4};
    MPI_Type_create_subarray( 2 , sizes , subsizes , displ , MPI_ORDER_C , MPI_INT , &subtype);

    MPI_Type_commit( &subtype);

    //TODO: Send data

    if (rank == 0) {
        MPI_Send( &array , 1 , subtype , 1 , 1 , MPI_COMM_WORLD);
    } else if (rank == 1) {
        MPI_Recv( &array , 1 , subtype , 0 , 1 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
    }

    //TODO: Free datatype

    MPI_Type_free( &subtype);

    // Print out the result on rank 1
    if (rank == 1) {
        printf("Received data\n");
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                printf("%3d", array[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();

    return 0;
}
