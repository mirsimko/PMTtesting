#!/bin/bash

read -p 'supply voltage in Volts? ' voltage

for txtFile in linearityInputs/"$voltage"/*.txt; do
  root -l -b -q fitLinearity.C+'("'"$txtFile"'")'
done

# merge the output pdfs
gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile=linearity"$voltage"V.pdf linearityInputs/"$voltage"/*.pdf
