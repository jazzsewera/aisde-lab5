#!/bin/bash

mkdir -p out/pdf
rm -f out/*.jsonl

SAMPLES_VERTS_MST="50 100 200"
EDGE_MST=220

for vert in $(echo $SAMPLES_VERTS_MST); do
  ./graphgen $vert $EDGE_MST > /dev/null
  ./mst graf.txt >> out/mst.jsonl
done
python3 plot.py -m
