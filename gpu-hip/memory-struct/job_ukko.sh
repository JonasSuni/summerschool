#!/bin/bash
#SBATCH --job-name=csc_gpu
#SBATCH -M ukko
#SBATCH --partition=gpu-short
#SBATCH --time=00:05:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --constraint=v100
#SBATCH --gres=gpu:1

srun ./main