#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/DispatchingRule.h"
#include "../include/HardPenalty.h"
#include "../include/InitializeData.h"
#include "../include/InputData.h"
#include "../include/Swap.h"

#define NUMBER 1000
#define READ_DEBUG 0 /* ファイル読み込み確認用(デバッグ用) */
#define DEBUG 0

int last_soft_penalty(int *student, int **student_timeslot){
	int i;
	int count = 0;
	for(i = 0; i < *student; i++){
		count += student_timeslot[i][8];
		count += student_timeslot[i][17];
		count += student_timeslot[i][26];
		count += student_timeslot[i][35];
		count += student_timeslot[i][44];
	}
	return(count);
}
int one_soft_penalty(int *student, int **student_timeslot){
	int i, j, k;
	int count = 0;
	int flag;
	for(i = 0; i < *student; i++){
		for(j = 0; j < 5; j++){
			flag = 0;
			for(k = 0; k < 9; k++){
				flag += student_timeslot[i][j * 9 + k];
			}
			if(flag == 1){
				count += 1;
			}
		}
	}
	return(count);
}
void serial_soft_penalty2(int **student_timeslot, int *i, int *j, int *k, int *sp){
	if(student_timeslot[*i][*k] >= 1 && *k < (*j + 1) * 9){
		*sp += 1;
		*k += 1;
		serial_soft_penalty2(student_timeslot, i, j, k, sp);
	}
}

int serial_soft_penalty(int *student, int **student_timeslot){
	int i, j, k;
	int count = 0;
	for(i = 0; i < *student; i++){
		for(j = 0; j < 5; j++){
			for(k = j * 9 + 1; k < (j + 1) * 9; k++){
					if(student_timeslot[i][k - 1] >= 1 && student_timeslot[i][k++] >= 1){
						serial_soft_penalty2(student_timeslot, &i, &j, &k, &count);
				}
			}
		}
	}
	return(count);
}
int soft_penalty(int *student, int **student_timeslot){
	int sp = 0;
	int last, one;
	sp += last_soft_penalty(student, student_timeslot);
	last = sp;
	printf("last = %d ", last);
	sp += one_soft_penalty(student, student_timeslot);
	one = sp - last;
	printf("one = %d ", one);
	sp += serial_soft_penalty(student, student_timeslot);
	printf("serial = %d sp = %d\n", sp - last - one, sp);
	return(sp);
}
void in_room_2optsoft(int **event_timeslot, int **room_timeslot, int **student_timeslot, int *room, int *event, int *student, int **event_student, int **student_event, int *student_hp, int *sp, int **roomfeature_event, int **room_event){
	int i;
	int a_time, b_time;
	int a_event, b_event;
	int hp, soft;

	for(i = 0; i < *room; i++){
		for(a_time = 0; a_time < 45; a_time++){
			for(b_time = 0; b_time < 45; b_time++){
				if(room_timeslot[i][a_time] >= 1 && room_timeslot[i][b_time] >= 1 && a_time != b_time){
					a_event = room_event[i][a_time];
					b_event = room_event[i][b_time];
					hp = student_hard_penalty_2opt(student_timeslot, event_student, &a_time, &b_time, &a_event, &b_event);
					if(hp > 0){
						student_hard_penalty_2opt_difference(student_timeslot, event_student, &a_event, &b_event, &b_time, &a_time);
					}else{
						soft = soft_penalty(student, student_timeslot);
						if(soft > *sp){
							student_hard_penalty_2opt_difference(student_timeslot, event_student, &a_event, &b_event, &b_time, &a_time);
						}else{
							swap(&event_timeslot[a_event][1], &event_timeslot[b_event][1]);
							swap(&room_event[i][a_time], &room_event[i][b_time]);
							*student_hp += hp;
							*sp = soft;
						}
					}
				}else if(room_timeslot[i][a_time] >= 1 && room_timeslot[i][b_time] == 0){
					a_event = room_event[i][a_time];
					hp = student_hard_penalty_2optzero(student_timeslot, event_student, &a_time, &b_time, &a_event);
					if(hp > 0){
						student_hard_penalty_2optzero_difference(student_timeslot, event_student, &a_event, &a_time, &b_time, 1);
					}else{
						soft = soft_penalty(student, student_timeslot);
						if(soft > *sp){
							student_hard_penalty_2optzero_difference(student_timeslot, event_student, &a_event, &a_time, &b_time, 1);
						}else{
							event_timeslot[a_event][1] = b_time;
							swap(&room_event[i][a_time], &room_event[i][b_time]);
							*student_hp += hp;
							*sp = soft;
							room_timeslot[i][a_time] -= 1;
							room_timeslot[i][b_time] += 1;
						}
					}
				}else if(room_timeslot[i][a_time] == 0 && room_timeslot[i][b_time] >= 1){
					b_event = room_event[i][b_time];
					hp = student_hard_penalty_2optzero(student_timeslot, event_student, &b_time, &a_time, &b_event);
					if(hp > 0){
						student_hard_penalty_2optzero_difference(student_timeslot, event_student, &b_event, &b_time, &a_time, 1);
					}else{
						soft = soft_penalty(student, student_timeslot);
						if(soft > *sp){
							student_hard_penalty_2optzero_difference(student_timeslot, event_student, &b_event, &b_time, &a_time, 1);
						}else{
							event_timeslot[b_event][1] = a_time;
							swap(&room_event[i][a_time], &room_event[i][b_time]);
							*student_hp += hp;
							*sp = soft;
							room_timeslot[i][a_time] += 1;
							room_timeslot[i][b_time] -= 1;
						}
					}
				}
			}
		}
	}
}

