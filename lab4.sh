#!/bin/bash

mkdir -p out/pdf
rm -f out/*.jsonl

SAMPLES_EDGES_MINPATH="8400 16800 24300 32900 44000 50000 56300 65000 78000 83000 98000"
VERT_MINPATH=450

for edge in $(echo $SAMPLES_EDGES_MINPATH); do
  ./digraphgen $VERT_MINPATH $edge > /dev/null
  ./minpath_raw.sh digraf.txt >> out/minpath.jsonl
done
python3 plot.py -p
