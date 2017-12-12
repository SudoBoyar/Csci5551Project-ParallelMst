#!/bin/sh
#SBATCH --job-name=ProjectSequential
#SBATCH --mail-type=NONE
#SBATCH --nodes=3
#SBATCH --nodelist=node[5,6,7]
#SBATCH --output=sequential_results.out


#srun ~/csc5551/project/sequential 10 1
#srun ~/csc5551/project/sequential 100 1
#srun ~/csc5551/project/sequential 1000 1
#srun ~/csc5551/project/sequential 10000 1
#srun ~/csc5551/project/sequential 100000 1

srun ~/csc5551/project/sequential 192 1
srun ~/csc5551/project/sequential 960 1
srun ~/csc5551/project/sequential 1920 1
srun ~/csc5551/project/sequential 9600 1
srun ~/csc5551/project/sequential 19200 1
srun ~/csc5551/project/sequential 57600 1
