#!/bin/bash
#SBATCH --job-name=training090_mpi
#SBATCH --account=project_2000745
#SBATCH --partition=large
#SBATCH --time=00:05:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=4
#SBATCH --reservation=summerschool

srun ./main