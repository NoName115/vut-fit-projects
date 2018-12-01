# Author: Róbert Kolcún, FIT
# <xkolcu00@stud.fit.vutbr.cz>
# Priprava na skusku z ISJ, 2015/2016


import subprocess

p = subprocess.Popen(["pwqcheck", "-1"], stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE)
output = p.communicate(input = "heslo")

print output