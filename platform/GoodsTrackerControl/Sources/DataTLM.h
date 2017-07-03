/*
 * Data.h
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_DATATLM_H_
#define SOURCES_DATATLM_H_

#include <stdint.h>
#include <string.h>

#define	TIMEOUT_TLM		10

typedef struct{

		float		Lat;
		float		Lng;
		uint16_t	Acc[3];
		uint16_t	Inc[3];
		int			Speed ;
		int			Level;
		int			Lock;
		char		Time[11];
		char		Date[7];
	} DataTLM;


void updateDataLevel(void);
void updateDataAcce(void);
void updateDataGPS(void);
void updateTLM(void);

#define clearDataTLM(f) memset((void*)f,0,sizeof(DataTLM));

extern DataTLM			dataTLM;
extern int				_lock;



#endif /* SOURCES_DATATLM_H_ */
