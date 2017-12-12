#!/bin/sh
#SBATCH --job-name=ProjectHybrid
#SBATCH --mail-type=NONE
#SBATCH --nodes=4
#SBATCH --nodelist=node[13,14,15,16]
#SBATCH --ntasks=4
#SBATCH --ntasks-per-node=1
#SBATCH --output=mpi_results.out

srun --mpi=pmi2 ~/csc5551/project/mpi 192 1
srun --mpi=pmi2 ~/csc5551/project/mpi 192 1
srun --mpi=pmi2 ~/csc5551/project/mpi 192 1

srun --mpi=pmi2 ~/csc5551/project/mpi 960 1
srun --mpi=pmi2 ~/csc5551/project/mpi 960 1
srun --mpi=pmi2 ~/csc5551/project/mpi 960 1

srun --mpi=pmi2 ~/csc5551/project/mpi 1920 1
srun --mpi=pmi2 ~/csc5551/project/mpi 1920 1
srun --mpi=pmi2 ~/csc5551/project/mpi 1920 1

srun --mpi=pmi2 ~/csc5551/project/mpi 9600 1
srun --mpi=pmi2 ~/csc5551/project/mpi 9600 1
srun --mpi=pmi2 ~/csc5551/project/mpi 9600 1

srun --mpi=pmi2 ~/csc5551/project/mpi 19200 1
srun --mpi=pmi2 ~/csc5551/project/mpi 19200 1
srun --mpi=pmi2 ~/csc5551/project/mpi 19200 1

srun --mpi=pmi2 ~/csc5551/project/mpi 57600 1
srun --mpi=pmi2 ~/csc5551/project/mpi 57600 1
srun --mpi=pmi2 ~/csc5551/project/mpi 57600 1
