#!/bin/bash

#!/bin/bash
#SBATCH --job-name=OMP_feed
#SBATCH --time=0-0:4
#SBATCH --partition=test
#SBATCH --output=trap2-%j.out

./feed 1048576
./feed 2097152
./feed 4194304
./feed 8388608
./feed 16777216
./feed 33554432
