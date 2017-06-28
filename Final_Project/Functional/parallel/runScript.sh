#!/bin/bash

#SBATCH --job-name=OMP_feed
#SBATCH --time=0-0:4
#SBATCH --partition=test
#SBATCH --output=trap2-%j.out



THREADS=2 #Starting threads
THREAD_MAX=64
n=1048576 #starts with 2^20
N_MAX=33554432 #Finishes with 2^24

while [[ $n -lt $N_MAX || $n -eq $N_MAX ]]; do
  THREADS=2
  echo "------------ FOR $n NEURONS ----------------"
  while [[ $THREADS -lt $THREAD_MAX || $THREADS -eq $THREAD_MAX ]]; do
    ./my_feed $n $THREADS
    let "THREADS*=2"
  done
  let "n*=2"
done
