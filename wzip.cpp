/*
- open file
- read each character until repeat stops, return the number of repeats
-  5byte formatted solution
*/

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
// #include <bitset>

#include <fcntl.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace std;

/*
Return  -1 for no file descriptors that open
return 0 for useable file descriptors
file descriptors stored at FDarr
*/

int OpenFile(int* FDarr, int* FDcount, int arg_c, char* arg_v[]) {

        *FDcount = 0;        
	//number of file descriptors in command line

        // no arguments
        if (arg_c == 1) {
                cout << "wzip: file1 [file2 ...]" << endl;
                return -1;

        } else {

                //gets the specific file descriptors here
                for(int i=0; i<(arg_c-1); i++) {

                        FDarr[*FDcount] = open(arg_v[(i+1)], O_RDONLY);

                        //if you can't open file
                        if (FDarr[*FDcount] == -1) {
                                cout << "wgrep: cannot open file" << endl;
                                return -1;
                        }
                        *FDcount+=1;
                }
        }
        return 1;
}

  


int WriteToFD(char* let, int* repeats) {
        // cout<<*let<< " "<<*repeats<<endl;

        // bitset<4> reps(*repeats);
        // bitset<8> character(*let);
        // string R = reps.to_string();
        // string C = character.to_string();
        
        // cout<<R<<" "<<C<<endl;
        //ERROR EXISTS HERE SOMEWHERE
        // write(STDOUT_FILENO, R.c_str(), 4);
        // write(STDOUT_FILENO, C.c_str(), 1);
        write(STDOUT_FILENO, repeats, 4);
        write(STDOUT_FILENO, let, 1);
        
        return 0;
}
                
                

// stores letter and number of repeats in parameter pointers
                        
void LetNumRepeats(char* let, int* repeats, int* FDs, int num_FDs) {
        char buffer[8];
        // int bytesRead = 0;
                 
        int readSuccess = 0;
        int num_reads = 0;
        char prev;

        for(int i=0; i<num_FDs; i++) {
                //MAKE SURE THE FDs ARE CORRECT
                //while there are things 
		//left to read in a single given file 
                while (  ((readSuccess = read(FDs[i], buffer, 1)) >0)  ) {

                        //first read ever = set first letter to be prev
                        if(num_reads == 0) {
                                prev = *buffer;
                                *let = *buffer;
                                *repeats = 1;
                                // cout<<"FIRST"<<endl;
                        } else {
                                if(*buffer == prev) {
                                        *let = prev;
                                        *repeats += 1;
                                } else {   
                                        
                                        WriteToFD(let, repeats);
                                        *let = *buffer;
                                        *repeats = 1;
                                        
                                }
                                prev = *buffer;
                        }
                        num_reads += 1;
                }
                // if at thee end of all the files, need to write
                if( (i+1) == num_FDs )  {
                        WriteToFD(let, repeats);
                        
                }
                if (FDs[i] != STDIN_FILENO) {
                        close(FDs[i]);
                }
        }
       
}

 
                
int main (int argc, char* argv[]) {
        // argv = char**  type
        
        //array of file descriptors
        int fileDescriptors[argc-1];
        int fd_count = 0;
                
        int opened_succ = OpenFile(fileDescriptors, &fd_count,  argc, 
argv);

        //if there was an error opening the file
        if (opened_succ == -1) {
                return 1;
        }

        char let  = '\0';
        int reps = -1;

        //needs to append the files together and then zip 
       
        LetNumRepeats( &let, &reps, fileDescriptors, (argc-1));
         

        return 0; 
}
        
        


