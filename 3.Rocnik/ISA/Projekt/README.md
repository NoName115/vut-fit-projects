# README

## Krátky popis programu
* Ako program je implementovaný jednoduchý ldap server(LDAPv2)
* Program je implementovaný v programovacom jazyku C++
* Pre preklad je použitý prekladač g++ so štandartom c++11

## Rozšírenia a obmedzenia
* Program nemá žiadne známe obmedzenia oproti zadaniu projektu
* Pre rožširenia projektu je implementovaná podpora UTF-8 znakov

## Príklad spustenia programu
```
$ ./myldap -p 12345 -f isa2017-ldap.csv
```
```
$ ./myldap {-p <port>} -f <soubor>
```
* Pri neuvedení portu a teda spustení na predvolenej hodnote je potrebné program spustiť s root právami

#### Prepínače
* -p \<port\>
    * Umožnuje špecifikovať konkrétny port, na ktorom má server poslúchať požiadavky klientov. Argument je volitelný a predvolená hodnota portu je 389.
* -f \<súbor\>
    * Cesta k vstupnému textovému súboru vo formáte CSV. Tento argument je povinný

## Zoznam odovzdaných súborov
* Zdrojové súbory
    ```
    myldap.cc
    basefunc.cc
    basefunc.h
    ldapfilter.cc
    ldapfilter.h
    responser.cc
    responser.h
    ```
* Ostatné súbory
    ```
    Makefile
    README.md
    manual.pdf
    ```
