#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){

    pid_t p = fork();

    if (p < 0)
    {
        perror("fork fail");
        _exit(1);
    }

    printf("Process ID(PID) = %d \n",getpid());
        



    return 0; 
}