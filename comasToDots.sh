#!/bin/bash

workDir=${1:-gainPerformance}

for txtFile in "$workDir"/*.txt; do 
  sed -i 's/,/./g' "$txtFile"
done
