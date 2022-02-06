#include <stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

	

struct fan{
char a;
};

typedef enum {T,F} boolean;
pthread_mutex_t locker = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t locker2 = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;
sem_t semaphore;

pthread_cond_t condt1;
pthread_cond_t condt2;
boolean gates_open=F;
int team_a=0;
int team_b=0;
int total_count=0;
int captain_counter=0;
int semap_value=-999;


void* car_func(void* arg){
struct fan* args = (struct fan*)arg;
boolean can_exit=F;
printf("Thread ID: %ld, Team %c, I am looking for a car\n",pthread_self(),args->a);

pthread_mutex_lock(&locker);	
while(can_exit==F){

	if(args->a=='A'&& team_a==2){
		if(team_b==1){
			
			can_exit=F;
			pthread_cond_wait(&condt1,&locker);			
		}
		else{
		can_exit=T;
		pthread_mutex_unlock(&locker);
		}
	}
	else if(args->a=='B'&& team_b==2){
		if(team_a==1){
			can_exit=F;
			
			pthread_cond_wait(&condt2,&locker);
		}
		else{
		can_exit=T;
		pthread_mutex_unlock(&locker);
		}
	}
	else if(args->a=='A'&& team_b==3){
	can_exit=F;
	pthread_cond_wait(&condt1,&locker);
	}
	else if(args->a=='B'&& team_a==3){
	can_exit=F;
	pthread_cond_wait(&condt2,&locker);
	}
	else{
	can_exit=T;
	pthread_mutex_unlock(&locker);
	}
	
	}

sem_wait(&semaphore);
pthread_mutex_lock(&locker);	
if(args->a=='A'){
	team_a++;
		if((team_a+team_b)==4){
		team_a=0;
		team_b=0;
		}
}
else{
	team_b++;
	if((team_a+team_b)==4){
		team_a=0;
		team_b=0;
		}
}

pthread_mutex_unlock(&locker);






pthread_barrier_wait(&barrier);
printf("Thread ID: %ld, Team %c, I have found a spot in a car\n",pthread_self(),args->a);
pthread_mutex_lock(&locker2);
captain_counter++;
pthread_mutex_unlock(&locker2);

pthread_mutex_lock(&locker2);
if(captain_counter==4){
printf("Thread ID: %ld, Team %c, I am the captain and driving the car\n",pthread_self(),args->a);	
gates_open=T;
captain_counter=0;
	
}
pthread_mutex_unlock(&locker2);

sem_post(&semaphore);
pthread_cond_broadcast(&condt1);
pthread_cond_broadcast(&condt2);

return NULL;
}


int main (int argc,char* argv[]) {

int a_fans= atoi(argv[1]);
int b_fans= atoi(argv[2]);
if(a_fans==0&&b_fans==0){
printf("The main terminates \n");
return 0;
}
if(a_fans<0){
printf("The main terminates \n");
return 0;
}
if(b_fans<0){
printf("The main terminates \n");
return 0;
}
if((a_fans%2!=0)){
printf("The main terminates \n");
return 0;
}

if(b_fans%2!=0){
printf("The main terminates \n");
return 0;
}
if((a_fans+b_fans)%4!=0){
printf("The main terminates \n");
return 0;
}

pthread_t th1[a_fans];
pthread_t th2[b_fans];
struct fan p1={'A'};
struct fan p2={'B'};
sem_init(&semaphore,0,4);
pthread_barrier_init(&barrier,NULL,4);
pthread_cond_init(&condt1,NULL);
pthread_cond_init(&condt2,NULL);
for (int i =0; i<a_fans;i++){
pthread_create(&th1[i],NULL,car_func,&p1);

}
for (int i =0; i<b_fans;i++){
pthread_create(&th2[i],NULL,car_func,&p2);

}
for(int i=0; i<a_fans;i++){
pthread_join(th1[i],NULL);
}
for(int i=0; i<b_fans;i++){
pthread_join(th2[i],NULL);
}
printf("The main terminates\n");
return 0;
}
