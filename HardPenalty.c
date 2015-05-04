#include<stdio.h>
int student_hard_penalty(int *student, int **student_timeslot){
	int i, j;
	int count = 0;
	for(i = 0; i < *student; i++){
		for(j = 0; j <= 44; j++){
			if(student_timeslot[i][j] > 1){
				count += student_timeslot[i][j] - 1;
			}
		}
	}
	return(count);
}
void student_hard_penalty_2opt_difference(int **student_timeslot, int **event_student, int *a_event, int *b_event, int *a_time, int *b_time){
	int i;
	for(i = 0; event_student[*a_event][i] != -1; i++){
		student_timeslot[event_student[*a_event][i]][*a_time] -= 1;
		student_timeslot[event_student[*a_event][i]][*b_time] += 1;
	}
	for(i = 0; event_student[*b_event][i] != -1; i++){
		student_timeslot[event_student[*b_event][i]][*b_time] -= 1;
		student_timeslot[event_student[*b_event][i]][*a_time] += 1;
	}
}
int student_hard_penalty_2opt(int **student_timeslot, int **event_student, int *a_time, int *b_time, int *a_event, int *b_event){
	/* 2optのハードペナルティがいくつか数える関数 */
	int i;
	int count_before = 0, count_after = 0;
	for(i = 0; event_student[*a_event][i] != -1; i++){
		if(student_timeslot[event_student[*a_event][i]][*a_time] > 1){
			count_before += student_timeslot[event_student[*a_event][i]][*a_time] - 1;
		}
		if(student_timeslot[event_student[*a_event][i]][*b_time] > 1){
			count_before += student_timeslot[event_student[*a_event][i]][*b_time] - 1;
		}
	}
	for(i = 0; event_student[*b_event][i] != -1; i++){
		if(student_timeslot[event_student[*b_event][i]][*b_time] > 1){
			count_before += student_timeslot[event_student[*b_event][i]][*b_time] - 1;
		}
		if(student_timeslot[event_student[*b_event][i]][*a_time] > 1){
			count_before += student_timeslot[event_student[*b_event][i]][*a_time] - 1;
		}
	}

	student_hard_penalty_2opt_difference(student_timeslot, event_student, a_event, b_event, a_time, b_time);

	for(i = 0; event_student[*a_event][i] != -1; i++){
		if(student_timeslot[event_student[*a_event][i]][*a_time] > 1){
			count_after += student_timeslot[event_student[*a_event][i]][*a_time] - 1;
		}
		if(student_timeslot[event_student[*a_event][i]][*b_time] > 1){
			count_after += student_timeslot[event_student[*a_event][i]][*b_time] - 1;
		}
	}
	for(i = 0; event_student[*b_event][i] != -1; i++){
		if(student_timeslot[event_student[*b_event][i]][*b_time] > 1){
			count_after += student_timeslot[event_student[*b_event][i]][*b_time] - 1;
		}
		if(student_timeslot[event_student[*b_event][i]][*a_time] > 1){
			count_after += student_timeslot[event_student[*b_event][i]][*a_time] - 1;
		}
	}
	return(count_after - count_before);
}
void student_hard_penalty_2optzero_difference(int **student_timeslot, int **event_student, int *a_event, int *a_time, int *b_time, int flag){
	int i;
	if(flag == 0){
		for(i = 0; event_student[*a_event][i] != -1; i++){
			student_timeslot[event_student[*a_event][i]][*a_time] -= 1;
			student_timeslot[event_student[*a_event][i]][*b_time] += 1;
		}
	}else{
		for(i = 0; event_student[*a_event][i] != -1; i++){
			student_timeslot[event_student[*a_event][i]][*b_time] -= 1;
			student_timeslot[event_student[*a_event][i]][*a_time] += 1;
		}
	}
}

int student_hard_penalty_2optzero(int **student_timeslot, int **event_student, int *a_time, int *b_time, int *a_event){
	int i;
	int count_before = 0, count_after = 0;
	for(i = 0; event_student[*a_event][i] != -1; i++){
		if(student_timeslot[event_student[*a_event][i]][*a_time] > 1){
			count_before += student_timeslot[event_student[*a_event][i]][*a_time] - 1;
		}
		if(student_timeslot[event_student[*a_event][i]][*b_time] > 1){
			count_before += student_timeslot[event_student[*a_event][i]][*b_time] - 1;
		}
	}

	student_hard_penalty_2optzero_difference(student_timeslot, event_student, a_event, a_time, b_time, 0);

	for(i = 0; event_student[*a_event][i] != -1; i++){
		if(student_timeslot[event_student[*a_event][i]][*a_time] > 1){
			count_after += student_timeslot[event_student[*a_event][i]][*a_time] - 1;
		}
		if(student_timeslot[event_student[*a_event][i]][*b_time] > 1){
			count_after += student_timeslot[event_student[*a_event][i]][*b_time] - 1;
		}
	}
	return(count_after - count_before);
}
