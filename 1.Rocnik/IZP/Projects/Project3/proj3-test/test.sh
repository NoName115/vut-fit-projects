#!/usr/bin/env bash

file='proj3.c'
binary='proj3'

# Output settings
TEXT_BOLD=`tput bold`
TEXT_GREEN=`tput setaf 2`
TEXT_RED=`tput setaf 1`
TEXT_RESET=`tput sgr0`

# IO file names
function errorFileName () {
	printf "test${1}-error"
}
function inFileName () {
	printf "test${1}-in"
}
function outFileName () {
	printf "test${1}-out"
}
function myFileName () {
	printf "test${1}-my"
}

# Test initialization
function initTest () {
	testNumber=$1
	testError=$(errorFileName $testNumber)
	testIn=$(inFileName $testNumber)
	testOut=$(outFileName $testNumber)
	testMy=$(myFileName $testNumber)
}

# Test evaluation
function testName () {
	testNumber=$1
	printf "Test ${testNumber}.. "
}
function ok () {
	printf "${TEXT_GREEN}OK${TEXT_RESET}\n"
}
function fail () {
	printf "${TEXT_RED}FAIL${TEXT_RESET}\n"
}

function isOk () {
	local testNumber=$1
	local testResult=$2
	`diff -q ${testOut} ${testMy} > /dev/null`
	local diffResult=$?

	testName $testNumber
	if [ $testResult == 0 ] && [ $diffResult == 0 ]
	then
		ok
	else
		fail
	fi
}

function isFail () {
	local testNumber=$1
	local testResult=$2
	local testError=$3

	testName $testNumber
	if [ $testResult != 0 ] && [ -s $testError ]
	then
		ok
	else
		fail
	fi
}

# Cleaning
if [ "$1" == "-c" ]
then
	ls | grep 'test.\+\-\(my\|error\)$' | xargs -d "\n" rm
	exit 0
fi

# File exists
if [ -e $file ]
then
	printf "${TEXT_BOLD}Testing file ${file}${TEXT_RESET}\n"
else
	printf "${TEXT_RED}Cannot run test without file ${file}.${TEXT_RESET}\n"
	exit 1
fi

# Compile source
printf "Compile source code...\n"
`gcc -std=c99 -Wall -Wextra -pedantic -DNDEBUG ${file} -o ${binary} -lm`

# Test 1
initTest 1
`./${binary} ${testIn} 20 > ${testMy} 2> ${testError}`
isOk $testNumber $?

# Test 2
initTest 2
`./${binary} ${testIn} 8 > ${testMy} 2> ${testError}`
isOk $testNumber $?

# Test 3
initTest 3
`./${binary} ${testIn} blabla > ${testMy} 2> ${testError}`
isFail $testNumber $? $testError

# Test 4
initTest 4
`./${binary} ${testIn} 3 blabla > ${testMy} 2> ${testError}`
isFail $testNumber $? $testError

# Test 5
initTest 5
`./${binary} > ${testMy} 2> ${testError}`
isFail $testNumber $? $testError

# Test 6
initTest 6
`./${binary} blablafile.txt 1 > ${testMy} 2> ${testError}`
isFail $testNumber $? $testError

# Test 7
initTest 7
`./${binary} ${testIn} > ${testMy} 2> ${testError}`
isOk $testNumber $?

# Test 8
initTest 8
`./${binary} ${testIn} > ${testMy} 2> ${testError}`
isFail $testNumber $? $testError

# Test 9
initTest 9
`./${binary} ${testIn} > ${testMy} 2> ${testError}`
isOk $testNumber $?

# Test 10
initTest 10
`./${binary} ${testIn} > ${testMy} 2> ${testError}`
isFail $testNumber $? $testError

# Test 11
initTest 11
`./${binary} ${testIn} > ${testMy} 2> ${testError}`
isFail $testNumber $? $testError

# Test 12
initTest 12
`./${binary} ${testIn} > ${testMy} 2> ${testError}`
isFail $testNumber $? $testError

# Test 13
initTest 13
`./${binary} ${testIn} > ${testMy} 2> ${testError}`
isFail $testNumber $? $testError

# Test 14
initTest 14
`./${binary} ${testIn} > ${testMy} 2> ${testError}`
isFail $testNumber $? $testError

# Test 15
initTest 15
`./${binary} ${testIn} > ${testMy} 2> ${testError}`
isFail $testNumber $? $testError

# Test 16
initTest 16
`./${binary} ${testIn} > ${testMy} 2> ${testError}`
isFail $testNumber $? $testError

# Test 17
initTest 17
`./${binary} ${testIn} > ${testMy} 2> ${testError}`
isFail $testNumber $? $testError

# Test 18
initTest 18
`./${binary} ${testIn} > ${testMy} 2> ${testError}`
isFail $testNumber $? $testError


# Valgrind installed
`valgrind --version > /dev/null 2>&1`
valgrindResult=$?
if [ "$1" == "-m" ] && [ $valgrindResult == 0 ]
then
	printf "Memmory leaks will be checked...\n"

	# Test 19
	initTest 19
	`valgrind -q --leak-check=full ./${binary} ${testIn} 20 > ${testMy} 2>&1`
	isOk $testNumber $?

	# Test 20
	initTest 20
	`valgrind -q --leak-check=full ./${binary} ${testIn} 8 > ${testMy} 2>&1`
	isOk $testNumber $?
else
	if [ "$1" == "-m" ]
	then
		printf "${TEXT_RED}Valgrind not installed. Cannot check memmory leaks.${TEXT_RESET}\n"
	fi
fi

printf "\n"