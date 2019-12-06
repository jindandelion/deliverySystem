#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

//#define   ROW     4 //GARO 
//#define   COLUMN  6 //SERO
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
	char passwd[PASSWD_LEN+1];//이렇게 해줘야 크기 5인 배열 생성 되서 마지막 칸에 \0저장해줄 수 있으니까. 
	//deliverySystem[x][y].passwd[PASSWD_LEN+1]
	char *context;
} storage_t;


static storage_t** deliverySystem; 			//deliverySystem
static int storedCnt = 0;					//number of cells occupied
static int systemSize[2] = {0, 0};  		//row/column of the delivery system 메모장에 이거 적혀있음 잘 끌어와봐. 
static char masterPassword[PASSWD_LEN+1];	//master password

static int row;
static int column;


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
	
	if(deliverySystem[x][y])
	
	
}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check ???????????????????//
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y) {
	
	char inputpasswd[PASSWD_LEN+1];
	
	printf("- input password for (%d,%d) :",x,y);
	scanf("%s",&inputpasswd); 
	
	if(!strcmp(inputpasswd,passwd)//if inputpasswd&passwd is same.  
	{
		return 0;//password is matching
	}
	else
	{
		return -1;//password is not matching
	}
}





// ------- API function for main.c file ---------------

//backup the delivery system context to the file system
//char* filepath : filepath and name to write
//return : 0 - backup was successfully done, -1 - failed to backup
int str_backupSystem(char* filepath) {
	
}



//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {
	//create variable 

	FILE *fp=NULL;//여기 이렇게 NULL하는거 맞겠지 
	fp=fopen(STORAGE_FILEPATH,"r");
	int i;
	int j;
	int inputrow,inputcolumn;
	char c;
	//if storage_filepath was not be opened, 
	/*if(fp!=NULL)
	{
		return 0;
	} 
	
	else
	{
		return -1;
	}*/
	deliverySystem=(struct storage_t**)malloc(ROW*sizeof(struct storage_t*));
	for(i=0;i<ROW;i++)
		deliverySystem[i]=(struct storage_t*)malloc(COLUMN*sizeof(storage_t));
	
	while((c=fgetc(fp))!=EOF)
		fscanf(fp,"%d %d %c",systemSize[0],systemSIze[1],passwd[PASSWD_LEN+1]);
				
	//fscanf(fp,"%d %d %d %s %s",&inputrow,&deliverySystem[i][j].)//메모장에 있는거를 diliverysystem이차원 배열에 저장 
	
}

//free the memory of the deliverySystem 
void str_freeSystem(void) {
	
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
	//if deliverySystem[x][y] is empty, we can store package there so return 0;
	if(deliverySystem[x][y]==NULL)
	{
		//storage.txt 에 저장해준다 아니 어떻게?  
		FILE *fp;
		//문자로 담을 변수 
		fp=fopen("storage.txt","w");
		
		scanf("%d %d %d %d %s %s",&x,&y,&nBuilding,&nRoom,&passwd,&msg);
		//fclose(fp);해줘야 되나? 
		return 0;
		//근데 입력해준게 맞는지도 이 함수에서 해야 되나? 
	}
	else
	{
		return -1;
	} 
	storedCnt++;//+1 number of cells occupied
}



//extract the package context with password checking
//after password checking->function putpasswd, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {
	//If password is matching, inputPasswd function return 0.
	inputPasswd(int x, int y);
	
	//if password not matching I want get out.
	if(inputPasswd(x,y)!=0)
	{
		break;/////////////////////////////////////////////////////////
	}
	else
	{
		printStorageInside(x,y);//print the inside context of a specific cell.
		initStorage(x,y);//extract package initialize that storage.
	}
	storedCnt--;//--number of cells occupied
}

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) {
	
	File *fp=fopen("storage.txt","r");
	int i;
	//int cnt;
	//메인에서 fopen해줬나? 아님 여기서 써줘야되는지 확인. 
	while((c=fgetc(fp))!=EOF)
	{
		 
	}
	//매개변수가 nBuilding이랑 nRoom이야.
	//메모장에 있는걸 하나씩 다 읽어야 되나? 
	
	/*if(deliverySystem[x][y].room==nRoom&&deliverySystem[x][y].building==nBuilding)
	{
		
		for(i=0;i<cnt;i++)
		{
			printf("------------>Found a package in (%d,%d)\n",x,y);
			deliverySystem[x][y].cnt++;
		}	
	}*/
	fclose(*fp);
	
	return cnt;
}
