#!/bin/bash

mkdir -p out/pdf
rm -f out/*.jsonl

SAMPLES_EDGES_MST="100 200 500 1000 1300 1700 2000"
VERT_MST=50

for edge in $(echo $SAMPLES_EDGES_MST); do
  ./graphgen $VERT_MST $edge > /dev/null
  ./mst graf.txt >> out/mst.jsonl
done
python3 plot.py -m
