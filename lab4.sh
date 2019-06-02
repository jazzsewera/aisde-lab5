#!/bin/bash

mkdir -p out/pdf
rm -f out/*.jsonl

SAMPLES_EDGES_MINPATH="200 250 300 350 400 450 500 550"
VERT_MINPATH=200

for edge in $(echo $SAMPLES_EDGES_MINPATH); do
  ./digraphgen $VERT_MINPATH $edge > /dev/null
  ./minpath_raw.sh digraf.txt >> out/minpath.jsonl
done
python3 plot.py -p
