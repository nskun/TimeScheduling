#include<stdio.h>
void swap(int *a, int *b){
	int temp;
	/*
	if(*a == -1 || *b == -1){
		printf("a : %d b : %d�}�C�i�X�̒l�ƌ������Ă܂��B", *a, *b);
		exit(-1);
	}else if(*a >= 45 || *b >= 45){
		printf("45�ȏ�̒l�ƌ������Ă܂��B");
		exit(-1);
	}
	*/
	temp = *a;
	*a = *b;
	*b = temp;
}
