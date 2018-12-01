#!/usr/bin/env python3

# Author: Róbert Kolcún - FIT
# <xkolcu00@stud.fit.vutbr.cz>
# Login: xkolcu00
# Implementacia triedz Polznomial


class Polynomial(object):

    #Inicializacna metoda pre vytvorenie pola polynomov
    def __init__(self, *args, **kwargs):
        if len(args) > 0:

            #Pre (a,b,...)
            if type(args[0]) == type(0):

                self.pole = []

                for key in range(len(args)):
                    self.pole.append(args[key])
            else:
                #Pre [a,b,...]
                pole = args[0]

                self.pole = pole

        #Pre [x1=a,x2=b,...]
        if len(kwargs) > 0:
            pole = []

            for key, value in kwargs.items():
                pole.append(key[1:])

            maximum = int(max(pole))
            pole = [0]*(maximum + 1)

            for key, value in kwargs.items():
                pole[int(key[1:])] = value

            self.pole = pole


    #Metoda pre scitanie 2 Polynomov
    #Vysledok ulozeni do self.pole
    def __add__(self, other):

        buffCount = [];
        buffWrite = [];

        for i in range(len(self.pole)):
            buffCount.append(self.pole[i])

        for i in range(len(other.pole)):
            buffWrite.append(other.pole[i])

        if len(buffCount) > len(other.pole):
            buffCount = [];
            buffWrite = [];

            for i in range(len(other.pole)):
                buffCount.append(other.pole[i])

            for i in range(len(self.pole)):
                buffWrite.append(self.pole[i])

        for i in range(len(buffCount)):
            buffWrite[i] += buffCount[i]

        #self.pole = buffWrite

        return Polynomial(buffWrite)


    #Metoda pre umocnenie zadaneho polynomu
    #power - cislo mocniny > 0
    #vysledok ulozeni do self.pole
    def __pow__(self, power, modulo=None):

        pole = [0]*((len(self.pole) - 1) * power + 1)

        doc = [];

        for i in range(len(self.pole)):
            doc.append(self.pole[i])

        #Pouziva sa pre doc hranicu, aby nesiel do nul
        hranica = len(self.pole)

        for i in range(power - 1):
            for j in range(hranica):
                for k in range(len(self.pole)):
                    pole[j+k] += doc[j] * self.pole[k]

            doc = [0]*len(pole)
            for l in range(len(pole)):
                doc[l] = pole[l]

            pole = [0]*((len(self.pole) - 1) * power + 1)
            hranica += (len(self.pole) - 1)

        return Polynomial(doc)


    #Metoda pre derivaciu polynomu
    #Vrati vysledok ako novy polynom
    def derivative(self):

        buff = []

        for i in range(len(self.pole)):
            buff.append(self.pole[i] * i)

        buff.pop(0)

        return Polynomial(buff)


    #Metoda pre vypocitanie hodnoty polynomu
    #*args moznost zadanie 1/2 cisel
    #len(args) == 1, hodnota args zadana do X pre vypocet vysledku
    #len(args) == 2, rozdiel hodnot polynomov po zadani do X, 2.-1.Polynom
    #Navratova hodnota (int)
    def at_value(self, *args):
        if len(args) == 1:
            vys = 0

            for i in range(len(self.pole)):
                vys += self.pole[i]*(args[0] ** i)

            return vys

        elif len(args) == 2:
            vys1 = 0
            vys2 = 0

            for i in range(len(self.pole)):
                vys1 += self.pole[i]*(args[0] ** i)
                vys2 += self.pole[i]*(args[1] ** i)

            return vys2 - vys1

    def __eq__(self, other):
        if (str(self) == str(other)):
            return True;
        else:
            return False;


    #Metoda pre vypis polynomu
    #Fromat vystupu (string): ax^n + bx^(n-1) + ... + d
    def __repr__(self):

        vys = ""

        countZero = 0
        maximum = len(self.pole)

        #Skladanie formatu polynomu
        for i in range(len(self.pole) - 1, 0, -1):
            if self.pole[i] == 0:
                countZero += 1
                continue

            if self.pole[i] > 0:
                if i != maximum - 1 - countZero:
                    vys += " + "
            else:
                if i != maximum - 1 - countZero:
                    vys += " - "
                else:
                    vys += "- "

            if (abs(self.pole[i])) != 1:
                vys += str(abs(self.pole[i]))

            vys += "x"

            if i != 1:
                vys += "^"
                vys += str(i)

        if (len(self.pole) == 0):
            vys += "0"
            return vys;

        #Pridanie absolutny clen
        if self.pole[0] > 0:
            if (vys != ""):
                vys += " + "
            vys += str(self.pole[0])
        elif self.pole[0] < 0:
            if (vys != ""):
                vys += " - "
            else:
                vys += "-"
            vys += str(abs(self.pole[0]))

        if vys == "":
            vys += "0"

        return vys


def test():
    assert str(Polynomial(0,1,0,-1,4,-2,0,1,3,0)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial([-5,1,0,-1,4,-2,0,1,3,0])) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x - 5"
    assert str(Polynomial(x7=1, x4=4, x8=3, x9=0, x0=0, x5=-2, x3= -1, x1=1)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial(x2=0)) == "0"
    assert str(Polynomial(x0=0)) == "0"
    assert Polynomial(x0=2, x1=0, x3=0, x2=3) == Polynomial(2,0,3)
    assert Polynomial(x2=0) == Polynomial(x0=0)
    assert str(Polynomial(x0=1)+Polynomial(x1=1)) == "x + 1"
    assert str(Polynomial([-1,1,1,0])+Polynomial(1,-1,1)) == "2x^2"
    pol1 = Polynomial(x2=3, x0=1)
    pol2 = Polynomial(x1=1, x3=0)
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(Polynomial(x0=-1,x1=1)**1) == "x - 1"
    assert str(Polynomial(x0=-1,x1=1)**2) == "x^2 - 2x + 1"
    pol3 = Polynomial(x0=-1,x1=1)
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(Polynomial(x0=2).derivative()) == "0"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative()) == "6x^2 + 3"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative().derivative()) == "12x"
    pol4 = Polynomial(x3=2,x1=3,x0=2)
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert Polynomial(-2,3,4,-5).at_value(0) == -2
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3) == 20
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3,5) == 44
    pol5 = Polynomial([1,0,-2])
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-1,3.6) == -23.92
    assert pol5.at_value(-1,3.6) == -23.92

if __name__ == '__main__':
    test()
