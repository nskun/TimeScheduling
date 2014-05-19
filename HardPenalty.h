#ifndef HARDPENALTY_H
#define HARDPENALTY_H
int student_hard_penalty(int *student, int **student_timeslot);
void student_hard_penalty_2opt_difference(int **student_timeslot, int **event_student, int *a_event, int *b_event, int *a_time, int *b_time);
int student_hard_penalty_2opt(int **student_timeslot, int **event_student, int *a_time, int *b_time, int *a_event, int *b_event);
void student_hard_penalty_2optzero_difference(int **student_timeslot, int **event_student, int *a_event, int *a_time, int *b_time, int flag);
int student_hard_penalty_2optzero(int **student_timeslot, int **event_student, int *a_time, int *b_time, int *a_event);
#endif
