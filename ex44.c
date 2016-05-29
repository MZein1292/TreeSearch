//PLEASE READ BEFORE TESTING
// the way to run it is you specify the directory and size: as such ./ex44 /home/user/hw/ 1000
//bin 0 to 999: 13
//bin 2000 to 2999: 1
//bin 13000 to 13999: 1
//bin 14000 to 14999: 1
//bin 34000 to 34999: 1


#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>


//max file size bucket 1GB, still really space inefficient! :(
#define MAX_FILESIZE (1 << 30)
//#define MAX_FILESIZE 3000

void directoryRead(char* dirName, int bin_size, int bins[]);
int min (int a, int b);

int main(int argc, char *argv[]) {

	
	//simulate user intput # bytes bin size
	//int bin_size = 1000;

    char *name = argv[1];
    int bin_size = atoi(argv[2]);
	
	int size = (MAX_FILESIZE / bin_size) + 1;
    int bins[size];
    int i;
    for (i = 0; i < size; i++) {
        bins[i] = 0;
    }

	directoryRead(name, bin_size, bins);
	for (i= 0; i < size-1; i++) {
	    if (bins[i] != 0) {
	        printf("bin %d to %d: %d\n", bin_size*i, bin_size*(i+1)-1, bins[i]);
	    }
	}
	
	if (bins[size-1] != 0) {
	    printf("bin %d+: %d\n", bin_size*(size-1), bins[size-1]);
	}

}

void directoryRead(char* dirName, int bin_size, int bins[]) {
	struct dirent * entry; 
	DIR * directory;

	directory = opendir(dirName);

	if(directory != NULL)
	{
		while(entry = readdir(directory))
		{
			if(entry->d_type == DT_REG) {
	            struct stat st;
	            
	            char tempPath[500] = "";
				strcat(tempPath, dirName);
				strcat(tempPath, "/");
				strcat(tempPath, entry->d_name);
				
				//printf("%s\n", entry->d_name);
				stat(tempPath, &st);
				long bytes = st.st_size;
				//printf("%ld\n", bytes);

                //increment the bucket that this file belongs to
                int index = min(bytes / bin_size, MAX_FILESIZE / bin_size);
                bins[index]++;

			}
			//here it make sure not to read current directory and parent directory
			if((entry->d_type == DT_DIR) && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0 )
			{
            	char newDir[256] = "";
            	char tempDir[256] = "";

				strcpy(newDir, entry->d_name);
				strcat(newDir, "/");            //adds / to the end of directory name
				//printf("%s\n", entry->d_name);	
				strcpy(tempDir, dirName);          //appends newDirectory to big directory
                strcat(tempDir, "/");
                strcat(tempDir, newDir);
                //printf("%s\n", dirName);
				directoryRead(tempDir, bin_size, bins);           //recursively call new directory
			}
			
		}
	}

}


int min(int a, int b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}



