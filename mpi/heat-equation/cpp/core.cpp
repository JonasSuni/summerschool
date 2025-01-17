// Main solver routines for heat equation solver

#include <mpi.h>

#include "heat.hpp"

// Exchange the boundary values
void exchange(Field& field, const ParallelData parallel)
{

    double* sbuf = field.temperature.data(1,0);
    double* rbuf = field.temperature.data(field.nx + 1,0);
    // TODO start: implement halo exchange  

    // You can utilize the data() method of the Matrix class to obtain pointer
    // to element, e.g. field.temperature.data(i, j) 

    // Send to up, receive from down

    // MPI_Send( sbuf , field.ny + 2 , MPI_DOUBLE , parallel.nup , parallel.nup , MPI_COMM_WORLD);
    // MPI_Recv( rbuf , field.ny + 2 , MPI_DOUBLE , parallel.ndown , parallel.rank , MPI_COMM_WORLD , MPI_STATUS_IGNORE);

    MPI_Sendrecv( sbuf , field.ny + 2 , MPI_DOUBLE , parallel.nup , parallel.nup , rbuf , field.ny + 2 , MPI_DOUBLE , parallel.ndown , parallel.rank , parallel.communicator , MPI_STATUS_IGNORE);

    // Send to down, receive from up

    sbuf = field.temperature.data(field.nx,0);
    rbuf = field.temperature.data();

    // MPI_Recv( rbuf , field.ny + 2 , MPI_DOUBLE , parallel.nup , parallel.rank , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
    // MPI_Send( sbuf , field.ny + 2 , MPI_DOUBLE , parallel.ndown , parallel.ndown , MPI_COMM_WORLD);

    MPI_Sendrecv( sbuf , field.ny + 2 , MPI_DOUBLE , parallel.ndown , parallel.ndown , rbuf , field.ny + 2 , MPI_DOUBLE , parallel.nup , parallel.rank , parallel.communicator , MPI_STATUS_IGNORE);

    // TODO end
}

void exchange_init(Field& field, ParallelData& parallel) 
{
    double* sbuf = field.temperature.data(1,0);
    double* rbuf = field.temperature.data(field.nx + 1,0);

    MPI_Isend( sbuf , field.ny + 2 , MPI_DOUBLE , parallel.nup , parallel.nup , parallel.communicator , &parallel.request[0]);
    MPI_Irecv( rbuf , field.ny + 2 , MPI_DOUBLE , parallel.ndown , parallel.rank , parallel.communicator , &parallel.request[1]);

    sbuf = field.temperature.data(field.nx,0);
    rbuf = field.temperature.data();

    MPI_Isend( sbuf , field.ny + 2 , MPI_DOUBLE , parallel.ndown , parallel.ndown , parallel.communicator , &parallel.request[2]);
    MPI_Irecv( rbuf , field.ny + 2 , MPI_DOUBLE , parallel.nup , parallel.rank , parallel.communicator , &parallel.request[3]);
}

void exchange_fin(ParallelData& parallel)
{
    MPI_Waitall( 4 , parallel.request , parallel.status);
}

// Update the temperature values using five-point stencil */
void evolve(Field& curr, const Field& prev, const double a, const double dt)
{

  // Compilers do not necessarily optimize division to multiplication, so make it explicit
  auto inv_dx2 = 1.0 / (prev.dx * prev.dx);
  auto inv_dy2 = 1.0 / (prev.dy * prev.dy);

  // Determine the temperature field at next time step
  // As we have fixed boundary conditions, the outermost gridpoints
  // are not updated.
  for (int i = 1; i < curr.nx + 1; i++) {
    for (int j = 1; j < curr.ny + 1; j++) {
            curr(i, j) = prev(i, j) + a * dt * (
	       ( prev(i + 1, j) - 2.0 * prev(i, j) + prev(i - 1, j) ) * inv_dx2 +
	       ( prev(i, j + 1) - 2.0 * prev(i, j) + prev(i, j - 1) ) * inv_dy2
               );
    }
  }

}

void evolve_inner(Field& curr, const Field& prev, const double a, const double dt)
{

    auto inv_dx2 = 1.0 / (prev.dx * prev.dx);
    auto inv_dy2 = 1.0 / (prev.dy * prev.dy);

    for (int i = 2; i < curr.nx; i++) {
      for (int j = 1; j < curr.ny + 1; j++) {
            curr(i, j) = prev(i, j) + a * dt * (
	       ( prev(i + 1, j) - 2.0 * prev(i, j) + prev(i - 1, j) ) * inv_dx2 +
	       ( prev(i, j + 1) - 2.0 * prev(i, j) + prev(i, j - 1) ) * inv_dy2
               );
      }
    }
}

void evolve_edge(Field& curr, const Field& prev, const double a, const double dt)
{

    auto inv_dx2 = 1.0 / (prev.dx * prev.dx);
    auto inv_dy2 = 1.0 / (prev.dy * prev.dy);

    int i = 1;
    for (int j = 1; j < curr.ny + 1; j++) {
          curr(i, j) = prev(i, j) + a * dt * (
        ( prev(i + 1, j) - 2.0 * prev(i, j) + prev(i - 1, j) ) * inv_dx2 +
        ( prev(i, j + 1) - 2.0 * prev(i, j) + prev(i, j - 1) ) * inv_dy2
              );
    }

    i = curr.nx;
    for (int j = 1; j < curr.ny + 1; j++) {
          curr(i, j) = prev(i, j) + a * dt * (
        ( prev(i + 1, j) - 2.0 * prev(i, j) + prev(i - 1, j) ) * inv_dx2 +
        ( prev(i, j + 1) - 2.0 * prev(i, j) + prev(i, j - 1) ) * inv_dy2
              );
    }
}


