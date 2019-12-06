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
	//initialize specific storage
	
	
	//Delete the saved contents of the file.
	/*File *fp;
	fp=fopen(STORAGE_FILEPATH,"w");//쓰기모드로 열면 원래 있던거 다 자동으로 지워져.  */
	
	//파일 쓰지말고 그냥 deliverySystem이걸로 초기화 하는 방법 생각.
	//deliverySystem[x][y]= 
		
	
	
}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check 
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y){
	 
	char inputpasswd[PASSWD_LEN+1];
	
	printf("- input password for (%d,%d) :",x,y);
	//Input password
	scanf("%s",&inputpasswd); 
	/////////////////////////////////////////////////////////
	//Compare password is match or not.
	if(!strcmp(inputpasswd,deliverySystem[x][y].passwd[PASSWD_LEN+1]))//if inputpasswd&passwd is same.  
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
	
	int i;
	
	FILE *fp;
	//I open file mode"w",so storage.txt will be empty.
	fp=fopen(filepath,"w");
	
	//Print All Information again in storage.txt
	fprintf("%d %d\n",systemSize[0],systemSize[1]);
	fprintf("%d\n",masterPassword[PASSWD_LEN+1]);
	
	for(i=0;i<storedCnt;i++)
	{
		//아니 그래서 내가 헷갈리는건 .....음음음음ㅇ 어케해!!! 그 row랑 column불러오는것부터,,, x,y어케 저장해주냐고.. 
		//Requier of declare variables(x,y)
		fprintf("%d %d %d %d",/*row*/,/*column*/,deliverySystem[x][y].building,deliverySystem[x][y].room)
		fprintf("%s %s",deliverySystem[x][y].passwd,deliverySystem[x][y].context);
	}
	//바뀐것대로 다 저장해줘야되는건가? 어떻게? 
	//프로그램이 한번 돌아갈때마다 백업 해줘야 하는 건데, 그러면 원래 있던 내용 싹다 지우고
	//바뀐 내용부터 다시 처음부터 써줘도 되는가요?
	//백업이 실패한다는게 무슨뜻인지 잘 모르겠습니다. 
}



//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {
	//create variable 
	int i,j;
	int inputrow,inputcolumn;
	char c;
	
	FILE *fp;//여기 이렇게 NULL하는거 맞겠지 
	fp=fopen(filepath,"r");//open storage.txt file
	fscanf(fp,"%d %d",&systemSize[0],&systemSize[0]);
	fscanf(fp,"%s",masterPassword);
	
	if(fp=NULL)
	{	
		return -1;
	} 
	
	else
	{
		//allocate memory
		deliverySystem=(struct storage_t**)malloc(systemSize[0]*sizeof(struct storage_t*));
	
		for(i=0;i<systemSize[0];i++)
			deliverySystem[i]=(struct storage_t*)malloc(systemSize[1]*sizeof(storage_t));
		
		while(fp!=EOF)
		{
			//Require declare variables(x,y)
			fscanf(fp,"%d %d",&x,&y);
			fscanf(fp,"%d %d %s\n",&deliverySystem[x][y].building,deliverySystem[x][y].room,deliverySystem[x][y].passwd/*[PASSWD_LEN+1]*/);
		}
		
		deliverySystem[x][y].context = (char*)malloc(sizeof(char)*20);
	    fscanf(fp,"%s",deliverySystem[x][y].context);
	    
		return 0;
	}		
	
	fclose(fp);
}

//free the memory of the deliverySystem 
void str_freeSystem(void) {
	//Use "for" and free memory of the deliverySystem.
	int i,j;
	
	for(i=0;i<systemSize[0];i++)
	{
		for(j=0;j<systemSize[1];j++)
		{
			free(deliverySystem[i][j].context);
		}
	}

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
	if(deliverySystem[x][y]==/*NU에 저장된 값이 없다면,LL*/)
	{
		//print input number at the storage.txt 
		//fprintf("%d %d %d %d %s %s",x,y,nBuilding,nRoom,msg[MAX_MSG_SIZE+1],passwd[PASSWD_LEN+1]);
		diliverySystem[x][y]->building=nBuilding;
		diliverySystem[x][y]->building=nRoom;
		diliverySystem[x][y]->building=msg[MAX_MSG_SIZE+1];
		diliverySystem[x][y]->building=passwd[PASSWD_LEN+1];
		
		return 0;
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
	
	//if password not matching I want finish this turn.
	if(inputPasswd(x,y)!=0)
	{
		return -1;/////////////////////////////////////////////////////////
	}
	else
	{
		printStorageInside(x,y);//print the inside context of a specific cell.
		initStorage(x,y);//extract package initialize that storage.
		
		return 0;
	}
	storedCnt--;//--number of cells occupied
}

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) {
	//packagecnt is return value of this function. if return value is 0, print"failed find package" in the main function 
	//if there are package that user want to find, packagecnt is not 0 anymore.
	int packagecnt;
	int i,j;
	/*file에서 찾지마라!!*/
	for(i=0;i<systemSize[0];i++)
	{
		for(j=0;j<systemSize[1];j++)
		{
			if(deliverySystem[i][j].building==nBuilding&&deliverySystem[i][j].building==nRoom)
			{
				printf("------------>Found a package in (%d,%d)\n",x,y);
				packagecnt++;
			}
		}
	 
	return packagecnt;
}
