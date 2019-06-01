#include <stdio.h>
#include <stdlib.h>

struct stack{
    int value[1000];
    int top;
};

void push(struct stack *stk, int S){
    stk->value[stk->top] = S;
    stk->top++;
}

void TopSort(int number, int** arr, int* check, int N, struct stack *out){
    if (check[number] == 1){
        printf("impossible to sort");
        exit(0);
    } else if(check[number] == 2) return;
    check[number] = 1;
    for (int i = 1; i < arr[number][0] + 1; i++){
        if (arr[number][i] != 0) TopSort(arr[number][i]-1, arr, check, N, out);
    }
    push (out, number+1);
    check[number] = 2;
}

void Print_stack(struct stack *stk){
    int i;
    i = stk->top;
    if (stk->top == 0) return;
    while (i>0){
        i--;
        printf("%d ", stk->value[i]);
    }
}

int main(){
    int N = 0; int M = 0; int a = 0; int b = 0; int S = 0;
    FILE *in = fopen("in.txt","r");
    struct stack *out;
    out = (struct stack*)malloc(sizeof(struct stack));
    out->top = 0;
    fscanf(in,"%d",&N);
    if (fscanf(in,"%d",&M) == EOF){  //проверка на наличие данных об узлах
        printf("bad number of lines");
        return 0;
    }
    if ((N<0) || (N>1000)){             //проверка N
        printf ("bad number of vertices");
        return 0;
    }
    if ((M<0) || (M>N*(N+1)/2)){        //проверка M
        printf("bad number of edges");
        return 0;
    }
    S = (N<M) ? N : M;    //
    int **arr = (int **)malloc(N*sizeof(int*));   //
    for(int i = 0; i < N; i++){
        arr[i] = (int *)malloc((S+1) * sizeof(int));
    }
    int *check = (int *)malloc(N * sizeof(int));  //
    for (int i = 0; i < N; i++){  //зануление массива
        check[i] = 0;
        arr[i][0] = 0;
    }
    for (int i = 0; i < M; i++){
        if (fscanf(in, "%d%d", &a, &b) == EOF) {
            printf("bad number of lines");
            free(arr);
            return 0;
        }
        if ((a <=0) || (a > N) || (b<=0) || (b > N)){
            printf ("bad vertex");
            free(arr);
            return 0;
        }
        arr[a-1][0]++;
        arr[a-1][arr[a-1][0]] = b;
    }
    for(int i = 0; i < N; i++) if (check[i] == 0) TopSort(i, arr, check, N, out); //сортировка
    Print_stack(out); //вывод стека
    free(out);
    for(int i = 0; i < N; i++) free(arr[i]);
    free(arr);
    free(check);
    fclose(in);
    return 0;
}
