#include<stdio.h>
#include<stdlib.h>
void getBenchmarkProblem(int **Se, int *Es, int **Rf, int *Fr, int **Ef, int *Fe, int *roomsize, int *event, int *room, int *feature, int *student, int *attendance){

	int i, j, count = 0;

	// ファイルポインタの宣言
	FILE *fp;

	if ((fp = fopen("./tim/competition07.tim", "r")) == NULL) {
		printf("file open error!!\n");
		exit(-1);	/* エラーの場合は通常、異常終了する */
	}

	/* ファイル1行目からそれぞれの数を読む */
	fscanf(fp, "%d %d %d %d", event, room, feature, student);
        
	// 部屋のサイズを読む
	for(i = 0; i < *room; i++){
		fscanf(fp, "%d", &roomsize[i]);
	}
        
	// どの授業に生徒が出席するか
	for(i = 0; i < *student; i++){ /* event_studentの0-1マトリクスを読む */
		for(j = 0; j < *event; j++){
			fscanf(fp, "%d", &Se[i][j]);
		}
	}
        
	// どの部屋にどの特徴があるか
	for(i = 0; i < *room; i++){ /* feature_roomの0-1マトリクスを読む */
		for(j = 0; j < *feature; j++){
			fscanf(fp, "%d", &Rf[i][j]);
		}
	}
	// どの授業にどの特徴がいるか
	for(i = 0; i < *event; i++){ /* feature_eventの0-1マトリクスを読む */
		for(j = 0; j < *feature; j++){
			fscanf(fp, "%d", &Ef[i][j]);
		}
	}
	fclose(fp); /* ファイルのクローズ */

	for(i = 0; i < *event; i++){ /* 各科目に何人の生徒が出席しているかを計測 */
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
	for(i = 0; i < *room; i++){ /* 特徴の多い教室を計測 */
		for(j = 0; j < *feature; j++){
			if(Rf[i][j] == 1){
				count += 1;
			}
		}
		Fr[i] = count;
		count = 0;
	}
	for(i = 0; i < *event; i++){ /* 特徴の多い科目を計測 */
		for(j = 0; j < *feature; j++){
			if(Ef[i][j] == 1){
				count += 1;
			}
		}
		Fe[i] = count;
		count = 0;
	}





#if 0

	printf("read関数内チェック:\n");
	printf( "科目数 : %d\n部屋数 : %d\n特徴の数 : %d\n生徒数 : %d\n\n", *event, *room, *feature, *student);
	printf("roomsize:\n");
	for(i = 0; i < *room; i++){
		printf("%d\n", roomsize[i]);
	}puts("");
	printf("event_student:\n");
	for(i = 0; i < *student; i++){ /* event_studentの0-1マトリクスを出力 */
		for(j = 0; j < *event; j++){
			printf("%d", Se[i][j]);
		}puts("");
	}puts("");
	printf("feature_room\n");
	for(i = 0; i < *room; i++){ /* feature_roomの0-1マトリクスを出力 */
		for(j = 0; j < *feature; j++){
			printf("%d", Rf[i][j]);
		}puts("");
	}puts("");
	printf("feature_event\n");
	for(i = 0; i < *event; i++){ /* feature_eventの0-1マトリクスを出力 */
		for(j = 0; j < *feature; j++){
			printf("%d", Ef[i][j]);
		}puts("");
	}puts("");
	for(i = 0; i < *event; i++){ /* 各科目に何人の生徒が出席しているかを出力 */
		printf("Es[%d] : %d\n", i, Es[i]);
	}puts("");

	for(i = 0; i < *room; i++){ /* 各部屋にいくつの特徴があるかを出力 */
		printf("Fr[%d] : %d\n", i, Fr[i]);
	}puts("");

	for(i = 0; i < *event; i++){ /* 各科目にいくつの特徴があるかを出力 */
		printf("Fe[%d] : %d\n", i, Fe[i]);
	}puts("");
#endif


}
