import com.a7soft.examxml.ExamXML;
import com.a7soft.examxml.Options;

import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.File;
import java.io.IOException;

/**
 * This sample program illustrates how to use the
 * JExamXML application within user's java code.
 *
 */
public class Main {

   /**
    * This method reads XML file and returns a string object
    * representing XML document.
    *
    */
   static String readFile(String name) {
      char[] buff = new char[0];
      try {
         File file = new File(name);
         long len = file.length();
         buff = new char[(int) len];
         FileReader reader = new FileReader(file);
         reader.read(buff);
      } catch (FileNotFoundException e) {
         e.printStackTrace();
      } catch (IOException e) {
         e.printStackTrace();
      }
      return new String( buff );
   }

   /**
    * The main method of the sample program.
    */
   static public void main(String[] argv) {
      // Sets log file
      ExamXML.setLogFile("xml.log");

      // Reads two XML files into two strings
      String s1 = readFile("orders1.xml");
      String s2 = readFile("orders.xml");

      // Loads options saved in a property file
      Options.loadOptions("options");

      // Compares two Strings representing XML entities
      if ( ExamXML.compareXMLEntities( s1, s2 ) == 1) {
         System.out.println( "OK" );
      }

      // Compares two Strings representing XML entities
      // and prints differences between XML entities
      String s = ExamXML.compareXMLString( s1, s2 );
      System.out.println( s );

      // Assign log file to null,
      // all error messages will be printing to the standard error stream
      ExamXML.setLogFile(null);

      // Compares two XML files using options file
      // and prints differences to the delta xml file.
      int res = ExamXML.compareXMLFiles( "orders.xml",
                                         "orders1.xml",
                                         "delta.xml",
                                         "options" );
      System.out.println(res);

   }
}
