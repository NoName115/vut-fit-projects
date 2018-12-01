#####################################
# 2.Project IPP - CLS, 2016/2017    #
# script - argumentParser.lib.py    #
# Author: Róbert Kolcún, FIT        #
# <xkolcu00@stud.fit.vutbr.cz>      #
#####################################
import sys
from xml.dom.minidom import *


class ArgumentParserAndFile():

    def __init__(self):
        self.inputFile = sys.stdin
        self.outputFile = sys.stdout
        self.xmlSpaces = " " * 4     # 4x spaces
        self.isDetails = False
        self.detailsData = None

        self.inputArg = False
        self.outputArg = False
        self.prettyArg = False
        self.detailsArg = False

    def parse(self):
        self.argv = sys.argv[1:]

        for argument in self.argv:
            if (len(self.argv) == 1 and (self.argv[0] == "--help" or self.argv[0] == "-h")):
                print(
                    "IPP - 2.Project_CLS\n" +
                    "Author: xkolcu00 <xkolcu00@stud.fit.vutbr.cz>\n" +
                    '\t{0:20} {1:1}\n'.format("--help", "This help output") +
                    '\t{0:20} {1:1}\n'.format("--input=file", "Input file") +
                    '\t{0:20} {1:1}\n'.format("--output=file", "Output file") +
                    '\t{0:20} {1:1}\n'.format("--pretty-xml=k", "k mean number of spaces in XML format") +
                    '\t{0:20} {1:1}'.format("--details=class", "Print detail output of class")
                    )
                exit(0)
            else:
                try:
                    splitArgument = argument.split("=")
                    if (len(splitArgument) > 2):
                        print(
                            "Wrong input argumets",
                            file=sys.stderr
                            )
                        exit(1)

                    if (splitArgument[0] == "--input" and not self.inputArg):
                        try:
                            if (len(splitArgument) != 2 or not splitArgument[1]):
                                print(
                                    "Wrong input arguments",
                                    file=sys.stderr
                                    )
                                exit(1)

                            self.inputArg = True
                            self.inputFile = open(splitArgument[1], 'r+')
                        except IOError:
                            print(
                                "I/O error of input file",
                                file=sys.stderr
                                )
                            exit(2)

                    elif (splitArgument[0] == "--output" and not self.outputArg):
                        try:
                            if (len(splitArgument) != 2 or not splitArgument[1]):
                                print(
                                    "Wrong input arguments",
                                    file=sys.stderr
                                    )
                                exit(1)

                            self.outputArg = True
                            self.outputFile = open(splitArgument[1], 'w+')
                        except IOError:
                            print(
                                "I/O error of output file",
                                file=sys.stderr
                                )
                            exit(3)

                    elif (splitArgument[0] == "--pretty-xml" and len(splitArgument) == 2 and not self.prettyArg):
                        if (not splitArgument[1]):
                                print(
                                    "Wrong input arguments",
                                    file=sys.stderr
                                    )
                                exit(1)

                        self.prettyArg = True
                        self.xmlSpaces = " " * int(splitArgument[1])

                    elif (splitArgument[0] == "--details" and not self.detailsArg):
                        self.detailsArg = True
                        self.isDetails = True
                        if (len(splitArgument) == 2 and splitArgument[1]):
                            self.detailsData = [splitArgument[1]]

                    else:
                        print(
                            "Wrong input arguments",
                            file=sys.stderr
                            )
                        exit(1)

                except Exception as err:
                    print(
                        "Error: " + repr(err),
                        file=sys.stderr
                        )
                    exit(1)

    def getInputData(self):
        inputData = self.inputFile.read()
        self.inputFile.close()
        return inputData

    def writeToOutputFile(self, outputData):
        self.outputFile.write(outputData)
        self.outputFile.close()

    def resolveOutput(self, classesData):
        if (self.isDetails):
            self.printDetails(classesData)
        else:
            self.printTree(classesData)

    def printTree(self, classesData):
        """
        """
        def createClassNode(document, cppClass):
            """
            """
            classNode = document.createElement('class')
            classNode.setAttribute("name", cppClass.name)
            classNode.setAttribute("kind", cppClass.kind)
            return classNode

        def addChildren(document, cppClass, classNode):
            """
            """
            if (cppClass.children):
                for key, value in cppClass.children.items():
                    inheritNode = createClassNode(doc, value)

                    # Recursively addChild class
                    addChildren(document, value, inheritNode)

                    classNode.appendChild(inheritNode)

        doc = Document()
        modelNode = doc.createElement('model')

        for cppClass in classesData:
            if (cppClass.inheritFrom):
                continue

            classNode = createClassNode(doc, cppClass)
            addChildren(doc, cppClass, classNode)
            modelNode.appendChild(classNode)

        doc.appendChild(modelNode)
        print(
            doc.toprettyxml(indent=self.xmlSpaces, encoding='utf-8').decode('UTF-8'),
            file=self.outputFile
            )

    def printDetails(self, classesData):
        # Create dictionary for cppClasses
        classesDic = {}
        for cppClass in classesData:
            classesDic.update({cppClass.name: cppClass})

        # Create XML document
        doc = Document()
        modelNode = None

        # Fill detailsData with all className
        if (not self.detailsData):
            self.detailsData = [str(cClass.name) for cClass in classesData]
            modelNode = doc.createElement('model')

        # Check that all classes exists
        for classDetailName in self.detailsData:
            if (not classDetailName in classesDic):
                print(
                    "Wrong input '--details' argument",
                    file=sys.stderr
                    )
                exit(1)


        # Create inheritance nodes
        for detailName in self.detailsData:
            actualCppClass = classesDic[detailName]
            mainClassNode = doc.createElement('class')
            mainClassNode.setAttribute('name', actualCppClass.name)
            mainClassNode.setAttribute('kind', actualCppClass.kind)

            # Create privacy nodes
            publicNode = doc.createElement('public')
            protectedNode = doc.createElement('protected')
            privateNode = doc.createElement('private')
            privacyDic = {
                "public": [publicNode, False],
                "protected": [protectedNode, False],
                "private": [privateNode, False]
                }

            # Create inheritance node
            if (actualCppClass.inheritFrom):
                inheritNode = doc.createElement('inheritance')
                # Create from nodes
                for inheritClass in actualCppClass.inheritFrom:
                    parentNode = doc.createElement('from')
                    parentNode.setAttribute('name', inheritClass.cClass.name)
                    parentNode.setAttribute('privacy', inheritClass.privacy)
                    inheritNode.appendChild(parentNode)

                mainClassNode.appendChild(inheritNode)

            # Create attributes
            for privacyAttr, attributeDic in actualCppClass.attributes.items():
                privacyDic[privacyAttr][1] = True
                attributesNode = doc.createElement('attributes')

                # Loop attributeDic and create every attribute
                for attrName, attribute in attributeDic.items():
                    # Dont add attribute if is unprintable
                    if (attribute.printable == False):
                        continue

                    attrNode = doc.createElement('attribute')
                    attrNode.setAttribute('name', attrName)
                    attrNode.setAttribute('type', attribute.dataType)
                    attrNode.setAttribute('scope', attribute.scope)

                    # Add fromNode if needed
                    if (attribute.inheritFrom):
                        inheritNode = doc.createElement('from')
                        inheritNode.setAttribute('name', attribute.inheritFrom.name)
                        attrNode.appendChild(inheritNode)

                    attributesNode.appendChild(attrNode)

                # Add attributesNode to privacyNode
                if (attributesNode.childNodes):
                    privacyDic[privacyAttr][0].appendChild(attributesNode)

            # Create methods
            for privacyMethod, methodList in actualCppClass.methods.items():
                privacyDic[privacyMethod][1] = True
                methodsNode = doc.createElement('methods')

                # Loop methodList and create every method
                for method in methodList:
                    # Dont print attribute if is unprintable
                    if (method.printable == False):
                        continue

                    methodNode = doc.createElement('method')
                    methodNode.setAttribute('name', method.name)
                    methodNode.setAttribute('type', method.dataType)
                    methodNode.setAttribute('scope', method.scope)

                    # Virtual node
                    if (method.isVirtual):
                        virtualNode = doc.createElement('virtual')
                        virtualNode.setAttribute('pure', 'yes' if (method.isPureVirtual) else 'no')
                        methodNode.appendChild(virtualNode)

                    # Inherit from
                    if (method.inheritFrom):
                        fromNode = doc.createElement('from')
                        fromNode.setAttribute('name', method.inheritFrom.name)
                        methodNode.appendChild(fromNode)

                    # ArgumentsNode
                    argumentsNode = doc.createElement('arguments')
                    for argum in method.arguments:
                        argumNode = doc.createElement('argument')
                        argumNode.setAttribute('name', argum.name)
                        argumNode.setAttribute('type', argum.dataType)
                        argumentsNode.appendChild(argumNode)

                    # Add argumentsNode
                    methodNode.appendChild(argumentsNode)
                    # Add method
                    methodsNode.appendChild(methodNode)

                # Add methodsNode to privacyNode
                if (methodsNode.childNodes):
                    privacyDic[privacyMethod][0].appendChild(methodsNode)

            # Add privacyNodes to mainClassNode
            for key, value in privacyDic.items():
                if (value[0].childNodes):
                    mainClassNode.appendChild(value[0])

            if (modelNode):
                modelNode.appendChild(mainClassNode)

        # Add classNode
        if (modelNode):
            doc.appendChild(modelNode)
        else:
            doc.appendChild(mainClassNode)

        # Print XML document
        print(
            doc.toprettyxml(indent=self.xmlSpaces, encoding='utf-8').decode('UTF-8'),
            end='',
            file=self.outputFile
            )
