#!/bin/bash

read -p 'supply voltage in Volts? ' voltage

for txtFile in linearityInputs/$voltage/*.txt; do
  root -l -b -q fitLinearity.C+'("'"$txtFile"'")'
done
