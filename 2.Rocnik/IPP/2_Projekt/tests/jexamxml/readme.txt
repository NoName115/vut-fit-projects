JExamXML Version 1.01
JExamXML Copyright 2007-2010 A7Soft ALL RIGHTS RESERVED.

I. INTRODUCTION.

JExamXML is an XML comparison Java based application.
It can compare and merge XML documents printing differences between them 
to the delta XML file.

Aimed at professional java developers, this tool may be very useful for 
every users working with XML.

This software can be used either as a standalone program or as a jar 
file class library that is integrated with user's own java application.

Pure java architecture allows running JExamXML on heterogeneous hardware platforms.

II. SYSTEM REQUIREMENTS

- JRE 1.5 or higher
- Xerces 2.0 

III. DOWNLOAD

The latest version of JExamXML can always be downloaded from 
http://www.a7soft.com

IV. INSTALLATION.

Download jexamxml.zip file.
Unpack this file into any directory (C:\JExamXML is a good choice)

V. HOW TO USE

There are two ways to use this program.

JExamXML can be used as a standalone java application

     java -jar jexamxml.jar <arguments>
  
 or  java -classpath "jexamxml.jar" com.a7soft.examxml.ExamXML <arguments>

where arguments are:
     <1-xml-file> <2-xml-file> [<delta-xml>] [<command>] [<option-file>]
     <1-xml-file>  - the first XML file to compare
     <2-xml-file>  - the second XML file to compare
     <delta-xml>   - the delta XML file to print differences (optional)
     <command>     - /D : print differences or /M : merge XML files
     <option-file> - the option file containing all options as pairs name=value

Also JExamXML can be used as a jar library, 
integrated into user's own java application.

To do this the jexamxml.jar file should be included in the application classpath
and the import statement should be inserted into java code:
      
import com.a7soft.examxml.ExamXML;
or
import com.a7soft.examxml.*;

The ExamXML class contains several static methods that can be used
within java code.

To compare XML files and print differences to delta xml file
ExamXML.compareXMLFiles(String file1,String file2,String delta,String options);

To compare XML files without printing differences, just to determine if two XML files are equal
ExamXML.compareXMLFiles(String file1,String file2);

To compare strings representing XML entities
ExamXML.compareXMLEntities(String entity1, String entity2);


VI. LICENSE AGREEMENT

JExamXML is a freeware application. It can be used on free of charge basis. 

VII. TROUBLESHOTING

If the "OutOfMemory" Exception occurs while application is running 
you can increase the heap size by using -Xmx and -Xms properties 
example: 
java -Xmx512M -Xms256M -jar jexamxml.jar .....
