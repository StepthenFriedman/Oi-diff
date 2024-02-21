#include <cstdio>
#include <cstdlib>
#include <ctime>

int n=5;

int main(){
	srand(time(0));
	printf("%d %d 1\n",n,(n*(n-1))/2);
	for(int i=1;i<=n;i++){
		for (int j=i+1;j<=n;j++){
			printf("%d %d %d\n",i,j,(rand()%1000)+1);
		}
	}
}
