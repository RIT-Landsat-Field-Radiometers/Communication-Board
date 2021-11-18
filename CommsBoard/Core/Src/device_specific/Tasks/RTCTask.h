/*
 * RTCTask.h
 *
 *  Created on: Nov 7, 2021
 *      Author: reedt
 */

#ifndef SRC_DEVICE_SPECIFIC_TASKS_RTCTASK_H_
#define SRC_DEVICE_SPECIFIC_TASKS_RTCTASK_H_

#include "TaskInterface.h"
#include "rtc.h"
#include <ctime>
#include <string>

class RTCTask : public TaskInterface
{
public:
	typedef struct {
		RTC_TimeTypeDef time;
		RTC_DateTypeDef date;
	} RTC_DateTime;

	enum ALARMTRIGGERED : uint32_t {
		NONE = 0x00,
		ALARMA = 0x01,
		ALARMB = 0x02,
		BOTH = 0x03
	};

	RTCTask();
	~RTCTask();

	bool start() override;
	bool kill() override;

	uint32_t getUnixTime();
	void setUnixTime(uint32_t time);

	RTC_DateTime getDateTime();

	char * getStringTime();

	void enable1HourAlarm();
	void enable24HourAlarm();
	void disableAlarms();

	static RTC_DateTime convertUnixToDateTime(uint32_t time)
	{
		RTC_DateTime retval{0};

		time_t cur = time;
		struct tm *tptr = gmtime(&cur);

		retval.time.Hours = tptr->tm_hour;
		retval.time.Minutes = tptr->tm_min;
		retval.time.Seconds = tptr->tm_sec;
		retval.time.TimeFormat = RTC_HOURFORMAT_24;
		retval.time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		retval.time.StoreOperation = RTC_STOREOPERATION_RESET;

		retval.date.Year = tptr->tm_year - 100; // Unix is since 1900, stm32 is since 2000
		retval.date.Month = tptr->tm_mon + 1;
		retval.date.Date = tptr->tm_mday;
		auto wday = tptr->tm_wday;
		if (wday == 0)
			wday = 7;
		retval.date.WeekDay = wday;

		return retval;
	}

	static uint32_t convertDateTimeToUnix(RTC_DateTime datetime)
	{
		struct tm conv{0};
		conv.tm_year = datetime.date.Year + 100;
		conv.tm_mon = datetime.date.Month - 1;
		conv.tm_mday = datetime.date.Date;
		conv.tm_hour = datetime.time.Hours;
		conv.tm_min = datetime.time.Minutes;
		conv.tm_sec = datetime.time.Seconds;

		return mktime(&conv);
	}

private:
	static RTCTask * singleton;
};

#endif /* SRC_DEVICE_SPECIFIC_TASKS_RTCTASK_H_ */
