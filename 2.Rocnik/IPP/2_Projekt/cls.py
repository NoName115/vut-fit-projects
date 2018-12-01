#!/usr/bin/python3
#####################################
# 2.Project IPP - CLS, 2016/2017    #
# script - cls.py                   #
# Author: Róbert Kolcún, FIT        #
# <xkolcu00@stud.fit.vutbr.cz>      #
#####################################
import imp
imp.load_source('parserLib', 'parser.lib.py')
imp.load_source('argvParser', 'argumentParser.lib.py')

from parserLib import *
from argvParser import *


try:
    # Command line argument parser and file operator
    argvParserAndFile = ArgumentParserAndFile()
    argvParserAndFile.parse()

    # Parset input file to tokens
    tokenParser = TokenParser(argvParserAndFile.getInputData())

    # Parse input file and fill classes with data
    mainParser = ClassParser(tokenParser)
    mainParser.parse()

    # Get filled classes
    allClasses = mainParser.getClasses()

    # Do inheritation for every class
    for cppClass in allClasses:
        cppClass.inheritEverything()

    # Print output to XML
    argvParserAndFile.resolveOutput(allClasses)

    # DEBUG
    # Print all classes
    #for cppClass in allClasses:
    #    print(cppClass)

except Exception as err:
    print(
        "Error: " + str(err),
        file=sys.stderr
        )
    exit(100)
