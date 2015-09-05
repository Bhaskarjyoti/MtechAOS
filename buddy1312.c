// This is the latest of the BUDDY Program Dated 13/12/2014 
//@copyright Bhaskarjyoti Das

// For a node numbered n, left sibling n-1
// For a node numbered n, right sibling n+1
// For a node numbered n, left child 2*n +1
// For a node numbered n, right child  2*n +2
// For a node numbered n , parent (n-1)/2 ..integer value of the result...
// First root node is 0 
// total nodes in tree of height h is (2 POW h+1 - 1).. this should be the arraysize at least 
// At any level l, the total number of nodes is 2 pow l 


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>

//*********************************************************************
//#define DEBUG  // set this switch if u are really keen to understand the logic using debug messages
//*********************************************************************

#define MAXNODES 10000  //hypthetical maximum size here 
#define MAXLEVEL 100

int tree[MAXNODES]={2};  // initialize the tree node value
int total_level_free[MAXLEVEL]={0};
int total_level_alloc[MAXLEVEL]={0};
int total_level_used[MAXLEVEL]={0};
int lazy =0; // default is not LAZY

// in this, the node value 0 if free, 1 if divided  and >1 ( or the buddy size) if allocated 

int i, match;
int maxlevel;
int maxnodes;
int flevel;
int created_node;


// array of total no of blocks(N) at a certain level 
// array of total no of available blocks(L) Locally at a certain level 
// array of total no of allocated blocks( A ) Locally at a certain level 
// the DELAY D = A-L = N-2L-G for a level ;for D=0, we coalesce;
// we increment total_level_free when we mark tree[node]=0
// we increment total_level_alloc when we mark tree[node]>0 ..it means either divided or allocated

int segmentalloc(int,int);
void makedivided(int),makefree(int),printing(int,int);
void debug_print();
int place(int),power(int,int);
void array_dynamic(int *ptr, int n);
void incrementArray(int [], int );
void decrementArray(int[], int);
int getLevel(int);
void incrementNode();

//####################################################################

