#!/usr/bin/env bash

# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# IPP - syn - doplňkové testy - 2016/2017
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# Činnost:
# - vytvoří výstupy studentovy úlohy v daném interpretu na základě sady testů
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# Popis (README):
#
# Struktura skriptu _stud_tests.sh (v kódování UTF-8):
# Každý test zahrnuje až 4 soubory (vstupní soubor, případný druhý vstupní
# soubor, výstupní soubor, soubor logující chybové výstupy *.err vypisované na
# standardní chybový výstup (pro ilustraci) a soubor logující návratový kód
# skriptu *.rc). Pro spuštění testů je nutné do stejného adresáře zkopírovat i
# váš skript. V komentářích u jednotlivých testů jsou uvedeny dodatečné
# informace jako očekávaný návratový kód.
#
# Proměnné ve skriptu _stud_tests.sh pro konfiguraci testů:
#  INTERPRETER - využívaný interpret
#  EXTENSION - přípona souboru s vaším skriptem (jméno skriptu je dáno úlohou)
#  LOCAL_IN_PATH/LOCAL_OUT_PATH - testování různých cest ke vstupním/výstupním
#    souborům
#
# Další soubory archivu s doplňujícími testy:
# V adresáři ref-out najdete referenční soubory pro výstup (*.out nebo *.xml),
# referenční soubory s návratovým kódem (*.rc) a pro ukázku i soubory s
# logovaným výstupem ze standardního chybového výstupu (*.err). Pokud některé
# testy nevypisují nic na standardní výstup nebo na standardní chybový výstup,
# tak může odpovídající soubor v adresáři chybět nebo mít nulovou velikost.
#
# Doporučení a poznámky k testování:
# Tento skript neobsahuje mechanismy pro automatické porovnávání výsledků vašeho
# skriptu a výsledků referenčních (viz adresář ref-out). Pokud byste rádi
# využili tohoto skriptu jako základ pro váš testovací rámec, tak doporučujeme
# tento mechanismus doplnit.
# Dále doporučujeme testovat různé varianty relativních a absolutních cest
# vstupních a výstupních souborů, k čemuž poslouží proměnné začínající
# LOCAL_IN_PATH a LOCAL_OUT_PATH (neomezujte se pouze na zde uvedené triviální
# varianty).
# Výstupní soubory mohou při spouštění vašeho skriptu již existovat a pokud není
# u zadání specifikováno jinak, tak se bez milosti přepíší!
# Výstupní soubory nemusí existovat a pak je třeba je vytvořit!
# Pokud běh skriptu skončí s návratovou hodnotou různou od nuly, tak není
# vytvoření souboru zadaného parametrem --output nutné, protože jeho obsah
# stejně nelze považovat za validní.
# V testech se jako pokaždé určitě najdou nějaké chyby nebo nepřesnosti, takže
# pokud nějakou chybu najdete, tak na ni prosím upozorněte ve fóru příslušné
# úlohy (konstruktivní kritika bude pozitivně ohodnocena). Vyhrazujeme si také
# právo testy měnit, opravovat a případně rozšiřovat, na což samozřejmě
# upozorníme na fóru dané úlohy.
#
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

TASK=syn
INTERPRETER="php"
EXTENSION=php
#INTERPRETER="python3.6"
#EXTENSION=py

# cesty ke vstupním a výstupním souborům
LOCAL_IN_PATH="./" # (simple relative path)
LOCAL_IN_PATH2="" #Alternative 1 (primitive relative path)
LOCAL_IN_PATH3="`pwd`/" #Alternative 2 (absolute path)
LOCAL_OUT_PATH="./logs/" # (simple relative path)
LOCAL_OUT_PATH2="./logs/" #Alternative 1 (primitive relative path)
LOCAL_OUT_PATH3="`pwd`/logs/" #Alternative 2 (absolute path)
# cesta pro ukládání chybového výstupu studentského skriptu
LOG_PATH="./logs/"
REF_PATH="./ref-out/"
mkdir -p ${LOG_PATH}

function compareToRef {
    echo -n $? > ${LOG_PATH}${1}.rc
    echo "##### "${1}": #####"
    diff ${LOG_PATH}${1}.rc ${REF_PATH}${1}.rc
    #diff ${LOG_PATH}${1}.err ${REF_PATH}${1}.err
    diff ${LOG_PATH}${1}.out ${REF_PATH}${1}.out
}

# test01: Argument error; Expected output: test01.out; Expected return code: 1
$INTERPRETER $TASK.$EXTENSION --error 2> ${LOG_PATH}test01.err > ${LOG_PATH}test01.out
compareToRef test01

# test02: Input error; Expected output: test02.out; Expected return code: 2
$INTERPRETER $TASK.$EXTENSION --input=nonexistent --output="${LOCAL_OUT_PATH3}test02.out" 2> ${LOG_PATH}test02.err > ${LOG_PATH}test02.out
compareToRef test02


