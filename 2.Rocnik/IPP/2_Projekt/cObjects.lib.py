#####################################
# 2.Project IPP - CLS, 2016/2017    #
# script - cObjects.lib.py          #
# Author: Róbert Kolcún, FIT        #
# <xkolcu00@stud.fit.vutbr.cz>      #
#####################################
import sys
import copy


class C_Class():

    def __init__(self, name):
        self.name = name
        self.kind = "concrete"

        # key: value = {child.name: childClass}
        self.children = {}

        # key = (protected, private, public)
        # value = [method_1, method_2]
        self.methods = {}

        # key = (protected, private, public)
        # value = {attributeName: attribute_object}
        self.attributes = {}

        # key: value = {attributeName: usingClass}
        self.usingAttr = {}
        self.inheritFrom = []

    def __setAbstract(self):
        for privacy, methodList in self.methods.items():
            for method in methodList:
                if (method.isPureVirtual):
                    self.kind = "abstract"

    def setConcrete(self):
        self.kind = "concrete"

    def addChild(self, child):
        self.children.update({child.name: child})

    def addUsing(self, attributeName, usingClass):
        self.usingAttr.update({attributeName: usingClass})

    def containMethodAttribute(self, name, returnObject=False):
        for key, value in self.attributes.items():
            if (name in value):
                if (returnObject):
                    return value[name]
                else:
                    return True

        for key, value in self.methods.items():
            for method in value:
                if (name == method.name):
                    if (returnObject):
                        return method
                    else:
                        return True

        if (returnObject):
            return None
        else:
            return False

    def addMethod(self, privacy, returnDataType, methodName, arguments, isStatic, isVirtual):
        # Check if attribute and method already exists
        if (self.containAttribute(methodName)):
            print(
                "Name" + methodName + " already exists",
                file=sys.stderr
                )
            exit(4)

        if (self.containMethod(returnDataType, methodName, arguments)):
            print(
                "Method" + methodName + " already exists",
                file=sys.stderr
                )
            exit(4)

        newMethod = C_Method(
            methodName,
            returnDataType,
            "static" if (isStatic) else "instance",
            arguments
            )

        if (isVirtual):
            newMethod.setVirtual()

        if (privacy not in self.methods):
            self.methods.update({privacy: []})

        self.methods[privacy].append(newMethod)

        return newMethod

    def containMethod(self, returnDataType, methodName, arguments, returnMethod=False):
        """
        arguments - list
        """
        def sameArguments(argsFirst, argsSecond):
            """Bla bla bla
            """
            if (len(argsFirst) != len(argsSecond)):
                return False

            for first, second in zip(argsFirst, argsSecond):
                if (first.dataType != second.dataType):
                    return False
            return True

        for privacy, methodList in self.methods.items():
            for method in methodList:
                # Method with same name already exists
                if (method.name == methodName):
                    # Same returnDataType
                    if (returnDataType == method.dataType):
                        # Check all arguments
                        if (sameArguments(arguments, method.arguments)):
                            if (returnMethod):
                                return method
                            else:
                                return True
        if (returnMethod):
            return None
        else:
            return False

    def addAttribute(self, privacy, attributeDataType, attributeName, isStatic):
        # Check if attribute already exists
        if (self.containMethodAttribute(attributeName)):
            print(
                "Name: " + attributeName + " already exists",
                file=sys.stderr
                )
            exit(4)

        # Create attribute
        newAttribute = C_Attribute(
            attributeName,
            attributeDataType,
            "static" if (isStatic) else "instance"
            )

        if (privacy not in self.attributes):
            self.attributes.update({privacy: {}})

        self.attributes[privacy].update({
            attributeName: newAttribute
            })

    def getAttribute(self, attributeName):
        for key, value in self.attributes.items():
            if (attributeName in value):
                return self.attributes[key][attributeName]
        return None

    def containAttribute(self, attributeName):
        for key, value in self.attributes.items():
            if (attributeName in value):
                return True
        return False

    def isAttributePrivate(self, attributeName):
        for key, value in self.attributes.items():
            if (key == "private"):
                if (attributeName in value):
                    return True
        return False

    def addInheritClass(self, inheritFrom):
        self.inheritFrom.append(inheritFrom)

    def inheritUsing(self, parentClass, attributeName, privacy):
        # Check that using Attribute exist in parentClass
        if (not parentClass.containAttribute(attributeName)):
            print(
                "Class: \'" + parentClass.name +
                "\' does not contain variable: \'" + attributeName + "\'",
                file=sys.stderr
                )
            exit(4)

        # Check if attribute does not exists in this class already
        if (self.containAttribute(attributeName)):
            print(
                "Attribute: " + attributeName + " already exists",
                file=sys.stderr
                )
            exit(21)

        # Check if attribute is not private
        if (parentClass.isAttributePrivate(attributeName)):
            print(
                "Attribute \'" + attributeName + "\' is private",
                file=sys.stderr
                )
            exit(4)

        # Add attribute
        if (privacy not in self.attributes):
            self.attributes.update({privacy: {}})

        newAttribute = copy.copy(parentClass.getAttribute(attributeName))
        newAttribute.setInheritFrom(parentClass)

        # Add to dictionaries
        self.attributes[privacy].update({
            attributeName: newAttribute
            })
        self.usingAttr.update({
            attributeName: parentClass
            })

    def inheritEverything(self):
        for inheritClass in self.inheritFrom:
            # Inherit Methods
            for privacy, methodList in inheritClass.cClass.methods.items():
                for method in methodList:
                    # Check if method is constructor or destructor
                    if (method.isContructor or method.isDestructor):
                        continue

                    # Check if method-attribute already exists
                    if (self.containAttribute(method.name)):
                        if (self.getAttribute(method.name).inheritFrom != None):
                            print(
                                "Conflict - Method" + method.name + " already exists",
                                file=sys.stderr
                                )
                            exit(21)
                        else:
                            continue

                    # Check if method already exists
                    checkmethod = self.containMethod(
                        method.dataType,
                        method.name,
                        method.arguments,
                        True
                        )
                    if (checkmethod):
                        if (checkmethod.inheritFrom != None):
                            print(
                                "Conflict - Method" + checkmethod.name + " already exists",
                                file=sys.stderr
                                )
                            exit(21)
                        else:
                            continue

                    # Inherit method
                    inheritMethod = copy.copy(method)
                    if (not inheritMethod.inheritFrom):
                        inheritMethod.setInheritFrom(inheritClass.cClass)

                    # Set unprintable if attribute is inheritated as private
                    if (privacy == "private"):
                        inheritMethod.setUnprintable()

                    # Add method
                    if (inheritClass.privacy not in self.methods):
                        self.methods.update({inheritClass.privacy: []})

                    self.methods[inheritClass.privacy].append(inheritMethod)

            # Inherit attributes
            for privacy, attributeDic in inheritClass.cClass.attributes.items():
                for attrName, attribute in attributeDic.items():
                    if (attrName in self.usingAttr):
                        continue

                    # Check if method-attribute already exists
                    if (self.containMethodAttribute(attrName)):
                        # Check if it is attribute or method
                        if (not self.getAttribute(attrName)):
                            print(
                                "Conflict - Attribute/Method" + attrName + " already exists",
                                file=sys.stderr
                                )
                            exit(21)
                        else:
                            if (self.getAttribute(attrName).inheritFrom != None):
                                print(
                                    "Conflict - Attribute" + attrName + " already exists",
                                    file=sys.stderr
                                    )
                                exit(21)
                            else:
                                continue

                    # Inherit attribute
                    inheritAttribute = copy.copy(attribute)
                    if (not inheritAttribute.inheritFrom):
                        inheritAttribute.setInheritFrom(inheritClass.cClass)

                    # Set unprintable if attribute is inheritated as private
                    if (inheritClass.cClass.isAttributePrivate(inheritAttribute.name)):
                        inheritAttribute.setUnprintable()

                    # Add attribute
                    if (inheritClass.privacy not in self.attributes):
                        self.attributes.update({inheritClass.privacy: {}})

                    self.attributes[inheritClass.privacy].update({
                        inheritAttribute.name: inheritAttribute
                        })


        self.__setAbstract()

    def containInheritClass(self, inheritClassName):
        for inheritClass in self.inheritFrom:
            if (inheritClassName == inheritClass.cClass.name):
                return True

        return False

    def __str__(self):
        return (
            "-------------------------\n" +
            "------- CLASS \'" + self.name + "\' -------\n" +
            "kind: \'" + self.kind + "\'\n" +
            "--- Children ---\n" +
            '\t' + ', '.join(("\'" + key + "\'") for key in self.children) +
            '\n' +
            "--- Methods ---\n" +
            '\n'.join(
                (key + ":\n" + str(
                    '\n'.join(str(method) for method in value)
                    )
                )
                for key, value in self.methods.items()
                ) + '\n' +
            "--- Attributes ---\n" +
            '\n'.join(
                (key + ":\n" + str(
                    '\n'.join(str(valueA) for keyA, valueA in value.items())
                    )
                )
                for key, value in self.attributes.items()
                ) +
            '\n'
            )


