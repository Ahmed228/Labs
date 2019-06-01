#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct graph {
    int start;
    int end;
    int weight;
};

int check1(int vershina, int rebro,FILE *out) {
    if ((vershina < 0) || (vershina > 5000)) {
        fprintf(out, "bad number of vertices");
        return 0;
    }
    if ((rebro < 0) || (rebro > (vershina * (vershina + 1) / 2))) {
        fprintf(out, "bad number of edges");
        return 0;
    }
    if (vershina == 0) {
        fprintf(out, "no spanning tree");
        return 0;
    }
    return 1;
}

int check_Rebro(struct graph *gr, int ver, int i,FILE *out) {
    if (gr[i].start < 1 || gr[i].start > ver) {
        fprintf(out, "bad vertex");
        return 0;
    }
    if (gr[i].end < 1 || gr[i].end > ver) {
        fprintf(out, "bad vertex");
        return 0;
    }
    if (gr[i].weight < 0 || gr[i].weight > INT_MAX) {
        fprintf(out, "bad length");
        return 0;
    }
    return 1;
}

void Sort(struct graph *gr, int l, int r) {
    int i = l, j = r;
    int a = gr[(l + r) / 2].weight;
    do {
        while (gr[i].weight < a) i++;
        while (gr[j].weight > a) j--;
        if (i <= j) {
            struct graph c = gr[i];
            gr[i] = gr[j];
            gr[j] = c;
            j--;
            i++;
        }
    } while (i < j);
    if (l < j) Sort(gr, l, j);
    if (i < r) Sort(gr, i, r);
}

void makeSet(int *status, int x){
    status[x] = x;
}

int find(int *status, int x){
    if (status[x] == x) return x;
    return status[x] = find(status, status[x]);
}

void unit(int x, int y, int *rank, int *status){
    x = find(status, x);
    y = find(status, y);
    if (rank[x] < rank[y]) status[x] = y;
    else {
        status[y] = x;
        if (rank[x] == rank[y]) rank[x]++;
    }
}

void crascal(int *status, struct graph *gr, int vershina, int rebro, int *rank, FILE *out) {
    for (int i = 1; i <= vershina; i++) makeSet(status, i);
    for (int i = 0; i < rebro; i++){
        if (find(status, gr[i].start) != find(status, gr[i].end)) {
            unit(gr[i].start, gr[i].end, rank, status);
            fprintf(out, "%d %d\n", gr[i].start, gr[i].end);
        }
    }
}

int main() {
    FILE *in = fopen("in.txt", "r");
    FILE *out = fopen("out.txt", "w");
    struct graph *Grrra = NULL;
    int vershina = 0;
    int rebro = 0;
    int err = 0;
    int ha = 0;
    int letter = 0;
    int *status = NULL;
    int *rank = NULL;
    int *full = NULL;
    fscanf(in, "%d", &vershina);
    fscanf(in, "%d", &rebro);

    if (check1(vershina, rebro, out)) {
        Grrra = malloc(rebro * sizeof(struct graph));
        status = malloc((vershina + 1) * sizeof(int));
        rank = malloc((vershina + 1) * sizeof(int));
        full = malloc((vershina + 1) * sizeof(int));
        if ((vershina == 1) && (rebro == 0)) exit(0);
        while ((fscanf(in, "%d", &letter)) != EOF) {
            switch (err) {
                case 0:
                    Grrra[ha].start = letter;
                    err++;
                    break;
                case 1:
                    Grrra[ha].end = letter;
                    err++;
                    break;
                case 2:
                    Grrra[ha].weight = letter;
                    err = 0;
                    break;
            }
            if (err == 0) {
                if (check_Rebro(Grrra, vershina, ha, out)) {
                    full[Grrra[ha].end] = 1;
                    full[Grrra[ha].start] = 1;
                    ha++;
                }
            }
        }
        if (ha != rebro) {
            fprintf(out, "bad number of lines");
            return 0;
        }
        for (int i = 1; i <= vershina; i++) {
            if (full[i] != 1) {
                fprintf(out, "no spanning tree");
                return 0;
            }
        }
        Sort(Grrra, 0, rebro - 1);
        crascal(status, Grrra, vershina, rebro, rank, out);
        free(status);
        free(rank);
        free(full);
        free(Grrra);
        fclose(in);
        fclose(out);
        return 0;
    }
}
