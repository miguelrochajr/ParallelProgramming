#!/bin/bash

n=32 #starts with 32
thread_count=2 # minomum threads number
a=2
b=10
N_MAX=8192

echo "The excpected value is around 330.67"

while [[ $n -lt $N_MAX || $n -eq $N_MAX ]]; do
  thread_count=2
  echo "------------ FOR n=$n ----------------"
  while [[ $thread_count -lt n || $thread_count -eq $n ]]; do
    ./omp_trap1 $thread_count $a $b $n
    let "thread_count*=2"
  done
  let "n*=2"
done
