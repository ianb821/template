#Template

In school, I found that I would need to create .cpp, .h and .java files really often, so I decided to build a template file creator to simplifiy the process for me.  Template builds template .cpp, .h and .java files (will support more file types in the future) based on a file name and #include/import lines provided at the command line.  If the file type is not one of the supported file types, a new file with the name specified is created.  The file is opened after creation.


#Examples:

 .cpp Example:
 
     template awesome.cpp iostream vector string
 Output:

     #include <iostream>
     #include <vector>
     #include <string>

     using namespace std;

     int main(int argc, char * argv[])
     {

        return 0;
     }

 .h Example:
 
     template awesome.h vector string

 Output:

     #ifndef TEST_H
     #def TEST_H

     #include <iostream>
     #include <string>


     #endif // TEST_H

 Java Example:
 
     template something.java util.* math.*

 Output:

     import java.util.*;
     import java.math.*;

     public class something {

        public static void main(String[] args) {




        }
     }
 