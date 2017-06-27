#!/bin/bash

#!/bin/bash
#SBATCH --job-name=OMP_feed
#SBATCH --time=0-0:4
#SBATCH --partition=test
#SBATCH --output=trap2-%j.out

./my_feed 128 1
