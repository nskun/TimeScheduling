#include<stdio.h>
void swap(int *a, int *b){
	int temp;
	/*
	if(*a == -1 || *b == -1){
		printf("a : %d b : %dマイナスの値と交換してます。", *a, *b);
		exit(-1);
	}else if(*a >= 45 || *b >= 45){
		printf("45以上の値と交換してます。");
		exit(-1);
	}
	*/
	temp = *a;
	*a = *b;
	*b = temp;
}
