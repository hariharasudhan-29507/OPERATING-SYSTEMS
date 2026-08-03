#include<stdio.h>
#include<unistd.h>
void main() {
   printf("Process Created :- ");
   int p2;
   p2 = fork();
   if (p2==0) {
       int p4;
       p4 = fork();
       if (p4==0) {
	  printf("P4\n Process id :%d\t Parent id:%d\n",getpid(),getppid()); }
       else {
	  printf("P2\n Process id :%d\tParent id:%d\n",getpid(),getppid());}
   }
   else {
       int p3;
       p3 = fork();
       if (p3==0) {
           int p5;
	   p5 = fork();
	   if(p5==0) {
	       printf("P5\n Process id :%d\t Parent id:%d\n",getpid(),getppid());
	    }
	    else {
	       printf("P3\n Process id :%d\t Parent id:%d\n",getpid(),getppid()); 
            }
	}
	else {
	    printf("P1\nProcess id :%d\n",getpid());
	    wait(NULL);
	}
    }
 }

