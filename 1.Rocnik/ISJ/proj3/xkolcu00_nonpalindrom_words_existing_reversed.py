#!/usr/bin/env python3

# Author: Róbert Kolcún - FIT
# <xkolcu00@stud.fit.vutbr.cz>
# Login: xkolcu00

import fileinput

words = set()
for line in fileinput.input():
    words.add(line.rstrip())

palindroms = set()
for w in words:
	if w == w[::-1]:
		palindroms.add(w)
#palindroms = [w for w in words if w == w[::-1]]

result = [w for w in words if w not in palindroms and w[::-1] in words]

print(sorted(result))