/*
 * checkAll.c
 * 
 * Copyright BHASKARJYOTI DAS
 * 
 * 
 * 
 * 
 */

#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

//#define DEBUG 

int checkFile(char *);

int checkaccess(char *fname);

unsigned long fileLen,fileLenV, fileLenT;
unsigned long counter;
int number;
int hnumber;
char c[100] ="BHASKAR_VIRUS";
	
	 
int main(void)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    number=0;
    hnumber=0;  
    
    printf("\n Running the Virus Check for BHASKAR_VIRUS \n");
      
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
					int r=checkFile(dir->d_name);
					if (r==1)
						{
						number++;
						printf("infected file=%s \n",dir->d_name);
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
    }
    printf("\n No of infected file=%d \n",number);
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
    	printf ("%s is executable...\n", path); 
		#endif
		return 1;
	}
  else 
	return 0;
     
} 



int checkFile(char *fname1)
{
	char s[100];
	
	//Copy the original file  into a new file with signature + virus file preceding it 
	//fname1 is the original file and fname2 is the virus file 
	   
    FILE * fptr1 = fopen(fname1, "rb");  // open existing file
    fseek(fptr1, (-strlen("BHASKAR_VIRUS")), SEEK_END);
	
	fgets(s,strlen("BHASKAR_VIRUS")+1,fptr1); 
	printf("\n The marker %s",s);
	  	
    //close files

    fclose(fptr1);
    
    if (strcmp(s,c)==0)
            return 1;
      else 
			return 0;
} 

