#include <stdio.h>
#include <stdlib.h>


int main(){
	int n=0;
	if (system("gcc mine.c -o run/mine.run -Wall -Wextra >log.txt")){
		if (system("g++ mine.cpp -o run/mine.run -Wall -Wextra >log.txt")){
			printf("[Compile Error]\n");
			return 0;
		}
	}
	system("g++ gen.cpp -o run/gen.run -O2 -g -lm -std=c++17 -Wall -Wextra");
	if (system("g++ master.cpp -o run/master.run -w -std=c++17")){
		if (system("gcc master.c -o run/master.run -w -std=c++17")){
			printf("[Compile Error]\n");
			return 0;
		}
	}
	while(1){
		n++;
		system("run/gen.run >run/t.in");
		if (system("run/mine.run <run/t.in >run/mine.out")){
			printf("[Runtime Error at test %d]\n",n);
			return 0;
		}
		system("run/master.run <run/t.in >run/master.out");
		if(system("diff run/master.out run/mine.out")){
			printf("[Wrong Answer at test %d]\n",n);
			return 0;
		}
		else printf("[Acceppted]\n");
	}
	return 0;
}
