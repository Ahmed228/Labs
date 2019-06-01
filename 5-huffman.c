#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAX_SYM 256 
#define MAX_BIT 8   

struct tnode {  //это дерево
    int nam;
    int ch;
    struct tnode *left;
    struct tnode *right;
};

struct  tnode* create_tree(int nam, int ch) { //типа дерево создаем
    struct tnode *p = malloc(sizeof(struct tnode));
    if (p != NULL) {
        p->nam = nam;
        p->ch = ch;
        p->left = NULL;
        p->right = NULL;
    }
    return p;
}

struct stack {
    struct tnode** elem;
    int top;
};

void init_stack(struct stack *stk) { //инициализация стека
    stk->elem = malloc(MAX_SYM * sizeof(struct tnode*));
    stk->top = 0;
}

struct tnode* stk_top(struct stack *stk) { //функция возвращает верхний элемент из стека но не удаляет его
    if((stk->top) > 0) return (stk->elem[stk->top - 1]);
    return 0;
}

void push_stack(struct stack *stk, struct tnode *node) { //функция для складывания элементов в стек
    stk->elem[stk->top] = node;
    stk->top++;
}

struct tnode* pop_stack(struct stack *stk) { //извлечение элемента из стека
    stk->top--;
    struct tnode *p = stk->elem[stk->top];
    return(p);
}

int isempty_stack(struct stack *stk) { //проверка стека на пустоту
    if((stk->top) == 0) return(1);
    else return(0);
}

void push_stack_min(struct stack *stk, struct tnode *node, struct stack*help_min) { //сортировка стека
    while(1) {
        if (isempty_stack(stk) == 1 || stk_top(stk)->nam >= node->nam ) {
            stk->elem[stk->top] = node;
            stk->top++;
            break;
        } else push_stack(help_min, pop_stack(stk));
    }
    while(isempty_stack(help_min) != 1) push_stack(stk, pop_stack(help_min));
}

int c_rec_characters(int *c_characters, FILE *in){ //функция для подсчета символов
    int N = 0; int letter = 0;
    while((letter = fgetc(in)) != EOF) {
        N++;
        c_characters[letter]++;
    }
    return N;
}

void c_sort_characters(struct stack *stk, int c_nam_of_characters[]){ //записывает каждый елемент как новое дерево в стек деревьев
    int max = 0, max_i = 0;
    struct tnode *p;
    for(int i = 0; i < MAX_SYM; i++){
        if(c_nam_of_characters[i] > max){
            max = c_nam_of_characters[i];
            max_i = i;
        }
    }
    if(max != 0) {
        p = create_tree(c_nam_of_characters[max_i], max_i);
        push_stack(stk, p);
        c_nam_of_characters[max_i] = 0;
    }
}

struct tnode* fill_tree(struct stack *stk){ //создание дерева, где листья - элементы, которые нужно закодировать
    struct stack *help_min = (struct stack*)malloc(sizeof(struct stack));
    init_stack(help_min);
    while(1) {
        struct tnode *p = malloc(sizeof(struct tnode));
        struct tnode *first = pop_stack(stk);
        struct tnode *second;
        if (stk->top != 0) second = pop_stack(stk);
        else return first;
        p->nam = first->nam + second->nam;
        p->ch = -1;
        p->left = first;
        p->right = second;
        push_stack_min(stk, p, help_min);
    }
}

int count_code(struct tnode *tree, int codes[MAX_SYM][MAX_SYM], int help_mass[], int *i){ //продолжение функции для кодирования листьев
    if(tree->left != NULL){
        help_mass[*i] = 0;
        (*i)++;
        count_code(tree->left, codes, help_mass, i);
        help_mass[*i] = 1;
        (*i)++;
        count_code(tree->right, codes, help_mass, i);
        (*i)--;
        help_mass[*i] = -1;
    }else{
        for(int f = 0; f < MAX_SYM; f++){
            if(help_mass[f] != -1) codes[tree->ch][f] = help_mass[f];
            else break;
        }
        (*i)--;
        help_mass[*i] = -1;
        return 0;
    }
    return 0;
}

void  assign_codes(struct tnode *tree, int codes[MAX_SYM][MAX_SYM]){ //функция для кодировки листьев
    int help_mass[MAX_SYM];
    for(int f = 0; f < MAX_SYM; f++) help_mass[f] = -1;
    int i = 0;
    count_code(tree, codes, help_mass, &i);
}

struct queue_bit { //для побитового ввода и вывода
    int byte[MAX_BIT];
    int rear, frnt;
};

void init_queue(struct queue_bit *q) { //инициализация очереди битов
    q->frnt = 0;
    q->rear = -1;
}

int isempty_queue(struct queue_bit *q) { //проверка очереди на пустоту
    if(q->rear < q->frnt) return(1);
    else return(0);
}

int pop_queue(struct queue_bit *q) { //вытаскивание элемента из очереди
    int x, h;
    if(isempty_queue(q)==1) return(-1);
    x = q->byte[q->frnt];
    for(h = q->frnt; h < q->rear; h++) q->byte[h] = q->byte[h+1];
    q->rear--;
    return(x);
}

void print_byte(FILE *out, struct queue_bit *q){ //вывести байт в файл
    char x = 0;
    for(int i = 0; i < MAX_BIT; i++)
        if (pop_queue(q) == 1) x |= (1 << (7-i));
        else x &= ~(1 << (7-i));
    fprintf(out, "%c", x);
}