void room_to_room(int *event, int *student, int *room, int **event_timeslot, int **room_timeslot, int **student_timeslot, int **event_student, int **eventfeature_room, int **roomfeature_event, int *student_hp, int *sp, int **room_event){
	int i, j, k;
	int a_time, b_time;
	int a_room, b_room;
	int a_event, b_event;
	int hp, soft;

	for(i = 0; i < *event; i++){
		a_event = i;
		a_room = event_timeslot[a_event][0];
		a_time = event_timeslot[a_event][1];
		for(j = 0; eventfeature_room[a_event][j] != -1; j++){
			if(eventfeature_room[a_event][j] != a_room){
				b_room = eventfeature_room[a_event][j];
				for(b_time = 0; b_time < 45; b_time++){
					if(room_timeslot[b_room][b_time] == 1){
						for(k = 0; roomfeature_event[a_room][k] != -1; k++){
							if(room_event[b_room][b_time] == roomfeature_event[a_room][k]){
								b_event = room_event[b_room][b_time];
								hp = student_hard_penalty_2opt(student_timeslot, event_student, &a_time, &b_time, &a_event, &b_event);
								if(hp > 0){
									student_hard_penalty_2opt_difference(student_timeslot, event_student, &a_event, &b_event, &b_time, &a_time);
								}else{
									soft = soft_penalty(student, student_timeslot);
									if(soft > *sp){
										student_hard_penalty_2opt_difference(student_timeslot, event_student, &a_event, &b_event, &b_time, &a_time);
									}else{
										swap(&event_timeslot[a_event][1], &event_timeslot[b_event][1]);
										swap(&event_timeslot[a_event][0], &event_timeslot[b_event][0]);
										swap(&room_event[a_room][a_time], &room_event[b_room][b_time]);
										*student_hp += hp;
										*sp = soft;
										goto ROOP;
									}
								}
							}
						}
					}else if(room_timeslot[b_room][b_time] == 0){
						hp = student_hard_penalty_2optzero(student_timeslot, event_student, &a_time, &b_time, &a_event);
						if(hp > 0){
							student_hard_penalty_2optzero_difference(student_timeslot, event_student, &a_event, &a_time, &b_time, 1);
						}else{
							soft = soft_penalty(student, student_timeslot);
							if(soft > *sp){
								student_hard_penalty_2optzero_difference(student_timeslot, event_student, &a_event, &a_time, &b_time, 1);
							}else{
								event_timeslot[a_event][1] = b_time;
								event_timeslot[a_event][0] = b_room;
								*student_hp += hp;
								*sp = soft;
								swap(&room_event[a_room][a_time], &room_event[b_room][b_time]);
								room_timeslot[a_room][a_time] -= 1;
								room_timeslot[b_room][b_time] += 1;
								goto ROOP;
							}
						}
					}
				}
			}
		}
		ROOP:;
	}
}

