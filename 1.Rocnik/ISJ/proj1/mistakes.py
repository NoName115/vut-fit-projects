#!/usr/bin/env python3

# Author: Róbert Kolcún - FIT
# <xkolcu00@stud.fit.vutbr.cz>
# Login: xkolcu00

from __future__ import division, print_function
import re
import itertools

def words(text): return set(text.split())

winners = words('''washington adams jefferson jefferson madison madison monroe 
    monroe adams jackson jackson van-buren harrison polk taylor pierce buchanan 
    lincoln lincoln grant grapartnt hayes garfield cleveland harrison cleveland mckinley
    mckinley roosevelt taft wilson wilson harding coolidge hoover roosevelt 
    roosevelt roosevelt roosevelt truman eisenhower eisenhower kennedy johnson nixon 
    nixon carter reagan reagan bush clinton clinton bush bush obama obama''')

losers = words('''clinton jefferson adams pinckney pinckney clinton king adams 
    jackson adams clay van-buren van-buren clay cass scott fremont breckinridge 
    mcclellan seymour greeley tilden hancock blaine cleveland harrison bryan bryan 
    parker bryan roosevelt hughes cox davis smith hoover landon willkie dewey dewey 
    stevenson stevenson nixon goldwater humphrey mcgovern ford carter mondale 
    dukakis bush dole gore kerry mccain romney''')

winners & losers

losers = losers - winners

def mistakes(regex, winners, losers):
    "The set of mistakes made by this regex in classifying winners and losers."
    return ({"Should have matched: " + W 
             for W in winners if not re.search(regex, W)} |
            {"Should not have matched: " + L 
             for L in losers if re.search(regex, L)})

def verify(regex, winners, losers): 
    assert not mistakes(regex, winners, losers)
    return True

xkolcu00 = "[gikuj]..n|a.[alt]|[pivo].l|i..o|[jocy]e|sh|di|oo"

#f = open('xkolcu00.mistakes', 'w')
print(mistakes(xkolcu00, winners, losers))#, file = f)
