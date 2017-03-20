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

typedef struct {
	unsigned int s1;
	unsigned int s2;
	unsigned int s3;
	unsigned int s4;
}SAM_position12;
extern SAM_position12 samPosition12;
extern SAM sam1;
extern volatile unsigned char flagReadBusy;
extern volatile unsigned  char flagReadSuccess;


#endif /* SAM_H_ */
