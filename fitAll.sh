#!/bin/bash

./comasToDots.sh

for txtFile in gainPerformance/*.txt; do
  root -l -b -q fitPmtYield.C+'("'"$txtFile"'")'
done

# merge the output pdfs
gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile=gainFit.pdf gainPerformance/*.pdf