int main()
{
	int totsize,totalalloc,ch,n,req=0;

	for(i=0;i<80;i++) printf("%c",5);
	printf("\n	************************************************************");
	printf("\n 	WELCOME TO (LAZY) BUDDY KERNEL MEMORY ALLOCATOR SIMULATION  ");
	printf("\n	************************************************************\n");
	for(i=0;i<80;i++) printf("%c",5);
	printf("\n We will run the simulation with a memory size = 1024 x (2's power) Bytes");
	printf("\n For power=1, it is 2 power 1 , 3 is 2 power 3 etc. ");
	printf("\n Our default is 1024 and if you enter power 0, it will stay 1024\n");
	printf("\n Enter the POWER OF 2 with which we will multiply default memory size: ");
	scanf("%d",&n);
	
	if (n>5)
		{
			printf("\n Sorry ..we do not support that much of memory in this simulation ...");
			exit(0);
		}
	
	totsize=1024*power(2,n);
	printf("\n TOTAL SYSTEM MEMORY IN BYTES FOR SIMULATION=%d \n", totsize);
	maxlevel=getLevel(totsize);
	maxnodes=power(2,maxlevel+1)-1;
	
	printf("\n ****************************************************************");
	printf("\n NOTE : MAXIMUM HEIGHT OF FULLY SPLIT BUDDY TREE =%d \n", maxlevel);
	printf("\n Starting from node size of 2 bytes, we can have maximum nodes =%d \n", maxnodes);
	printf("\n Our Array implementation of binary tree has ONLY size = %d ",MAXNODES);
	printf("\n So,accordingly select your memory request size..................");
	printf("\n We will allow a minimum size of 16 bytes");	
	printf("\n ****************************************************************");
	
    printf("\n\n Press a key to continue...");
	getchar();

	while(1)
	{
		for(i=0;i<80;i++) printf("%c",5);
		printf("\n ............................................\n");
		printf("\n LAZY BUDDY  SYSTEM  ALGORITHM SIMULATION....\n");
	
		for(i=0;i<80;i++) printf("%c",5);
		printf("	*  1)   Allocation request \n");
		printf("	*  2)   Free request \n");
		printf("	*  3)   Show Memory allocation \n");
		printf("	*  4)   Enable LAZY mode \n");
		printf("	*  5)   Disable LAZY mode \n");
		printf("	*  6)   End simulation\n");
		for(i=0;i<80;i++) printf("%c",5);
		printf("\n.........  Enter your choice  :  \n");
		scanf("%d",&ch);
		switch(ch)
		{
			case 1:
				//clear_screen();
				printf(" ");
				for(i=0;i<80;i++) printf("%c",5);
				printf(" ");
				printf("A L L O C A T I O N\n");
				for(i=0;i<80;i++) printf("%c",5);
				printf("\n  Enter the request size  : ");
				scanf("%d",&req);
				printf("\n total memmory in system : %d \n",totsize);
				printf("\n memory block allocated so far is : %d \n",totalalloc);
				if (req<16)
					{
						printf("\n Sorry ..minimum size of 16 bytes allowed");
						continue;
					}
				
				if (req <=(totsize-totalalloc))
				{
					totalalloc=totalalloc+segmentalloc(totsize,req);
					printf("\n total memory available is : %d \n",(totsize-totalalloc));
				}
				else
					printf("\n Not enough memory left in system\n");
				break;
			case 2:
				printf(" ");
				for(i=0;i<80;i++) printf("%c",5);
				printf(" ");
				printf("D E A L L O C A T I O N \n");
				for(i=0;i<80;i++) printf("%c",5);
				printf("	* requested  size  :  \n");
				scanf("%d",&req);
				makefree(req);
				break;
			case 3:
				
				printf(" ");
				for(i=0;i<80;i++) printf("%c",5);
				printf("CURRENT  M E M O R Y   A L L O C A T I O N   \n");
				for(i=0;i<80;i++) printf("%c",5);
				printf(" ");
				printing(totsize,0);
				printf(" ");
				for(i=0;i<80;i++) printf("%c",5);
				getchar();
				break;
			case 4:
				printf("LAZY MODE is now enabled..Memory COALESCING WILL BE DELAYED.\n");
				lazy=1;
				continue;
			case 5:
				printf(" Lazy mode is NOW DISABLED..Now free any block to reclaim all possible cases...\n");
				lazy=0;
				continue;  
			case 6: 
				printf("\n Thank you for using memory allocator simulation ..GoodBye ");
				exit(0);	
			default:
				printf("\n We expect input 1 to 6 only... ");
			 	continue;
		}
	}
	
}

//#####################################################################

int segmentalloc(int totalsize,int request)
{
	
#ifdef DEBUG 
printf("\n Inside the allocation routine:\n");
#endif 

flevel=0;	//everytime I come to allocation I reset flevel to 0 
 
	if(request>totalsize)
	{
	
		//check if requested size > total size 
		
		printf(" * Requesting more than total memory ??? \n");
		printf(" * Requested memory size= %d,total memory size=%d ", request,totalsize);
		printf("\n Press a key to continue...");
		getchar();
		return 0; // Should  I return 0 here ?
	}
	// DEBUG let us see the array at beginning
    
    
	while(1)  // This infinite loop is safe as we have already checked requesd size vs total size
	{
		#ifdef DEBUG 
		printf("Requested memory size= %d, current memory block size being examined=%d ", request,totalsize);
		#endif	
	
	if((request<=totalsize) && (request>(totalsize/2)))
			break; // found the right buddy level to check the availability of memory
	else	
			{  
			#ifdef DEBUG
			printf("\n Not yet found the right block; tree level=%d ; current memory block is : %d \n",flevel,totalsize);
			#endif
			//go dividing till you reach the right level
			totalsize/=2;
			flevel++;
			}
	} //end of while
	
	#ifdef DEBUG
	printf("\n correct tree level found=%d ; memory block to be allocated is : %d \n",flevel,totalsize);
    #endif 
    
	// Check at level=flevel of the tree from left to right to see where the match is happening 
	 
	for(i=power(2,flevel)-1;i<=(power(2,flevel+1)-2);i++)  // this formula is corrected provided root node =0
		{
#ifdef DEBUG 
		printf("DEBG : In allocation : value of i=%d\n",i);
#endif		 
		if(tree[i]==0 )  //  && place(i) same thing ?? place(i) will return 1 if available
		{
			// I am storing block which is allocated
			incrementNode();
			tree[i]=totalsize;
			//this is also the place to record the no of nodes that are allocated at a tree level 
			incrementArray(total_level_alloc,flevel);
			
			#ifdef DEBUG
			printf("\n DBG: allocated ; flevel= %d, current value=%d",flevel,total_level_alloc[flevel]); 
			#endif 
			
			makedivided(i); // without this, the allocation will NOT show
			printf(" \n ......Successful Allocation.......... \n");
			break;
		}
	//Here it comes out at least once after exhausting this loop 
	
	if(i==power(2,flevel+1)-1)  // first element in the next level
	{
		printf("				   Result  :  ");
		printf("*  NOT enough free memory\n");
		totalsize=0;
		}
	} //end of for 

#ifdef DEBUG 	
	debug_print();
#endif	
	return totalsize; // return the value that has been successfully allocated
} 
//##########################################################################

