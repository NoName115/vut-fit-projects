# Author: Róbert Kolcún, FIT
# <xkolcu00@stud.fit.vutbr.cz>
# Priprava na skusku z ISJ, 2015/2016


class slovnik(dict):
    def __init__(self, *args, **kw):
        super(slovnik,self).__init__(*args, **kw)
        self.itemlist = super(slovnik,self).keys()
 
    def __setitem__(self, key, value):
        self.itemlist.append(key)
        super(slovnik,self).__setitem__(key, value)
 
    def __iter__(self):
        return iter(self.itemlist)
 
    def __add__(self):
        pass
 
    def __str__(self):
        return str(self.itemlist)
 
    def keys(self):
        return self.itemlist
 
    def values(self):
        return [self[key] for key in self]
 
    def itervalues(self):
        return (self[key] for key in self)

x = slovnik()
x[0] = 1.0 #__setitem__
x[3] = 2.0
x[5] = 3.0
print(len(x)) #vypise 6, __len__
print (v for v in x) #vypise [1.0, 0.0, 0.0, 2.0, 0.0, 3.0], __iter__
a = [1.0, 0.0, 0.0, 2.0, 0.0, 3.0]
#print (x*x) #vypise 10, skalarni soucin, __mul__
#print (x*a) #funguje i pro seznamy