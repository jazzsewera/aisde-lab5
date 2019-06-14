#!/bin/bash
# Boruvka and Kruskal

mkdir -p out/pdf
rm -f out/*.jsonl

SAMPLES_EDGES_MST="4500 8500 16500 33000 65000 138000 266000 532000 1124000"
VERT_MST=2500

for edge in $(echo $SAMPLES_EDGES_MST); do
  ./graphgen $VERT_MST $edge > /dev/null
  ./mst graf.txt >> out/mst.jsonl
done
python3 plot.py -m