//default is 0 for a level 
//when it is set to a value greated than 1 i.e. 1024 or 256 etc, then it is getting used 
//then this counter should be incremented for a level 


void incrementNode()
{

created_node++;
if (created_node==(MAXNODES-1))
	{
		printf("\n You are reaching the limit of array implementing the binary tree...");
		exit(0);
	}
	
}

//#####################################################################
//debug print 

void debug_print()

{
	int index;
	for (index=0;index<=100;index++)
		{
	printf("%d th value in tree =%d\n", index, tree[index]); 
	printf("%d th value in total_level_alloc =%d\n", index, total_level_alloc[index]); 
	printf("%d th value in total_level_free =%d\n", index, total_level_free[index]); 
	
		}
	
}
//#################################################################################
// POWER OF I 

int power(int x,int y)
{
	int z,ans;
	if(y==0)
	return 1;
	ans=x;
	for(z=1;z<y;z++)
	ans*=x;
	return ans;
} 
//#############################################################################

void makedivided(int node)
{
#ifdef DEBUG 
printf("DEBUG makeDivided : starting value of node=%d\n", node);	
#endif

while(node!=0)  // This is a recursive division
	{
node=(node%2==0?(node-1)/2:node/2); // ex- for 16, it becomes 7 and for 17, it becomes 8 

#ifdef DEBUG 
printf("DEBUG makeDivided : node=%d marked as 1 or divided \n", node); 
#endif

tree[node]=1;
//here we are marking the node as divided and it is going away from the pool 
//here we should decrement total_level_free but this is NOT allocation
incrementArray(total_level_used,flevel);
//printf("\n DBG: node used ; flevel= %d, current value=%d",flevel,total_level_alloc[flevel]); 
	}
}
//###############################################################################

int place(int node)
{
while(node!=0) // for recursive
	{

	#ifdef DEBUG 	
	printf("DEBUG place(int) : starting value of node=%d\n", node);
	#endif
	
	node=node%2==0?(node-1)/2:node/2;

	//divided 1 Free 0 and Allocated >1 .. so check for free
	// Check recursively

#ifdef DEBUG 
	printf("DEBUG placeint : tree[node]=%d\n", tree[node]);
#endif 
	
	if(tree[node]>1)
	return 0; // not available
	}//for recursive while
	return 1;  // available
}
//################################################################################
//  FREE NODE  & COALESCE RECURSIVELY

