#####################################
# 2.Project IPP - CLS, 2016/2017    #
# script - parser.lib.py          #
# Author: Róbert Kolcún, FIT        #
# <xkolcu00@stud.fit.vutbr.cz>      #
#####################################
import sys
import re
import imp
imp.load_source('cObjects', 'cObjects.lib.py')

from cObjects import *


class TokenParser():

    def __init__(self, inputText):
        output = re.split(
            '(\ |{|}|\(|\)|=|;|::|:|,|\n|\*+|\&\&|\&|\~|\t)',
            inputText
            )
        self.tokenList = [x for x in output if (
            x != "" and x != " " and x != "\t" and x != "\n"
            )]
        self.tokenList.reverse()

    def pop(self):
        if (not self.tokenList):
            return None

        return self.tokenList.pop()

    def push(self, inputToken):
        self.tokenList.append(inputToken)


class ClassParser():

    # States
    CLASS_START = 0
    CLASS_BODY = 1
    CLASS_INHERITANCE = 2
    CLASS_END = 3
    CLASS_DEFINE = 4
    CLASS_SEMICOLON = 5
    DEFINE_USING = 6
    DEFINE_DESTRUCTOR = 7
    DEFINE_CONSTRUCTOR = 8
    DEFINE_NAME = 9
    DEFINE_METHOD = 10
    DATA_TYPE = 11
    METHOD_ARGUMENTS = 12
    METHOD_END = 13

    def __init__(self, tokenController):
        self.tokenController = tokenController

        self.privacyStatus = set({
            "public",
            "private",
            "protected"
            })

        self.singleDataTypes = set({
            "bool",
            "char",
            "char16_t",
            "char32_t",
            "wchar_t",
            "int",
            "float",
            "double",
            "void"
            })

        self.doubleDataTypes = set({
            "signed char",
            "short int",
            "long int",
            "unsigned char",
            "unsigned int",
            "long double"
            })

        self.tripleDataType = set({
            "long long int",
            "unsigned short int",
            "unsigned long int"
            })

        self.kvadraDataType = set({
            "unsigned long long int"
            })

        self.keyWords = set({
            "class",
            "public",
            "protected",
            "private",
            "using",
            "virtual",
            "static",
            "=",
            "*",
            "&",
            "&&",
            "{",
            "}",
            ":",
            "::",
            ";",
            ",",
            "~",
            "int",
            "float",
            "long",
            "bool",
            "char",
            "char16_t",
            "char32_t",
            "wchar_t",
            "signed",
            "short",
            "unsigned",
            "double",
            "void"
            })

        self.allClasses = []

    def getClasses(self):
        return self.allClasses

    def checkName(self, inputName):
        """Return True if inputName is in keyWords list
        """
        return inputName in self.keyWords

    def classExist(self, inputClassName):
        for cppClass in self.allClasses:
            if (inputClassName == cppClass.name):
                return True
        return False

    def getClass(self, inputClassName):
        for cppClass in self.allClasses:
            if (inputClassName == cppClass.name):
                return cppClass

        print(
            "Class \'" + inputClassName + "\'does not exists ",
            file=sys.stderr
            )
        exit(4)

    def parse(self):
        state = self.CLASS_START
        token = self.tokenController.pop()

        mainClass = None
        privacyMethodAttribute = "private"
        nameMethodAttribute = ""
        returnTypeMethodAttribute = ""
        isVirtual = False
        isStatic = False
        isContructor = False
        arguments = []

        while (token != None):

            if (state == self.CLASS_START):
                if (token == "class"):
                    className = self.tokenController.pop()

                    # Check if className is from keyWords
                    if (self.checkName(className)):
                        print(
                            "Expect: className but get: \'" + className + "\'",
                            file=sys.stderr
                            )
                        exit(4)

                    # Check if class already exists
                    if (self.classExist(className)):
                        print(
                            "Expect: Class but get: \'" + className + "\'",
                            file=sys.stderr
                            )
                        exit(4)

                    # Create main class and add to dictionary
                    mainClass = C_Class(className)
                    self.allClasses.append(mainClass)

                    # Add className to singleDataTypes
                    self.singleDataTypes.update({className})

                    token = self.tokenController.pop()
                    if (token == "{"):
                        state = self.CLASS_BODY
                        continue

                    elif (token == ":"):
                        state = self.CLASS_INHERITANCE
                        continue

                    else:
                        print(
                            "Expect: { or : but get: \'" + token + "\'",
                            file=sys.stderr
                            )
                        exit(4)
                else:
                    print(
                        "Expect: class but get: \'" + token + "\'",
                        file=sys.stderr
                        )
                    exit(4)

            elif (state == self.CLASS_INHERITANCE):
                token = self.tokenController.pop()
                if (token in self.privacyStatus):
                    parentName = self.tokenController.pop()

                    # Check if className is from keyWords
                    if (self.checkName(parentName)):
                        print(
                            "Expect: className but get: \'" + parentName + "\'",
                            file=sys.stderr
                            )
                        exit(4)

                    # Check that does not inherite from itself
                    if (parentName == mainClass.name):
                        print(
                            "Class inherit: \'" + parentName + "\' already exists",
                            file=sys.stderr
                            )
                        exit(4)

                    # Check if already inherit from this class
                    if (mainClass.containInheritClass(parentName)):
                        print(
                            "Already inherit: \'" + parentName + "\'",
                            file=sys.stderr
                            )
                        exit(4)

                    parentClass = self.getClass(parentName)

                    parentClass.addChild(mainClass)
                    parenClassInherit = C_Class_Inherit(parentClass)
                    parenClassInherit.setPrivacy(token)
                    mainClass.addInheritClass(parenClassInherit)

                else:
                    parentName = token

                    # Check if className is from keyWords
                    if (self.checkName(parentName)):
                        print(
                            "Expect: className but get: \'" + parentName + "\'",
                            file=sys.stderr
                            )
                        exit(4)

                    # Check that does not inherite from itself
                    if (parentName == mainClass.name):
                        print(
                            "Class inherit: \'" + parentName + "\' already exists",
                            file=sys.stderr
                            )
                        exit(4)

                    # Check if already inherit from this class
                    if (mainClass.containInheritClass(parentName)):
                        print(
                            "Already inherit: \'" + parentName + "\'",
                            file=sys.stderr
                            )
                        exit(4)

                    parentClass = self.getClass(parentName)

                    parentClass.addChild(mainClass)
                    parentClassInherit = C_Class_Inherit(parentClass)
                    mainClass.addInheritClass(parentClassInherit)

                token = self.tokenController.pop()
                if (token == ","):
                    continue

                elif (token == "{"):
                    state = self.CLASS_BODY
                    continue

                else:
                    print(
                        "Expect: , or { but get: \'" + token + "\'",
                        file=sys.stderr
                        )
                    exit(4)

            elif (state == self.CLASS_BODY):
                token = self.tokenController.pop()

                if (token == "}"):
                    state = self.CLASS_END
                    continue

                elif (token in self.privacyStatus):
                    # Set actual privacy for methods/attributes
                    privacyMethodAttribute = token
                    token = self.tokenController.pop()

                    if (token == ":"):
                        state = self.CLASS_DEFINE
                        continue
                    else:
                        print(
                            "Expect: : but get: \'" + token + "\'",
                            file=sys.stderr
                            )
                        exit(4)

                else:
                    state = self.CLASS_DEFINE
                    self.tokenController.push(token)
                    continue

            elif (state == self.CLASS_DEFINE):
                isVirtual = False
                isStatic = False
                isContructor = False
                token = self.tokenController.pop()

                if (token == "virtual"):
                    isVirtual = True
                    state = self.DATA_TYPE
                    continue

                elif (token == "static"):
                    isStatic = True
                    state = self.DATA_TYPE
                    continue

                elif (token == "using"):
                    state = self.DEFINE_USING
                    continue

                elif (token == "~"):
                    state = self.DEFINE_DESTRUCTOR
                    continue

                elif (token == mainClass.name):
                    state = self.DEFINE_CONSTRUCTOR
                    continue

                else:
                    self.tokenController.push(token)
                    state = self.DATA_TYPE
                    continue

            elif (state == self.DEFINE_USING):
                parentName = self.tokenController.pop()
                if (self.checkName(parentName)):
                    print(
                        "Expect: attributeName but get: \'" + parentName + "\'",
                        file=sys.stderr
                        )
                    exit(4)

                # Check if mainClass inheritate from this class
                if (not mainClass.containInheritClass(parentName)):
                    print(
                        "\'using\' from invalid class \'" + parentName + "\' already",
                        file=sys.stderr
                        )
                    exit(4)

                # Expect token "::"
                token = self.tokenController.pop()
                if (token == "::"):
                    variableName = self.tokenController.pop()
                    if (self.checkName(variableName)):
                        print(
                            "Expect: variableName but get: \'" + variableName + "\'",
                            file=sys.stderr
                            )
                        exit(4)

                    token = self.tokenController.pop()
                    if (token == ";"):
                        mainClass.inheritUsing(
                            self.getClass(parentName),
                            variableName,
                            privacyMethodAttribute
                            );

                        state = self.CLASS_SEMICOLON
                        continue
                    else:
                        print(
                            "Expect: ; but get: \'" + token + "\'",
                            file=sys.stderr
                            )
                        exit(4)
                else:
                    print(
                        "Expect: :: but get: \'" + token + "\'",
                        file=sys.stderr
                        )
                    exit(4)

            elif (state == self.DEFINE_DESTRUCTOR):
                mainClassName = self.tokenController.pop()
                if (mainClassName != mainClass.name):
                    print(
                        "Expect: className but get: \'" + mainClassName + "\'",
                        file=sys.stderr
                        )
                    exit(4)

                if (self.tokenController.pop() == "(" and self.tokenController.pop() == ")" and self.tokenController.pop() == ";"):
                    # Add destructor
                    newMethod = mainClass.addMethod(
                        privacyMethodAttribute,
                        "void",
                        "~" + mainClassName,
                        [],
                        False,
                        False
                        )
                    newMethod.setDestructor()

                    state = self.CLASS_SEMICOLON
                    continue
                else:
                    print(
                        "Expect: ();  but get: \'" + "..." + "\'",
                        file=sys.stderr
                        )
                    exit(4)

            elif (state == self.DEFINE_CONSTRUCTOR):
                if (self.tokenController.pop() == "("):
                    token = self.tokenController.pop()
                    if (token == ")"):
                        # Add constructor
                        newMethod = mainClass.addMethod(
                            privacyMethodAttribute,
                            "void",
                            mainClass.name,
                            [],
                            False,
                            False
                        )
                        newMethod.setConstructor()

                        state = self.CLASS_SEMICOLON
                        continue

                    returnTypeMethodAttribute = "void"
                    nameMethodAttribute = mainClass.name
                    isContructor = True
                    self.tokenController.push(token)

                    state = self.METHOD_ARGUMENTS
                    continue
                else:
                    print(
                        "Expect: ( but get: \'" + "..." + "\'",
                        file=sys.stderr
                        )
                    exit(4)

            elif (state == self.DATA_TYPE):
                try:
                    returnTypeMethodAttribute = ""
                    token = self.tokenController.pop()
                    if (token in self.singleDataTypes):
                        returnTypeMethodAttribute = token
                    else:
                        token += " " + self.tokenController.pop()
                        if (token in self.doubleDataTypes):
                            returnTypeMethodAttribute = token
                        else:
                            token += " " + self.tokenController.pop()
                            if (token in self.tripleDataType):
                                returnTypeMethodAttribute = token
                            else:
                                token += " " + self.tokenController.pop()
                                if (token in self.kvadraDataType):
                                    returnTypeMethodAttribute = token
                                else:
                                    print(
                                        "Expect: dataType but get: \'" + token + "\'",
                                        file=sys.stderr
                                        )
                                    exit(4)

                except Exception:
                    print(
                        "Invalid data type",
                        file=sys.stderr
                        )
                    exit(4)

                # Read &/* and name
                token = self.tokenController.pop()
                if (re.search('\*+|&&|&', token)):
                    returnTypeMethodAttribute += token
                else:
                    self.tokenController.push(token)

                state = self.DEFINE_NAME
                continue

            elif (state == self.DEFINE_NAME):
                nameMethodAttribute = self.tokenController.pop()
                if (self.checkName(nameMethodAttribute)):
                    print(
                        "methodAttributeName: dataType but get: \'" + nameMethodAttribute + "\'",
                        file=sys.stderr
                        )
                    exit(4)

                nextToken = self.tokenController.pop()
                if (nextToken == ";"):
                    # Add attribute
                    mainClass.addAttribute(
                        privacyMethodAttribute,
                        returnTypeMethodAttribute,
                        nameMethodAttribute,
                        isStatic
                        )

                    # Nulovanie
                    returnTypeMethodAttribute = ""
                    nameMethodAttribute = ""

                    state = self.CLASS_SEMICOLON
                    continue

                elif (nextToken == "("):
                    state = self.DEFINE_METHOD
                    continue

                else:
                    print(
                        "Expect: : or ( but get: \'" + nextToken + "\'",
                        file=sys.stderr
                        )
                    exit(4)

            elif (state == self.DEFINE_METHOD):
                token = self.tokenController.pop()
                if (token == ")"):
                    # Add new method
                    newMethod = mainClass.addMethod(
                        privacyMethodAttribute,
                        returnTypeMethodAttribute,
                        nameMethodAttribute,
                        [],
                        isStatic,
                        isVirtual
                        )

                    state = self.METHOD_END
                    continue
                else:
                    self.tokenController.push(token)
                    state = self.METHOD_ARGUMENTS
                    continue

            elif (state == self.METHOD_ARGUMENTS):
                try:
                    # Read dataType
                    dataType = ""
                    token = self.tokenController.pop()
                    if (token in self.singleDataTypes):
                        dataType = token
                        if (token == "void"):
                            token = self.tokenController.pop()
                            if (token == ")"):
                                # Check if arguments are empty
                                if (arguments):
                                    print(
                                        "\'void\' argument with another argument",
                                        file=sys.stderr
                                        )
                                    exit(4)

                                # Add new method
                                newMethod = mainClass.addMethod(
                                    privacyMethodAttribute,
                                    returnTypeMethodAttribute,
                                    nameMethodAttribute,
                                    [],
                                    isStatic,
                                    isVirtual
                                    )

                                state = self.METHOD_END
                                continue
                            else:
                                self.tokenController.push(token)
                    else:
                        token += " " + self.tokenController.pop()
                        if (token in self.doubleDataTypes):
                            dataType = token
                        else:
                            token += " " + self.tokenController.pop()
                            if (token in self.tripleDataType):
                                dataType = token
                            else:
                                token += " " + self.tokenController.pop()
                                if (token in self.kvadraDataType):
                                    dataType = token
                                else:
                                    print(
                                        "Expect: dataType but get: \'" + token + "\'",
                                        file=sys.stderr
                                        )
                                    exit(4)

                except Exception:
                    print(
                        "Invalid data type",
                        file=sys.stderr
                        )
                    exit(4)

                # Read &/* and name
                token = self.tokenController.pop()
                if (re.search('\*+|&&|&', token)):
                    dataType += token
                    # Get argument name
                    token = self.tokenController.pop()

                if (self.checkName(token)):
                    print(
                        "Expect: argumentName  but get: \'" + token + "\'",
                        file=sys.stderr
                        )
                    exit(4)

                # Check if attribute already exists
                try:
                    if (arguments[token]):
                        print(
                            "Argument " + token + "\' already exists",
                            file=sys.stderr
                            )
                        exit(4)
                except Exception:
                    pass

                # Create new argument of method
                newArgument = C_Argument(token, dataType)
                arguments.append(newArgument)

                token = self.tokenController.pop()
                if (token == ")"):
                    # Add new method
                    newMethod = mainClass.addMethod(
                        privacyMethodAttribute,
                        returnTypeMethodAttribute,
                        nameMethodAttribute,
                        arguments,
                        isStatic,
                        isVirtual
                        )

                    if (isContructor):
                        if (self.tokenController.pop() == ";"):
                            newMethod.setConstructor()
                            state = self.CLASS_SEMICOLON
                        else:
                            print(
                                "Expect: ;  but get: \'" + "..." + "\'",
                                file=sys.stderr
                                )
                            exit(4)

                    else:
                        state = self.METHOD_END
                    continue

                elif (token == ","):
                    continue

                else:
                    print(
                        "Expect: ) or , but get: \'" + token + "\'",
                        file=sys.stderr
                        )
                    exit(4)

            elif (state == self.METHOD_END):
                token = self.tokenController.pop()
                if (token == "{"):
                    if (self.tokenController.pop() == "}"):
                        state = self.CLASS_SEMICOLON
                        continue

                    else:
                        print(
                            "Expect: } but get: \'" + token + "\'",
                            file=sys.stderr
                            )
                        exit(4)

                elif (token == "=" and isVirtual):
                    if (self.tokenController.pop() == "0" and self.tokenController.pop() == ";"):
                        # Set pureVirtual method and abstract to class
                        newMethod.setPureVirtual()

                        state = self.CLASS_SEMICOLON
                        continue
                    else:
                        print(
                            "Expect: =0; but get: \'" + token + "\'",
                            file=sys.stderr
                            )
                        exit(4)
                else:
                    print(
                        "Expect: { or = but get: \'" + token + "\'",
                        file=sys.stderr
                        )
                    exit(4)

            elif (state == self.CLASS_SEMICOLON):
                # Ignore all semicolons
                token = self.tokenController.pop()
                while (token == ";"):
                    token = self.tokenController.pop()

                # Null variables
                returnTypeMethodAttribute = ""
                nameMethodAttribute = ""
                arguments = []

                self.tokenController.push(token)
                state = self.CLASS_BODY

            elif (state == self.CLASS_END):
                if (self.tokenController.pop() == ";"):
                    mainClass = None
                    privacyMethodAttribute = "private"

                    # Ignore ; at the end
                    token = self.tokenController.pop()
                    while (token == ";"):
                        token = self.tokenController.pop()

                    state = self.CLASS_START
                    continue

                else:
                    print(
                        "Expect: ; but get: \'" + "..." + "\'",
                        file=sys.stderr
                        )
                    exit(4)
            else:
                print(
                    "Unknown state!",
                    file=sys.stderr
                    )
                exit(4)
