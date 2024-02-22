#include <cstdio>
#include <cstdlib>
#include <ctime>


int main(){
	srand(time(0));
	int n=100000;
	printf("100000000 %d\n",n);
	for(int i=1;i<=n;i++){
		printf("%d\n",i);
	}
}
