#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>


typedef struct{
char gender_name[40];
char first_name[40];
char last_name[40];

}person_info;

person_info* person_array;
person_info person_line;

int line_counter=0;
typedef enum {T,F} boolean;
boolean bool_check=F;
int database_reader(){

FILE* reader;
reader= fopen("database.txt","r");
if(reader==NULL){
printf("we could not read the database.txt\n");
return 1;
}

int word_counter=0;
char word1[40];
char word2[40];
char word3[40];
char word4[40];
int loop_counter=0;
person_info arr1[40];
char buffer[200];
fgets(buffer,200,reader);
int return_count=0;
while(!feof(reader)){

	person_info* p=person_array+return_count;
	sscanf(buffer,"%s %s %s",p->gender_name,p->first_name,p->last_name);
	fgets(buffer,200,reader);	
	return_count++;
}

fclose(reader);





return 0;
}

void function1(char* name_dir){

DIR* dir = opendir(name_dir);
if(dir==NULL){

return ;
}
struct dirent* type_dir;
type_dir=readdir(dir);
printf("reading files from %s \n",name_dir);
while(type_dir != NULL){
	char concete[100]={0};
		strcat(concete,name_dir);
		strcat(concete,"/");
		strcat(concete,type_dir->d_name);
	char * is_exist;
	is_exist=strstr(type_dir->d_name,".txt");
	if(is_exist){
		if(strcmp(concete,"./database.txt")==0){
		int rand_num=0;
		}
		else{
		// read the text
		FILE * read2;
		char word2[40];
		read2= fopen(concete,"r+");
		if(read2==NULL){
		return;
		}

		while(fscanf(read2,"%s",word2)==1){
		
			for(int i=0;i<line_counter;i++){
				if(strcmp(word2,person_array[i].first_name)==0){
				int len_word=strlen(word2);
				int mr_word= -1*(len_word+4);
				char word3[40];
				fseek(read2,mr_word,SEEK_CUR);
				fscanf(read2,"%s",word3);
				if(strcmp(person_array[i].gender_name,"m")==0){
					if(strcmp(word3,"Mr.")!=0){
						fseek(read2,-3,SEEK_CUR);
						fputs("Mr.",read2);
					}
				}
				else if(strcmp(person_array[i].gender_name,"f")==0){
					if(strcmp(word3,"Ms.")!=0){
						fseek(read2,-3,SEEK_CUR);
						fputs("Ms.",read2);
					}
				}
				fseek(read2,len_word+1,SEEK_CUR);
				fscanf(read2,"%s",word3);
				if(strcmp(word3,person_array[i].last_name)!=0){
					int len_word2= strlen(word3);
					len_word2=-1*len_word2;
					fseek(read2,len_word2,SEEK_CUR);
					fputs(person_array[i].last_name,read2);
				}
				 }
				
			}
						}
		fclose(read2);	
		}

	}
	else{
	if(type_dir->d_type==DT_DIR &&strcmp(type_dir->d_name,".")!=0&&strcmp(type_dir->d_name,"..")!=0){
		bool_check=T;
		function1(concete);
	}
	}
	type_dir=readdir(dir);
	

}


closedir(dir);
//return NULL;
}



int main(int argc,char* argv[]){


FILE * read;
read= fopen("database.txt","r");
if(read==NULL){
printf("we could not read the database.txt\n");
return 1;
}

int word_counter=0;
char word[40];
//char *word=NULL;
while(fscanf(read,"%s",word)==1){
	word_counter++;
}
line_counter=word_counter/3;
fclose(read);
person_array= (person_info*)malloc(line_counter * sizeof(person_info));
database_reader();
function1(".");
return 0;
}
