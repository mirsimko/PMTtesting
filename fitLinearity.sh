#!/bin/bash

read -p 'supply voltage in Volts? ' voltage

directory=linearityInputsWoLED

for txtFile in "$directory"/"$voltage"/*.txt; do
  root -l -b -q fitLinearity.C+'("'"$txtFile"'")'
done

# merge the output pdfs
gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile=linearityWoLed"$voltage"V.pdf "$directory"/"$voltage"/*.pdf
