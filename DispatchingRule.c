#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "HardPenalty.h"
#include "InitializeData.h"
#include "TwoSwap.h"

#define NUMBER 1000

/*科目を満たす部屋がどの部屋か全て列挙*/
void ef_all_check(int *feature, int *room, int *event, int *Es, int **Ef, int **Rf, int *Fr, int **eventfeature_room, int **roomfeature_event, int *roomsize, int **order_fr, int *delay_er){
	int i, j, k;
	int count1 = 0, count2 = 0;

	int *sort_fr;/* featureの少ない順にroomを並べる */
	sort_fr = (int *)malloc(sizeof(int) * NUMBER);
	minus1(sort_fr);

	/* featureの少ない順にroomを並べる */
	for(i = 0; i <= *feature; i++){
		for(j = 0; j < *room; j++){
			if(Fr[j] == i){
				sort_fr[count1] = j;
				count1++;
			}
		}
	}
	/* 科目を満たしている部屋を全てeventfeature_room内に格納 eventfeature_room[event_number][i] */
	for(i = 0; i < *event; i++){
		count2 = 0;
		for(j = 0; j < *room; j++){
			count1 = 0;
			for(k = 0; k < *feature; k++){
				if(Ef[i][k] == Rf[sort_fr[j]][k] || Ef[i][k] == 0){
					count1 +=1;
				}
			}
			if(count1 == *feature && Es[i] <= roomsize[sort_fr[j]]){
				eventfeature_room[i][count2] = sort_fr[j];
				count2 += 1;
			}
			/*printf("event i : %d room j : %d count1 : %d count2 : %d\n", i, j , count1, count2);*/
		}
	}
	/* 科目による条件を全て満たしている部屋は各科目何教室あるかdelay_room内に格納 delay_room[event_number] */
	for(i = 0; i < *event; i++){
		count1 = 0;
		for(j = 0; j <= *room; j++){
			if(eventfeature_room[i][j] == -1){
				delay_er[i] = count1;
				count1 =0;
				break;
			}else{
				count1 += 1;
			}
		}
	}
	for(i = 0; i < *room; i++){
		count1 = 0;
		for(j = 0; j < *event; j++){
			for(k = 0; eventfeature_room[j][k] != -1; k++){
				if(eventfeature_room[j][k] == i){
					roomfeature_event[i][count1++] = j;
				}
			}
		}
	}

#if 0
	for(i = 0; i <= *feature; i++){
		printf("sort_fr[%d] : %d\n", i, sort_fr[i]);
	}
	for(i = 0; i < *event; i++){
		printf("eventfeature_room[%d] :", i);
		for(j = 0; j < *room; j++){
			printf("%d ", eventfeature_room[i][j]);
		}
		puts("");
	}
	for(i = 0; i < *room; i++){
		printf("roomfeature_event[%d] :", i);
		for(j = 0; j < *event; j++){
			printf("%d ", roomfeature_event[i][j]);
		}
		puts("");
	}
	for(i = 0; i < *event; i++){
		printf("delay_er[%d] : %d\n", i, delay_er[i]);
	}
#endif
	free(sort_fr);
}
int all_plus(int room_number, int event_number, int *over_room, int **room_timeslot, int **event_timeslot, int **room_event){
	int day_time;
	day_time = over_room[room_number];
	if(over_room[room_number] <= 44){
		event_timeslot[event_number][0] = room_number;
		event_timeslot[event_number][1] = day_time;
		room_timeslot[room_number][day_time] = 1;
		room_event[room_number][day_time] = event_number;
		over_room[room_number] += 1;
		printf("room_timeslot[%d][%d] : %d  over_room[%d] : %d event_timeslot[%d][room_number] : %d event_timeslot[%d][day_time] : %d\n",room_number, day_time,room_timeslot[room_number][day_time], room_number, over_room[room_number], event_number, event_timeslot[event_number][0], event_number, event_timeslot[event_number][1]);
		return(1);
	}else{
		return(0);
	}
}

