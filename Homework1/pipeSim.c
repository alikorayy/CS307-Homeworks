#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc,char* argv[]){
int fd[2];
if (pipe(fd)==-1){
return 1;
}
printf("I'm SHELL process, with PID: %d -Main command is: man ping | grep -A 4 -- '-c' > output.txt \n",getpid());
int pid1 = fork();
if(pid1<0){

return 1;
}
if(pid1==0){
printf("I'm MAN process, with PID: %d -My command is: man ping \n",getpid());
dup2(fd[1],STDOUT_FILENO);
close(fd[0]);
close(fd[1]);
char* arg1[]={"man","ping",NULL};
execvp(arg1[0],arg1);


}
waitpid(pid1,NULL,0);
int pid2=fork();
if(pid2<0){
return 3;}

if (pid2==0){
printf("I'm GREP process, with PID: %d, -My command is: grep -A 4 -- '-c' > output.txt \n",getpid());
dup2(fd[0],STDIN_FILENO);


int txt= open("output.txt",O_WRONLY|O_CREAT,0777);
if (txt==-1){

return 4;
}
dup2(txt,STDOUT_FILENO);
close(txt);
close(fd[0]);
close(fd[1]);
char* arg2[]={"grep","-A","4","--","-c",NULL};
execvp(arg2[0],arg2);

}

close(fd[0]);
close(fd[1]);
waitpid(pid1,NULL,0);
waitpid(pid2,NULL,0);
printf("I'm SHELL process, with PID: %d -execution is completed, you can find the results in output.txt \n",getpid());
return 0;
}
