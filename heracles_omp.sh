#!/bin/sh
#SBATCH --job-name=ProjectSequential
#SBATCH --mail-type=NONE
#SBATCH --nodes=3
#SBATCH --nodelist=node[5-14]
#SBATCH --output=omp.out


srun ~/csc5551/project/omp 48 1
srun ~/csc5551/project/omp 480 1
srun ~/csc5551/project/omp 4800 1
srun ~/csc5551/project/omp 48000 1

