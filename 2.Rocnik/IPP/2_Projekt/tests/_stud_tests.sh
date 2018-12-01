#!/usr/bin/env bash

# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# IPP - cls - doplňkové testy - 2015/2016
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# Činnost:
# - vytvoří výstupy studentovy úlohy v daném interpretu na základě sady testů
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# Popis (README):
#
# Struktura skriptu _stud_tests.sh (v kódování UTF-8):
# Každý test zahrnuje až 4 soubory (vstupní soubor, případný druhý vstupní
# soubor, výstupní soubor, soubor logující chybové výstupy *.err (zde vynechány)
# a soubor logující návratový kód skriptu *.!!!).
# Pro spuštění testů je nutné do stejného adresáře zkopírovat i
# váš skript. V komentářích u jednotlivých testů jsou uvedeny dodatečné
# informace jako očekávaný návratový kód.
# Tyto doplňující testy obsahují i několik testů rozšíření (viz konec skriptu).
#
# Proměnné ve skriptu _stud_tests.sh pro konfiguraci testů:
#  INTERPRETER - využívaný interpret
#  EXTENSION - přípona souboru s vaším skriptem (jméno skriptu je dáno úlohou)
#  LOCAL_IN_PATH/LOCAL_OUT_PATH - testování různých cest ke vstupním/výstupním
#    souborům
#
# Další soubory archivu s doplňujícími testy:
# V adresáři ref-out najdete referenční soubory pro výstup (*.out nebo *.xml),
# referenční soubory s návratovým kódem (*.!!!) a pro ukázku i soubory s
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

#!!!!POZOR NA UMIESTENIE TASK!!!
TASK=cls
#INTERPRETER="php -d open_basedir=\"\""
#EXTENSION=php
INTERPRETER=python3
EXTENSION=py