void makefree(int request)
{
int node =0;
int match=0;

if (node >maxlevel)
			{
		printf("The Size you are trying to free has not been allocated\n");
		printf("Please check current memory allocation \n");
		   return;
			}

for (i=0;i<=MAXNODES;i++)  // if not matching it will NOT stop..logically I should match till maxnodes ..if not found say not there
	{
	#ifdef DEBUG 
	printf("DEBUG makefree : node=%d,  tree[node]=%d and request = %d\n", node, tree[node],request);
	#endif

	if(tree[node]==request)
		{  // match the requested size in the array .. if matched, then break from while with node
		match=1;
		break;
		}
	else
		 node++;
	} //for loop

// now check if we have found what is to be freed 
if (match)
	{
	tree[node]=0; 
	decrementArray(total_level_alloc,flevel);
	printf ("\n DEBG : flevel =%d, current value =%d",flevel,total_level_alloc[flevel]);
	//BUT how do I know the level from the node number ??
	//below is the coalescing logic
	if (lazy==1)
		return; //this is the LAZY mode exit 
	}
else 
	{
	printf("\n Sorry !! Trying to free a memory block that is not yet allocated ?");
	return;
	}

// set the flag to "free"
// here we are creating this node at this level 
// her we should increment total_level_free[level]


while(node!=0) 

    //If it is the 1st node, no coalescing 
	// Otherwise we try to find its buddy and megre !
	{
	if(tree[node%2==0?node-1:node+1]==0 && tree[node]==0) 
	//if(tree[node%2==0?node-1:node+1]==0) // check if the buddy node is free .see .[buddy logic]..free means 0
	// In case of remainder 0, it's the even and pick up the previous one for buddy like 1,2,3,4,5,6,...
	// Otherwise, it is the odd and pick up the next one for buddy  
		{
		tree[node%2==0?(node-1)/2:node/2]=0;
		node=(node%2==0?(node-1)/2:node/2);  // This is where it is recursively happening..check now for the next level
		}
	else  //nothing else to merge
		break; //come out of wheel 
		}
	} // end of while 



//PRINT NODE ALLOCATION

void printing(int totsize,int node)
 {
int permission=0,llimit,ulimit,tab;

if(node==0)
	permission=1;
else 

if(node%2==0)
	permission=tree[(node-1)/2]==1?1:0;
else
	permission=tree[node/2]==1?1:0;

if(permission)
	{
llimit=ulimit=tab=0;
	while(1)
	{
	if(node>=llimit && node<=ulimit)
		{
		#ifdef DEBUG 
		printf("DEBUG1 value of node: %d \n",node);
		#endif 
		break;
		}
		else
		{
		tab++;
		printf(" ");
		llimit=ulimit+1;
		ulimit=2*ulimit+2;
		}
	}//while 

// a node can have 3 status : divided (=1), free (=0) and allocated (>1) frequested size

printf("\n Current Buddy Block: %d",totsize/power(2,tab)); 
 
	if(tree[node]>1)
		printf("--->allocated :%d",tree[node]);
	else 
		if(tree[node]==1)
		printf("--->divided"); 
	else printf("-->Free");
		printing(totsize,2*node+1);
		printing(totsize,2*node+2);
	}//permission
}

void array_dynamic(int *ptr, int n)
 {
     
    ptr=(int*)malloc(n*sizeof(int));  //memory allocated using malloc
    
    if(ptr==NULL)                     
		{
        printf("Error! memory not allocated.");
        exit(0);
		}
	
	#ifdef DEBUG 
    printf("initialize the array...");
	#endif 
    
    for(i=0;i<n;++i)
		{
        *(ptr+i)=0;
		}
   //U need to free this memory
   
}
//############################################

void incrementArray(int arr[], int index)
{
    
    arr[index]++;     // this alters values in array in main()
}

//##################################################

void decrementArray(int arr[], int index)
{
    
    arr[index]--;     // this alters values in array in main()
}

//######################################################

int getLevel(int totalMem)
	{
 int l=1;
 int total =totalMem;
 
 while (total != power(2,l))
		{ 	
     l++;
     #ifdef DEBUG 
     printf("\n DEBG : Total memory= %d",total);
     printf("\n DEBG getLevel %d l=",l);	
     #endif
		}
	return l;
	}
