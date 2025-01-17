#!/bin/bash
#SBATCH --job-name=training090_gpu
#SBATCH --account=project_2000745
#SBATCH --partition=gpu
#SBATCH --time=00:05:00
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --gres=gpu:v100:1
#SBATCH --reservation=summerschool-gpu

nvidia_smi

srun ./main