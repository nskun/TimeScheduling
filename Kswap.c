#include<stdio.h>
#include"HardPenalty.h"
#include"Swap.h"
void InRoomKswapRoop(int *room, int *time, int **event_timeslot, int **room_timeslot, int **student_timeslot, int **event_student, int **room_event, int *student_hp){
	int i;
	int a_time, b_time;
	int a_event, b_event;
	int hp;
	if(*time != 1){
		b_time = *time - 1;
		a_time = b_time - 1;
		if(room_timeslot[i][a_time] >= 1 && room_timeslot[i][b_time] >= 1){
			a_event = room_event[i][a_time];
			b_event = room_event[i][b_time];
			hp = student_hard_penalty_2opt(student_timeslot, event_student, &a_time, &b_time, &a_event, &b_event);
			if(hp > 0){
				student_hard_penalty_2opt_difference(student_timeslot, event_student, &a_event, &b_event, &b_time, &a_time);
			}else{
				swap(&event_timeslot[a_event][1], &event_timeslot[b_event][1]);
				swap(&room_event[i][a_time], &room_event[i][b_time]);
				*student_hp += hp;
				InRoomKswapRoop(&i, &b_time, event_timeslot, room_timeslot, student_timeslot, event_student, room_event, student_hp);
			}
		}else if(room_timeslot[i][a_time] >= 1 && room_timeslot[i][b_time] == 0){
			a_event = room_event[i][a_time];
			hp = student_hard_penalty_2optzero(student_timeslot, event_student, &a_time, &b_time, &a_event);
			if(hp > 0){
				student_hard_penalty_2optzero_difference(student_timeslot, event_student, &a_event, &a_time, &b_time, 1);
			}else{
				swap(&room_event[i][a_time], &room_event[i][b_time]);
				event_timeslot[a_event][1] = b_time;
				*student_hp += hp;
				room_timeslot[i][a_time] -= 1;
				room_timeslot[i][b_time] += 1;
				InRoomKswapRoop(&i, &b_time, event_timeslot, room_timeslot, student_timeslot, event_student, room_event, student_hp);
			}
		}else if(room_timeslot[i][a_time] == 0 && room_timeslot[i][b_time] >= 1){
			b_event = room_event[i][b_time];
			hp = student_hard_penalty_2optzero(student_timeslot, event_student, &b_time, &a_time, &b_event);
			if(hp > 0){
				student_hard_penalty_2optzero_difference(student_timeslot, event_student, &b_event, &b_time, &a_time, 1);
			}else{
				swap(&room_event[i][a_time], &room_event[i][b_time]);
				event_timeslot[b_event][1] = a_time;
				*student_hp += hp;
				room_timeslot[i][a_time] += 1;
				room_timeslot[i][b_time] -= 1;
				InRoomKswapRoop(&i, &b_time, event_timeslot, room_timeslot, student_timeslot, event_student, room_event, student_hp);
			}
		}
	}
}
void RoomTimeslotKswap(int **event_timeslot, int **room_timeslot, int **student_timeslot, int *room, int *event, int *student, int **event_student, int **student_event, int *student_hp, int **roomfeature_event, int **room_event){
	int i;
	int a_time, b_time;
	int a_event, b_event;
	int hp;

	for(i = 0; i < *room; i++){
		for(b_time = 1; b_time < 45; b_time++){
			a_time = b_time - 1;
			if(room_timeslot[i][a_time] >= 1 && room_timeslot[i][b_time] >= 1){
				a_event = room_event[i][a_time];
				b_event = room_event[i][b_time];
				hp = student_hard_penalty_2opt(student_timeslot, event_student, &a_time, &b_time, &a_event, &b_event);
				if(hp > 0){
					student_hard_penalty_2opt_difference(student_timeslot, event_student, &a_event, &b_event, &b_time, &a_time);
				}else{
					swap(&event_timeslot[a_event][1], &event_timeslot[b_event][1]);
					swap(&room_event[i][a_time], &room_event[i][b_time]);
					*student_hp += hp;
					InRoomKswapRoop(&i, &b_time, event_timeslot, room_timeslot, student_timeslot, event_student, room_event, student_hp);
				}
			}else if(room_timeslot[i][a_time] >= 1 && room_timeslot[i][b_time] == 0){
				a_event = room_event[i][a_time];
				hp = student_hard_penalty_2optzero(student_timeslot, event_student, &a_time, &b_time, &a_event);
				if(hp > 0){
					student_hard_penalty_2optzero_difference(student_timeslot, event_student, &a_event, &a_time, &b_time, 1);
				}else{
					swap(&room_event[i][a_time], &room_event[i][b_time]);
					event_timeslot[a_event][1] = b_time;
					*student_hp += hp;
					room_timeslot[i][a_time] -= 1;
					room_timeslot[i][b_time] += 1;
					InRoomKswapRoop(&i, &b_time, event_timeslot, room_timeslot, student_timeslot, event_student, room_event, student_hp);
				}
			}else if(room_timeslot[i][a_time] == 0 && room_timeslot[i][b_time] >= 1){
				b_event = room_event[i][b_time];
				hp = student_hard_penalty_2optzero(student_timeslot, event_student, &b_time, &a_time, &b_event);
				if(hp > 0){
					student_hard_penalty_2optzero_difference(student_timeslot, event_student, &b_event, &b_time, &a_time, 1);
				}else{
					swap(&room_event[i][a_time], &room_event[i][b_time]);
					event_timeslot[b_event][1] = a_time;
					*student_hp += hp;
					room_timeslot[i][a_time] += 1;
					room_timeslot[i][b_time] -= 1;
					InRoomKswapRoop(&i, &b_time, event_timeslot, room_timeslot, student_timeslot, event_student, room_event, student_hp);
				}
			}
		}
	}
}
