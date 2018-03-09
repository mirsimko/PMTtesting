#!/bin/bash

read -p 'supply voltage in Volts? ' voltage

directory=linearityInputsCorrectVoltage

./comasToDots.sh "$directory"/"$voltage"

for txtFile in "$directory"/"$voltage"/*.txt; do
  root -l -b -q fitLinearity.C+'("'"$txtFile"'")'
done

# merge the output pdfs
gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile=linearityCorrectVoltage"$voltage"V.pdf "$directory"/"$voltage"/*.pdf
