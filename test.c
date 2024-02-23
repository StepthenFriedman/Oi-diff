#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void intHandler(int sig) {
    printf("exit!-----------------------\n");
	exit(0);
}

int main(){
    signal(SIGINT, intHandler);
    int n=0;
    while (1){
        system("sleep 1");
        printf("%d ",n++);
    }
}