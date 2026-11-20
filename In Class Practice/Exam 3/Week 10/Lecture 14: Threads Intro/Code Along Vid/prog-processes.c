#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int task(int n){
    int result = 0;
    for (int i = 1; i <= n; i++)
    {
        result += i;
        sleep(i); //ex i = 2: sleep for 3 seconds, i = 10: sleep for 55 seconds (for each iteration it will sleep for ith seconds)
    }
    printf("result for n=%d is %d\n", n, result);

    return result;
}

int main(){ //now we are running this "parallel"
    //int *p = malloc(10000); -> problem with using processes for parallel computing is that each process gets a copy but what if we don't want each process to get 10000 bytes of memory?
    int a = fork();

    if (a == 0){ //child process
        int r = task(5);
        return 0;
    }
    //parent completes this task
    int r = task(6); 
    return 0;
}

//Parent process -------------task(6)------------- 21 secs
//Child process -------------task(5)---------- 15 secs
//child was already done by the time parent was done so overall code took around 21 seconds 