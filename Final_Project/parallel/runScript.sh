#!/bin/bash

#SBATCH --job-name=OMP_feed
#SBATCH --time=0-0:4
#SBATCH --partition=test
#SBATCH --output=trap2-%j.out


n=1048576 #starts with 2^20
thread_count=2 #Starting threads
N_MAX=33554432 #Finishes with 2^24

while [[ $n -lt $N_MAX || $n -eq $N_MAX ]]; do
  thread_count=2
  echo "------------ FOR $n NEURONS ----------------"
  while [[ $thread_count -lt n || $thread_count -eq $n ]]; do
    ./my_feed $n $thread_count
    let "thread_count*=2"
  done
  let "n*=2"
done
