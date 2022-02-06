#include <stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


struct player_inf{

char a;
int thread_id;
};


//creating mutex
pthread_mutex_t mat_locker = PTHREAD_MUTEX_INITIALIZER;


int self;
int turn=0;
int N;
char **table; 
typedef enum {T,F} boolean;
boolean full_check= F;
boolean winner_check= F;

void* game_func(void* arg){
struct player_inf* args = (struct player_inf*)arg;
int row,column;
boolean keep_search1;
while(full_check==F){
while(turn!=args->thread_id){
;
}
pthread_mutex_lock(&mat_locker);

if(winner_check==T){
break;
}
if(full_check==T){
	break;
}
boolean index_exist= T;
while(index_exist==T){
	row= rand()%N;
	column= rand()%N;
	if(table[row][column]==' '){
		if(winner_check==F){
	table[row][column]=args->a;
	printf("PLayer %c played on: (%d,%d)\n",args->a,row,column);
	index_exist=F;
	}
	}
}

keep_search1=F;

for (int i=0;i<N;i++){
	if(table[row][i]!=args->a){
	keep_search1=T;
	break;
}
}
if(keep_search1==T){
	keep_search1=F;
	for(int i=0;i<N;i++){
		if(table[i][column]!=args->a){
			keep_search1=T;
			break;
		}
	}
}

if(keep_search1==T){
keep_search1=F;
if(row==column){
	for(int i=0;i<N;i++){
		if(table[i][i]!=args->a){
		keep_search1=T;
		break;
		}
	}

}
else{
keep_search1=T;
}
}
if(keep_search1==T){
	keep_search1=F;
	if(row==N-(column+1)){
		for(int i=0; i<N;i++){
			for(int k=0;k<N;k++){
				if(i==N-(k+1)){
					if(table[i][k]!=args->a){
						keep_search1=T;
						break;
					}
				}
			}	
		}

}	
else{
keep_search1=T;
}

}
if(keep_search1==F){
	winner_check=T;
	printf("Game End\n");
	printf("Winner is %c \n",args->a);
	self=args->thread_id;
	if(self==0){
	turn=1;
	}	
	else if(self==1){
	turn=0;
	}
	pthread_mutex_unlock(&mat_locker);
	break;
}
else if(keep_search1==T){
	winner_check=F;	
}

full_check=T;
for (int i = 0 ; i<N;i++){
	for(int k=0;k<N;k++){
		if(table[i][k]==' '){
			full_check=F;
			break;
		}
	}

}
self=args->thread_id;
if(self==0){
turn=1;
}
else if(self==1){
turn=0;
}
pthread_mutex_unlock(&mat_locker);
//sleep(1);
}
if(winner_check==F){
	if(full_check==T){
	if(args->thread_id==0){
	printf("Game End\n");
	printf("It is a tie \n");
	}
	}
}
return NULL;
}










int main(int argc,char* argv[]){
clock_t start= clock();
srand(time(NULL));
N= atoi(argv[1]);
table= (char **) malloc(sizeof(char*)*N);
for(int j = 0; j<N;j++){
	table[j]= (char*)malloc(sizeof(char)*N);
}
printf("Board Size: %dx%d\n",N,N);
for(int i=0;i<N;i++){
	for(int k =0;k<N;k++){
		table[i][k]=' ';
	}
}


pthread_t threadx,thready;
//struct variables
struct player_inf p1={'X',0};
struct player_inf p2={'O',1};

// creating threads and checking them if they created succesfully
if(pthread_create(&threadx,NULL,game_func,&p1)!=0){
	return 1;
}

if(pthread_create(&thready,NULL,game_func,&p2)!=0){
	return 2;
}

pthread_join(threadx,NULL);
pthread_join(thready,NULL);
for(int i =0;i<N;i++){
	for(int k=0;k<N;k++){
		
		printf("[%c]",table[i][k]);

	
}
printf("\n");
}




return 0;
}
