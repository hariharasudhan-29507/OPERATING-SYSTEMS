#include<stdio.h>
#include<unistd.h>
void printprime(int n) {
   printf("p2[Parent]\n First %d prime numbers are...\n",n);
   int count =0,num=2;
   while (count<n) {
      int i,isPrime =1;
      for (i=2;i*i<=num;i++) {
	 if(num%i==0) {
	    isPrime =0;
	    break;}
      }
      if (isPrime) {
	 printf("%d\t",num);
	 count++;
      }
      num++;
   }
   printf("\n");
}
void printfibo (int n) {
   printf("P3[Child]\n First %d Fibonacci Numbers are...\n",n);
   long a=0,b=1;
   int i;
   for (i=0;i<n;i++) {
      printf("%d\t",a);
      long next = a+b;
      a=b;
      b=next;
   }
   printf("\n");
}
void main() {
   int p2,n;
   printf("Enter the value of n:");
   scanf("%d",&n);
   p2 = fork();
   if(p2==0)
      printprime(n);
   else {
      int p3;
      p3 = fork();
      if (p3==0)
	 printfibo(n);
   }
}

