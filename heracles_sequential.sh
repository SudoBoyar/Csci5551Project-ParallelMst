#!/bin/sh
#SBATCH --job-name=ProjectSequential
#SBATCH --mail-type=NONE
#SBATCH --nodes=3
#SBATCH --nodelist=node[5,6,7]
#SBATCH --output=sequential_results.out


#srun sequential 10 1
#srun sequential 100 1
#srun sequential 1000 1
#srun sequential 10000 1
#srun sequential 100000 1

srun sequential 192 1
srun sequential 960 1
srun sequential 1920 1
srun sequential 9600 1
srun sequential 19200 1
srun sequential 57600 1
