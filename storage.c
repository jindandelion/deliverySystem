#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

/* 
  definition of storage cell structure ----
  members :
  int building : building number of the destination
  int room : room number of the destination
  int cnt : number of packages in the cell
  char passwd[] : password setting (4 characters)
  char *contents : package context (message string)
*/
typedef struct {
	int building;
	int room;
	int cnt;
	char passwd[PASSWD_LEN+1];
	//deliverySystem[x][y].passwd[PASSWD_LEN+1]
	char *context;
} storage_t;


static storage_t** deliverySystem; 			//deliverySystem
static int storedCnt = 0;					//number of cells occupied
static int systemSize[2] = {0, 0};  		//row/column of the delivery system 
static char masterPassword[PASSWD_LEN+1];	//master password

//static int row;
//static int column;


// ------- inner functions ---------------

//print the inside context of a specific cell
//int x, int y : cell to print the context
static void printStorageInside(int x, int y) {
	printf("\n------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n");
	if (deliverySystem[x][y].cnt > 0)
		printf("<<<<<<<<<<<<<<<<<<<<<<<< : %s >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", deliverySystem[x][y].context);
	else
		printf("<<<<<<<<<<<<<<<<<<<<<<<< empty >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		
	printf("------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n\n");
}

//initialize the storage
//set all the member variable as an initial value
//and allocate memory to the context pointer
//int x, int y : cell coordinate to be initialized
static void initStorage(int x, int y) {
	//initialize specific storage
	deliverySystem[x][y].cnt=0;
//	printf("package is getout of the deliverySystem\n");
	
	deliverySystem[x][y].context = (char*)malloc(sizeof(char)*20);
	deliverySystem[x][y].building = 0;
	deliverySystem[x][y].room = 0;
	strcpy(deliverySystem[x][y].passwd, "aaaa");
	strcpy(deliverySystem[x][y].context, "qqqq");
	
	//fscanf(fp,"%s",deliverySystem[x][y].context);
	 
		
}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check 
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y){
	 
	char inputpasswd[PASSWD_LEN+1];
	
	printf("- input password for (%d,%d) :",x,y);
	//Input password
	scanf("%s",inputpasswd);
	//Compare password is match or not.
	
	//if inputpasswd&(passwd or masterpassword) is same. 
	
	if(strcmp(inputpasswd,deliverySystem[x][y].passwd)==0||strcmp(inputpasswd,masterPassword)==0)
	{
		return 0;
		//password is matching
	}
	//if inputpassword not correct
	else
	{
		return -1;
		//password is not matching
	}
	
}



// ------- API function for main.c file ---------------

//backup the delivery system context to the file system
//char* filepath : filepath and name to write
//return : 0 - backup was successfully done, -1 - failed to backup
int str_backupSystem(char* filepath) {
	//variable information:x is specific row, y is specific column variable
	int x,y; 
	
	FILE *fp;
	//I open file mode"w",so storage.txt will be empty.
	fp=fopen(filepath,"w");
	
	//Print All Information again in storage.txt
	fprintf(fp,"%d %d\n",systemSize[0],systemSize[1]);
	fprintf(fp,"%s\n",masterPassword);
	
	for(x=0;x<systemSize[0];x++)
	{
		for(y=0;y<systemSize[1];y++)
		{
			if(deliverySystem[x][y].cnt!=0)
			{
				fprintf(fp,"%d %d %d %d ",x,y,deliverySystem[x][y].building,deliverySystem[x][y].room);
				fprintf(fp,"%s %s\n",deliverySystem[x][y].passwd, deliverySystem[x][y].context);
			}
		
		}
	
	}
	//close the file.
	fclose(fp);
	return 0;
}



//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {
	//create variable 
	int i,j;
	int x,y;//this variables for row&column
	char c;
	
	FILE *fp;
	fp=fopen(filepath,"r");//open storage.txt file mode reading.
	
	fscanf(fp,"%d %d ",&systemSize[0],&systemSize[1]);
	
	deliverySystem=(storage_t**)malloc(systemSize[0]*sizeof(struct storage_t*));
	for(i=0;i<systemSize[0];i++)
	{
		deliverySystem[i]=(storage_t*)malloc(systemSize[1]*sizeof(storage_t));
	}
	
	for(i=0;i<systemSize[0];i++)
	{
		for(j=0;j<systemSize[1];j++)
		{
			initStorage(i, j);
		}
	}

	fscanf(fp,"%s ",masterPassword);
	
	
		while( !feof(fp) )
		{
			//Require declare variables(x,y)
			fscanf(fp,"%d %d",&x,&y);
			deliverySystem[x][y].cnt=1;
			fscanf(fp,"%d %d %s",&deliverySystem[x][y].building,&deliverySystem[x][y].room,deliverySystem[x][y].passwd);
			fscanf(fp,"%s",deliverySystem[x][y].context);
			storedCnt++;
		}
		//for test
		
		/*for(i=0;i<systemSize[0];i++)
		{
			for(j=0;j<systemSize[1];j++)
			{
				printf("%d %d %d %d %s\n", i, j, deliverySystem[i][j].building, deliverySystem[i][j].room, deliverySystem[i][j].passwd);
			}
		}*/
		
		
		//allocate memory again for context.
		//deliverySystem[x][y].context = (char*)malloc(sizeof(char)*20);
	    //fscanf(fp,"%s",deliverySystem[x][y].context);

		if(fp == NULL)
		{
			return -1;
		}

		fclose(fp);
	
		return 0;
		
	
}

