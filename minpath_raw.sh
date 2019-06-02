#!/bin/bash
echo -ne "[ "
echo -e $(./minpath $@  | \
  sed -re "s/$/\\\n/g") | \
sed -re "/^\s*$/d"        \
    -e "s/^ *//g"       | \
grep -Eo "[0-9]+"       | \
xargs                   | \
sed -e "s/ /, /g"       | \
tr -d '\n'
echo -e " ]"
