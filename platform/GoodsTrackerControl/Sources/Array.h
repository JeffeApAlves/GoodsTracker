/*
 * Array.h
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_ARRAY_H_
#define SOURCES_ARRAY_H_

#include "stdlib.h"

#define LEN_HEADER		30
#define LEN_PAYLOAD		256
#define LEN_FRAME		300
#define LEN_CHECKSUM	3

typedef struct{

	int		CountCheckSum;
	char	checksum[LEN_CHECKSUM];

	int		Length;
	char	Data[LEN_FRAME];

} ArrayFrame;


typedef struct{

	int		Length;
	char	Data[LEN_PAYLOAD];

} ArrayPayLoad;


typedef struct{

	int		Length;
	char	data[LEN_HEADER];

} ArrayHeader;


unsigned int calcChecksum(const char *buff, size_t sz);

#define clearArrayFrame(f)		memset((void*)f,0,sizeof(ArrayFrame));
#define clearArrayPayLoad(f)	memset((void*)f,0,sizeof(ArrayPayLoad));

unsigned int putDataArray(ArrayFrame *frame, char data);
void AppendPayLoad(ArrayPayLoad* payload,const char* data);
void AppendFrame(ArrayFrame* frame,const char* data);

#endif /* SOURCES_ARRAY_H_ */
