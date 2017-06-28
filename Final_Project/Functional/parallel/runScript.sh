#!/bin/bash

#SBATCH --job-name=OMP_feed
#SBATCH --time=0-0:4
#SBATCH --partition=test
#SBATCH --output=trap2-%j.out



THREADS=2 #Starting threads
THREAD_MAX=64
n=60000000 #starts with 60*10^6
N_MAX=120000000  #Finishes with 120*10^6

while [[ $n -lt $N_MAX || $n -eq $N_MAX ]]; do
  THREADS=2
  echo "------------ FOR $n NEURONS ----------------"
  while [[ $THREADS -lt $THREAD_MAX || $THREADS -eq $THREAD_MAX ]]; do
    ./parallel_feed $n $THREADS
    let "THREADS*=2"
  done
  let "n+=10000000"
done
