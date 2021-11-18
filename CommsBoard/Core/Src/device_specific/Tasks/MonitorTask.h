/*
 * MonitorTask.h
 *
 *  Created on: Nov 8, 2021
 *      Author: reedt
 */

#ifndef SRC_DEVICE_SPECIFIC_TASKS_MONITORTASK_H_
#define SRC_DEVICE_SPECIFIC_TASKS_MONITORTASK_H_

#include "protob/hourly.pb.h"
#include <vector>
#include "Logging/Logger.h"


class MonitorTask
{
private:
	osThreadId_t MonitorTaskHandle;
	static MonitorTask * singleton;

	osMutexId_t taskMutex;
	osEventFlagsId_t taskEvents;

	std::vector<_HourlyData_SystemEvents> events;

	static void MonitorTaskBody(void * arg);

	MonitorTask();

public:

	static MonitorTask * getInstance()
	{
		static MonitorTask inst;

		if(singleton == nullptr)
		{
			singleton = &inst;
		}

		return singleton;
	}

	~MonitorTask();

	bool start();
	bool kill();

	std::vector<_HourlyData_SystemEvents> getEvents();

	void addEvent(_HourlyData_SystemEvents &event);

	void clearEvents();
};

#endif /* SRC_DEVICE_SPECIFIC_TASKS_MONITORTASK_H_ */
