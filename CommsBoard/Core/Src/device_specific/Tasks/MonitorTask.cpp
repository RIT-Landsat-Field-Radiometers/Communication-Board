/*
 * MonitorTask.cpp
 *
 *  Created on: Nov 8, 2021
 *      Author: reedt
 */

#include <Tasks/MonitorTask.h>
#include "adc.h"
#include "dma.h"

MonitorTask *MonitorTask::singleton = nullptr;

extern uint32_t extGetUnixTime();

MonitorTask::MonitorTask()
{
	taskEvents = osEventFlagsNew(nullptr);
	taskMutex = osMutexNew(nullptr);
}

extern uint64_t allocCount;
extern uint64_t freeCount;
extern uint64_t data_cnt;
extern uint64_t save_cnt;
extern uint64_t up_cnt;

void MonitorTask::MonitorTaskBody(void *arg)
{
	if (arg == nullptr)
		osThreadExit();

	MonitorTask *_this = (MonitorTask*) arg;

	Logger mlog("MEM");
	Logger plog("PWR");

	uint32_t samples[2] = {0};
	HAL_ADC_Start_DMA(&hadc1, samples, 2);

	bool lowVoltage = false;
	bool lowMem = false;

	uint64_t lastDiff = 0;

	for (;;)
	{
		auto curFree = xPortGetFreeHeapSize();
		auto minFree = xPortGetMinimumEverFreeHeapSize();
		auto maxMem = configTOTAL_HEAP_SIZE;

		uint64_t newDiff = allocCount - freeCount;
		if(lastDiff != newDiff)
		{
//			mlog.debug(
//			"Free Memory: %d bytes (%.1f%%), Min Free Memory: %d bytes (%.1f%%)\tAllocs: %lld, Frees: %lld, diff: %lld",
//			curFree, (curFree * 100.0) / maxMem, minFree,
//			(minFree * 100.0) / maxMem, allocCount, freeCount,
//			allocCount - freeCount);
			lastDiff = newDiff;
		}

		if(curFree < 10'000 && !lowMem)
		{
			_HourlyData_SystemEvents event = HourlyData_SystemEvents_init_zero;
			event.type = HourlyData_SystemEvents_EventType_OTHER;
			event.details = (char *) pvPortMalloc(12);
			sprintf(event.details, "LOW MEMORY!");

			_this->addEvent(event);

			mlog.warn("Low memory! %d bytes", curFree);

			lowMem = true;
		}
		if(curFree > 20'000 && lowMem)
		{
			lowMem = false;
			mlog.info("Memory restored, avil: %d bytes", curFree);
		}


		// Wait for first conversion
		if(samples[0] != 0x00)
		{
			uint16_t raw = samples[0] & 0xfff;
			double voltage = (raw / (4096.0)) * 3.3 * 11.0;

			if (voltage < 11.0 && !lowVoltage)
			{
				_HourlyData_SystemEvents event = HourlyData_SystemEvents_init_zero;
				event.type = HourlyData_SystemEvents_EventType_LOW_BATTERY;
				_this->addEvent(event);

				plog.warn("Low Battery Voltage! %4.2f V", voltage);

				lowVoltage = true; // Prevent repeat events
			}
			if (voltage > 12.0 && lowVoltage)
			{
				_HourlyData_SystemEvents event = HourlyData_SystemEvents_init_zero;
				event.type = HourlyData_SystemEvents_EventType_NORMAL_BATTERY;
				_this->addEvent(event);

				plog.info("Battery Voltage restored %4.2f V", voltage);

				lowVoltage = false; // Prevent repeat events
			}
		}


		osDelay(1005);
	}
}

MonitorTask::~MonitorTask()
{
	// TODO Auto-generated destructor stub
}

bool MonitorTask::start()
{
	osThreadAttr_t Task_attributes{0};
	Task_attributes.stack_size = 2048 * 4;
	Task_attributes.name = "Monitor";
	Task_attributes.priority = (osPriority_t) osPriorityNormal;
	MonitorTaskHandle = osThreadNew(MonitorTaskBody, this, &Task_attributes);
}

bool MonitorTask::kill()
{
}

std::vector<_HourlyData_SystemEvents> MonitorTask::getEvents()
{
	osMutexAcquire(taskMutex, osWaitForever);
	std::vector<_HourlyData_SystemEvents> retval(this->events); // Copy events
	osMutexRelease(taskMutex);
	return retval;
}

void MonitorTask::addEvent(_HourlyData_SystemEvents &event)
{
	event.time.time.unixTime= extGetUnixTime();
	if(event.time.time.unixTime != 0)
	{
		event.has_time = true;
		event.time.which_time = TimeStamp_unixTime_tag;
	}
	osMutexAcquire(taskMutex, osWaitForever);
	this->events.emplace_back(event);
	osMutexRelease(taskMutex);
}

void MonitorTask::clearEvents()
{
	osMutexAcquire(taskMutex, osWaitForever);
	for(const auto &evt : events)
	{
		if(evt.details != nullptr)
		{
			vPortFree(evt.details);
		}
	}
	this->events.clear();
	std::vector<_HourlyData_SystemEvents>().swap(this->events);
	osMutexRelease(taskMutex);
}
