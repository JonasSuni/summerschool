#!/bin/bash
#SBATCH --job-name=090_mpi
#SBATCH --account=project_2000745
#SBATCH --partition=large
#SBATCH --reservation=summerschool
#SBATCH --time=00:05:00
#SBATCH --ntasks=4

srun ./main
