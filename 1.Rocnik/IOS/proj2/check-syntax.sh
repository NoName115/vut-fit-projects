#!/bin/bash

# testuje spravnou sekvenci cislovani akci a format vystupu (pouze syntax)
# bez zaruky, muze obsahovat chyby

outf=$1

tr -d " \t" < ${outf} > $$

# test cislovani akci
# tiskne radky s chybnym cislovanim
cat $$ | awk -F":" ' { c++; if (c != $1) print NR, " => ", $1, " : chyba v cislovani akci"; } '

declare -a lines
lines[0]='^[1-9][0-9]*:C[1-9][0-9]*:started$'
lines[1]='^[1-9][0-9]*:C[1-9][0-9]*:load$'
lines[2]='^[1-9][0-9]*:C[1-9][0-9]*:run$'
lines[3]='^[1-9][0-9]*:C[1-9][0-9]*:unload$'
lines[4]='^[1-9][0-9]*:C[1-9][0-9]*:finished$'
lines[5]='^[1-9][0-9]*:P[1-9][0-9]*:started$'
lines[6]='^[1-9][0-9]*:P[1-9][0-9]*:board$'
lines[7]='^[1-9][0-9]*:P[1-9][0-9]*:boardorder[1-9][0-9]*$'
lines[8]='^[1-9][0-9]*:P[1-9][0-9]*:boardlast$'
lines[9]='^[1-9][0-9]*:P[1-9][0-9]*:unboard$'
lines[10]='^[1-9][0-9]*:P[1-9][0-9]*:unboardorder[0-9]*$'
lines[11]='^[1-9][0-9]*:P[1-9][0-9]*:unboardlast$'
lines[12]='^[1-9][0-9]*:P[1-9][0-9]*:finished$'

# kontrola sytaxe vystupu
# vytiskne radky, ktere neodpovidaji formatu vytupu
echo "=== radky, ktere neodpovidaji formatu vystupu"

for i in `seq 0 12`
do
    echo "/${lines[$i]}/d" >> $$-sed
done

sed -f $$-sed $$

# kontrola chybejicich vystupu
# tiskne informaci, ktery text ve vystupu chybi
echo "=== chybejici vystupy"

for i in `seq 0 12`
do
    cat $$ | grep "${lines[$i]}" >/dev/null || echo "${lines[$i]}"
done


rm $$*
