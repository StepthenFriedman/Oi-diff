#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXN 100005
#define MAXM 810005
#define LARGE 1000000001
#define ckmin(x,y) ((x)<(y)?(x):(x=y))

typedef struct Edge{
    int next,to,w;
}Edge;

int count=0;
int head[MAXM]={[0 ... MAXM-1]=-1};
Edge edge[MAXM];

int res[MAXN]={[0 ... MAXN-1]=LARGE};
char itered[MAXN]={0};

void add(int u,int v,int w)
{
    edge[count].w = w;
    edge[count].to = v;
    edge[count].next = head[u];
    head[u] = count++;
}

int n;

void dijkstra(){
    for (int t=1;t<=n;t++){
        int indx=0;
        for (int i=1;i<=n;i++) if ((!itered[i]) && res[indx]>res[i]){
            indx=i;
        }
        itered[indx]=1;

        for (int i=head[indx];~i;i=edge[i].next){
            ckmin(res[edge[i].to],res[indx]+edge[i].w);
        }
    }
}

int main(){
    int m,s;
    scanf("%d%d%d",&n,&m,&s);
    int u,v,w;
    for (int i=1;i<=m;i++){
        scanf("%d%d%d",&u,&v,&w);
        add(u,v,w);
    }
    res[s]=0;
    dijkstra();
    for (int i=1;i<=n;i++) printf("%d ",res[i]);
}