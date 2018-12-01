#!/usr/bin/env python3

# Author: Róbert Kolcún - FIT
# <xkolcu00@stud.fit.vutbr.cz>
# Login: xkolcu00

import re
import sys

sys.path.append('./')

login = 'xkolcu00'

try:
	vstup = open('xkcd1313.ipynb','r')
	vystup = open(login + '.ipynb','w')
except IOError as e:
	print ("I/O error: ", sys.exc_info()[0])
	exit()

o_lines = vstup.readlines()
n_lines = []

for i in range(len(o_lines)):
	if re.search('xkcd', o_lines[i]):
		n_lines.append(re.sub('bu.*ls',r'[gikuj]..n|a.[alt]|[pivo].l|i..o|[jocy]e|sh|di|oo', o_lines[i]))
	else:
		n_lines.append(o_lines[i])
	if re.search('\=', n_lines[i]) and re.search('\[', n_lines[i]) and not re.search('\!', n_lines[i]):
		n_lines[i]  = re.sub('xkcd', login, n_lines[i])

	vystup.write(n_lines[i])