void c_push_queue(FILE *out, struct queue_bit *q, int x) { //положить элемент в очередь при кодировании
    if(q->rear < MAX_BIT-1) {
        q->rear++;
        q->byte[q->rear]=x;
    }
    else {
        print_byte(out, q);
        q->rear++;
        q->byte[q->rear] = x;
    }
}

void d_push_queue(struct queue_bit *q, int x) { //положить элемент в очередь при декодировании
    q->rear++;
    q->byte[q->rear]=x;
}

void write_bit(FILE *out, int bit, struct queue_bit *q){ //записать бит в очередь при кодировании
    c_push_queue(out, q, bit);
}

void write_letter(FILE *out, int letter, struct queue_bit *q){
    for(int i = 0; i < MAX_BIT; i++)
        if(letter & (1 << (7 - i))) write_bit(out, 1, q);
        else write_bit(out, 0, q);
}

void read_letter(FILE *in, struct queue_bit *q){ //считывание буквы при декодировании
    int letter = fgetc(in);
    for(int i = 0; i < MAX_BIT; i++)
        if(letter & (1 << (7 - i))) d_push_queue(q, 1);
        else d_push_queue(q, 0);
}

void tree_cod(FILE *out, struct tnode *tree, struct queue_bit *q){ //кодировка дерева
    if(tree->left){
        write_bit(out, 0, q);
        tree_cod(out, tree->left, q);
        tree_cod(out, tree->right, q);
    }
    else{
        write_bit(out, 1, q);
        write_letter(out, tree->ch, q);
    }

}

void txt_cod(FILE *in, FILE *out, int codes[MAX_SYM][MAX_SYM], struct queue_bit *q){ //кодировка текста
    int letter = 0;
    fgetc(in);
    fgetc(in);
    fgetc(in);
    while((letter = fgetc(in)) != EOF) {
        int i = 0;
        while(codes[letter][i] != -1) {
            write_bit(out, codes[letter][i], q);
            i++;
        }
    }
    print_byte(out, q);
}

int read_bit(FILE *in, struct queue_bit *q){ //считывает один бит из файла
    int bit;
    if((bit = pop_queue(q)) != -1) return bit;
    else{
        read_letter(in, q);
        return read_bit(in, q);
    }
}

int decod_letter(FILE *in, struct queue_bit *q){ //декодирование букввы
    int x = 0;
    for(int i = 0; i < MAX_BIT; i++)
        if (read_bit(in, q) == 1) x |= (1 << (7-i));
        else x &= ~(1 << (7-i));
    return x;
}

struct tnode* decod_tree(FILE *in, struct queue_bit *q){ //функция для декодирования дерева
    struct tnode* tree;
    int bit = read_bit(in, q);
    if(bit == 0){
        tree = create_tree(0, -1);
        tree->left = decod_tree(in, q);
        tree->right = decod_tree(in, q);
    }
    else{
        int i = decod_letter(in, q);
        tree = create_tree(0, i);
    }
    return tree;
}

int txt_decod(FILE *in, FILE *out, int N, struct tnode* tree, struct queue_bit *q){ //функция для декодирования текста
    if(tree->left == NULL) {
        fprintf(out, "%c", tree->ch);
        return 0;
    }
    int bit = read_bit(in, q);
     if(bit == 0) txt_decod(in, out, N, tree->left, q);
     else txt_decod(in, out, N, tree->right, q);
    return 0;
}

int main() {
    FILE *in = fopen("in.txt", "rb");
    FILE *out = fopen("out.txt", "wb");
    if(fgetc(in) == 'c') {
        fgetc(in); //взятие символа
        fgetc(in);
        struct stack *stk = (struct stack *) malloc(sizeof(struct stack));
        init_stack(stk);
        struct queue_bit *q = (struct queue_bit *) malloc(sizeof(struct queue_bit));
        init_queue(q);
        int c_nam_of_characters[MAX_SYM] = {0}; //массив для хранения колличества символов
        int N = c_rec_characters(c_nam_of_characters, in); //считаем символы (скока каждого символа в тексте)
        if(N == 0) return 0; //N - колличество символов в файле (для отсекания конца)
        fwrite(&N, sizeof(int), 1, out); //записываем колличество символов в файл
        fclose(in);
        FILE *in2 = fopen("in.txt", "rb");
        for (int i = 0; i < MAX_SYM; i++) c_sort_characters(stk, c_nam_of_characters);
        struct tnode *tree = fill_tree(stk); //создание большого дерева
        int codes[MAX_SYM][MAX_SYM]; //создание таблицы кодов
        for (int i = 0; i < MAX_SYM; i++)
            for (int j = 0; j < MAX_SYM; j++) codes[i][j] = -1; //заполняем всю таблицу -1
        assign_codes(tree, codes); //кодируем листья
        tree_cod(out, tree, q); //кодируем дерево
        txt_cod(in2, out, codes, q); //кодируем текст
    }
    else{
        fgetc(in);
        fgetc(in);
        int N = 0;
        fread(&N, sizeof(int), 1, in); //считываем число элементов
        struct queue_bit *q = (struct queue_bit *) malloc(sizeof(struct queue_bit));//выделяем память под структуру побитового считывания и побитовой записи
        init_queue(q); //инициализация предыдущей структуры
        struct tnode *tree = decod_tree(in, q); //обратно из строки делаем дерево
        for(int i = 0; i < N; i++) txt_decod(in, out, N, tree, q); //декодируем текст
    }
    fclose(in);
    fclose(out);
    return 0;
} //купил крокодила, иду домой
//пока
