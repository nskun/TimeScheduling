#include<stdio.h>
#define NUMBER 1000
/* �z���-1�ɏ��������� */
void minus2(int **a){
	int i, j;
	for(i = 0; i < NUMBER; i++){
		for(j = 0; j < NUMBER; j++){
			a[i][j] = -1;
		}
	}
}
/* �z���-1�ɏ��������� */
void minus1(int *a){
	int i;
	for(i = 0; i < NUMBER; i++){
		a[i] = -1;
	}
}
/* �z���0�ɏ��������� */
void zero2(int **a){
	int i, j;
	for(i = 0; i < NUMBER; i++){
		for(j = 0; j < NUMBER; j++){
			a[i][j] = 0;
		}
	}
}

/* �z���0�ɏ��������� */
void zero1(int *a){
	int i;
	for(i = 0; i < NUMBER; i++){
		a[i] = 0;
	}
}
