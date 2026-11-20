#include <stdio.h>
#include <unistd.h>

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

int main(){
    int r1 = task(5); //when we use larger numbers on functions like this it will take a long time
    int r2 = task(6); //can make these run on different processes
}

// ------task(5)-------- ----------task(6)---------- //