/*
 * FSM.h - Contains functions prototypes necessary to implement the FSM needed for Assignment-6
 *
 *  Created on: Nov 8, 2021
 *      Author: Deekshith Reddy Patil, patil.deekshithreddy@colorado.edu
 */

#ifndef FSM_H_
#define FSM_H_

/*
 * Brief - This function is used to accumulate a line (i.e until is ENTER is hit)
 */
void accumulate_line(char *str);

/*
 * Brief - This function is used to perform lexical analysis on the accumulated line
 */
void process_command(char * str);

#endif /* FSM_H_ */
