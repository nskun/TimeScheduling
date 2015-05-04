/* 
 * File:   DispatchingRule.h
 * Author: nskun
 *
 * Created on 2015/04/06, 15:59
 */

#ifndef DISPATCHINGRULE_H
#define	DISPATCHINGRULE_H

#ifdef	__cplusplus
extern "C" {
#endif
void DispatchingRule(int **Se, int *Es, int **Rf, int *Fr, int **Ef, int *Fe, int *roomsize, int *event, int *room, int *feature, int *student, int **room_timeslot, int **order_fr, int **order_fe, int **order_se, int **event_timeslot, int **eventfeature_room, int **roomfeature_event, int *delay_er, int **delay_re, int *delay_r, int **student_timeslot, int *max_attendance, int **event_student, int **student_event, int *student_hp, int **room_event, int *seed);
#ifdef	__cplusplus
}
#endif

#endif	/* DISPATCHINGRULE_H */

