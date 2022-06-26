#!/bin/bash
#SBATCH --job-name=training090_vc
#SBATCH --account=project_2000745
#SBATCH --partition=test
#SBATCH --time=00:05:00
#SBATCH --nodes=1
#SBATCH --ntasks=24
##SBATCH --reservation=summerschool

srun prog