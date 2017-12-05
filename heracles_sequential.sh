#!/bin/sh
#SBATCH --job-name=ProjectSequential
#SBATCH --mail-type=NONE
#SBATCH --nodes=3
#SBATCH --nodelist=node[5-14]
#SBATCH --output=sequential.out


srun ~/csc5551/project/sequential 10 1
srun ~/csc5551/project/sequential 100 1
srun ~/csc5551/project/sequential 1000 1
srun ~/csc5551/project/sequential 10000 1

