#!/bin/bash
# Dijkstra and Floyd

mkdir -p out/pdf
rm -f out/*.jsonl

SAMPLES_VERTS_MINPATH="100 125 150 175 200 225 250 275 300 325 350 375 400 425 450 475"
EDGE_MINPATH=5000

for vert in $(echo $SAMPLES_VERTS_MINPATH); do
  ./digraphgen $vert $EDGE_MINPATH > /dev/null
  ./minpath_raw.sh digraf.txt >> out/minpath.jsonl
done
python3 plot.py -p
