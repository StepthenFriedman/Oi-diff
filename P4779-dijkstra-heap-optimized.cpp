#include <bits/stdc++.h>
#include <cstdio>
#define ckmin(x,y) ((x)<(y)?(x):(x=y))

const int MAXN=100005,MAXM=500005,LARGE=1000000001;

typedef struct Edge{
    int next,to,w;
}Edge;

struct node
{
    int res;
    int id;
    bool operator <(const node &x)const
    {
        return x.res<res;
    }
};

int count=0;
int head[MAXM];
Edge edge[MAXM];

int res[MAXN];
char itered[MAXN]={0};

void add(int u,int v,int w)
{
    edge[count].w = w;
    edge[count].to = v;
    edge[count].next = head[u];
    head[u] = count++;
}

int n;
std::priority_queue<node> q;

void dijkstra(){
    res[1]=0;
    q.push((node){0,1});
    while(!q.empty()){
        node tmp=q.top();
        q.pop();
        int r=tmp.res, id=tmp.id;
        if (itered[id]) continue;

        itered[id]=1;
        for (int i=head[id];~i;i=edge[i].next){
            int v=edge[i].to;
            if (res[v]>res[id]+edge[i].w){
                res[v]=res[id]+edge[i].w;
                if (!itered[v]){
                    q.push((node){res[v],v});
                }
            }
        }
    }//every point is updated only once.
}

/*
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
}*/

int main(){
    int m,s;
    //freopen("../../data/P4779_1.in","r",stdin);
    //freopen("../../data/P4779.out","w",stdout);
    
    scanf("%d%d%d",&n,&m,&s);
    memset(head,-1,(m+2)*sizeof(int));
    std::fill_n(res,n+2,LARGE);
    
    int u,v,w;
    for (int i=1;i<=m;i++){
        scanf("%d%d%d",&u,&v,&w);
        add(u,v,w);
    }
    res[s]=0;
    dijkstra();
    for (int i=1;i<=n;i++) printf("%d ",res[i]);
}