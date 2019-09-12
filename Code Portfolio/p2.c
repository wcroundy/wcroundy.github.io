//This program takes a linux command as an argument from the command line.  
//It then forks from a parent process to a child process which executes the linux
//command argument.  The child process terminates and the parten process resumes to 
//display information and then the parent process terminates as well.


#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>




int main(int argc, char *argv[])
{
 int args = argc-1;
 //argv[argc+1] = NULL;
 pid_t childpid = fork();
//error
 if(childpid < 0)
 {
  perror("fork() error");
  exit(-1);
 }
//parent process
 if(childpid != 0)
 {
  printf("Parent Process started, now waiting for ID: %d\n",childpid);
  wait(NULL);
  printf("Parent Process resumeed. Child exit code 0. Now terminating\n");
  exit(0);
 }
//child process
  if(args > 0)
  {
   printf("Child process has begun.  %d argument/s provided\n",args);
   //execvp(argv[1],  argv);
   int i;
   for(i = 1; i <= argc; i++)
   {
     if(argc <= 2)
     {
        execlp(argv[i] ,argv[i], NULL);
     }
     else
     {
        //execpv(argv[i], argv);
        execvp(argv[i], &argv[i]);
     }
   }
  }
  else
  {
   printf("No arguments provided, terminating child\n");
  }
 return 0;
}
