#!/usr/bin/env python3

def lev(a, b):
    """Recursively calculate the Levenshtein edit distance between two strings,
    a and b.
    Returns the edit distance.
    """
    if("" == a):
        return len(b)   # returns if a is an empty string
    if("" == b):
        return len(a)   # returns if b is an empty string
    return min(lev(a[:-1], b[:-1])+(a[-1] != b[-1]), lev(a[:-1], b)+1, lev(a, b[:-1])+1)

print(lev('abracadabra', 'abba'))

# EditaÄnĂ­ vzdĂĄlenostĂ­ mĹŻĹžeme mÄĹit vzdĂĄlenost, odliĹĄnost, dvou ĹetÄzcĹŻ.
# V pĹĂ­padÄ editaÄnĂ­ vzdĂĄlenosti je vzdĂĄlenost definovĂĄna jako poÄet operacĂ­
# smazĂĄnĂ­ znaku, vloĹženĂ­ znaku a zĂĄmÄny znaku tak, aby se jeden ĹetÄzec
# transformoval na druhĂ˝.
# VĂ˝poÄet mĹŻĹžeme popsat nĂĄsledujĂ­cĂ­m zpĹŻsobem:
# MĂĄme dĂĄny dva ĹetÄzce a = a_1 a_2... a_m a b = b_1 b_2... b_n.
# EditaÄnĂ­ vzdĂĄlenost d_i,j pĹedpony dĂŠlky i ĹetÄzce a
# a pĹedpony dĂŠlky j ĹetÄzce b mĹŻĹžeme vypoÄĂ­tat jako
# 1: d_i 1,j + 1 d d i,j = min i,j 1 + 1 (1) d i 1,j 1 + 1 pokud a i b j d i 1,j 1
# pokud a i = b j pro 1 i m a 1 j n.
# PrvnĂ­ hodnota odpovĂ­dĂĄ vymazĂĄnĂ­ j-tĂŠho znaku z prvnĂ­ho ĹetÄzce, druhĂĄ hodnota
# odpovĂ­dĂĄ vloĹženĂ­ znaku na (j+1)-nĂ­ pozici do prvnĂ­ho ĹetÄzce, tĹetĂ­ hodnota
# odpovĂ­dĂĄ vĂ˝mÄnÄ j-tĂŠho a i-tĂŠho znaku. ÄtvrtĂĄ hodnota se uplatnĂ­ jen
# v pĹĂ­padÄ, Ĺže jsou znaky v obou ĹetÄzcĂ­ch shodnĂŠ.
# Na konci hodnota d_mn udĂĄvĂĄ editaÄnĂ­ vzdĂĄlenost ĹetÄzcĹŻ a a b. DĂĄle je
# pochopitelnÄ definovĂĄno: d 0,0 = 0 d i,0 = i, pro 1 i m d 0,j = j, pro 1 j n Z
# pĹedchozĂ­ho textu je patrnĂŠ, Ĺže celĂ˝ vĂ˝poÄet mĹŻĹžeme implementovat jako
# rekurzivnĂ­ funkci, kde vĂ˝poÄet hodnoty d mn se rozpadne na vĂ˝poÄet hodnot d m
# 1,n, d m,n 1, pĹĂ­padnÄ d m 1,n 1 a tak dĂĄle, aĹž k definovanĂ˝m koncovĂ˝m hodnotĂĄm
# d 0,0 atd. PĹĂ­klad Hodnota d ij udĂĄvĂĄ editaÄnĂ­ vzdĂĄlenost mezi prvnĂ­mi i znaky
# ĹetÄzce a a prvnĂ­mi j znaky ĹetÄzce b. Tuto hodnotu vĹĄak neumĂ­me vypoÄĂ­tat
# pĹĂ­mo, ale vypoÄteme ji na zĂĄkladÄ znalostĂ­ vzdĂĄlenostĂ­ mezi prvnĂ­mi i 1 znaky
# ĹetÄzce a a prvnĂ­mi j 1 znaky ĹetÄzce b. TĂ­mto zpĹŻsobem postupnÄ redukujeme
# problĂŠm aĹž na ĂşroveĹ pĹĂ­padĹŻ, kdy je hodnota znĂĄma pĹĂ­mo, napĹĂ­klad pro d 0,0 =
# 0. PĹedpoklĂĄdejme, Ĺže a = abcabba a b = cbabac, odtud m = 7 a n = 6. EditaÄnĂ­
# vzdĂĄlenost tÄchto dvou ĹetÄzcĹŻ se bude poÄĂ­tat jako hodnota d 7,6 d 7,6 = min d
# 6,6 + 1 d 7,5 + 1 d 6,5 + 1 protoĹže a 7 b 6 tj. a c EditaÄnĂ­ vzdĂĄlenost celĂ˝ch
# ĹetÄzcĹŻ a a b je jinak ĹeÄeno rovna editaÄnĂ­ vzdĂĄlenosti prvnĂ­ch 7 znakĹŻ
# ĹetÄzce a a prvnĂ­ch 6 znakĹŻ ĹetÄzce b. Hodnotu d 6,6 1 PĹedponou dĂŠlky i
# ĹetÄzce a se myslĂ­ prvnĂ­ch i znakĹŻ od zaÄĂĄtku ĹetÄzce. 9
