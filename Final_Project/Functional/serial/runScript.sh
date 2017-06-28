#!/bin/bash

#SBATCH --job-name=OMP_feed
#SBATCH --time=0-0:4
#SBATCH --partition=test
#SBATCH --output=trap2-%j.out

N=1048576 #starts with 2^20
N_MAX=33554432 #Finishes with 2^24

while [[ $N -lt $N_MAX || $N -eq $N_MAX ]]; do
  ./serial_feed $N
  let "N*=2"
done
