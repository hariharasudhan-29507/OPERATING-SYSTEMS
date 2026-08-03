#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int gcd(int a, int b) {
   while (b!=0) {
      int temp = b;
      b = a%b;
      a = temp;
   }
   return a;
}
int main () {
   int fd[2];
   pid_t pid;
   int n1,n2;
   if (pipe(fd)==-1) {
      perror("\nPipe creation failed\n");
      exit(1);
   }
   pid=fork();
   if (pid==0) {
      close(fd[0]);
      printf("Child Process P1 :\nEnter number 1 : ");
      scanf("%d",&n1);
      printf("Enter number 2 : ");
      scanf("%d",&n2);
      write(fd[1],&n1,sizeof(int));
      write(fd[1],&n2,sizeof(int));
      close(fd[1]);
   }
   else {
      close(fd[1]);
      read(fd[0],&n1,sizeof(int));
      read(fd[0],&n2,sizeof(int));
      close(fd[0]);
      wait(NULL);
      printf("Parent Process P2:\n Numbers recieved is %d and %d",n1,n2);
      printf("\nParent Process P2:\n GCD of the given numbers is :  %d \n",gcd(n1,n2));
   }
   return 0;
}

