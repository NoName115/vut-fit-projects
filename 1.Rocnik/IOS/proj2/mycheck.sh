#!/bin/bash

# Author: Róbert Kolcún - FIT
# <xkolcu00@stud.fit.vutbr.cz>
# Login: xkolcu00
# Program pre automaticke spustanie testov

./proj2 4 2 0 0
vystup=$( ./check-syntax.sh proj2.out)
pocet=$( ./check-syntax.sh proj2.out | wc -l )
if test $pocet -eq 2
then
	echo Spravne
else
	echo Chyba:
	echo $vystup
fi
