/* 
 * File:   Kswap.h
 * Author: nskun
 *
 * Created on 2015/04/06, 16:03
 */

#ifndef KSWAP_H
#define	KSWAP_H

#ifdef	__cplusplus
extern "C" {
#endif
    
void RoomTimeslotKswap(int **event_timeslot, int **room_timeslot, int **student_timeslot, int *room, int *event, int *student, int **event_student, int **student_event, int *student_hp, int **roomfeature_event, int **room_event);

#ifdef	__cplusplus
}
#endif

#endif	/* KSWAP_H */

