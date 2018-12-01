# Author: Róbert Kolcún, FIT
# <xkolcu00@stud.fit.vutbr.cz>
# Priprava na skusku z ISJ, 2015/2016


def pow2minus1(number):
	for i in range(2, number + 1):
		result = pow(i, 2) - 1
		yield result

print pow2minus1(2)