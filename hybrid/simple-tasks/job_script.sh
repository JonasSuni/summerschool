#!/bin/bash
#SBATCH --job-name=training090_mpi
#SBATCH --account=project_2000745
#SBATCH --partition=large
#SBATCH --time=00:05:00
#SBATCH --ntasks=3
#SBATCH --cpus-per-task=4
#SBATCH --reservation=summerschool

srun ./main