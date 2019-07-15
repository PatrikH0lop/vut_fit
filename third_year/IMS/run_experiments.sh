#!/bin/sh

# Prave hodnoty
./logistika 0 4 2 8 10 10 10 10 10
# Experimenty
# Pristavani 2 vykladacich plosin
./logistika 1 4 4 8 10 10 10 10 10
# Zvyseni poctu vozu
./logistika 2 4 2 8 12 12 12 12 12
# Znizeni vykladaciho prostoru a zabrani casti parkoviste
./logistika 3_0 4 1 5 10 10 10 10 10
./logistika 3_1 4 1 6 10 10 10 10 10
./logistika 3_2 4 1 7 10 10 10 10 10
# Aplikace vsech zmen z predoslych experimentu
./logistika 4_0 4 1 7 12 12 12 12 12
./logistika 4_1 4 2 7 12 12 12 12 12
