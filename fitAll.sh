#!/bin/bash

./comasToDots.sh

for txtFile in gainPerformance/*.txt; do
  root -l -b -q fitPmtYield.C+'("'"$txtFile"'")'
done