void k_opt(int *roomsize, int *event, int *room, int *feature, int *student, int **room_timeslot, int **event_timeslot, int **student_timeslot, int *student_hp, int **event_student, int **student_event, int *sp, int **eventfeature_room, int **roomfeature_event, int **room_event){
	int time;
	clock_t start, end;
	int count;
	int flag;

	*sp = soft_penalty(student, student_timeslot);

	//printf("解の改善が見られないときに何秒走査するか入力 : ");fflush(stdout);scanf("%d", &time);
	time = 1;
	while(1){
		count = *sp;
		in_room_2optsoft(event_timeslot, room_timeslot, student_timeslot, room, event, student, event_student, student_event, student_hp, sp, roomfeature_event, room_event);
		if(count == *sp){
			room_to_room(event, student, room, event_timeslot, room_timeslot, student_timeslot, event_student, eventfeature_room, roomfeature_event, student_hp, sp, room_event);
			if(count == *sp && flag == 0){
				start = clock();
				flag = 1;
			}else if(count == *sp && flag == 1){
				end = clock();
			}else{
				start = clock();
				end = clock();
				flag = 0;
			}
		}
		printf("student_hp : %d\n", *student_hp);
		printf("sp = %d\n", *sp);
		if(time < (end - start) / CLOCKS_PER_SEC && count == *sp){
			break;
		}
	}
	printf("time = %f", (double)(end - start) / CLOCKS_PER_SEC);
}
void RoomtimeslotRandomKick(int **event_timeslot, int **room_timeslot, int **student_timeslot, int *room, int *event, int *student, int **eventfeature_room, int **roomfeature_event, int **event_student, int *random_hp, int *seed, int **room_event){
	int i;
	int a_time, b_time;
	int a_event, b_event;
	int hp;
	srand(*seed);

	for(i = 0; i < *room; i++){
		a_time = rand() % 45;
		b_time = rand() % 45;
		if(room_timeslot[i][a_time] == 1 && room_timeslot[i][b_time] == 1){
			a_event = room_event[i][a_time];
			b_event = room_event[i][b_time];
			hp = student_hard_penalty_2opt(student_timeslot, event_student, &a_time, &b_time, &a_event, &b_event);
			if(hp > 0){
				student_hard_penalty_2opt_difference(student_timeslot, event_student, &a_event, &b_event, &b_time, &a_time);
			}else{
				swap(&event_timeslot[a_event][1], &event_timeslot[b_event][1]);
				*random_hp += hp;
				swap(&room_event[i][a_time], &room_event[i][b_time]);
			}
		}else if(room_timeslot[i][a_time] == 1 && room_timeslot[i][b_time] == 0){
			a_event = room_event[i][a_time];
			hp = student_hard_penalty_2optzero(student_timeslot, event_student, &a_time, &b_time, &a_event);
			if(hp > 0){
				student_hard_penalty_2optzero_difference(student_timeslot, event_student, &a_event, &a_time, &b_time, 1);
			}else{
				event_timeslot[a_event][1] = b_time;
				*random_hp += hp;
				room_timeslot[i][a_time] -= 1;
				room_timeslot[i][b_time] += 1;
				swap(&room_event[i][a_time], &room_event[i][b_time]);
			}
		}else if(room_timeslot[i][a_time] == 0 && room_timeslot[i][b_time] == 1){
			b_event = room_event[i][b_time];
			hp = student_hard_penalty_2optzero(student_timeslot, event_student, &b_time, &a_time, &b_event);
			if(hp > 0){
				student_hard_penalty_2optzero_difference(student_timeslot, event_student, &b_event, &b_time, &a_time, 1);
			}else{
				event_timeslot[b_event][1] = a_time;
				*random_hp += hp;
				room_timeslot[i][a_time] += 1;
				room_timeslot[i][b_time] -= 1;
				swap(&room_event[i][a_time], &room_event[i][b_time]);
			}
		}
	}
	/*for(i = 0; i < *event; i++){  a_event = i b_event =
		a_room = event_timeslot[i][0];
		a_time = event_timeslot[i][1];
		a_event = i;
		b_room = rand() % *room;
		for(j = 0; eventfeature_room[i][j] != -1; j++){
			if(a_room != eventfeature_room[i][j] && b_room == eventfeature_room[i][j]){
				for(b_time = 0; b_time < 45; b_time++){
					if(room_timeslot[b_room][b_time] == 0){
						*random_hp += student_hard_penalty_2optzero(student_timeslot, event_student, &a_time, &b_time, &a_event);
						room_timeslot[a_room][a_time] -= 1;
						room_timeslot[b_room][b_time] += 1;
						event_timeslot[a_event][1] = b_time;
						event_timeslot[a_event][0] = b_room;
						swap(&room_event[a_room][a_time], &room_event[b_room][b_time]);
						goto ROOP;
					}
				}
			}
		}
		ROOP:;
	}*/
}
void random_roop(int *event, int *room, int *feature, int *student, int **room_timeslot, int **event_timeslot, int **student_timeslot, int *student_hp, int **event_student, int **student_event, int *sp, int **eventfeature_room, int **roomfeature_event, int **room_event, int *roop, int **random_event, int **random_room, int **random_student, int **random_re, int *random_hp, int *random_sp){
	int i;
	int count;

	for(i = 0; i < *roop; i++){
		RoomtimeslotRandomKick(random_event, random_room, random_student, room, event, student, eventfeature_room, roomfeature_event, event_student, random_hp, &i, random_re);
	}
	*random_sp = soft_penalty(student, random_student);

	while(1){
		count = *random_sp;
		printf("aaaa\n");
		in_room_2optsoft(random_event, random_room, random_student, room, event, student, event_student, student_event, random_hp, random_sp, roomfeature_event, random_re);
		if(count == *random_sp){
			printf("bbb\n");
			room_to_room(event, student, room, random_event, random_room, random_student, event_student, eventfeature_room, roomfeature_event, random_hp, random_sp, random_re);
			printf("cccc\n");
			if(count == *random_sp){
				printf("random_hp : %d hp = %d\n", *random_hp, *student_hp);
				printf("random_sp = %d sp = %d\n", *random_sp, *sp);
				break;
			}
		}
	}
/*
	if(*sp > *random_sp){
		*sp = *random_sp;
		for(i = 0; i < *room; i++){
			for(j = 0; j < 45; j++){
				room_timeslot[i][j] = random_room[i][j];
				room_event[i][j] = random_re[i][j];
			}
		}
		for(i = 0; i < *event; i++){
			event_timeslot[i][0] = random_event[i][0];
			event_timeslot[i][1] = random_event[i][1];
		}
		for(i = 0; i < *student; i++){
			for(j = 0; j < 45; j++){
				student_timeslot[i][j] = random_student[i][j];
			}
		}
	}
	*/

}

