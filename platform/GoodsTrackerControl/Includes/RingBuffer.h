/*
 * CircularBuffer.h
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_RINGBUFFER_H_
#define SOURCES_RINGBUFFER_H_

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE		256

typedef struct{

	unsigned char	data[BUFFER_SIZE];
	short	index_producer;
	short	index_consumer;
	short	count;
	short	max_count;
} RingBuffer;

bool getData(RingBuffer* buffer,char* ch);
bool putData(RingBuffer* buffer,char ch);
short getCount(RingBuffer* buffer);
void putString(RingBuffer* buffer,char* str);

#define clearBuffer(buf)	memset((void*)buf,0,sizeof(RingBuffer))
#define hasData(buf)		(getCount(buf)>0)
#define isFull(buf)			(getCount(buf)>= BUFFER_SIZE)

#endif /* SOURCES_RINGBUFFER_H_ */
