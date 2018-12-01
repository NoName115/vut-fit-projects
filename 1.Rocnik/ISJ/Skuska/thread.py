# Author: Róbert Kolcún, FIT
# <xkolcu00@stud.fit.vutbr.cz>
# Priprava na skusku z ISJ, 2015/2016


from threading import Thread

class Spracuj():
	def __init__(self, fun, *args, **kwargs):
		self.fun = fun
		self.args = args
		self.kwargs = kwargs

	def spust(self):
		t1 = Thread(target=self.fun, args=(self.args))
		t1.start()
		t1.join()

def count(n):
	while n > 0:
		n -= 1
		print n

s = Spracuj(count, 10)
s.spust()