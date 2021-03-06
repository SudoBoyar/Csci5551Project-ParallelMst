#!/bin/sh
#SBATCH --job-name=ProjectHybrid
#SBATCH --mail-type=NONE
#SBATCH --nodes=4
#SBATCH --nodelist=node[13,14,15,16]
#SBATCH --ntasks=4
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=48
#SBATCH --output=hybrid_results.out

srun --mpi=pmi2 hybrid 192 1
srun --mpi=pmi2 hybrid 192 1
srun --mpi=pmi2 hybrid 192 1

srun --mpi=pmi2 hybrid 960 1
srun --mpi=pmi2 hybrid 960 1
srun --mpi=pmi2 hybrid 960 1

srun --mpi=pmi2 hybrid 1920 1
srun --mpi=pmi2 hybrid 1920 1
srun --mpi=pmi2 hybrid 1920 1

srun --mpi=pmi2 hybrid 9600 1
srun --mpi=pmi2 hybrid 9600 1
srun --mpi=pmi2 hybrid 9600 1

srun --mpi=pmi2 hybrid 19200 1
srun --mpi=pmi2 hybrid 19200 1
srun --mpi=pmi2 hybrid 19200 1

srun --mpi=pmi2 hybrid 57600 1
srun --mpi=pmi2 hybrid 57600 1
srun --mpi=pmi2 hybrid 57600 1
