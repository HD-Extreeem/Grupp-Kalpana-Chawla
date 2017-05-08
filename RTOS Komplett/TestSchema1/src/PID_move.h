/*
 * PID_move.h
 *
 * Created: 2017-04-26 14:38:22
 *  Author: Yurdaer Dalkic && Hadi Deknache
 */


#ifndef PID_MOVE_H_
#define PID_MOVE_H_

uint16_t counter_1; //Encoder counter for rightWheel
uint16_t counter_2; //Encoder counter for leftWheel
bool c1Loop; //Boolean for checking if encoder counter 1 triggered interrupt
bool c2Loop; //Boolean for checking if encoder counter 2 triggered interrupt

void moveTo (int distance, int direction);
void rotation (int degree, int rotationSpeed);


#endif /* PID_MOVE_H_ */
