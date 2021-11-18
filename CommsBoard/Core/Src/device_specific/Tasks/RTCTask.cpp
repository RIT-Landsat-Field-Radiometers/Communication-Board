/*
 * RTCTask.cpp
 *
 *  Created on: Nov 7, 2021
 *      Author: reedt
 */

#include <Tasks/RTCTask.h>

RTCTask *RTCTask::singleton = nullptr;
static RTCTask *_singleton = nullptr;


uint32_t extGetUnixTime()
{
	if(_singleton != nullptr)
	{
		return _singleton->getUnixTime();
	}
	return 0;
}

RTCTask::RTCTask()
{
	// TODO Auto-generated constructor stub
	log = new Logger("RTC");
}

RTCTask::~RTCTask()
{
	// TODO Auto-generated destructor stub
}

bool RTCTask::start()
{
	taskEvents = osEventFlagsNew(nullptr);
	static osMutexAttr_t mutex_attr = {
	  "RTCMutex",                          // human readable mutex name
	  osMutexRecursive | osMutexPrioInherit,    // attr_bits
	  NULL,                                     // memory for control block
	  0U                                        // size for control block
	};
	taskMutex = osMutexNew(&mutex_attr);
	singleton = this;
	_singleton = this;
	osEventFlagsClear(taskEvents, 0xFFFFFFFF);
	enable1HourAlarm();
	enable24HourAlarm();
	return true;
}

bool RTCTask::kill()
{
	disableAlarms();
	singleton = nullptr;
	_singleton = nullptr;
	return true;
}

uint32_t RTCTask::getUnixTime()
{
	return convertDateTimeToUnix(getDateTime());
}

void RTCTask::setUnixTime(uint32_t time)
{
	auto conv = convertUnixToDateTime(time);
	osMutexAcquire(this->taskMutex, osWaitForever);
	HAL_RTC_SetTime(&hrtc, &conv.time, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &conv.date, RTC_FORMAT_BIN);
	osMutexRelease(this->taskMutex);
	log->info("Set time to: %s", getStringTime());
}

RTCTask::RTC_DateTime RTCTask::getDateTime()
{
	RTC_DateTime retval;
	osMutexAcquire(this->taskMutex, osWaitForever);
	HAL_RTC_GetTime(&hrtc, &retval.time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &retval.date, RTC_FORMAT_BIN);
	osMutexRelease(this->taskMutex);
	return retval;
}

void RTCTask::enable1HourAlarm()
{
	osMutexAcquire(this->taskMutex, osWaitForever);
	RTC_AlarmTypeDef hAlarm =
	{ 0 };
	hAlarm.Alarm = RTC_ALARM_A;
	hAlarm.AlarmMask = RTC_ALARMMASK_HOURS | RTC_ALARMMASK_DATEWEEKDAY; // Every Hour
	hAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	hAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT_24;
	hAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;

	hAlarm.AlarmTime.Seconds = 0;
	hAlarm.AlarmTime.Minutes = 0;

	HAL_RTC_SetAlarm_IT(&hrtc, &hAlarm, RTC_FORMAT_BIN);
	HAL_RTC_RegisterCallback(&hrtc, HAL_RTC_ALARM_A_EVENT_CB_ID,
			[](RTC_HandleTypeDef *hrtc)
			{
				if(singleton != nullptr)
				{
					osEventFlagsSet(singleton->taskEvents, ALARMA);
				}
			});
	osMutexRelease(this->taskMutex);
}

void RTCTask::enable24HourAlarm()
{
	osMutexAcquire(this->taskMutex, osWaitForever);
	RTC_AlarmTypeDef dAlarm =
	{ 0 };
	dAlarm.Alarm = RTC_ALARM_B;
	dAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY; // Every Day
	dAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	dAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT_24;
	dAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	dAlarm.AlarmTime.Seconds = 30; // Wait 30 seconds from after the last scheduled file to make sure its there before uploading
	dAlarm.AlarmTime.Minutes = 0;
	dAlarm.AlarmTime.Hours = 0;

	HAL_RTC_SetAlarm_IT(&hrtc, &dAlarm, RTC_FORMAT_BIN);
	HAL_RTC_RegisterCallback(&hrtc, HAL_RTC_ALARM_B_EVENT_CB_ID,
			[](RTC_HandleTypeDef *hrtc)
			{
				if(singleton != nullptr)
				{
					osEventFlagsSet(singleton->taskEvents, ALARMB);
				}
			});
	osMutexRelease(this->taskMutex);
}

void RTCTask::disableAlarms()
{
	osMutexAcquire(this->taskMutex, osWaitForever);
	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);
	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_B);
	osMutexRelease(this->taskMutex);
}

char * RTCTask::getStringTime()
{
	  static const char wday_name[][4] = {
	    "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
	  };
	  static const char mon_name[][4] = {
	    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
	    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	  };
	auto curtime = getDateTime();
	static char retval[25];

	sprintf(retval, "%.3s %.3s%3d %.2d:%.2d:%.2d %d",
			   wday_name[curtime.date.WeekDay - 1],
			    mon_name[curtime.date.Month - 1],
			    curtime.date.Date, curtime.time.Hours,
			    curtime.time.Minutes, curtime.time.Seconds,
			    2000 + curtime.date.Year);

	return retval;
}
