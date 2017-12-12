#!/bin/sh
#SBATCH --job-name=ProjectSequential
#SBATCH --mail-type=NONE
#SBATCH --nodes=3
#SBATCH --nodelist=node[8,9,10]
#SBATCH --cpus-per-task=48
#SBATCH --output=omp_results.out


#srun ~/csc5551/project/omp 48 1
#srun ~/csc5551/project/omp 240 1
#srun ~/csc5551/project/omp 480 1
#srun ~/csc5551/project/omp 2400 1
#srun ~/csc5551/project/omp 4800 1
#srun ~/csc5551/project/omp 24000 1
#srun ~/csc5551/project/omp 48000 1

srun ~/csc5551/project/omp 192 1
srun ~/csc5551/project/omp 960 1
srun ~/csc5551/project/omp 1920 1
srun ~/csc5551/project/omp 9600 1
srun ~/csc5551/project/omp 19200 1
srun ~/csc5551/project/omp 57600 1

