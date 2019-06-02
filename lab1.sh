#!/bin/bash

mkdir -p out/pdf
rm -f out/*.jsonl

SAMPLES_VERTS_MST="1500 2500 5000 10000 25000 50000 100000 150000"
EDGE_MST=1000000

for vert in $(echo $SAMPLES_VERTS_MST); do
  ./graphgen $vert $EDGE_MST > /dev/null
  ./mst graf.txt >> out/mst.jsonl
done
python3 plot.py -m
