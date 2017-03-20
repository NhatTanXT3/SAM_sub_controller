/*
 * SAM.h
 *
 *  Created on: Feb 27, 2017
 *      Author: Nhat Tan
 */

#ifndef SAM_H_
#define SAM_H_

#define SAM_WRITE_POSITION_ 0
typedef struct {
	unsigned char id;
	unsigned char SAMmode;
	unsigned char mode;
	unsigned char position8;
	unsigned int position12;
} SAM;

extern SAM sam1;


#endif /* SAM_H_ */
