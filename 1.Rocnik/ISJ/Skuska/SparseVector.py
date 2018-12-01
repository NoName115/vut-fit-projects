# Author: Róbert Kolcún, FIT
# <xkolcu00@stud.fit.vutbr.cz>
# Priprava na skusku z ISJ, 2015/2016


class sparseVector():
	def __init__(self):
		self.vectors = {}

	def __setitem__(self, key, value):
		self.vectors.update({key : value})

	def __getitem__(self, key):
		return self.vectors[key]

	def __len__(self):
		return len(self.vectors)

	def __iter__(self):
		for i in super(sparseVector, self).__iter__():
			yield i

	def __mul__(self, other):
		vys = sparseVector()

		for key in self.vectors:
			if (key in other.vectors):
				vys[key] = self.vectors[key] * other.vectors[key]

		return vys

	def __str__(self):
		return str(self.vectors)


x = sparseVector()
x[0] = 1.0 #__setitem__
x[3] = 2.0
x[5] = 3.0

print(len(x)) #vypise 6, __len__
print(v for v in x) #vypise [1.0, 0.0, 0.0, 2.0, 0.0, 3.0], __iter__

a = [1.0, 0.0, 0.0, 2.0, 0.0, 3.0]

print (x*x) #vypise 10, skalarni soucin, __mul__
#print (x*a) #funguje i pro seznamy
