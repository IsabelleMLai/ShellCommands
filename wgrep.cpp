#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
                                
#include <fcntl.h>
#include <stdlib.h>
         
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
         
using namespace std;
 
                
                        
// 1 char = 8bits =  1byte
//need to read through each char/byte and check for \n

int Search (int fileDescriptor, string searchWord) {
        
        char buffer[8];
        int bytesRead = 0;
        
        int readSuccess = 0;
        int matchesSoFar = 0;
        int hasWord = 0;
        stringstream strOut;


        // read 1 byte at a time
        while ( (readSuccess = read(fileDescriptor, buffer, 1)) >0) {
                strOut << buffer[0];
                // new line was typed
                if (*buffer == '\n') {
                        // strOut << buffer[0];
                        if (hasWord > 0) {
                                string str = strOut.str() + "\n";
                                // cout<<str;

                                write(STDOUT_FILENO, str.c_str(), 
bytesRead);
                                char newline = '\n';
                                write(STDOUT_FILENO, &newline, 1);
                        }
                        

                        // clear string stream  
                        bytesRead = 0;
                        strOut.str("");
                                
                        // reset whether the line has the search word
                        matchesSoFar = 0;
                        hasWord = 0;

                } else {
       
                        // strOut << buffer[0];
                        bytesRead += 1;
        
                        if (matchesSoFar >= (int)searchWord.length()) {
                                matchesSoFar = 0;
                                hasWord = 1;
                        } else if (*buffer == searchWord[matchesSoFar]) {
                                matchesSoFar += 1;
                        } else {
                                matchesSoFar = 0;
                        }
                }     
                        
        }
                
        if (fileDescriptor != STDIN_FILENO) {
                close(fileDescriptor);
                return 0;
        }
        else if(readSuccess == 0){
                return 1;
                
                
        } else {
                return -1;
                
        }
}
                        
                                
                                
                        
                                        
int main (int argc, char* argv[]) {

        /*
    following code borrowed from lecture source code
    same as the code used in the start of wcat.cpp
    */
    
    int fileDescriptors[argc-1]; 
    string searchW = "";
//number of file descriptors in command line
    int FDcount = 0;       
                    
                            
    // no arguments
    if (argc == 1) {
        cout << "wgrep: searchterm [file ...]" << endl;
        return 1;

    } else {
        searchW = argv[1];    
        //gets the specific file descriptors here
        for(int i=0; i<(argc-2); i++) {

            fileDescriptors[FDcount] = open(argv[(i+2)], O_RDONLY);
            //if you can't open file
            if (fileDescriptors[FDcount] == -1) {
                
                // if we didn't get a search word yet
                // if(searchW == "") {
        
                //     searchW = argv[i+1];
                //     //<<"seearch W: "<<searchW<<endl;
                // } else {
                    cout << "wgrep: cannot open file" << endl;
                    return 1;
                // }

            } else {
                FDcount+=1;
            }   

        }        
                    
        // passed empty search string
        if (searchW == "") {
                return 0;
        }
    }
    
        
            
    // no file descriptor given
    if (FDcount == 0) {

        // int x=
        Search (STDIN_FILENO, searchW);
        //  cout<<"x= "<<x<<endl;
    }
        
    // loop through each file descriptor
    for (int i=0; i<(argc-2); i++) {
            Search (fileDescriptors[i], searchW);                        
    }
    return 0;
}                               
                         
        
        
                
            