# test03: Output error; Expected output: test03.out; Expected return code: 3
$INTERPRETER $TASK.$EXTENSION --input="${LOCAL_IN_PATH3}empty" --output=nonexistent/${LOCAL_OUT_PATH2}test03.out 2> ${LOG_PATH}test03.err > ${LOG_PATH}test03.out
compareToRef test03


# test04: Format table error - nonexistent parameter; Expected output: test04.out; Expected return code: 4
$INTERPRETER $TASK.$EXTENSION --input="${LOCAL_IN_PATH3}empty" --output=${LOCAL_OUT_PATH}test04.out --format=error-parameter.fmt 2> ${LOG_PATH}test04.err
compareToRef test04


# test05: Format table error - size; Expected output: test05.out; Expected return code: 4
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}empty --output="${LOCAL_OUT_PATH3}test05.out" --format=error-size.fmt 2> ${LOG_PATH}test05.err
compareToRef test05

# test06: Format table error - color; Expected output: test06.out; Expected return code: 4
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH2}empty --output=${LOCAL_OUT_PATH}test06.out --format=error-color.fmt 2> ${LOG_PATH}test06.err
compareToRef test06

# test07: Format table error - RE syntax; Expected output: test07.out; Expected return code: 4
$INTERPRETER $TASK.$EXTENSION --input="${LOCAL_IN_PATH3}empty" --output="${LOCAL_OUT_PATH3}test07.out" --format=error-re.fmt 2> ${LOG_PATH}test07.err
compareToRef test07

# test08: Empty files; Expected output: test08.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH2}empty --output="${LOCAL_OUT_PATH3}test08.out" --format=empty 2> ${LOG_PATH}test08.err
compareToRef test08

# test09: Format parameters; Expected output: test09.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH2}basic-parameter.in --output="${LOCAL_OUT_PATH3}test09.out" --format=basic-parameter.fmt 2> ${LOG_PATH}test09.err
compareToRef test09

# test10: Argument swap; Expected output: test10.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --format=basic-parameter.fmt --output="${LOCAL_OUT_PATH3}test10.out" --input=${LOCAL_IN_PATH}basic-parameter.in 2> ${LOG_PATH}test10.err
compareToRef test10

# test11: Standard input/output; Expected output: test11.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --format=basic-parameter.fmt >"${LOCAL_OUT_PATH3}test11.out" <${LOCAL_IN_PATH}basic-parameter.in 2> ${LOG_PATH}test11.err
compareToRef test11

# test12: Basic regular expressions; Expected output: test12.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH2}basic-re.in --output="${LOCAL_OUT_PATH3}test12.out" --format=basic-re.fmt 2> ${LOG_PATH}test12.err
compareToRef test12

# test13: Special regular expressions; Expected output: test13.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input="${LOCAL_IN_PATH3}special-re.in" --output="${LOCAL_OUT_PATH3}test13.out" --format=special-re.fmt 2> ${LOG_PATH}test13.err
compareToRef test13

# test14: Special RE - symbols; Expected output: test14.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH2}special-symbols.in --output=${LOCAL_OUT_PATH2}test14.out --format=special-symbols.fmt 2> ${LOG_PATH}test14.err
compareToRef test14

# test15: Negation; Expected output: test15.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}negation.in --output="${LOCAL_OUT_PATH3}test15.out" --format=negation.fmt 2> ${LOG_PATH}test15.err
compareToRef test15

# test16: Multiple format parameters; Expected output: test16.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input="${LOCAL_IN_PATH3}multiple.in" --output="${LOCAL_OUT_PATH3}test16.out" --format=multiple.fmt 2> ${LOG_PATH}test16.err
compareToRef test16

# test17: Spaces/tabs in format parameters; Expected output: test17.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input="${LOCAL_IN_PATH3}multiple.in" --output="${LOCAL_OUT_PATH3}test17.out" --format=spaces.fmt 2> ${LOG_PATH}test17.err
compareToRef test17

# test18: Line break tag; Expected output: test18.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input="${LOCAL_IN_PATH3}newlines.in" --output=${LOCAL_OUT_PATH}test18.out --format=empty --br 2> ${LOG_PATH}test18.err
compareToRef test18

# test19: Overlap; Expected output: test19.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH2}overlap.in --output="${LOCAL_OUT_PATH3}test19.out" --format=overlap.fmt 2> ${LOG_PATH}test19.err
compareToRef test19

# test20: Perl RE; Expected output: test20.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input="${LOCAL_IN_PATH3}special-symbols.in" --output="${LOCAL_OUT_PATH3}test20.out" --format=re.fmt 2> ${LOG_PATH}test20.err
compareToRef test20

# test21: Example; Expected output: test21.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH2}example.in --br --format=example.fmt > "${LOCAL_OUT_PATH3}test21.out" 2> ${LOG_PATH}test21.err
compareToRef test21

# test22: Simple C program; Expected output: test22.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH2}cprog.c --br --format=c.fmt > ${LOCAL_OUT_PATH2}test22.out 2> ${LOG_PATH}test22.err
compareToRef test22
