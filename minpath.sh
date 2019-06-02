#!/bin/bash
echo -e $(./minpath $@                                  | \
  sed -e "s/Algorytm//g"                                  \
      -re "s/[0-9]+/\\\e[96m\0\\\e[39m/g"                 \
      -re "s/$/\\\n/g"                                    \
      -re "s/Dijkstry|Floyda|DST/\\\e[32m\0\\\e[39m/g") | \
sed -re "/^\s*$/d"                                        \
    -e "s/^ *//g"
