#!/bin/bash
#SBATCH --job-name=training090_mpi
#SBATCH --account=project_2000745
#SBATCH --partition=large
#SBATCH --time=00:05:00
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=16
#SBATCH --reservation=summerschool

export OMP_NUM_THREADS=16

srun ./main