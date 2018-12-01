#!/bin/bash

# --------------------------- #
# Run make before this script #
# --------------------------- #

PRJ=c202

# Check binary files
if ! [ -a $PRJ-test ];
then
    echo "ERROR:" $PRJ-test "binary file not found, run make";
    exit 1;
fi

# BASIC TEST
./$PRJ-test >$PRJ-test-my.out
diff $PRJ-test.output $PRJ-test-my.out >$PRJ-test-diff.out
if [ -s $PRJ-test-diff.out ];
then
    echo -e "---------------\nFAILED: Basic test -" $PRJ-test-diff.out "\n---------------";
else
    echo -e "---------------\nOK: Basic test\n---------------";
fi
