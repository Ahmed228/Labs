#define _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <stdio.h> 

//меняет два элемента массива
void zamena(int a, int b, int *mass){
	int c = mass[a];
	mass[a] = mass[b];
	mass[b] = c;
}

//первый проход при считывании массива
void one(int *mass, int i){
	while ((i>0) && ((mass[(i-1)/2])<mass[i])){
		zamena (i, ((i-1)/2), mass);
		i = (i-1)/2;
	}
}

//второй проход
void two(int size, int *mass){
	int i = 0; int j = 0;
	while ((i*2+1)<size){
		if (mass[i*2+1]>mass[j]) j = i*2+1;
		if (((i*2+2)<size) && (mass[i*2+2]>mass[j])) j = i*2+2;
		if (i == j) break;
		zamena(i,j,mass);
		i = j;
	}
}

int main (){
	int size; int *mass; int i;
	scanf ("%ld", &size);
	if (size <=0) return 0;
	mass = (int*)malloc(size*sizeof(int));
	for (i=0; i<size; i++){
		scanf ("%d", &mass[i]);
		one(mass, i);
	}
	int newsize = size;
	for (i=0; i<size; i++){
		zamena(0, newsize-1, mass);
		two(newsize-1, mass);
		newsize--;
	}
	for (i=0; i<size; i++) printf("%d ", mass[i]);
	free(mass);
	return 0;
}
