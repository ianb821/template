/*********************************************************
* template builds template .cpp, .c, .h and .java files (will support
* more file types in the future) based on a file name and 
* #include, import lines provided at the command line.  The
* file is opened after creation.
*
*
* .cpp Example:
*     template awesome.cpp iostream vector string
* Output:
*
*     #include <iostream>
*     #include <vector>
*     #include <string>
*
*     using namespace std;
*
*     int main(int argc, char * argv[])
*     {
*
*        return 0;
*     }
*
* .c Example:
*     template something.c stdio.h
* Output:
*
*     #include <stdio.h>
*
*     int main(int argc, char * argv[])
*     {
*
*        return 0;
*     }
*
*
* .h Example:
*     template awesome.h vector string
*
* Output:
*
*     #ifndef TEST_H
*     #def TEST_H
*
*
*     #include <iostream>
*     #include <string>
*
*
*     #endif // TEST_H
*
*
*
*
* .java Example:
*     template something.java util.* math.*
*
* Output:
*
*     import java.util.*;
*     import java.math.*;
*
*     public class something {
*
*        public static void main(String[] args) {
*
*
*
*
*        }
*     }
*
*********************************************************/

#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>


using namespace std;

// helper functions
string getFileType(string & filename);
void addImportAndIncludeLines(vector<string> & linesOfFile,
                              string & fileType, char * dependency);
void buildFile(vector<string> & linesOfFile, string & fileType);
void buildCOrCPlusPlusFile(vector<string> & linesOfFile, string & fileType);
void buildHeaderFile(vector<string> & linesOfFile, string & filename);
void buildJavaFile(vector<string> & linesOfFile, string & filename);
void writeToFile(vector<string> & linesOfFile, string & filename) throw(string);
void addHeaderFileDeclaration(vector<string> & linesOfFile, string & filename);
void capitalizeHeaderFileName(string & filename);


/*********************************************************
* main - handles the command line arguments and builds
* the template file, then opens it.
*********************************************************/
int main(int argc, char * argv[])
{
   // declare variables
   string filename;
   string fileType;
   vector<string> linesOfFile;
   
   
   // make sure something was actually passed in
   // if not, show proper usage
   if (argc > 1)
   {
      filename = argv[1];
   }
   else
   {
      cout << "Usage: template <filename> (optional: <files> <to> <include>)\n";
      return 0;
   }
   
   // get the file extension
   fileType = getFileType(filename);
   
   if (strcmp(fileType.c_str(), "h") == 0)
      addHeaderFileDeclaration(linesOfFile, filename);
   
   // add any #include / import files that were entered
   // at the command line
   for (int i = 2; i < argc; i++)
      addImportAndIncludeLines(linesOfFile, fileType, argv[i]);
   
   // build the template file
   buildFile(linesOfFile, filename);
   
   // try to write the template to the provided file name
   // then open it to begin editing
   try
   {
      writeToFile(linesOfFile, filename);
      
      // command to open the file after creation
      string command = "open ";
      command += filename;
      
      // open the file
      system(command.c_str());
      
   }
   catch (string error)
   {
      cout << error << endl;
   }
   
   

   return 0;
}

/*********************************************************
* returns the file extension
*********************************************************/
string getFileType(string & filename)
{
   return filename.substr(filename.find('.') + 1);
}

/*********************************************************
* addHeaderFileDeclaration adds the #ifndef statement
* for the header file
*********************************************************/
void addHeaderFileDeclaration(vector<string> & linesOfFile, string & filename)
{
   string headerDeclaration = filename.substr(0, filename.find('.'));
   capitalizeHeaderFileName(headerDeclaration);
   string ifStatement = "#ifndef ";
   ifStatement += headerDeclaration;
   linesOfFile.push_back(ifStatement);
   string defStatement = "#def ";
   defStatement += headerDeclaration;
   linesOfFile.push_back(defStatement);
   linesOfFile.push_back("\n");
   
}