class C_Class_Inherit():

    def __init__(self, cClass):
        self.cClass = cClass
        self.privacy = "private"

    def setPrivacy(self, privacy):
        self.privacy = privacy


class C_Method():

    def __init__(self, name, dataType, scope, arguments):
        self.name = name
        self.dataType = dataType
        self.scope = scope          # static/instance
        self.arguments = arguments
        self.isVirtual = False
        self.isPureVirtual = False
        self.inheritFrom = None
        self.isContructor = False
        self.isDestructor = False
        self.printable = True

    def setUnprintable(self):
        self.printable = False

    def setVirtual(self):
        self.isVirtual = True

    def setPureVirtual(self):
        self.isPureVirtual = True

    def setInheritFrom(self, inheritClass):
        self.inheritFrom = inheritClass

    def setConstructor(self):
        self.isContructor = True

    def setDestructor(self):
        self.isDestructor = True

    def __str__(self):
        return (
            "\t" + self.dataType + " \'" + self.name + "\'(" +
            ', '.join(str(arg) for arg in self.arguments) +
            ") | " + "\'" + self.scope + "\' | " +
            ("pure " if (self.isPureVirtual) else "") +
            ("virtual" if (self.isVirtual) else "")
            )


class C_Argument():

    def __init__(self, name, dataType):
        self.name = name
        self.dataType = dataType

    def __str__(self):
        return (self.dataType + " " + self.name)


class C_Attribute():

    def __init__(self, name, dataType, scope="instance"):
        self.name = name
        self.dataType = dataType
        self.scope = scope
        self.inheritFrom = None
        self.printable = True

    def setUnprintable(self):
        self.printable = False

    def setInheritFrom(self, inheritClass):
        self.inheritFrom = inheritClass

    def __str__(self):
        return (
            '\t' + self.dataType + " \'" + self.name + "\' | " +
            "\'" + self.scope + "\'"
            )
