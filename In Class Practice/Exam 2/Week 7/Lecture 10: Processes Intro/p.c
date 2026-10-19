#include <stdio.h>
#include <sys/wait.h> //must include this to use waitpid() function
#include <unistd.h>

int main() {
  int a = fork(); //process 1
  int b = fork(); //process 2
  //technically we can't exactly determine which process (parent or child) will run first, but we can make the parent wait for the chil
  printf("a = %d,b=%d,pid = %d\n", a, b, getpid()); //print the process ids
  if (a > 0 && b > 0) { //if we're in the parent, make the parent wait 
		waitpid(a,NULL,0);
		waitpid(b,NULL,0);
    printf("The main process 1, the god father\n");
  }
	if(a==0){
		if(b==0){
			printf("This is process 4\n");
		}
		if(b>0){
			printf("This is process 2\n");
		}
	}
	if(a>0 && b==0){
		printf("This is process 3\n");
	}	
  return 0;
}