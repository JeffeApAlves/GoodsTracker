/*
 * Array.h
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_ARRAY_H_
#define SOURCES_ARRAY_H_

#include "stdlib.h"

#define ARRAY_LEN_PAYLOAD	256
#define ARRAY_LEN_FRAME		300
#define ARRAY_LEN_CHECKSUM	3

typedef struct{

	int		CountCheckSum;
	char	checksum[ARRAY_LEN_CHECKSUM];

	int		Length;
	char	Data[ARRAY_LEN_FRAME];

} ArrayFrame;

typedef struct{

	int		Length;
	char	Data[ARRAY_LEN_PAYLOAD];

} PayLoad;

unsigned int calcChecksum(const char *buff, size_t sz);

#define clearArrayFrame(f)		memset((void*)f,0,sizeof(ArrayFrame));
#define clearArrayPayLoad(f)	memset((void*)f,0,sizeof(PayLoad));

unsigned int putDataArray(ArrayFrame *frame, char data);
void AppendPayLoad(PayLoad* payload,const char* data);
void AppendFrame(ArrayFrame* frame,const char* data);

#endif /* SOURCES_ARRAY_H_ */
