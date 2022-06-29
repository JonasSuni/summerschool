#!/bin/bash
#SBATCH --job-name=training090_mpi
#SBATCH --account=project_2000745
#SBATCH --partition=large
#SBATCH --time=00:05:00
##SBATCH --nodes=1
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --reservation=summerschool

srun ./writer