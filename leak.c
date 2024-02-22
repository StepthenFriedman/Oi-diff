#include<stdio.h>
#include<stdlib.h>
#define BYTE 1000000

int main(){
    int n;
    scanf("%d",&n);
    int* arr=malloc(n*BYTE*sizeof(int));
    for (int i=0;i<n*BYTE-1;i++) arr[i+1]=i+arr[i];
    printf("%d\n",arr[n*BYTE-1]);
}