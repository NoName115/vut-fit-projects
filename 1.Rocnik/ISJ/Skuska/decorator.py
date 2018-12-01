# Author: Róbert Kolcún, FIT
# <xkolcu00@stud.fit.vutbr.cz>
# Priprava na skusku z ISJ, 2015/2016


"""
dic = {}

def decor(fun):
	def inside(*args, **kwargs):
		for key in dic:
			if (str(*args) + str(**kwargs) in dic):
				dic[str(*args) + str(**kwargs)] += 1
				return

		dic.update({str(*args) + str(**kwargs) : 1})
		return
	return inside

@decor
def myfunc(*args, **kwargs):
	return 


myfunc(520)
myfunc(520)
myfunc("sdfs")
myfunc("sdfs")
myfunc([25, 35])
myfunc([25, 35])
myfunc("FUCK Y..")

print dic
"""

def count(wrapped):
	def inner(*args, **kwargs):
		if (str(*args) + str(**kwargs) in inner.counter):
			inner.counter[str(*args) + str(**kwargs)] += 1
		else:
			inner.counter.update({str(*args) + str(**kwargs) : 1})
		return wrapped(*args, **kwargs)
	inner.counter = {}
	return inner

@count
def myfunc(*args, **kwargs):
	pass

myfunc()
myfunc(25)
myfunc(25)
myfunc("sdfsd")
print myfunc.counter