#include<stdio.h>
#include<stdlib.h>
void InputData(int **Se, int *Es, int **Rf, int *Fr, int **Ef, int *Fe, int *roomsize, int *event, int *room, int *feature, int *student, int *attendance){

	int i, j, count = 0;

	// �t�@�C���|�C���^�̐錾
	FILE *fp;

	if ((fp = fopen("competition20.tim", "r")) == NULL) {
		printf("file open error!!\n");
		exit(-1);	/* �G���[�̏ꍇ�͒ʏ�A�ُ�I������ */
	}

	/* �t�@�C��1�s�ڂ��炻�ꂼ��̐���ǂ� */
	fscanf(fp, "%d %d %d %d", event, room, feature, student);

	// �����̃T�C�Y��ǂ�
	for(i = 0; i < *room; i++){
		fscanf(fp, "%d", &roomsize[i]);
	}

	// �ǂ̎��Ƃɐ��k���o�Ȃ��邩
	for(i = 0; i < *student; i++){ /* event_student��0-1�}�g���N�X��ǂ� */
		for(j = 0; j < *event; j++){
			fscanf(fp, "%d", &Se[i][j]);
		}
	}

	// �ǂ̕����ɂǂ̓��������邩
	for(i = 0; i < *room; i++){ /* feature_room��0-1�}�g���N�X��ǂ� */
		for(j = 0; j < *feature; j++){
			fscanf(fp, "%d", &Rf[i][j]);
		}
	}
	// �ǂ̎��Ƃɂǂ̓��������邩
	for(i = 0; i < *event; i++){ /* feature_event��0-1�}�g���N�X��ǂ� */
		for(j = 0; j < *feature; j++){
			fscanf(fp, "%d", &Ef[i][j]);
		}
	}
	fclose(fp); /* �t�@�C���̃N���[�Y */

	for(i = 0; i < *event; i++){ /* �e�Ȗڂɉ��l�̐��k���o�Ȃ��Ă��邩���v�� */
		for(j = 0; j < *student; j++){
			if(Se[j][i] == 1){
				count += 1;
			}
		}
		if(*attendance < count){
			*attendance = count;
		}
		Es[i] = count;
		count = 0;
	}
	for(i = 0; i < *room; i++){ /* �����̑����������v�� */
		for(j = 0; j < *feature; j++){
			if(Rf[i][j] == 1){
				count += 1;
			}
		}
		Fr[i] = count;
		count = 0;
	}
	for(i = 0; i < *event; i++){ /* �����̑����Ȗڂ��v�� */
		for(j = 0; j < *feature; j++){
			if(Ef[i][j] == 1){
				count += 1;
			}
		}
		Fe[i] = count;
		count = 0;
	}





#if 0

	printf("read�֐����`�F�b�N:\n");
	printf( "�Ȗڐ� : %d\n������ : %d\n�����̐� : %d\n���k�� : %d\n\n", *event, *room, *feature, *student);
	printf("roomsize:\n");
	for(i = 0; i < *room; i++){
		printf("%d\n", roomsize[i]);
	}puts("");
	printf("event_student:\n");
	for(i = 0; i < *student; i++){ /* event_student��0-1�}�g���N�X���o�� */
		for(j = 0; j < *event; j++){
			printf("%d", Se[i][j]);
		}puts("");
	}puts("");
	printf("feature_room\n");
	for(i = 0; i < *room; i++){ /* feature_room��0-1�}�g���N�X���o�� */
		for(j = 0; j < *feature; j++){
			printf("%d", Rf[i][j]);
		}puts("");
	}puts("");
	printf("feature_event\n");
	for(i = 0; i < *event; i++){ /* feature_event��0-1�}�g���N�X���o�� */
		for(j = 0; j < *feature; j++){
			printf("%d", Ef[i][j]);
		}puts("");
	}puts("");
	for(i = 0; i < *event; i++){ /* �e�Ȗڂɉ��l�̐��k���o�Ȃ��Ă��邩���o�� */
		printf("Es[%d] : %d\n", i, Es[i]);
	}puts("");

	for(i = 0; i < *room; i++){ /* �e�����ɂ����̓��������邩���o�� */
		printf("Fr[%d] : %d\n", i, Fr[i]);
	}puts("");

	for(i = 0; i < *event; i++){ /* �e�Ȗڂɂ����̓��������邩���o�� */
		printf("Fe[%d] : %d\n", i, Fe[i]);
	}puts("");
#endif


}
