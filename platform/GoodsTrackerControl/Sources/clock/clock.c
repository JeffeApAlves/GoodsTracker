/*
 * clock.c
 *
 *  Created on: Aug 28, 2017
 *      Author: Jefferson
 */
#include "Cpu.h"
#include "stdlib.h"
#include "string.h"
#include "XF1.h"
#include "Telemetria.h"
#include "clock.h"

// Referencias:	https://github.com/msolters/make-unix-timestamp-c
//				https://community.nxp.com/docs/DOC-94734

const static char *DayOfWeekName[] = {
  "Dom","Seg","Ter","Qua","Qui","Sex","Sab"
};

const static int days_per_month[2][MOS_PER_YEAR] = {
  { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
  { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

const static int days_per_year[2] = {
  365, 366
};

/* User includes (#include below this line is not maintained by Processor Expert) */
LDD_TDeviceData		*MyRTCPtr;
LDD_RTC_TTime		Time;


STATUS_CLOCK statuc_clock = CLOCK_INIT;

void initClock(){

	MyRTCPtr = RTC1_Init((LDD_TUserData *)NULL, FALSE);        /* Initialize the device, preserve time settings */

	LDD_RTC_TTime date_time;

	date_time.Hour		= 0;
	date_time.Minute	= 0;
	date_time.Second	= 0;
	date_time.Year		= 1970;
	date_time.Month		= 1;
	date_time.Day		= 1;

	setClock(&date_time);

	statuc_clock = CLOCK_STARTED;
}
//-------------------------------------------------------------------------------------------------------------------

/*
 *
 * Configura novo horario
 *
 *
 */
bool setClockByString(char* date,char* time){

	LDD_RTC_TTime date_time;

	strToData(&date_time,date,time);

	return setClock(&date_time);
}
//-------------------------------------------------------------------------------------------------------------------

bool setClock(LDD_RTC_TTime* time){

	LDD_TError	Error;

	bool flag = FALSE;

	if(time!=NULL && time->Year>=2017){

		Time	= *time;
		Error	= RTC1_SetTime(MyRTCPtr, &Time);

		if(Error==ERR_OK){

			getClock(&Time);

			flag = TRUE;
		}
	}

	return flag;
}
//-------------------------------------------------------------------------------------------------------------------

void strToData(	LDD_RTC_TTime* date_time,char* date,char* time){

	char year[5];	year[4]		= '\0';
	char month[3];	month[2]	= '\0';
	char day[3];	day[2]		= '\0';
	char hrs[3];	hrs[2] 		= '\0';
	char min[3];	min[2] 		= '\0';
	char sec[3];	sec[2] 		= '\0';

	strncpy(hrs, 	time,2);
	strncpy(min, 	time+2,2);
	strncpy(sec, 	time+4,2);
	strncpy(day, 	date,2);
	strncpy(month,	date+2,2);
	strncpy(year,	"20",2);	// ano 4 digitos
	strncpy(year+2,	date+4,2);

	date_time->Hour		= atoi(hrs);
	date_time->Minute	= atoi(min);
	date_time->Second	= atoi(sec);
	date_time->Year		= atoi(year);
	date_time->Month	= atoi(month);
	date_time->Day		= atoi(day);
}
//-------------------------------------------------------------------------------------------------------------------

void getClock(LDD_RTC_TTime* time){

	RTC1_GetTime(MyRTCPtr, time);
	Time.timestamp = getCurrentTimeStamp();
}
//-------------------------------------------------------------------------------------------------------------------

void updateEntityClock(){

	if(statuc_clock == CLOCK_ADJUSTED){

		RTC1_GetTime(MyRTCPtr, &Time);
		Time.timestamp++;
	}
}
//-------------------------------------------------------------------------------------------------------------------

void adjusteClock(){

	if(statuc_clock == CLOCK_STARTED){

		if(setClockByString(telemetria.GPS.Date,telemetria.GPS.Time)){

			statuc_clock = CLOCK_ADJUSTED;
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------

uint32_t getCurrentTimeStamp(){

	return unix_time_in_seconds((uint8_t)Time.Second, (uint8_t)Time.Minute, (uint8_t)Time.Hour,
			(uint8_t)Time.Day, (uint8_t)Time.Month, (uint16_t)Time.Year);
}
//-------------------------------------------------------------------------------------------------------------------

uint32_t unix_time_in_seconds(uint8_t sec, uint8_t min, uint8_t hrs,
		uint8_t day, uint8_t mon, uint16_t year) {

	uint32_t ts = 0;

	//  Add up the seconds from all prev years, up until this year.
	uint8_t years = 0;
	uint8_t leap_years = 0;
	for (uint16_t y_k = EPOCH_YEAR; y_k < year; y_k++) {
		if (IS_LEAP_YEAR(y_k)) {
			leap_years++;
		} else {
			years++;
		}
	}
	ts += ((years * days_per_year[0]) + (leap_years * days_per_year[1]))
			* SEC_PER_DAY;

	//  Add up the seconds from all prev days this year, up until today.
	uint8_t year_index = (IS_LEAP_YEAR(year)) ? 1 : 0;
	for (uint8_t mo_k = 0; mo_k < (mon - 1); mo_k++) { //  days from previous months this year
		ts += days_per_month[year_index][mo_k] * SEC_PER_DAY;
	}
	ts += (day - 1) * SEC_PER_DAY; // days from this month

	//  Calculate seconds elapsed just today.
	ts += hrs * SEC_PER_HOUR;
	ts += min * SEC_PER_MIN;
	ts += sec;

	return ts;
}
//---------------------------------------------------------------------------------------------------------
