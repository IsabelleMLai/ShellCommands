
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


/*
reeturns -1 if eerror or no arguments
otherwise the number of file descriptors (1+) that weere successful
(never 0)
*/
int OpenFile(int* FDarr, int arg_c, char* arg_v[]) {

    int FDcount = 0;        
	//number of file descriptors in command line

    // no arguments
    if (arg_c == 1) {
        cout << "wunzip: file1 [file2 ...]" << endl;
        return -1;

    } else {
        //gets the specific file descriptors here
        for(int i=0; i<(arg_c-1); i++) {

            FDarr[FDcount] = open(arg_v[(i+1)], O_RDONLY);

            //if you can't open file
            if (FDarr[FDcount] == -1) {
                cout << "wgrep: cannot open file" << endl;
                return -1;
            }

            //if file opened, increase FDcount
            FDcount+=1;
        }
    }
    return FDcount;
}

/*
convert char array to int, 
return int
*/
int CharAToInt(char* int_buff, int buff_size) {
    int res = 0;
    int mult = 1;
// cout<<"int buff "<<int_buff[0]<<endl;

    for(int i= 0; i<buff_size; i++) {
        
        int int_equiv = (int_buff[i] - '0');
        if((int_equiv <= 9) && (int_equiv >= 0)) {
            res+=mult*int_equiv;

        } else {
            res+=mult*( (int)int_buff[i] );
        }
        //each index of buff = 1 byte
        //=8bits = 1111 1111 = 255 then +1 since counting from 0
        mult*=256;  
    }
    return res;
}


/*
Assumes correct zip format of 4 byte int with 1 byte char
*/
void UNZIP(char* let, int* repeats, int* FDs, int num_FDs) {
    
    //for each file
    for(int i=0; i<num_FDs; i++) {
        
        int num_read = 0;
        int offset_bytes = 0;

        int num_reps = 0;

        char read_buffer[1]; 
        //stores values read in
        char int_buff[4];  
        
    
        //while there is still one character left to read
        int readPossible = pread(FDs[i], read_buffer, 1, offset_bytes);

        //use -2 to show no reads done yet
        //shows succeess of read
        // int readInt = -2;
        // int readChar = -2;

        while(readPossible>0) {
            //even number of reads, 0th read, 2nd read, etc.
            if(num_read%2 == 0) {
                read(FDs[i], int_buff, 4);
                
                offset_bytes += 4;
                num_reps = CharAToInt(int_buff, 4);

            //otherwise odd read 
            // should have valid int_buf at this point
            } else {
                char c_buff[1];   
                read(FDs[i], c_buff, 1);
                offset_bytes += 1;

                string to_write = "";
                to_write += c_buff[0];
                // cout<<" string "<<to_write<<endl;
                for(int i=0; i<num_reps; i++) {
                    write(STDOUT_FILENO, to_write.c_str(), 1);
                }
                // write string here
                
            }

            num_read+=1;
            readPossible = pread(FDs[i], read_buffer, 1, offset_bytes);
        }
    }
        
        // //check that reads were successful
        // if( (readInt == -1) || (readChar == -1) ) {
        //     cout<<"error with read"<<endl;
        // }
       
}



int main (int argc, char* argv[]) {
    int FDs[argc-1];
    int FD_count = OpenFile(FDs, argc, argv);

    //if no files were opened
    if(FD_count == -1) {
        return 1;
    }

    int reps = 0;
    char let = '\0';
    UNZIP( &let, &reps, FDs, FD_count);

    return 0;
}
