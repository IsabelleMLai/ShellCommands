#include <stdio.h>
#include <iostream>
#include <sstream>

#include <fcntl.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace std;


/*
required to use the following routines to do file input and output:
open, read, write, and close

this code should open given file, and print contents of file
*/



int main(int argc, char* argv[]) {

        /*
        following code borrowed from lecture source code
        */

        int fileDescriptors[argc];

        if (argc == 1) {
            // no arguments, use STDIN
                //fileDescriptor = STDIN_FILENO;
                return 0;
        } else {
                //gets the specific file descriptor here
                for(int i=0; i<(argc-1); i++) {

                        fileDescriptors[i] = open(argv[(i+1)], O_RDONLY);

                        if (fileDescriptors[i] == -1) {
                                cout << "wcat: cannot open file" << endl;
                                return 1;
                        } else {
                                // file opened, do stuff to it
                                char buffer[4096];
                                int ret;
                                
                                //read>0 means read success, returns 
				//number of bytes read
                                while ((ret = read(fileDescriptors[i], 
buffer, sizeof(buffer))) > 0) {
                                        //stringstream size is based on 
					//specific computer, 
					//not sure about bounds?
                                        stringstream strOut;

                                        //bytesRead += ret;
                                        strOut << buffer;
                                        string str = strOut.str();
                                        write(STDOUT_FILENO, str.c_str(), 
ret);

                                }   

                                
                                if (fileDescriptors[i] != STDIN_FILENO) {
                                        close(fileDescriptors[i]);
                                }
                        }
                }

        }
  
        return 0;

}

