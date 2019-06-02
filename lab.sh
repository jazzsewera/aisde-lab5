#!/bin/bash

mkdir -p out/pdf
rm -f out/*.jsonl

SAMPLES_VERTS_MST="50 100 200"
EDGE_MST=220

SAMPLES_VERTS_MINPATH="15 20 25"
EDGE_MINPATH=25

for vert in $(echo $SAMPLES_VERTS_MST); do
  ./graphgen $vert $EDGE_MST > /dev/null
  ./mst graf.txt >> out/mst.jsonl
done
python3 plot.py -m

for vert in $(echo $SAMPLES_VERTS_MINPATH); do
  ./digraphgen $vert $EDGE_MINPATH > /dev/null
  ./minpath_raw.sh digraf.txt >> out/minpath.jsonl
done
python3 plot.py -p


rm -f out/*.jsonl
SAMPLES_EDGES_MST="100 200 500 1000 1300 1700 2000"
VERT_MST=50

SAMPLES_EDGES_MINPATH="25 50 100 170 200 300 350"
VERT_MINPATH=20

for edge in $(echo $SAMPLES_EDGES_MST); do
  ./graphgen $VERT_MST $edge > /dev/null
  ./mst graf.txt >> out/mst.jsonl
done
python3 plot.py -m

for edge in $(echo $SAMPLES_EDGES_MINPATH); do
  ./digraphgen $VERT_MINPATH $edge > /dev/null
  ./minpath_raw.sh digraf.txt >> out/minpath.jsonl
done
python3 plot.py -p