# cesty ke vstupním a výstupním souborům
LOCAL_IN_PATH="./" # (simple relative path)
LOCAL_IN_PATH2="" #Alternative 1 (primitive relative path)
LOCAL_IN_PATH3=`pwd`"/" #Alternative 2 (absolute path)
LOCAL_OUT_PATH="./moj-out/" # (simple relative path)
LOCAL_OUT_PATH2="./moj-out/" #Alternative 1 (primitive relative path)
LOCAL_OUT_PATH3=`pwd`"/moj-out/" #Alternative 2 (absolute path)
# cesta pro ukládání chybového výstupu studentského skriptu
LOG_PATH="./moj-out/"
COUNT=0
ALL=0
mkdir -p moj-out
rm moj-out/*

#10 test for invalid arguments;  Expected return code for all: 1

#test0H --help
$INTERPRETER $TASK.$EXTENSION --help 1>${LOCAL_OUT_PATH}0H.help 2> ${LOG_PATH}0H.err
echo -n $? > ${LOG_PATH}test0H.!!!

#test000
$INTERPRETER $TASK.$EXTENSION notThis 2> ${LOG_PATH}test000.err
echo -n $? > ${LOG_PATH}test000.!!!

#test001
$INTERPRETER $TASK.$EXTENSION  --invalidArgument 2> ${LOG_PATH}test001.err
echo -n $? > ${LOG_PATH}test001.!!!

#test002
$INTERPRETER $TASK.$EXTENSION  --input=${LOCAL_IN_PATH}test01.in --output=${LOCAL_OUT_PATH}test01.out --input=${LOCAL_IN_PATH}test01.in 2> ${LOG_PATH}test002.err
echo -n $? > ${LOG_PATH}test002.!!!

#test003
$INTERPRETER $TASK.$EXTENSION  --help --input=f 2> ${LOG_PATH}test003.err
echo -n $? > ${LOG_PATH}test003.!!!

#test004
$INTERPRETER $TASK.$EXTENSION  --help slovo 2> ${LOG_PATH}test004.err
echo -n $? > ${LOG_PATH}test004.!!!

#test005
$INTERPRETER $TASK.$EXTENSION  $TASK.$EXTENSION 2> ${LOG_PATH}test005.err
echo -n $? > ${LOG_PATH}test005.!!!

#test006
$INTERPRETER $TASK.$EXTENSION  --input 2> ${LOG_PATH}test006.err
echo -n $? > ${LOG_PATH}test006.!!!

#test007
$INTERPRETER $TASK.$EXTENSION  --input= 2> ${LOG_PATH}test007.err
echo -n $? > ${LOG_PATH}test007.!!!

#test008
$INTERPRETER $TASK.$EXTENSION  --pretty-xml= 2> ${LOG_PATH}test008.err
echo -n $? > ${LOG_PATH}test008.!!!

#test009
$INTERPRETER $TASK.$EXTENSION  --input=${LOCAL_IN_PATH}test01.in --output=${LOCAL_OUT_PATH}test009.out --pretty-xml=4 --details=C --seach="hello" --what 2> ${LOG_PATH}test009.err
echo -n $? > ${LOG_PATH}test009.!!!

#non existing file on input (if file with that name exists in your computer, you have some problems)
$INTERPRETER $TASK.$EXTENSION  --input=ILoveWindows 2> ${LOG_PATH}test00a.err
echo -n $? > ${LOG_PATH}test00a.!!!

#not a file, but directory, error 2
$INTERPRETER $TASK.$EXTENSION  --input=ILoveWindows/ 2> ${LOG_PATH}test00b.err
echo -n $? > ${LOG_PATH}test00b.!!!

#missing permission (if you run it as sudo, ask yourself -> Why?)
$INTERPRETER $TASK.$EXTENSION  --input=/etc/shadow 2> ${LOG_PATH}test00c.err
echo -n $? > ${LOG_PATH}test00c.!!!

#help with option? error 1
$INTERPRETER $TASK.$EXTENSION  --help=me 2> ${LOG_PATH}test00d.err
echo -n $? > ${LOG_PATH}test00d.!!!

#invalit output - no permission
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test01.in --output=/bin/kill 2> ${LOG_PATH}test00e.err
echo -n $? > ${LOG_PATH}test00e.!!!

# technically test01 but with output to stdout; Expected output: test00f.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test01.in >${LOCAL_OUT_PATH}test00f.out 2> ${LOG_PATH}test00f.err
echo -n $? > ${LOG_PATH}test00f.!!!

#test00g --conflicts withou --details
$INTERPRETER $TASK.$EXTENSION  --input=${LOCAL_IN_PATH}test01.in --output=${LOCAL_OUT_PATH}test00g.out --conflicts 2> ${LOG_PATH}test00g.err
echo -n $? > ${LOG_PATH}test00g.!!!

# test01: zakladni strom dedicnosti; Expected output: test01.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test01.in --output=${LOCAL_OUT_PATH}test01.out 2> ${LOG_PATH}test01.err
echo -n $? > ${LOG_PATH}test01.!!!

# test02: vypis bazove tridy; Expected output: test02.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test02.in > ${LOCAL_OUT_PATH}test02.out --details=A 2> ${LOG_PATH}test02.err
echo -n $? > ${LOG_PATH}test02.!!!

# test03: vypis dedici tridy; Expected output: test03.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test03.in --output=${LOCAL_OUT_PATH}test03.out --details=D 2> ${LOG_PATH}test03.err
echo -n $? > ${LOG_PATH}test03.!!!

# test04: dedeni ciste virtualni metody => vsechny tridy abstraktni; Expected output: test04.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION < ${LOCAL_IN_PATH3}test04.in > ${LOCAL_OUT_PATH}test04.out 2> ${LOG_PATH}test04.err
echo -n $? > ${LOG_PATH}test04.!!!

# test05: prepsani ciste virtualni metody => dedici tridy nejsou abstraktni; Expected output: test05.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test05.in --output=${LOCAL_OUT_PATH}test05.out 2> ${LOG_PATH}test05.err
echo -n $? > ${LOG_PATH}test05.!!!

# test06: dedici schema diamant => konflikt pri dedeni; Expected output: test06.out; Expected return code: 21
$INTERPRETER $TASK.$EXTENSION --output=${LOCAL_OUT_PATH3}test06.out < ${LOCAL_IN_PATH}test06.in --details=D 2> ${LOG_PATH}test06.err
echo -n $? > ${LOG_PATH}test06.!!!

# test07: dedici schema diamant => zabraneni konfliktu prepsanim konf. clenu; Expected output: test07.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --details=D --input=${LOCAL_IN_PATH}test07.in --output=${LOCAL_OUT_PATH2}test07.out 2> ${LOG_PATH}test07.err
echo -n $? > ${LOG_PATH}test07.!!!

# test08: reseni konfliktu pri dedeni kl. slovem using; Expected output: test08.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test08.in --output=${LOCAL_OUT_PATH3}test08.out --details=C 2> ${LOG_PATH}test08.err
echo -n $? > ${LOG_PATH}test08.!!!

# test09: ukazka hlubsiho vypisu lesu dedicnosti; Expected output: test09.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH2}test09.in --output=${LOCAL_OUT_PATH}test09.out 2> ${LOG_PATH}test09.err
echo -n $? > ${LOG_PATH}test09.!!!

# test10: vypsani detailu vsech trid v souboru; Expected output: test10.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --details --input=${LOCAL_IN_PATH3}test10.in --output=${LOCAL_OUT_PATH}test10.out 2> ${LOG_PATH}test10.err
echo -n $? > ${LOG_PATH}test10.!!!

# test12: BONUS: vypis konfliktniho clenu ve tride; Expected output: test12.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH3}test12.in --output=${LOCAL_OUT_PATH}test12.out --details=C --conflicts 2> ${LOG_PATH}test12.err
echo -n $? > ${LOG_PATH}test12.!!!

# test13: Nevypsani zdedeneho clenu, ktery byl v dedene tride private a ktery neni pure virtual metodou; Expected output:  test13.out; Exptected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH3}test13.in --output=${LOCAL_OUT_PATH}test13.out --details=B 2> ${LOG_PATH}test13.err
echo -n $? > ${LOG_PATH}test13.!!!

# test014: Test 09 but with awful format (for weak parsers); Expected output: test014.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH2}test014.in --output=${LOCAL_OUT_PATH}test014.out 2> ${LOG_PATH}test014.err
echo -n $? > ${LOG_PATH}test014.!!!

# test015: Test similiar to test08, but conflict in fuctions solved with using. Expected output: test015.out Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH2}test015.in --output=${LOCAL_OUT_PATH}test015.out --details=D 2> ${LOG_PATH}test015.err
echo -n $? > ${LOG_PATH}test015.!!!

# test016: Test 09, but with function and instances definition; Expected output: test09.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH2}test016.in --output=${LOCAL_OUT_PATH}test016.out 2> ${LOG_PATH}test016.err
echo -n $? > ${LOG_PATH}test016.!!!

# test017: Test 03, but with funky names of variables, methods, and classes; Expected output: test09.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH2}test017.in --output=${LOCAL_OUT_PATH}test017.out --details=____ 2> ${LOG_PATH}test017.err
echo -n $? > ${LOG_PATH}test017.!!!

# test019: Static methods and instances; Expected output: test019.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --details --input=${LOCAL_IN_PATH3}test019.in --output=${LOCAL_OUT_PATH}test019.out 2> ${LOG_PATH}test019.err
echo -n $? > ${LOG_PATH}test019.!!!

# test020: Instance and method has type of class; Expected output: test020.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --details=B --input=${LOCAL_IN_PATH3}test020.in --output=${LOCAL_OUT_PATH}test020.out 2> ${LOG_PATH}test020.err
echo -n $? > ${LOG_PATH}test020.!!!

# test021: Declared class cannot be used as base class; Expected output: -; Expected return code: 4
$INTERPRETER $TASK.$EXTENSION --details=B --input=${LOCAL_IN_PATH3}test021.in --output=${LOCAL_OUT_PATH}test021.out 2> ${LOG_PATH}test021.err
echo -n $? > ${LOG_PATH}test021.!!!

# test23: test for methods and instances declaration and definitions - INVALID: test23.out; Expected return code: 4
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test023.in --output=${LOCAL_OUT_PATH2}test023.out --details=B 2> ${LOG_PATH}test023.err
echo -n $? > ${LOG_PATH}test023.!!!

# test24: method redefinition: test24.out; Expected return code: 4
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test024.in --output=${LOCAL_OUT_PATH2}test024.out 2> ${LOG_PATH}test024.err
echo -n $? > ${LOG_PATH}test024.!!!

# test25: instance redefinition: test25.out; Expected return code: 4
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test025.in --output=${LOCAL_OUT_PATH2}test025.out 2> ${LOG_PATH}test025.err
echo -n $? > ${LOG_PATH}test025.!!!

# test26: overloading: test26.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test026.in --output=${LOCAL_OUT_PATH2}test026.out --details=A 2> ${LOG_PATH}test026.err
echo -n $? > ${LOG_PATH}test026.!!!

# test27: Contructor and Destructor: test27.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test027.in --output=${LOCAL_OUT_PATH2}test027.out --details 2> ${LOG_PATH}test027.err
echo -n $? > ${LOG_PATH}test027.!!!

# test28: Destructor overload: test28.out; Expected return code: 4
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test028.in --output=${LOCAL_OUT_PATH2}test028.out --details 2> ${LOG_PATH}test028.err
echo -n $? > ${LOG_PATH}test028.!!!

# test29: duplicate base class: test29.out; Expected return code: 4
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test029.in --output=${LOCAL_OUT_PATH2}test029.out --details 2> ${LOG_PATH}test029.err
echo -n $? > ${LOG_PATH}test029.!!!

# test30: conflict: test30.out; Expected return code: 21
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test030.in --output=${LOCAL_OUT_PATH2}test030.out --details 2> ${LOG_PATH}test030.err
echo -n $? > ${LOG_PATH}test030.!!!

# test31D: prettyXML test: test31D.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test01.in --output=${LOCAL_OUT_PATH2}test031D.outD --details=D --pretty-xml=7 2> ${LOG_PATH}test031D.err
echo -n $? > ${LOG_PATH}test031D.!!!

# test32: not a conflict: test032.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test032.in --output=${LOCAL_OUT_PATH2}test032.out 2> ${LOG_PATH}test032.err
echo -n $? > ${LOG_PATH}test032.!!!

# test33: one using on more fethods: test033.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test033.in --output=${LOCAL_OUT_PATH2}test033.out --details=C 2> ${LOG_PATH}test033.err
echo -n $? > ${LOG_PATH}test033.!!!

# test34: instance name and method name cannot be the same: test034.out; Expected return code: 21
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test034.in --output=${LOCAL_OUT_PATH2}test034.out 2> ${LOG_PATH}test034.err
echo -n $? > ${LOG_PATH}test034.!!!

# test35: instance name and method name cannot be the same when in base classes: test035.out; Expected return code: 21
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test035.in --output=${LOCAL_OUT_PATH2}test035.out 2> ${LOG_PATH}test035.err
echo -n $? > ${LOG_PATH}test035.!!!

# test36: empty input: test036.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=ref-out/test005.out --output=${LOCAL_OUT_PATH2}test036.out 2> ${LOG_PATH}test036.err
echo -n $? > ${LOG_PATH}test036.!!!

# test37: details of empty class: test037.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test037.in --output=${LOCAL_OUT_PATH2}test037.out --details=A 2> ${LOG_PATH}test037.err
echo -n $? > ${LOG_PATH}test037.!!!

# test38: details of unknown class - JeXamXML fail diff: test038.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test037.in --output=${LOCAL_OUT_PATH2}test038.outDF --details=B 2> ${LOG_PATH}test038.err
echo -n $? > ${LOG_PATH}test038.!!!

# test39: details of base class: test039.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test039.in --output=${LOCAL_OUT_PATH2}test039.out --details=A 2> ${LOG_PATH}test039.err
echo -n $? > ${LOG_PATH}test039.!!!

# test40: inheritance of private atributes: test40.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test40.in --output=${LOCAL_OUT_PATH2}test40.out --details 2> ${LOG_PATH}test40.err
echo -n $? > ${LOG_PATH}test40.!!!

# test41: using on more than one atribute: test41.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test41.in --output=${LOCAL_OUT_PATH2}test41.out --details=B 2> ${LOG_PATH}test41.err
echo -n $? > ${LOG_PATH}test41.!!!

# test42: private pure virtual method: test42.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test42.in --output=${LOCAL_OUT_PATH2}test42.out 2> ${LOG_PATH}test42.err
echo -n $? > ${LOG_PATH}test42.!!!

# test43: triangle conflict: test43.out; Expected return code: 21
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test43.in --output=${LOCAL_OUT_PATH2}test43.out 2> ${LOG_PATH}test43.err
echo -n $? > ${LOG_PATH}test43.!!!

# test44: triangle conflict solved by overriding: test44.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test44.in --output=${LOCAL_OUT_PATH2}test44.out 2> ${LOG_PATH}test44.err
echo -n $? > ${LOG_PATH}test44.!!!

# test45: triangle conflict solved by using:: : test45.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test45.in --output=${LOCAL_OUT_PATH2}test45.out --details=C 2> ${LOG_PATH}test45.err
echo -n $? > ${LOG_PATH}test45.!!!

# test46: triangle conflict solved by overiding method: test46.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test46.in --output=${LOCAL_OUT_PATH2}test46.out --details=C 2> ${LOG_PATH}test46.err
echo -n $? > ${LOG_PATH}test46.!!!

# test47: base class cannot be the same class: test47.out; Expected return code: 4
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test47.in --output=${LOCAL_OUT_PATH2}test47.out 2> ${LOG_PATH}test47.err
echo -n $? > ${LOG_PATH}test47.!!!

# test48: duplicit base class: test48.out; Expected return code: 4
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test48.in --output=${LOCAL_OUT_PATH2}test48.out 2> ${LOG_PATH}test48.err
echo -n $? > ${LOG_PATH}test48.!!!

# test49: unknown base class: test49.out; Expected return code: 4
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test49.in --output=${LOCAL_OUT_PATH2}test49.out 2> ${LOG_PATH}test49.err
echo -n $? > ${LOG_PATH}test49.!!!

# test50: static members: test50.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test50.in --output=${LOCAL_OUT_PATH2}test50.out --details=B 2> ${LOG_PATH}test50.err
echo -n $? > ${LOG_PATH}test50.!!!

# test51: types: test51.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test51.in --output=${LOCAL_OUT_PATH2}test51.out --details=A 2> ${LOG_PATH}test51.err
echo -n $? > ${LOG_PATH}test51.!!!

# test52: diamond conflict: test52.out; Expected return code: 21
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test52.in --output=${LOCAL_OUT_PATH2}test52.out 2> ${LOG_PATH}test52.err
echo -n $? > ${LOG_PATH}test52.!!!

# test53: diamond conflict private atrubutes: test53.out; Expected return code: 21
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test53.in --output=${LOCAL_OUT_PATH2}test53.out 2> ${LOG_PATH}test53.err
echo -n $? > ${LOG_PATH}test53.!!!

# test54: diamond conflict solved by overridng: test54.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test54.in --output=${LOCAL_OUT_PATH2}test54.out --details=D 2> ${LOG_PATH}test54.err
echo -n $? > ${LOG_PATH}test54.!!!

# test55: privacy in more blocks: test55.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test55.in --output=${LOCAL_OUT_PATH2}test55.out --details=A 2> ${LOG_PATH}test55.err
echo -n $? > ${LOG_PATH}test55.!!!

# test56: conrete and abstract: test56.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test56.in --output=${LOCAL_OUT_PATH2}test56.out 2> ${LOG_PATH}test56.err
echo -n $? > ${LOG_PATH}test56.!!!

# test57: type is different class: test57.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test57.in --output=${LOCAL_OUT_PATH2}test57.out --details=C 2> ${LOG_PATH}test57.err
echo -n $? > ${LOG_PATH}test57.!!!

# test58: static and virtual instance: test58.out; Expected return code: 4
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test58.in --output=${LOCAL_OUT_PATH2}test58.out 2> ${LOG_PATH}test58.err
echo -n $? > ${LOG_PATH}test58.!!!

# test59: static and virtual method: test59.out; Expected return code: 4
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test59.in --output=${LOCAL_OUT_PATH2}test59.out 2> ${LOG_PATH}test59.err
echo -n $? > ${LOG_PATH}test59.!!!

# test60: unknown class as type: test60.out; Expected return code: 4
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test60.in --output=${LOCAL_OUT_PATH2}test60.out 2> ${LOG_PATH}test60.err
echo -n $? > ${LOG_PATH}test60.!!!

# test61: Diamond conflict --conflicts: test61.out; Expected return code: 0
#$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test52.in --output=${LOCAL_OUT_PATH2}test61.out --details=D --conflicts 2> ${LOG_PATH}test61.err
#echo -n $? > ${LOG_PATH}test61.!!!

# test62: Diamond conflict private --conflicts: test62.out; Expected return code: 0
#$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test53.in --output=${LOCAL_OUT_PATH2}test62.out --details=D --conflicts 2> ${LOG_PATH}test62.err
#echo -n $? > ${LOG_PATH}test62.!!!

# test63: Conflict inside --conflicts: test63.out; Expected return code: 0
#$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test63.in --output=${LOCAL_OUT_PATH2}test63.out --details=E --conflicts 2> ${LOG_PATH}test63.err
#echo -n $? > ${LOG_PATH}test63.!!!

# test64: --conflicts: test64.out; Expected return code: 0
#$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test64.in --output=${LOCAL_OUT_PATH2}test64.out --details=C --conflicts 2> ${LOG_PATH}test64.err
#echo -n $? > ${LOG_PATH}test64.!!!

# test65: pure virtual but not virtual: test65.out; Expected return code: 4
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test65.in --output=${LOCAL_OUT_PATH2}test65.out 2> ${LOG_PATH}test65.err
echo -n $? > ${LOG_PATH}test65.!!!

# test66: Test 63 but --detials=C : test66.out; Expected return code: 0
#$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test63.in --output=${LOCAL_OUT_PATH2}test66.out --details=C --conflicts 2> ${LOG_PATH}test66.err
#echo -n $? > ${LOG_PATH}test66.!!!

# test67: test 62 but with method: test67.out; Expected return code: 0
#$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test67.in --output=${LOCAL_OUT_PATH2}test67.out --details=D --conflicts 2> ${LOG_PATH}test67.err
#echo -n $? > ${LOG_PATH}test67.!!!

# test68: Test 66 but with methods: test68.out; Expected return code: 0
#$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test68.in --output=${LOCAL_OUT_PATH2}test68.out --details=C --conflicts 2> ${LOG_PATH}test68.err
#echo -n $? > ${LOG_PATH}test68.!!!


# test69: Deep inheritance of members: test69.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test69.in --output=${LOCAL_OUT_PATH2}test69.out --details 2> ${LOG_PATH}test69.err
echo -n $? > ${LOG_PATH}test69.!!!

# test70: Anonymous method arguments: test70.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test70.in --output=${LOCAL_OUT_PATH2}test70.out --details 2> ${LOG_PATH}test70.err
echo -n $? > ${LOG_PATH}test70.!!!

#Print results
RED='\033[0;31m'
END='\033[0m'
GREEN='\033[1;32m'
BRED='\033[1;31m'

for i in `ls ./ref-out/ | grep -e '.*\.out$'`
    do
        ((ALL++))
        java -jar jexamxml/jexamxml.jar ./moj-out/"$i" ./ref-out/"$i" delta.xml ./cls_options 1>/dev/null
        m=$?
        if [ ! -s ./ref-out/"$i" ]; then
            d=`diff -aN ./moj-out/"${i%.out}.!!!" ./ref-out/"${i%.out}.!!!" 2> /dev/null`
            if [ $? -ne 0 ]; then
                printf "${GREEN}---------------------------------------------------${END}\n"
                printf "Test: ${BRED}$i${END} \n"
                ((COUNT++))
                printf "${RED}Wrong error code:${END}\n $d \n"
            fi
        elif [ "$m" -eq 1 ]; then
            printf "${GREEN}---------------------------------------------------${END}\n"
            printf "Test: ${BRED}$i${END} \n"
            ((COUNT++))
            printf "${RED}Wrong output:${END}\n "
            echo `head -n 10 delta.xml`
            d=`diff -aN ./moj-out/"${i%.out}.!!!" ./ref-out/"${i%.out}.!!!" 2> /dev/null`
            if [ $? -ne 0 ]; then
                printf "${RED}Wrong error code:${END}\n $d \n"
            fi
        elif [ "$m" -ne 0 ]; then
            printf "${GREEN}---------------------------------------------------${END}\n"
            printf "Test: ${BRED}$i${END} \n"
            ((COUNT++))
            printf "${RED}Failed because of JExamXml. JExamXml not where it should be, missing java...${END}\n"
            printf "${RED}OR same output files are missing!! make sure you save to correct files${END}\n"
            printf "${RED}OR you ended with error and thus no outputfile was written${END}\n"
        else
            d=`diff -aN ./moj-out/"${i%.out}.!!!" ./ref-out/"${i%.out}.!!!" 2> /dev/null`
            if [ $? -ne 0 ]; then
                printf "${GREEN}---------------------------------------------------${END}\n"
                printf "Test: ${BRED}$i${END} \n"
                ((COUNT++))
                printf "${RED}Wrong error code:${END}\n $d \n"
            fi
        fi
    done
    #one for pretty-XML=7
    ((ALL++))
    d=`grep -o '^\([[:space:]]*\)' "${LOCAL_OUT_PATH2}"test031D.outD`
    f=`cat ./ref-out/test031D.outD`
    v=`diff -aN <(echo "$d") <(echo "$f")`
    if [ $? -ne 0 ]; then
      printf "${GREEN}---------------------------------------------------${END}\n"
      ((COUNT++))
      printf "${RED}Wrong format when pretty-xml=7, test031D \n"
    fi

    #test for help
    ((ALL++))
    if [ ! -f "${LOCAL_OUT_PATH}"0H.help ]; then
      ((COUNT++))
      printf "${GREEN}---------------------------------------------------${END}\n"
      printf "${RED}No help! \n"
    else
        f=`cat ./moj-out/0H.help`
        len=${#f}
        if [ "$len" -lt 20 ]; then
          ((COUNT++))
          printf "${GREEN}---------------------------------------------------${END}\n"
          printf "${RED}Help is too short! \n"
      fi
    fi

    #one for only XML head
    ((ALL++))
    d=`cat "${LOCAL_OUT_PATH2}"test038.outDF`
    f=`grep -i "$d" ./ref-out/test038.outDF`
    if [[ ! $f ]]; then
      printf "${GREEN}---------------------------------------------------${END}\n"
      ((COUNT++))
      printf "${RED}When no result found, only XML head is printed, test038 \n"
    fi




    PASSED=$((ALL-COUNT))
    printf "${GREEN}===================================================${END}\n"
    printf "Failed ${BRED}$COUNT${END} "`[[ $COUNT -eq 1 ]] && echo -n "test" || echo -n "tests"`;
    printf " from $ALL tests.\n"
    printf "If maths is not your strong suit: you passed ${GREEN}$PASSED${END} ";
    printf "`[[ $PASSED -eq 1 ]] && echo -n "test" || echo -n "tests"`\n";
