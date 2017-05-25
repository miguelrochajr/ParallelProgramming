#!/bin/bash

#SBATCH --job-name=mpi_trap2
#SBATCH --time=0-0:4
#SBATCH --partition=test
#SBATCH --output=trap2-%j.out

srun -n 2 mpi_trap2
srun -n 4 mpi_trap2
srun -n 8 mpi_trap2
srun -n 16 mpi_trap2
srun -n 32 mpi_trap2