//free the memory of the deliverySystem 
void str_freeSystem(void) {
	//Use "for" and free memory of the deliverySystem.
	int i,j;

	for(i=0;i<systemSize[0];i++)
	{
		free(deliverySystem[i]);
	}
	free(deliverySystem);

}



//print the current state of the whole delivery system (which cells are occupied and the destination of the each occupied cells)
void str_printStorageStatus(void) {
	int i, j;
	printf("----------------------------- Delivery Storage System Status (%i occupied out of %i )-----------------------------\n\n", storedCnt, systemSize[0]*systemSize[1]);
	
	printf("\t");
	for (j=0;j<systemSize[1];j++)
	{
		printf(" %i\t\t",j);
	}
	printf("\n-----------------------------------------------------------------------------------------------------------------\n");
	
	for (i=0;i<systemSize[0];i++)
	{
		printf("%i|\t",i);
		for (j=0;j<systemSize[1];j++)
		{
			if (deliverySystem[i][j].cnt > 0)
			{
				printf("%i,%i\t|\t", deliverySystem[i][j].building, deliverySystem[i][j].room);
			}
			else
			{
				printf(" -  \t|\t");
			}
		}
		printf("\n");
	}
	printf("--------------------------------------- Delivery Storage System Status --------------------------------------------\n\n");
}



//check if the input cell (x,y) is valid and whether it is occupied or not
int str_checkStorage(int x, int y) {
	if (x < 0 || x >= systemSize[0])
	{
		return -1;
	}
	
	if (y < 0 || y >= systemSize[1])//systemSize[1] is size of column
	{
		return -1;
	}
	
	return deliverySystem[x][y].cnt;
}


//put a package (msg) to the cell
//input parameters
//int x, int y : coordinate of the cell to put the package
//int nBuilding, int nRoom : building and room numbers of the destination
//char msg[] : package context (message string)
//char passwd[] : password string (4 characters)
//return : 0 - successfully put the package, -1 - failed to put
int str_pushToStorage(int x, int y, int nBuilding, int nRoom, char msg[MAX_MSG_SIZE+1], char passwd[PASSWD_LEN+1]) {

	int i;
	//if deliverySystem[x][y] is empty, we can store package there so return 0;
	if(deliverySystem[x][y].cnt==0)
	{
		//print input number at the storage.txt 
		//fprintf("%d %d %d %d %s %s",x,y,nBuilding,nRoom,msg[MAX_MSG_SIZE+1],passwd[PASSWD_LEN+1]);
		deliverySystem[x][y].building=nBuilding;
		deliverySystem[x][y].room=nRoom;
		deliverySystem[x][y].context=msg;
		for(i=0;i<sizeof(passwd[PASSWD_LEN+1]); i++)
			deliverySystem[x][y].passwd[i]=passwd[i];
		
		deliverySystem[x][y].cnt=1;
		storedCnt++;//+1 number of cells occupied
		
		return 0;
	}
	else
	{
		return -1;
	} 
}



//extract the package context with password checking
//after password checking->function putpasswd, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {
	//If password is matching, inputPasswd function return 0.
	inputPasswd(x, y);
	
	//if password not matching I want finish this turn so return -1.
	if(inputPasswd(x,y)!=0)
	{
		printf(" -----------> Password is wrong!\n");
		return -1;
	}
	//if password is matching, delivery system give user's package.
	else
	{
		printStorageInside(x,y);//print the inside context of a specific cell.
		initStorage(x,y);//extract package initialize that storage.
		storedCnt--;
		
		return 0;
	}
	//-1 number of cells occupied
}

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) {
	//packagecnt is return value of this function. if return value is 0, print"failed find package" in the main function 
	
	//variable packagecnt:if there are package that user want to find, packagecnt is not 0 anymore.so return not zero.

	int i,j;
	int cnt=0;
	for(i=0;i<systemSize[0];i++)
	{
		for(j=0;j<systemSize[1];j++)
		{
			if(deliverySystem[i][j].building==nBuilding&&deliverySystem[i][j].room==nRoom)
			{
				printf("------------>Found a package in (%d,%d)\n",i,j);
				cnt++;
			}
		}
	}
	return cnt;
}
