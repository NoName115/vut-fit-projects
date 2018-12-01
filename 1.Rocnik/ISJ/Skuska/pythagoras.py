# Author: Róbert Kolcún, FIT
# <xkolcu00@stud.fit.vutbr.cz>
# Priprava na skusku z ISJ, 2015/2016


param_limit = 100

def pythagoras_triplets(limit):

    a = int()
    b = int()
    c = int()
    odvesna = 4

    for i in range(odvesna, limit+1):
 		if (i % 2 == 0):
 			c = (i // 2) ** 2

 			if (c > i):
 				a = i
 				b = c - 1
 			else:
 				a = c - 1
 				b = i

 			c = c + 1
 			yield(a, b, c)
 		else:
 			c = (i ** 2) // 2

 			if (c > i):
 				a = i
 				b = c
 			else:
 				a = c
 				b = i

 			c = c + 1
 			yield(a, b, c)
        

#Vypis
print sorted(list(pythagoras_triplets(param_limit)), key = lambda x: list(x)[1])