void RandomKick(int *event, int *room, int *feature, int *student, int **room_timeslot, int **event_timeslot, int **student_timeslot, int *student_hp, int **event_student, int **student_event, int *sp, int **eventfeature_room, int **roomfeature_event, int **room_event){
	int roop;
	int i, j;
	int **random_room, **random_event, **random_student;
	int random_hp, random_sp;
	int **random_re; /* room_event */

	random_room = (int **)malloc(sizeof(int) * NUMBER);
	random_event = (int **)malloc(sizeof(int) * NUMBER);
	random_student = (int **)malloc(sizeof(int) * NUMBER);
	random_re = (int **)malloc(sizeof(int) * NUMBER);
	for(i = 0; i < NUMBER; i++){
		random_room[i] = (int *)malloc(NUMBER * sizeof(int));
		random_event[i] = (int *)malloc(NUMBER * sizeof(int));
		random_student[i] = (int *)malloc(NUMBER * sizeof(int));
		random_re[i] = (int *)malloc(NUMBER * sizeof(int));
	}
	minus2(random_room);
	minus2(random_event);
	minus2(random_student);
	minus2(random_re);

	random_hp = *student_hp;
	random_sp = *sp;
	for(i = 0; i < *room; i++){
		for(j = 0; j < 45; j++){
			random_room[i][j] = room_timeslot[i][j];
			random_re[i][j] = room_event[i][j];
		}
	}
	for(i = 0; i < *event; i++){
		random_event[i][0] = event_timeslot[i][0];
		random_event[i][1] = event_timeslot[i][1];
	}
	for(i = 0; i < *student; i++){
		for(j = 0; j < 45; j++){
			random_student[i][j] = student_timeslot[i][j];
		}
	}

	//printf("ランダム関数を何回まわすか");fflush(stdout);scanf("%d", &roop);
	//printf("改善が見られない場合何秒まわすか");fflush(stdout);scanf("%d", &time);

	FILE *fp;
	if ((fp = fopen("../tim/competition01.tim", "w")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);	/* (3)エラーの場合は通常、異常終了する */
	}

	int k;
	for(k = 0; k <= 9; k++){


	i = 0;
	while(i != 50){
		srand(k);
		roop = rand() % 6 + 5;
		random_roop(event, room, feature, student, room_timeslot, event_timeslot, student_timeslot, student_hp, event_student, student_event, sp, eventfeature_room, roomfeature_event, room_event, &roop, random_event, random_room, random_student, random_re, &random_hp, &random_sp);
		i++;
	}
	fprintf(fp, "roop = %2d hp = %2d sp = %4d\n", k, random_hp, random_sp);
	random_hp = *student_hp;
	random_sp = *sp;
	for(i = 0; i < *room; i++){
		for(j = 0; j < 45; j++){
			random_room[i][j] = room_timeslot[i][j];
			random_re[i][j] = room_event[i][j];
		}
	}
	for(i = 0; i < *event; i++){
		random_event[i][0] = event_timeslot[i][0];
		random_event[i][1] = event_timeslot[i][1];
	}
	for(i = 0; i < *student; i++){
		for(j = 0; j < 45; j++){
			random_student[i][j] = student_timeslot[i][j];
		}
	}
	}fclose(fp);

	for(i = 0; i < NUMBER; i++){
		free(random_room[i]);
		free(random_event[i]);
		free(random_student[i]);
		free(random_re[i]);
	}
	free(random_room);
	free(random_event);
	free(random_student);
	free(random_re);
}
/* main関数 */
int main(void){
	printf("main関数内チェック:\n");
	int event, room, feature, student;
	int max_attendance = 0;
	int **Se; /* 生徒がどの科目を受講しているか */
	int *Es; /* 1科目何人受講しているか */
	int **Rf; /* どの部屋がどの特徴を満たしているか */
	int *Fr; /*どの部屋が何個の制約を満たしているか*/
	int **Ef; /* どの科目がどの特徴を満たす必要があるか */
	int *Fe; /*どの科目が何個の特徴を満たしているか*/
	int *roomsize; /* 部屋の大きさ */

	int student_hp; /* studentのハードペナルティがいくつあるか */
	int sp; /* ソフトペナルティがいくつあるか */

	int **order_fr, **order_fe; /* 部屋の特徴と科目の特徴を順番に並べ入れる配列 */
	int **order_se; /* m人が受講する順番に並べるorder[受講者数][i] */


	int **room_timeslot; /* 部屋の時間割 */
	int **room_event;
	int **event_timeslot; /* eventがどのroomに対応しているか */
	int **eventfeature_room; /* eventのfeatureを満たすroomを列挙 */
	int **roomfeature_event; /* roomのfeatureを満たすeventを列挙*/
	int **student_timeslot;
	int **event_student;/*eventに対してどのstudentが受講するか*/
	int **student_event;/*studentはどこのeventに受講するか*/

	int *delay_er; /* どのeventがいくつの部屋を選べるか */
	int **delay_re; /* 条件を満たす部屋数が余裕のある科目順番に列挙 */
	int *delay_r; /* 条件を満たす部屋数がいくつあるか */

	int i;

	Se = (int **)malloc(sizeof(int) * NUMBER);
	Es = (int *)malloc(sizeof(int) * NUMBER);
	Rf = (int **)malloc(sizeof(int) * NUMBER);
	Fr = (int *)malloc(sizeof(int) * NUMBER);
	Ef = (int **)malloc(sizeof(int) * NUMBER);
	Fe = (int *)malloc(sizeof(int) * NUMBER);
	roomsize = (int *)malloc(sizeof(int) * NUMBER);
	room_timeslot = (int **)malloc(sizeof(int) * NUMBER);
	room_event = (int **)malloc(sizeof(int) * NUMBER);
	event_timeslot = (int **)malloc(sizeof(int) * NUMBER);
	student_timeslot = (int **)malloc(sizeof(int) * NUMBER);
	event_student = (int **)malloc(sizeof(int) * NUMBER);
	student_event = (int **)malloc(sizeof(int) * NUMBER);
	order_fr = (int **)malloc(sizeof(int) * NUMBER);
	order_fe = (int **)malloc(sizeof(int) * NUMBER);
	order_se = (int **)malloc(sizeof(int) * NUMBER);
	delay_er = (int *)malloc(sizeof(int) * NUMBER);
	delay_r = (int *)malloc(sizeof(int) * NUMBER);
	delay_re = (int **)malloc(sizeof(int) * NUMBER);

	eventfeature_room = (int **)malloc(sizeof(int) * NUMBER);
	roomfeature_event = (int **)malloc(sizeof(int) * NUMBER);

	for(i = 0; i < NUMBER; i++){
		Se[i] = (int *)malloc(NUMBER * sizeof(int));
		Rf[i] = (int *)malloc(NUMBER * sizeof(int));
		Ef[i] = (int *)malloc(NUMBER * sizeof(int));
		room_timeslot[i] = (int *)malloc(NUMBER * sizeof(int));
		room_event[i] = (int *)malloc(sizeof(int) * NUMBER);
		event_timeslot[i] = (int *)malloc(NUMBER * sizeof(int));
		student_timeslot[i] = (int *)malloc(NUMBER * sizeof(int));
		event_student[i] = (int *)malloc(NUMBER * sizeof(int));
		student_event[i] = (int *)malloc(NUMBER * sizeof(int));
		order_fr[i] = (int *)malloc(NUMBER * sizeof(int));
		order_fe[i] = (int *)malloc(NUMBER * sizeof(int));
		order_se[i] = (int *)malloc(NUMBER * sizeof(int));
		eventfeature_room[i] = (int *)malloc(NUMBER * sizeof(int));
		roomfeature_event[i] = (int *)malloc(NUMBER * sizeof(int));
		delay_re[i] = (int *)malloc(sizeof(int) * NUMBER);
	}

	zero2(room_timeslot);
	minus2(room_event);
	minus2(event_timeslot);
	zero2(student_timeslot);
	minus2(eventfeature_room);
	minus2(roomfeature_event);
	minus2(event_student);
	minus2(student_event);
	minus1(delay_er);
	minus2(delay_re);
	zero1(delay_r);
	minus2(order_se);


	InputData(Se, Es, Rf, Fr, Ef, Fe, roomsize, &event, &room, &feature, &student, &max_attendance);

#if 1
	int j;
	printf("main関数内チェック:\n");
	printf( "科目数 : %d\n部屋数 : %d\n特徴の数 : %d\n生徒数 : %d\n\n", event, room, feature, student);
	printf("roomsize:\n");
	for(i = 0; i < room; i++){
		printf("%d\n", roomsize[i]);
	}puts("");
	printf("event_student:\n");
	for(i = 0; i < student; i++){ /* event_studentの0-1マトリクスを出力 */
		for(j = 0; j < event; j++){
			printf("%d", Se[i][j]);
		}puts("");
	}puts("");
	printf("feature_room\n");
	for(i = 0; i < room; i++){ /* feature_roomの0-1マトリクスを出力 */
		for(j = 0; j < feature; j++){
			printf("%d", Rf[i][j]);
		}puts("");
	}puts("");
	printf("feature_event\n");
	for(i = 0; i < event; i++){ /* feature_eventの0-1マトリクスを出力 */
		for(j = 0; j < feature; j++){
			printf("%d", Ef[i][j]);
		}puts("");
	}puts("");
	for(i = 0; i < event; i++){ /* 各科目に何人の生徒が出席しているかを出力 */
		printf("Es[%d] : %d\n", i, Es[i]);
	}puts("");


	for(i = 0; i < room; i++){ /* 各部屋にいくつの特徴があるかを出力 */
		printf("Fr[%d] : %d\n", i, Fr[i]);
	}puts("");

	for(i = 0; i < event; i++){ /* 各科目にいくつの特徴があるかを出力 */
		printf("Fe[%d] : %d\n", i, Fe[i]);
	}puts("");

	printf("受講人数が少ない順番に出力\n"); /* 受講人数が少ない科目を順番に出力 */
	for(i = 0; i <= max_attendance; i++){
		printf("%d人 :\n", i);
		for(j = 0; j < event ; j++){
			if(Es[j] == i){
				printf("Es[%d]\n", j);
			}
		}
	}puts("");

	for(i = 0; i <= feature; i++){ /* 各部屋にいくつの特徴があるかを出力 */
		printf("部屋の特徴の数%d :\n", i);
		for(j = 0; j < room; j++){
			if(Fr[j] == i){
				printf("Fr[%d] : %d\n", j, Fr[j]);
			}
		}
	}puts("");

	for(i = 0; i <= feature; i++){ /* 各科目にいくつの特徴があるかを出力 */
		printf("科目の特徴の数%d :\n", i);
		for(j = 0; j < event; j++){
			if(Fe[j] == i){
				printf("Fe[%d] : %d\n", j, Fe[j]);
			}
		}
	}puts("");

#endif
	DispatchingRule(Se, Es, Rf, Fr, Ef, Fe, roomsize, &event, &room, &feature, &student, room_timeslot, order_fr, order_fe, order_se, event_timeslot, eventfeature_room, roomfeature_event, delay_er, delay_re, delay_r, student_timeslot, &max_attendance, event_student, student_event, &student_hp, room_event, &i);
	k_opt(roomsize, &event, &room, &feature, &student, room_timeslot, event_timeslot, student_timeslot, &student_hp, event_student, student_event, &sp, eventfeature_room, roomfeature_event, room_event);
	RandomKick(&event, &room, &feature, &student, room_timeslot, event_timeslot, student_timeslot, &student_hp, event_student, student_event, &sp, eventfeature_room, roomfeature_event, room_event);

	/* メモリの開放 */
	for(i = 0; i < NUMBER; i++){
		free(Se[i]);
		free(Rf[i]);
		free(Ef[i]);
		free(room_timeslot[i]);
		free(room_event[i]);
		free(event_timeslot[i]);
		free(student_timeslot[i]);
		free(event_student[i]);
		free(student_event[i]);
		free(order_fr[i]);
		free(order_fe[i]);
		free(order_se[i]);
		free(eventfeature_room[i]);
		free(roomfeature_event[i]);
		free(delay_re[i]);
	}
	free(delay_re);
	free(delay_r);
	free(Se);
	free(Es);
	free(Rf);
	free(Fr);
	free(Ef);
	free(Fe);
	free(roomsize);
	free(room_timeslot);
	free(room_event);
	free(event_timeslot);
	free(student_timeslot);
	free(event_student);
	free(student_event);
	free(order_fr);
	free(order_fe);
	free(order_se);
	free(eventfeature_room);
	free(roomfeature_event);
	free(delay_er);

	return(0);
}
