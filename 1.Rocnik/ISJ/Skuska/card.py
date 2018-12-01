# Author: Róbert Kolcún, FIT
# <xkolcu00@stud.fit.vutbr.cz>
# Priprava na skusku z ISJ, 2015/2016


farba = ['S','D','C','H']
vysKarty = ['J','K','Q','A']

class Card():
	def __init__(self, karta, farba):
		self.hodnota = karta
		self.farba = farba
	def __str__(self):
		return str(self.hodnota) + " " + str(self.farba)

class Deck():
	def __init__(self):
		self.balicek = []
		for f in farba:
			for i in range(2, 11):
				self.balicek.append(Card(i, f))
			for k in vysKarty:
				self.balicek.append(Card(k, f))

	def __call__(self):
		for i in self.balicek:
			yield i

	def removecard(self, karta, farba):
		for i in range(len(self.balicek) - 1):
			if (self.balicek[i].hodnota is karta and self.balicek[i].farba is farba):
				del self.balicek[-(len(self.balicek) - i)]

	def specifictype(self, typ):
		for i in self.balicek:
			if (i.hodnota == typ):
				yield i

	def specificcolor(self, color):
		for i in self.balicek:
			if (i.farba == color):
				yield i

my_deck = Deck()
#for x in my_deck(): print x
my_deck.removecard(2, 'S')
for x in my_deck(): print x
#for x in my_deck.specifictype(2): print x
#for x in my_deck.specificcolor('S'): print x
		