#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct graph{
    int start;
    int end;
    int weight;
}graph;

int check_input(int N, int M, FILE *out){
    if ((N < 0) || (N > 5000)){
        fprintf (out,"bad number of vertices");
        return 0;
    }
    if ((M < 0) || (M > (N*(N-1)/2))){
        fprintf (out,"bad number of edges");
        return 0;
    }
    if (((N == 0)||(N > 1))&&(M == 0)){
        fprintf (out,"no spanning tree");
        return 0;
    }
    return 1;
}

int check_rebro(int start, int end, int weight, int N){
    if ((start < 1)||(start > N)||(end < 1)||(end > N)) return 1;
    if ((weight < 0)||(weight > INT_MAX)) return 2;
    return 0;
}

graph *find_min(graph *minmin, graph *Grrra, int *status, int N, int M){
    int min = INT_MAX;
    minmin = NULL;
    for (int i = 0; i < M; i++){
        if ((((status[Grrra[i].start] == 1) && (status[Grrra[i].end] == 0)) ||
             ((status[Grrra[i].end] == 1) && (status[Grrra[i].start] == 0))) && (Grrra[i].weight <= min) && (Grrra[i].weight != -1)){
            minmin = &Grrra[i];
            break;
        }
    }
    return minmin;
}

int compare(const void *a, const void *b){
    const graph *k = (const graph *)a;
    const graph *m = (const graph *)b;
    return ((k->weight) - (m->weight));
}

int prim(graph *Grrra, int N, int M, FILE *out){
    int nodes = 1; graph *minmin = NULL; graph *GG = NULL;
    int *status = (int*)malloc((N + 1) * sizeof(int));
    for (int i = 0; i < N+1; i++) status[i] = 0;
    status[Grrra[0].start] = 1;
    while ((GG = find_min(minmin, Grrra, &status[0], N, M))){
        fprintf(out,"%d %d\n",GG->start,GG->end);
        GG->weight = - 1;
        status[GG->start] = 1;
        status[GG->end] = 1;
        nodes++;
    }
    free(status);
    return nodes;
}

int main(){
    int N = 0; int M = 0; int tmp = 0;
    graph *Grrra = NULL;
    FILE *in = fopen("in.txt","r");
    FILE *out = fopen("out.txt","w");
    fscanf (in,"%d\n%d", &N, &M);
    if (!check_input(N, M, out)) goto END;
    if ((N == 1)&&(M == 0)) goto END;
    Grrra =(graph *)malloc(M * sizeof(graph));
    for (int i = 0; i < M; i++){
        if (fscanf(in,"%d %d %d",&Grrra[i].start, &Grrra[i].end, &Grrra[i].weight) == EOF){
            fprintf(out,"bad number of lines");
            goto END;
        }
        tmp = check_rebro(Grrra[i].start,Grrra[i].end,Grrra[i].weight,N);
        if (tmp){
            if (tmp == 1) fprintf(out,"bad vertex");
            else if (tmp == 2) fprintf(out,"bad length");
            goto END;
        }
    }
    qsort(Grrra, M, sizeof(graph), compare);
    if (prim(Grrra, N, M, out) != N){
        fclose(out);
        out = fopen("out.txt","w");
        fprintf (out,"no spanning tree");
    }
    END:
    fclose(in);
    fclose(out);
    if (Grrra) free(Grrra);
    return 0;
}
