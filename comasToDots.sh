#!/bin/bash

for txtFile in gainPerformance/*.txt; do 
  sed -i 's/,/./g' "$txtFile"
done