/*********************************************************
* capitalizeHeaderFileName capitilatizes the provided
* filename and adds '_H' to it for the #ifndef statement
*********************************************************/
void capitalizeHeaderFileName(string & filename)
{
   transform(filename.begin(), filename.end(),filename.begin(), ::toupper);
   filename += "_H";

}

/*********************************************************
* addImportAndIncludeLines adds the dependency properly
* formatted based on the file type:
* #include <dependency>;
* import java.dependency;
*********************************************************/
void addImportAndIncludeLines(vector<string> & linesOfFile, string & fileType, char * dependency)
{
   
   // decide whether to #include or import.java.. based on
   // file extension
   if (strcmp(fileType.c_str(), "cpp") == 0 || strcmp(fileType.c_str(), "h") == 0 || strcmp(fileType.c_str(), "c") == 0)
   {
         string includeString = "#include <";
         includeString += dependency;
         includeString += ">";
         linesOfFile.push_back(includeString);
   }
   else if (strcmp(fileType.c_str(), "java") == 0)
   {
         string importString = "import java.";
         importString += dependency;
         importString += ";";
         linesOfFile.push_back(importString);
      
   }
}

/*********************************************************
* buildFile builds the correct file based on file type
*********************************************************/
void buildFile(vector<string> & linesOfFile, string & filename)
{
    
    // calls the proper template builder based on file extension
    string fileType = getFileType(filename);
    if (strcmp(fileType.c_str(), "cpp") == 0 || strcmp(fileType.c_str(), "c") == 0)
        buildCOrCPlusPlusFile(linesOfFile, fileType);
    else if (strcmp(fileType.c_str(), "h") == 0)
        buildHeaderFile(linesOfFile, filename);
    else if (strcmp(fileType.c_str(), "java") == 0)
        buildJavaFile(linesOfFile, filename);
    
}


/*********************************************************
* buildCPlusPlusFile - builds the template .cpp file plus
* the #include lines added at the command line
*********************************************************/
void buildCOrCPlusPlusFile(vector<string> & linesOfFile, string & fileType)
{
    if (strcmp(fileType.c_str(), "cpp") == 0) {
        linesOfFile.push_back("\nusing namespace std;");
    }
    linesOfFile.push_back("\nint main(int argc, char * argv[])\n{\n");
    linesOfFile.push_back("\treturn 0;\n}");
    
}


/*********************************************************
* buildHeaderFile - builds the template .h file plus
* the #include lines added at the command line
*********************************************************/
void buildHeaderFile(vector<string> & linesOfFile, string & filename)
{
   string headerDeclaration = filename.substr(0, filename.find('.'));
   capitalizeHeaderFileName(headerDeclaration);
   linesOfFile.push_back("\n");
   string endIf = "#endif // ";
   endIf += headerDeclaration;
   linesOfFile.push_back(endIf);
}


/*********************************************************
* buildJavaFile - builds the template .java file plus
* the import lines added at the command line
*********************************************************/
void buildJavaFile(vector<string> & linesOfFile, string & filename)
{
   // adds the class name based on the file name
   string className = "\npublic class ";
   className += filename.substr(0, filename.find('.'));
   className += " {";
   linesOfFile.push_back(className);
   
   // ...a lot of \'s, sorry!
   linesOfFile.push_back("\n\tpublic static void main(String[] args) {\n\n\n\n\n\t}\n}");
   
}


/*********************************************************
* writeToFile writes the template to a file with the
* provided file name.
*********************************************************/
void writeToFile(vector<string> & linesOfFile, string & filename) throw(string)
{
   ofstream output;
   output.open(filename.c_str());
   
   // throws error if the file can't be opened to write
   if (!output.is_open())
   {
      string error = "Error opening file \'";
      error += filename;
      error += "\'";
      throw error;
   }
   
   for (int i = 0; i < linesOfFile.size(); i++)
      output << linesOfFile.at(i) << endl;
   
   output.close();
}
