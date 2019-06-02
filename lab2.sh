#!/bin/bash

mkdir -p out/pdf
rm -f out/*.jsonl

SAMPLES_VERTS_MINPATH="15 20 25"
EDGE_MINPATH=25

for vert in $(echo $SAMPLES_VERTS_MINPATH); do
  ./digraphgen $vert $EDGE_MINPATH > /dev/null
  ./minpath_raw.sh digraf.txt >> out/minpath.jsonl
done
python3 plot.py -p
