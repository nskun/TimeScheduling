#include<stdio.h>
#define NUMBER 1000
/* 配列を-1に初期化する */
void minus2(int **a){
	int i, j;
	for(i = 0; i < NUMBER; i++){
		for(j = 0; j < NUMBER; j++){
			a[i][j] = -1;
		}
	}
}
/* 配列を-1に初期化する */
void minus1(int *a){
	int i;
	for(i = 0; i < NUMBER; i++){
		a[i] = -1;
	}
}
/* 配列を0に初期化する */
void zero2(int **a){
	int i, j;
	for(i = 0; i < NUMBER; i++){
		for(j = 0; j < NUMBER; j++){
			a[i][j] = 0;
		}
	}
}

/* 配列を0に初期化する */
void zero1(int *a){
	int i;
	for(i = 0; i < NUMBER; i++){
		a[i] = 0;
	}
}
