#!/bin/bash
#SBATCH --job-name=csc_gpu
#SBATCH -M ukko
#SBATCH --partition=gpu
#SBATCH --time=00:05:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --constraint=v100
#SBATCH --gres=gpu:1

module purge
module load GCC/10.2.0
module load CUDA/11.1.1-GCC-10.2.0
module load CMake/3.18.4-GCCcore-10.2.0
module use /proj/group/spacephysics/modules/hip
module load hip-4.5

srun ./main