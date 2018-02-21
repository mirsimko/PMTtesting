#!/bin/bash

./comasToDots.sh

for txtFile in gainPerformance/*.txt; do
  root fitPmtYield.C+'("'"$txtFile"'")'
done
