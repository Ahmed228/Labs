#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h> 
#include <string.h> 
#include <conio.h>
#include <math.h>

//Позиция точки
int numberDot(char *number, int length) {
	int Dot; int summDot = 0;
	for (int i = 0; i < length; i++) {
		if (number[i] == '.') {
			Dot = i; summDot = summDot + 1;
		}
	}
	if (summDot == 0) Dot = length;
	else if (summDot > 1 || Dot == 0 || Dot == length - 1) Dot = 0;
	return Dot;
}

//Перевод целой части
long long IntegerPart(char *number, int cc1, int cc2, int Dot, char *number2) {
	long long int index = 1; long long int result = 0;
	for (int i = Dot - 1; i >= 0; i--) {
		if ((number[i] >= 'A') & (number[i] <= 'F')) result = ((long long)(number[i] - 55))*index + result;
		else if ((number[i] >= 'a') & (number[i] <= 'f')) result = ((long long)(number[i] - 87))*index + result;
		     else result = (number[i] - '0')*index + result;
		index = index * cc1;
	}                                 // перевод в 10 сс завершён
	int i = 0; int newlen = 0;
	if (result == 0) printf("%c", '0');
	else {
		while (result != 0) {
			if (result % cc2 > 9) number2[i] = (result % cc2) + 55;
			else number2[i] = result % cc2;
			result = result / cc2; newlen++; i++;
		}
	}                                 // перевод из 10 сс завершён
	for (i = newlen - 1; i >= 0; i--){
		if (number2[i] > 9) printf("%c", (int)number2[i]);
		else  printf("%d", (int)number2[i]);
	}                                 // вывод
	return 0;
}

//Перевод дробной части
long long FractionPart(char *number, int cc1, int cc2, int Dot, char *number2, int length){
	long long int index = cc1; long double result = 0; long double xxx = 0; int schetchik = 0;
	for (int i = Dot + 1; i < length; i++) {
		if ((number[i] >= 'A') & (number[i] <= 'F')) result = (long double)(number[i] - 55) / index + result;
	    else if ((number[i] >= 'a') & (number[i] <= 'f')) result = (long double)(number[i] - 87) / index + result;
		     else result = (long double)(number[i] - '0') / index + result;
		index = index * cc1;
	}                                   // перевод в 10 сс завершён
	if (result != 0){
		printf(".");
		while ((result != 0) & (schetchik<12)){
			result = modf(result*cc2, &xxx);
			if (xxx > 9) printf("%c", (int)xxx + 55);
			else printf("%d", (int)xxx);
			schetchik++;
		}                                   // перевод из 10 сс завершён
	}
	return 0;
}

//Проверка
int check(char *number, int cc1, int cc2, int Dot, int length) {
	if (cc1 < 2 || cc1 > 16 || cc2 < 2 || cc2 > 16 || Dot == 0 || length > 13) {
		printf("bad input");
		return 0;
	}
	for (int i = 0; i < length; i++) {
		if ((number[i]>'.' && number[i]<48) || (number[i]>67 && number[i]<'A') || (number[i]>'F' && number[i]<'a') || (number[i]>'f') || ((number[i] - 48) >= cc1 && number[i] <= 57 && number[i] >= 48) || ((number[i] - 55) >= cc1 && number[i] <= 'F' && number[i] >= 'A') || ((number[i] - 87) >= cc1 && number[i] >= 'a')){
			printf("bad input");
			return 0;
		}
	}
	return 1;
}

//Главная часть 
int main() {
	int cc1 = 0; int cc2 = 0; char number[256] = ""; char number2[256] = "";
	scanf("%d%d", &cc1, &cc2);
	scanf("%s", number);
	int length = strlen(number);
	if (check(number, cc1, cc2, numberDot(number, length), length)) {
		IntegerPart(number, cc1, cc2, numberDot(number, length), number2);
		FractionPart(number, cc1, cc2, numberDot(number, length), number2, length);
	} 
	return 0;
}
