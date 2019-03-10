#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<conio.h>
#define INFINITY 9999
#define MAX 100
typedef struct pc{
    char* nume;
    char* ip;
    int stare;
}calculator;
typedef struct router{
    char* nume;
    char* ip;
    int pc_conectate;
    calculator* pc;
}routere;
typedef struct graph{
    int V;
    int E;
    int **a;
}Graph;
typedef struct conex{
    char* prim;
    char* secund;
    int distanta;
}conex;
typedef struct operat{
    char* nume;
    char* prim;
    char* secund;
    int distanta;   
}operat;
Graph * create (int nr_routere, routere* router, conex* conexiuni, int nr_conexiuni){
    int i, j, k, o;
    Graph *g = (Graph *) malloc (sizeof(Graph));
    g->V=nr_routere;
    if (g==NULL) {
        printf ( " no memory for this graph");
        return NULL;
    }
    g->a = (int **)malloc(g->V*sizeof(int*));
    for (i=0;i<g->V;i++)
        g->a[i] = (int *) calloc(g->V, sizeof(int));
    if (g->a==NULL) {
        printf ( " no memory for this matrix");
        return NULL;
    }
    g->E = 0;
    for(i=0;i<nr_conexiuni;i++){
        for(j=0;j<nr_routere;j++){
            if(strcmp(conexiuni[i].prim,router[j].nume)==0)
                k=j;
            if(strcmp(conexiuni[i].secund,router[j].nume)==0)
                o=j;
        }
        g->a[k][o]=conexiuni[i].distanta;
        g->a[o][k]=conexiuni[i].distanta;
    }
    return g;
}
void printGraph(Graph *g){
    int i, j;
    for (i=0;i<g->V;i++) {
        for (j=0;j<g->V;j++){
            printf( " %d ",g->a[i][j]);
        }
        printf("\n");
    }
}
void ping(int nr_routere, routere* router, Graph* g, char* adresa1, char* adresa2, FILE* f2){
    int i,j,pozitie1,pozitie2;
    //printf("%s\n",adresa1);
    for(i=0;i<nr_routere;i++){
        for(j=0;j<router[i].pc_conectate;j++){
            if(strcmp(adresa1,router[i].pc[j].ip)==0){
                pozitie1=i;
                if(router[i].pc[j].stare==0){
                    fprintf(f2, "NO\n");
                    return;
                }
            }
            if(strcmp(adresa2,router[i].pc[j].ip)==0){
                pozitie2=i;
                if(router[i].pc[j].stare==0){
                    fprintf(f2, "NO\n");
                    return;
                }
            }
        }
    }
    if(g->a[pozitie1][pozitie2]!=0)
        fprintf(f2, "OK\n");
    else
        fprintf(f2, "NO\n");
    printGraph(g);
    printf("%d\n",g->a[pozitie1][pozitie2]);
    printf("%d\n",pozitie1);
    printf("%d\n",pozitie2);
}
void si(int nr_routere, routere* router,char* adresa1, FILE* f2){
    int i, j;
    for(i=0;i<nr_routere;i++){
        if(strcmp(adresa1,router[i].ip)==0){
            for(j=0;j<router[i].pc_conectate;j++){
                if(router[i].pc[j].stare==1)
                    fprintf(f2, "%s ",router[i].pc[j].ip);
                else if(router[i].pc[j].stare==0)
                    return;
            }
        }
    }
}
void sipn(int nr_routere, routere* router,char* adresa1,Graph* g, FILE* f2){
    int i, j, poz;
    for(i=0;i<nr_routere;i++){
        if(strcmp(adresa1,router[i].ip)==0){
            poz=i;
        }
    }
    for (j=0;j<g->V;j++){
            if(g->a[poz][j]!=0)
                fprintf(f2, "%s ",router[j].ip);
        }

}
Graph* add(int nr_routere, routere* router,char* adresa1,char* adresa2,int distanta, Graph* g, FILE* f2){
    int i, poz1, poz2;
    for(i=0;i<nr_routere;i++){
        if(strcmp(adresa1,router[i].ip)==0){
            poz1=i;
        }
        if(strcmp(adresa2,router[i].ip)==0){
            poz2=i;
        }
    }
    g->a[poz1][poz2]=distanta;
    g->a[poz2][poz1]=distanta;
    return g;

}
void up(int nr_routere, routere* router, char* adresa1, FILE* f2){
    int i,j;
    for(i=0;i<nr_routere;i++){
        for(j=0;j<router[i].pc_conectate;j++){
            if(strcmp(adresa1,router[i].pc[j].ip)==0){
                router[i].pc[j].stare=1;
            }
        }
    }
}
void broke(int nr_routere, routere* router, char* adresa1, FILE* f2){
    int i,j;
    for(i=0;i<nr_routere;i++){
        for(j=0;j<router[i].pc_conectate;j++){
            if(strcmp(adresa1,router[i].pc[j].ip)==0){
                router[i].pc[j].stare=0;
            }
        }
    }
}
Graph* lc(int nr_routere, routere* router,char* adresa1,char* adresa2, Graph* g, FILE* f2){
    int i, poz1, poz2;
    for(i=0;i<nr_routere;i++){
        if(strcmp(adresa1,router[i].ip)==0){
            poz1=i;
        }
        if(strcmp(adresa2,router[i].ip)==0){
            poz2=i;
        }
    }
    g->a[poz1][poz2]=0;
    g->a[poz2][poz1]=0;
    return g;
}
int minDistance(int dist[], int sps[], int V){
	int v, min = 100, min_index;
	for ( v = 0; v < V; v++)
		if (sps[v] == 0 && dist[v] <= min) {
			min = dist[v];
			min_index= v;
		}
	return min_index;
}
void printSolution(int dist[], int n){
	printf("Varf Distanta fata de sura\n");
	for(int i=0;i<n;i++)
		printf("%d   %d\n",i,dist[i]);
}
/*void dijkstra(Graph *g, int s){
	int i, j, u, k=0;
	int* b;
	int dist[g->V];
	int sps[g->V];
	b=(int*)malloc(sizeof(int)*30);
	for(i=0;i<g->V;i++){
		dist[i]=100;
		sps[i]=0;
	}
	dist[s]=s;
	sps[s]=1;
	for(j=0;j<g->V-1;j++){
		u=minDistance(dist,sps,g->V);
		printf("%d\n",u);
		b[k]=u;
		k++;
		sps[u]=1;
		for(i=0;i<g->V;i++){
			if(sps[i]==0 && g->a[u][i]!=0 && dist[u]!= 100 && dist[u]+g->a[u][i] < dist[i])
				dist[i]=dist[u]+g->a[u][i];
		}	
	}
	printSolution(dist,g->V);
}*/
/*void dijkstra_trace(int G[MAX][MAX],int n, char* ip1, char* ip2, int nr_routere, routere* router, FILE* f2)
{
 
    int cost[MAX][MAX],distance[MAX],pred[MAX], k=0, l=0, pozitie1, pozitie2;
    int visited[MAX],count,mindistance,nextnode,i,j;
    int b[50], a[100];
    //b=(int*)malloc(20*sizeof(int));
    for(i=0;i<nr_routere;i++){
        for(j=0;j<router[i].pc_conectate;j++){
            if(strcmp(ip1,router[i].pc[j].ip)==0){
                pozitie1=i;
                if(router[i].pc[j].stare==0){
                    //fprintf(f2, "NO\n");
                    //printf("NO\n");
                    return;
                }
            }
            if(strcmp(ip2,router[i].pc[j].ip)==0){
                pozitie2=i;
                if(router[i].pc[j].stare==0){
                    //fprintf(f2, "NO\n");
                    return;
                }
            }
        }
    }
    printf("%d\n",pozitie1);
    printf("%d\n",pozitie2);
    //pred[] stores the predecessor of each node
    //count gives the number of nodes seen so far
    //create the cost matrix
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            if(G[i][j]==0)
                cost[i][j]=INFINITY;
            else
                cost[i][j]=G[i][j];
    
    //initialize pred[],distance[] and visited[]
    for(i=0;i<n;i++)
    {
        distance[i]=cost[pozitie1][i];
        pred[i]=pozitie1;
        visited[i]=0;
    }
    
    distance[pozitie1]=0;
    visited[pozitie1]=1;
    count=1;
    
    while(count<n-1)
    {
        mindistance=INFINITY;
        
        //nextnode gives the node at minimum distance
        for(i=0;i<n;i++){
            if(distance[i]<mindistance&&!visited[i])
            {
                mindistance=distance[i];
                nextnode=i;
            }
            
            //check if a better path exists through nextnode            
            visited[nextnode]=1;
            for(i=0;i<n;i++)
                if(!visited[i])
                    if(mindistance+cost[nextnode][i]<distance[i])
                    {
                        distance[i]=mindistance+cost[nextnode][i];
                        pred[i]=nextnode;
                    }
        count++;
    	}
    }
 
    //print the path and distance of each node
    for(i=0;i<n;i++)
        if(i!=startnode)
        {
            printf("\nDistance of node%d=%d",i,distance[i]);
            printf("\nPath=%d",i);
            b[k]=distance[i];
            k++;
            
            j=i;
            do
            {
                j=pred[j];
                printf("<-%d",j);
            }while(j!=startnode);
    }*/
    /*for(i=0;i<n;i++)
        if(i==pozitie2)
        {
            printf("\nDistance of node%d=%d",i,distance[i]);
            printf("\nPath=%d",i);
            b[k]=distance[i];
            k++;
            
            j=i;
            do
            {
                j=pred[j];
                printf("<-%d",j);
                a[l]=j;
                l++;
            }while(j!=pozitie1);
    }
    for(i=0;i<l;i++){
    	fprintf(f2, "%s\n",router[i].ip);
    }
}*/
void dijkstra_ping(int G[MAX][MAX],int n,int startnode, char* ip1, char* ip2, int nr_routere, routere* router, FILE* f2)
{
 
    int cost[MAX][MAX],distance[MAX],pred[MAX], k=0,o;
    int visited[MAX],count,mindistance,nextnode,i,j;
    int b[50];
    //b=(int*)malloc(20*sizeof(int));
    for(i=0;i<nr_routere;i++){
        for(j=0;j<router[i].pc_conectate;j++){
            if(strcmp(ip1,router[i].pc[j].ip)==0){
                //pozitie1=i;
                if(router[i].pc[j].stare==0){
                    fprintf(f2, "NO\n");
                    //printf("NO\n");
                    return;
                }
            }
            if(strcmp(ip2,router[i].pc[j].ip)==0){
                //pozitie2=i;
                if(router[i].pc[j].stare==0){
                    fprintf(f2, "NO\n");
                    return;
                }
            }
        }
    }
    
    //pred[] stores the predecessor of each node
    //count gives the number of nodes seen so far
    //create the cost matrix
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            if(G[i][j]==0)
                cost[i][j]=INFINITY;
            else
                cost[i][j]=G[i][j];
    
    //initialize pred[],distance[] and visited[]
    for(i=0;i<n;i++)
    {
        distance[i]=cost[startnode][i];
        pred[i]=startnode;
        visited[i]=0;
    }
    
    distance[startnode]=0;
    visited[startnode]=1;
    count=1;
    
    while(count<n-1)
    {
        mindistance=INFINITY;
        
        //nextnode gives the node at minimum distance
        for(i=0;i<n;i++){
            if(distance[i]<mindistance&&!visited[i])
            {
                mindistance=distance[i];
                nextnode=i;
            }
            
            //check if a better path exists through nextnode            
            visited[nextnode]=1;
            for(i=0;i<n;i++)
                if(!visited[i])
                    if(mindistance+cost[nextnode][i]<distance[i])
                    {
                        distance[i]=mindistance+cost[nextnode][i];
                        pred[i]=nextnode;
                    }
        count++;
    	}
    }
 
    //print the path and distance of each node
    for(i=0;i<n;i++)
        if(i!=startnode)
        {
            //printf("\nDistance of node%d=%d",i,distance[i]);
            //printf("\nPath=%d",i);
            b[k]=distance[i];
            k++;
            
            j=i;
            do
            {
                j=pred[j];
                printf("<-%d",j);

            }while(j!=startnode);
    }
    for(i=0;i<nr_routere;i++){
    	if(strcmp(router[i].ip,ip2)==0)
    		o=i;
    }
    for(i=0;i<k;i++){
    	if(o==i && b[i]!=MAX){
    		fprintf(f2, "OK\n");
    		return;
    	}
    	else
    		fprintf(f2, "NO\n");
    }
}
void dijkstra_trace2(int G[MAX][MAX],int n,int startnode, char* ip1, char* ip2, int nr_routere, routere* router, FILE* f2)
{
 
    int visited[MAX],count,mindistance,nextnode,i,j;
    int cost[MAX][MAX],distance[MAX],pred[MAX], l=0, pozitie1, pozitie2, a[50];
    //b=(int*)malloc(20*sizeof(int));
    for(i=0;i<nr_routere;i++){
        for(j=0;j<router[i].pc_conectate;j++){
            if(strcmp(ip1,router[i].pc[j].ip)==0){
                pozitie1=i;
                if(router[i].pc[j].stare==0){
                    //fprintf(f2, "NO\n");
                    //printf("NO\n");
                    return;
                }
            }
            if(strcmp(ip2,router[i].pc[j].ip)==0){
                pozitie2=i;
                if(router[i].pc[j].stare==0){
                    //fprintf(f2, "NO\n");
                    return;
                }
            }
        }
    }
    startnode=pozitie1;
    //pred[] stores the predecessor of each node
    //count gives the number of nodes seen so far
    //create the cost matrix
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            if(G[i][j]==0)
                cost[i][j]=INFINITY;
            else
                cost[i][j]=G[i][j];
    
    //initialize pred[],distance[] and visited[]
    for(i=0;i<n;i++)
    {
        distance[i]=cost[startnode][i];
        pred[i]=startnode;
        visited[i]=0;
    }
    
    distance[startnode]=0;
    visited[startnode]=1;
    count=1;
    
    while(count<n-1)
    {
        mindistance=INFINITY;
        
        //nextnode gives the node at minimum distance
        for(i=0;i<n;i++){
            if(distance[i]<mindistance&&!visited[i])
            {
                mindistance=distance[i];
                nextnode=i;
            }
        }
            //check if a better path exists through nextnode            
            visited[nextnode]=1;
            for(i=0;i<n;i++)
                if(!visited[i])
                    if(mindistance+cost[nextnode][i]<distance[i])
                    {
                        distance[i]=mindistance+cost[nextnode][i];
                        pred[i]=nextnode;
                    }
        count++;
    }
 
    //print the path and distance of each node
    for(i=0;i<n;i++)
        if(i==pozitie2)
        {
            //printf("\nDistance of node%d=%d",i,distance[i]);
            //printf("\nPath=%d",i);
            
            j=i;
            do
            {
                j=pred[j];
                //printf("<-%d",j);
                a[l]=j;
                l++;
            }while(j!=startnode);
    }
    for(i=l-1;i>=0;i--)
    	fprintf(f2, "%s ",router[a[i]].ip);
    fprintf(f2, "%s ",router[pozitie2].ip );
    fprintf(f2, "\n");

}
/*void dijkstra(int G[MAX][MAX],int n,int startnode)
{
 
    int cost[MAX][MAX],distance[MAX],pred[MAX];
    int visited[MAX],count,mindistance,nextnode,i,j;
    
    //pred[] stores the predecessor of each node
    //count gives the number of nodes seen so far
    //create the cost matrix
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            if(G[i][j]==0)
                cost[i][j]=INFINITY;
            else
                cost[i][j]=G[i][j];
    
    //initialize pred[],distance[] and visited[]
    for(i=0;i<n;i++)
    {
        distance[i]=cost[startnode][i];
        pred[i]=startnode;
        visited[i]=0;
    }
    
    distance[startnode]=0;
    visited[startnode]=1;
    count=1;
    
    while(count<n-1)
    {
        mindistance=INFINITY;
        
        //nextnode gives the node at minimum distance
        for(i=0;i<n;i++)
            if(distance[i]<mindistance&&!visited[i])
            {
                mindistance=distance[i];
                nextnode=i;
            }
            
            //check if a better path exists through nextnode            
            visited[nextnode]=1;
            for(i=0;i<n;i++)
                if(!visited[i])
                    if(mindistance+cost[nextnode][i]<distance[i])
                    {
                        distance[i]=mindistance+cost[nextnode][i];
                        pred[i]=nextnode;
                    }
        count++;
    }
 
    //print the path and distance of each node
    for(i=0;i<n;i++)
        if(i!=startnode)
        {
            printf("\nDistance of node%d=%d",i,distance[i]);
            printf("\nPath=%d",i);
            
            j=i;
            do
            {
                j=pred[j];
                printf("<-%d",j);
            }while(j!=startnode);
    }
}*/
int main(int argc, char *argv[]){
    int i, j, nr_routere, nr_conexiuni, nr_operatii,f,d;
    int G[MAX][MAX];
    FILE* f1;
    FILE* f2;
    routere* router;
    conex* conexiuni;
    operat* operatii; 
    conexiuni=(conex*)malloc(sizeof(conex)*30);
    router=(routere*)malloc(sizeof(routere)*40);
    operatii=(operat*)malloc(sizeof(operat)*30);
    f1=fopen(argv[1],"r");
    if(f1==NULL){
        printf("fisierul nu a putut fi deschis");
        exit(1);
    }
    f2=fopen(argv[2],"w");
    if(f2==NULL){
        printf("fisierul nu a putut fi deschis");
        exit(1);
    }
    fscanf(f1,"%d",&nr_routere);
    for(i=0;i<nr_routere;i++){
        router[i].nume=(char*)malloc(sizeof(char)*10);
        router[i].ip=(char*)malloc(sizeof(char)*15);
        fscanf(f1,"%s",router[i].nume);
        fscanf(f1,"%s",router[i].ip);
        fscanf(f1,"%d",&router[i].pc_conectate);
        router[i].pc=(calculator*)malloc(sizeof(calculator)*100);
        for(j=0;j<router[i].pc_conectate;j++){
            router[i].pc[j].nume=(char*)malloc(sizeof(char)*10);
            router[i].pc[j].ip=(char*)malloc(sizeof(char)*15);
            fscanf(f1,"%s",router[i].pc[j].nume);
            fscanf(f1,"%s",router[i].pc[j].ip);
            fscanf(f1,"%d",&router[i].pc[j].stare);
        }
    }
    fscanf(f1,"%d",&nr_conexiuni);
    for(i=0;i<nr_conexiuni;i++){
        conexiuni[i].prim=(char*)malloc(sizeof(char)*15);
        conexiuni[i].secund=(char*)malloc(sizeof(char)*15);
        fscanf(f1,"%s",conexiuni[i].prim);
        fscanf(f1,"%s",conexiuni[i].secund);
        fscanf(f1,"%d",&conexiuni[i].distanta);
    }
    Graph *g = create(nr_routere, router, conexiuni, nr_conexiuni);
    //printGraph(g);
    /*for(i=0;i<g->V;i++)
        for(j=0;j<g->V;j++)
            G[i][j]=g->a[i][j];*/
    fscanf(f1,"%d",&nr_operatii);
    for(i=0;i<nr_operatii;i++){
    	    	for(f=0;f<g->V;f++){
        	for(d=0;d<g->V;d++)
            	G[f][d]=g->a[f][d];
    	}
        operatii[i].nume=(char*)malloc(sizeof(char)*10);
        fscanf(f1,"%s",operatii[i].nume);
        if(strcmp(operatii[i].nume,"ping")==0){
            operatii[i].prim=(char*)malloc(sizeof(char)*15);
            operatii[i].secund=(char*)malloc(sizeof(char)*15);
            fscanf(f1,"%s",operatii[i].prim);
            fscanf(f1,"%s",operatii[i].secund);
            /*printf("%s\n",operatii[i].prim);
            printf("%s\n",operatii[i].secund);*/
            //ping(nr_routere,router,g,operatii[i].prim,operatii[i].secund,f2);
            dijkstra_ping(G,g->V,0, operatii[i].prim,operatii[i].secund, nr_routere, router, f2);
        }
        else if(strcmp(operatii[i].nume,"sipn")==0){
            operatii[i].prim=(char*)malloc(sizeof(char)*15);
            fscanf(f1,"%s",operatii[i].prim);
            sipn(nr_routere,router,operatii[i].prim,g,f2);
            fprintf(f2, "\n");
        }
        else if(strcmp(operatii[i].nume,"si")==0){
            operatii[i].prim=(char*)malloc(sizeof(char)*15);
            fscanf(f1,"%s",operatii[i].prim);
            si(nr_routere,router,operatii[i].prim,f2);
            fprintf(f2, "\n");
        }
        else if(strcmp(operatii[i].nume,"trace")==0){
            operatii[i].prim=(char*)malloc(sizeof(char)*15);
            operatii[i].secund=(char*)malloc(sizeof(char)*15);
            fscanf(f1,"%s",operatii[i].prim);
            fscanf(f1,"%s",operatii[i].secund);
            //l=i;
            //printGraph(g);
            dijkstra_trace2(G,g->V,0, operatii[i].prim,operatii[i].secund, nr_routere, router, f2);
            //dijkstra(G,g->V,0);
            //dijkstra(G,g->V,0);
        }
        else if(strcmp(operatii[i].nume,"up")==0){
            operatii[i].prim=(char*)malloc(sizeof(char)*15);
            fscanf(f1,"%s",operatii[i].prim);
            up(nr_routere,router,operatii[i].prim,f2);
        }
        else if(strcmp(operatii[i].nume,"lc")==0){
            operatii[i].prim=(char*)malloc(sizeof(char)*15);
            operatii[i].secund=(char*)malloc(sizeof(char)*15);
            fscanf(f1,"%s",operatii[i].prim);
            fscanf(f1,"%s",operatii[i].secund);
            lc(nr_routere,router,operatii[i].prim,operatii[i].secund,g,f2);
        }
        else if(strcmp(operatii[i].nume,"broke")==0){
            operatii[i].prim=(char*)malloc(sizeof(char)*15);
            fscanf(f1,"%s",operatii[i].prim);
            broke(nr_routere,router,operatii[i].prim,f2);
        }
        else if(strcmp(operatii[i].nume,"add")==0){
            operatii[i].prim=(char*)malloc(sizeof(char)*15);
            operatii[i].secund=(char*)malloc(sizeof(char)*15);
            fscanf(f1,"%s",operatii[i].prim);
            fscanf(f1,"%s",operatii[i].secund);
            fscanf(f1,"%d",&operatii[i].distanta);
            add(nr_routere,router,operatii[i].prim,operatii[i].secund,operatii[i].distanta,g,f2);
                        //dijkstra(G,g->V,0);
        }
    }
    //dijkstra(g,0);
    //printf("\n");
    //printGraph(g);
	//dijkstra_trace(G,g->V,0, operatii[l].prim,operatii[l].secund, nr_routere, router, f2);
    //dijkstra(G,g->V,0);
    fclose(f1);
    fclose(f2);
    return 0;
}
