/*
 * Myshell.c
 * Author : BHASKARJYOTI DAS 
 * Date : Sept 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#define BUFFER_SIZE 1<<16
#define ARR_SIZE 1<<16

/* #define DEBUG */

void parse_args(char *buffer, char** args, 
                size_t args_size, size_t *nargs)
{
    char *buf_args[args_size]; /* You need C99 */
    char **cp;
    char *wbuf;
    size_t i, j;
    
    wbuf=buffer;
    buf_args[0]=buffer; 
    args[0] =buffer;
    
    for(cp=buf_args; (*cp=strsep(&wbuf, " \n\t")) != NULL ;){
        if ((*cp != '\0') && (++cp >= &buf_args[args_size]))
            break;
    }
    
    for (j=i=0; buf_args[i]!=NULL; i++){
        if(strlen(buf_args[i])>0)
            args[j++]=buf_args[i];
    }
    
    *nargs=j;
    args[j]=NULL;
}


 int main(int argc, char *argv[], char *envp[]){
    char buffer[BUFFER_SIZE];
    char *args[ARR_SIZE];

    int ret_status;
    size_t nargs;
    pid_t pid, parent_pid;
    int high_8, low_7, bit_7;    
    
    parent_pid = getpid();  /* First who am i ? */
	
    printf ("Disclaimer: \n");
    printf ("This simple shell will support all linux command that are available as executable \n");
    printf ("It will NOT support all such linux commands that are implemented as shell builtin such as cd \n");
    printf("It will NOT support pipe, redirection etc. as that is not single executable \n");
    printf("It does NOT implement programming features of shell language. \n\n\n\n");
    printf("Commands that are tested : \n\n");
    printf("1. man 2. who 3. whoami 4. pwd 5. ps 6. ls 7. dir 8. df 9. time 10. date \n\n");
    printf("11. vi 12. vmstat 13. chgrp 14. chown 15. chmod 16. more 17. cp 18. mv 19. rm 20. mkdir \n\n");
    printf("21.rmdir 22. head 23. tail 24. touch 25. echo etc \n\n");
    
  
    printf("Press any key to continue..\n");
    getchar(); 
    
    printf( "\nWelcome to the shell. \n" );
    printf( "Type exit to quit the shell. \n" );
  

	#ifdef DEBUG
    printf( "Our shell program;s PID is  : %d\n", parent_pid);
    #endif
    
    while(1){
        #ifdef c
	    printf( "Running as real UID %d and effective UID %d\n\n",getuid(),geteuid());
        #endif
        
        printf("Bhaskar $ ");
        fgets(buffer, BUFFER_SIZE, stdin);
        parse_args(buffer, args, ARR_SIZE, &nargs); 

        if (nargs==0) continue;
        if (!strcmp(args[0], "exit" )) exit(0);       


		pid = fork();
		 /* from here the child code starts */
     		
    switch (pid ) { 
     
    
	case -1:	/* ERROR */
		perror("fork failed");
		return 1;
		
	case 0:		/* CHILD */
	    execvp(args[0], args);
	    puts(strerror(errno));
	    perror(argv[0]);
	    printf("*** Could not execute '%s'\n", argv[0]);
        exit(1);// forked child process exits non-zero
     
				
	default:{	/* PARENT */
		
		#ifdef DEBUG
        printf("Waiting for child (%d)\n", pid);
        #endif 
        pid = wait(&ret_status);
        #ifdef DEBUG
        printf("Child (%d) finished\n", pid);
        #endif

	/* wait returns the child pid and it returns the status to the interger , the pointer to which is supplied as a wait parameter */

		#ifdef DEBUG	
		printf ( "Splitting and displaying the status ..\n");
		high_8=ret_status >> 8;  /* out of 16 bit, high 8 bits for exit value 1111 1111 0000 0000 .. if the child process exits normally*/
		low_7= ret_status & 0x7f; /* low 7 bit for signal number in case somebody killed it with a signal..0000 0000 0111 1111 */ 
		bit_7= ret_status & 0x8f;  /* in between 1 bit...0000 0000 1000 0000 shows whether it is a core dump case */
		printf("exit=%d, core =%d, signal %d\n", high_8, bit_7, low_7);  /* out of 16 but integer placed in the variable pointed to in the wait */
		#endif 
		
			} 	// end of default 
				
               
                    } // end of switch
        
        } //end of while 
        return 0;
    }    
   



