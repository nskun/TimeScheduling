/* 
 * File:   TwoSwap.h
 * Author: nskun
 *
 * Created on 2015/04/06, 16:05
 */

#ifndef TWOSWAP_H
#define	TWOSWAP_H

#ifdef	__cplusplus
extern "C" {
#endif
    
void RoomTimeslot2swap(int **event_timeslot, int **room_timeslot, int **student_timeslot, int *room, int *event, int *student, int **event_student, int **student_event, int *student_hp, int **roomfeature_event, int **room_event);

#ifdef	__cplusplus
}
#endif

#endif	/* TWOSWAP_H */