void rf_all_check(int *event, int *room, int *feature, int **eventfeature_room, int *delay_er, int **delay_re, int *delay_r, int **room_timeslot, int **event_timeslot, int *over_room, int **room_event, int *seed){
	int i, j, k;
	int count = 0;
	int day_time;


	for(i = 0; i <= *room; i++){
		count = 0;
		for(j = 0; j <= *event; j++){
			if(i == delay_er[j]){
				delay_re[i][count] = j;
				count += 1;
			}
		}
	}
	/*問題が成り立っているかチェック*/
	if(delay_re[0][0] != -1){
		printf("rf_all_check error\n特徴が満たせない科目があります");
		exit(-1);
	}
	for(i = 0; i <= *room; i++){
		printf("delay_re[%d] :", i);
		for(j = 0; j <= *event; j++){
			printf("%d ", delay_re[i][j]);
		}puts("");
	}
	for(i = 0; i <= *room; i++){
		for(j = 0; delay_re[i][j] != -1; j++){
			delay_r[i] += 1;
		}
	}
	for(i = 0; i <= *room; i++){
		printf("delay_r[%d] : %d\n", i, delay_r[i]);
	}

	for(i = 0; i <= *room; i++){ /* 制約条件を持つ科目を満たす部屋が少ない順番に */
		count = 0;
		for(j = 0; j < delay_r[i]; j++){
			for(k = 0; k <= i ; k++){
				day_time = all_plus(eventfeature_room[delay_re[i][j]][k], delay_re[i][j], over_room, room_timeslot, event_timeslot, room_event);
				if(day_time == 1){
					printf("i : %d j : %d okday_time : %d\n", i, j, day_time);
					printf("okevent_number %d  room : %d timeslot : %d\n\n", i, event_timeslot[i][0], event_timeslot[i][1]);
					break;
				}
			}
			if(day_time == 0){
				printf("rf_all_check error!\nall_plus内でエラーが発生しています\n");
				exit(-1);
			}
		}
	}
#if 1

	for(i = 0; i < *room; i++){
		printf("room_number %d\n", i);
		for(j = 0; j < 9; j++){
			for(k = 0; k < 5; k++, count = 9 * k){
				printf("%d ", room_timeslot[i][j + count]);
			}puts("");
			count = 0;
		}
		puts("");
	}

	for(i = 0; i < *event; i++){
		printf("event_number %d  room : %d timeslot : %d", i, event_timeslot[i][0], event_timeslot[i][1]);
		puts("");
	}

#endif

}
/* ディスパッチングルールを用いて初期解を作成 */
void DispatchingRule(int **Se, int *Es, int **Rf, int *Fr, int **Ef, int *Fe, int *roomsize, int *event, int *room, int *feature, int *student, int **room_timeslot, int **order_fr, int **order_fe, int **order_se, int **event_timeslot, int **eventfeature_room, int **roomfeature_event, int *delay_er, int **delay_re, int *delay_r, int **student_timeslot, int *max_attendance, int **event_student, int **student_event, int *student_hp, int **room_event, int *seed){
	int i, j, k;
	int time;
	int *oe_count; /* n個の特徴を持つ科目がいくつあるか */
	int *or_count; /* n個の特徴を持つ科目がいくつあるか */
	int *se_count; /* n人が受講する科目は何科目あるか*/
	int *e_count; /* eventがどの特徴を満たさないといけないか */
	int *over_room; /* room_timeslotが44超えてしまった場合1 それ以外は0 */
	clock_t start, end;


	oe_count = (int *)malloc(sizeof(int) * NUMBER);
	or_count = (int *)malloc(sizeof(int) * NUMBER);
	e_count = (int *)malloc(sizeof(int) * NUMBER);
	se_count = (int *)malloc(sizeof(int) * NUMBER);
	over_room = (int *)malloc(sizeof(int) * NUMBER);

	int count = 0;

	minus2(order_fr);
	minus2(order_fe);
	zero1(oe_count);
	zero1(or_count);
	zero1(over_room);



	for(i = 0; i <= *feature; i++){ /* 部屋の特徴の数が同じもの同士を並べる */
		for(j = 0; j < *room; j++){
			if(Fr[j] == i){
				order_fr[i][count] = j;
				count++;
			}
		}count = 0;
	}

	for(i = 0; i <= *feature; i++){ /* 科目の特徴の数が同じもの同士を並べる */
		for(j = 0; j < *event; j++){
			if(Fe[j] == i){
				order_fe[i][count] = j;
				count++;
			}
		}count = 0;
	}
	/* n個の特徴のある科目がいくつあるのか(oe_count[特徴の数] == 科目がいくつあるか)に入力 */
	for(i = 0; i <= *feature; i++){
		for(j = 0; j < *event; j++){
			if(order_fe[i][j] == -1){
				oe_count[i] = j;
				break;
			}
		}
	}

	/* n個の特徴のある部屋がいくつあるのか(or_count[特徴の数] == 部屋がいくつあるか)に入力 */
	for(i = 0; i <= *feature; i++){
		for(j = 0; j < *room; j++){
			if(order_fr[i][j] == -1){
				or_count[i] = j;
				break;
			}
		}
	}
	for(i = 0; i <= *max_attendance; i++){
		count = 0;
		for(j = 0; j < *event; j++){
			if(Es[j] == i){
				order_se[i][count] = j;
				count += 1;
			}
		}
		se_count[i] = count;
	}


	for(i = 0; i < *event; i++){ /* eventにどのstudentが出席するか */
		count = 0;
		for(j = 0; j < *student; j++){
			if(Se[j][i] == 1){
				event_student[i][count] = j;
				count += 1;
			}
		}
	}

	for(i = 0; i < *student; i++){ /* studentはどのeventに出席するか */
		count = 0;
		for(j = 0; j < *event; j++){
			if(Se[i][j] == 1){
				student_event[i][count] = j;
				count += 1;
			}
		}
	}



#if 0
	for(i = 0; i <= *max_attendance; i++){
		printf("%d人 :\n", i);
		for(j = 0; j < *event ; j++){
			if(Es[j] == i){
				printf("Es[%d]\n", j);
			}
		}
	}puts("");
	puts("");
	for(i = 0; i < *event; i++){
		printf("Es[%d] : %d\n", i, Es[i]);
	}
	for(i = 0; i <= *max_attendance; i++){
		printf("se_count[%d] : %d\n", i, se_count[i]);
	}
	for(i = 0; i <= *feature; i++){ /* 科目の特徴の多い順番に並んでいるかを出力 */
		printf("order_fr %d :\n", i); /* iは特徴の数 */
		for(j = 0; j < *room; j++){
			printf("%d ", order_fr[i][j]); /* order_fr[特徴の数][配列の要素数] */
		}puts("");
	}puts("");

	for(i = 0; i <= *feature; i++){ /* 部屋の特徴の多い順番に並んでいるかを出力 */
		printf("order_fe %d :\n", i);
		for(j = 0; j < *event; j++){
			printf("%d ", order_fe[i][j]); /* order_fe[特徴の数][科目番号] */
		}puts("");
	}



	fflush(stdout);
	scanf("%d", &i);
	for(i = 0; i <= max_attendance[i]; i++){
		for(j = 0; j < se_count[i]; j++){
			printf("order_se[%d][%d] = %d\n",i, j, order_se[i][j]);
		}
	}
	for(i = 0; i < *event; i++){ /* eventにどのstudentが出席するか */
		printf("event_student[%d] : ", i);
		for(j = 0; event_student[i][j] != -1; j++){
			printf("%d ", event_student[i][j]);
		}puts("");
	}


	/* n個の特徴を持つ科目がいくつあるのか(oe_count[特徴の数] == 科目がいくつあるか)を出力 */
	printf(" \n");
	for(i = 0; i <= *feature; i++){
		printf("oe_ count[%d] : %d\n", i, oe_count[i]);
	}
	/* n個の特徴を持つ科目がいくつあるのか(or_count[特徴の数] == 科目がいくつあるか)を出力 */
	printf(" \n");
	for(i = 0; i <= *feature; i++){
		printf("or_ count[%d] : %d\n", i, or_count[i]);
	}
	for(i = 0; i < *student; i++){ /* studentはどのeventに出席するかを出力 */
		printf("student[%d] : ", i);
		for(j = 0; student_event[i][j] != -1; j++){
			printf("%d ", student_event[i][j]);
		}
		puts("");
	}

#endif


	/* eventfeature_roomとdelay_erを持ち帰る */
	ef_all_check(feature, room, event, Es, Ef, Rf, Fr, eventfeature_room, roomfeature_event, roomsize, order_fr, delay_er);
	/* room_timeslotとevent_timeslotに値を入れる 副産物でover_roomに値が入る */
	rf_all_check(event, room, feature, eventfeature_room, delay_er, delay_re, delay_r, room_timeslot, event_timeslot, over_room, room_event, seed);
	for(i = 0; i < *room; i++){
		printf("room_number %d\n", i);
		for(j = 0; j < 9; j++){
			count = 0;
			for(k = 0; k < 5; k++, count = k * 9){
				printf("%d ", room_timeslot[i][j + count]);
			}puts("");
		}
		puts("");
	}
	/* event_studentに科目がいくつ入っているか */
	for(i = 0; i < *event; i++){
		for(j = 0; event_student[i][j] != -1 ; j++){
			student_timeslot[event_student[i][j]][event_timeslot[i][1]] += 1;
			printf("event_student : %d  event_timeslot[i][1] : %d\n", event_student[i][j], event_timeslot[i][1]);
			printf("gotyagotya : %d\n", student_timeslot[event_student[i][j]][event_timeslot[i][1]]);
		}
	}




	*student_hp = student_hard_penalty(student, student_timeslot);
	printf("student_hp : %d\n", *student_hp);
	time = 20;
	start = clock();
	while(1){
		end = clock();
		printf("-student_hp : %d\n", *student_hp);
		RoomTimeslot2swap(event_timeslot, room_timeslot, student_timeslot, room, event, student, event_student, student_event, student_hp, roomfeature_event, room_event);
		if(*student_hp == 0){
			break;
		}else if(time < (end - start) / CLOCKS_PER_SEC){
			break;
		}
	}
	if(*student_hp == 0){
		printf("ハード制約がなくなりました．\n");
	}else{
		printf("2opt-student_hp : %d\n", *student_hp);
	}
	printf("2-optの処理時間 : %f",(double)(end - start) / CLOCKS_PER_SEC);


#if 0
	for(i = 0; i < *room; i++){
		printf("room_number %d\n", i);
		for(j = 0; j < 9; j++){
			count = 0;
			for(k = 0; k < 5; k++, count = k * 9){
				printf("%d ", room_timeslot[i][j + count]);
			}puts("");
		}
		puts("");
	}

	for(i = 0; i < *student; i++){
		printf("student_number %d\n", i);
		for(j = 0; j < 9; j++){
			count = 0;
			for(k = 0; k < 5; k++, count = 9 * k){
				printf("%d ", student_timeslot[i][j + count]);
			}puts("");
		}
		puts("");
	}


	for(i = 0; i <= *feature; i++){ /* 科目の特徴の多い順番に並んでいるかを出力 */
		printf("order_fr %d :\n", i); /* iは特徴の数 */
		for(j = 0; j < *room; j++){
			printf("%d ", order_fr[i][j]); /* order_fr[特徴の数][配列の要素数] */
		}puts("");
	}puts("");

	for(i = 0; i <= *feature; i++){ /* 部屋の特徴の多い順番に並んでいるかを出力 */
		printf("order_fe %d :\n", i);
		for(j = 0; j < *event; j++){
			printf("%d ", order_fe[i][j]); /* order_fe[特徴の数][科目番号] */
		}puts("");
	}


	/* n個の特徴を持つ科目がいくつあるのか(oe_count[特徴の数] == 科目がいくつあるか)を出力 */
	printf(" \n");
	for(i = 0; i <= *feature; i++){
		printf("oe_ count[%d] : %d\n", i, oe_count[i]);
	}
	/* n個の特徴を持つ科目がいくつあるのか(or_count[特徴の数] == 科目がいくつあるか)を出力 */
	printf(" \n");
	for(i = 0; i <= *feature; i++){
		printf("or_ count[%d] : %d\n", i, or_count[i]);
	}

	/* 科目がどこに対応しているかを出力 */
	for(i = 0; i < *event; i++){
		printf("event_number %d\n room : %d timeslot : %d", i, event_timeslot[i][0], event_timeslot[i][1]);
		puts("");
	}
#endif
	free(oe_count);
	free(or_count);
	free(se_count);
	free(e_count);
	free(over_room);
}
