#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  printf("This is a demonstration\n");
  int a = fork(); //immediately spawns a child process which is a clone (identical to) the parent process
  //both parent and child have exact same code except with different a values
  //both parent and child process will be running the following code in parallel 
  //this is why a > 0 excutes in the parent and a == 0 executes in child
  if (a == 0) {
    printf("This is in child process\n");
    printf("Child process id = %d\n", getpid());
  }
  if (a > 0) {
    printf("This is in parent process\n");
    waitpid(a, NULL, 0);
    printf("Parent process id = %d\n", getpid());
  }
  //changes made to memory in parent or child process stay in parent or child 
  printf("Bye!\n");
  return 0;
}