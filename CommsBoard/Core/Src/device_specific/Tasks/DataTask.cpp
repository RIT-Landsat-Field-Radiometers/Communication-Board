/*
 * DataTask.cpp
 *
 *  Created on: Nov 7, 2021
 *      Author: reedt
 */

#include <Tasks/DataTask.h>
#include "Tasks/MonitorTask.h"
#include "Logging/Logger.h"


DataTask::DataTask()
{
	// TODO Auto-generated constructor stub

}

void DataTask::SamplingTask(void *arg)
{
	if (arg == nullptr)
	{
		_HourlyData_SystemEvents event = HourlyData_SystemEvents_init_zero;
		event.type = HourlyData_SystemEvents_EventType_OTHER;
//		monitor->addEvent(event);
		osThreadExit();
	}

	DataTask *_this = (DataTask *) arg;

	SensorReadings_t sensors[2];
	EnviromentalReading_t enviromental;

	_this->banks[0].initChannels(2, 8);
	_this->banks[0].reserveAll(3600);
	osEventFlagsClear(_this->taskEvents, 0xFFFFFFFF);
	uint32_t lastSample = HAL_GetTick();
	for (;;)
	{
		int32_t resp = osEventFlagsWait(_this->taskEvents, 0x01,
		osFlagsWaitAll, 10); // This will delay 10ms (most of the time)

		if ((resp > 0) && ((resp & 0x01) == 0x01))
		{
			_this->writeIDX = (_this->writeIDX + 1) % 2;
			_this->banks[_this->writeIDX].clearAll();
			_this->banks[_this->writeIDX].initChannels(2, 8);
			_this->banks[_this->writeIDX].reserveAll(16);
			osEventFlagsSet(_this->taskEvents, 0x02);
		}

		uint64_t timeSinceLastSample = (HAL_GetTick() - lastSample);

		if (timeSinceLastSample >= 1000)
		{
			sampleBuf *bank = &_this->banks[_this->writeIDX];

			resp = osEventFlagsWait(_this->taskEvents, 0x04, osFlagsWaitAll, 0);
			if ((resp > 0) && ((resp & 0x07) == 0x04))
			{
				bank->reserveAll(62 * 60);
			}

			readSensor(&sensors[0], 1);
			readSensor(&sensors[1], 2);
			readEnviromental(&enviromental);

			// Fill in the sensor data.....
			for (int sIdx = 0; sIdx < 2; sIdx++)
			{
				for (int cIdx = 0; cIdx < 8; cIdx++)
				{
					if (cIdx < 4)
					{
						bank->channels[sIdx][cIdx].emplace_back(
								sensors[sIdx].t1channels[cIdx]);
					}
					else
					{
						bank->channels[sIdx][cIdx].emplace_back(
								sensors[sIdx].t2channels[cIdx - 4]);
					}
				}
			}

			// Fill in the environmental data.....

			bank->hum.emplace_back(enviromental.humidity);
			bank->pres.emplace_back(enviromental.pressure);
			bank->windspd.emplace_back(enviromental.wind_speed);
			bank->winddir.emplace_back(enviromental.wind_direction);
			bank->rain.emplace_back(enviromental.rain_detected);
//			lastSample = HAL_GetTick() - (timeSinceLastSample - 1000); // adjust for delayed data, if it took 100ms extra, only wait 900ms for next one
			lastSample = HAL_GetTick();

			char * rainState = "ERR";
			if(enviromental.rain_detected == 0)
			{
				rainState = "NO";
			}
			if(enviromental.rain_detected == 1)
			{
				rainState = "YES";
			}

			//	@formatter:off
			_this->log->trace("\r\n"
					"Diff: %d (ms)\r\n"
					"Sensor1:\r\n"
					"\tT1CA: %8.8f\r\n"
					"\tT1CB: %8.8f\r\n"
					"\tT1CC: %8.8f\r\n"
					"\tT1CD: %8.8f\r\n"
					"\tT2CA: %8.8f\r\n"
					"\tT2CB: %8.8f\r\n"
					"\tT2CC: %8.8f\r\n"
					"\tT2CD: %8.8f\r\n"
					"Sensor2:\r\n"
					"\tT1CA: %8.8f\r\n"
					"\tT1CB: %8.8f\r\n"
					"\tT1CC: %8.8f\r\n"
					"\tT1CD: %8.8f\r\n"
					"\tT2CA: %8.8f\r\n"
					"\tT2CB: %8.8f\r\n"
					"\tT2CC: %8.8f\r\n"
					"\tT2CD: %8.8f\r\n"
					"Environmental:\r\n"
					"\tHumidity: %8.8f\r\n"
					"\tPressure: %8.8f\r\n"
					"\tWind Speed: %8.8f\r\n"
					"\tWind Direction: %8.8f\r\n"
					"\tRain: %s",
					timeSinceLastSample,
					sensors[0].t1channels[0], sensors[0].t1channels[1], sensors[0].t1channels[2], sensors[0].t1channels[3],
					sensors[0].t2channels[0], sensors[0].t2channels[1], sensors[0].t2channels[2], sensors[0].t2channels[3],

					sensors[1].t1channels[0], sensors[1].t1channels[1], sensors[1].t1channels[2], sensors[1].t1channels[3],
					sensors[1].t2channels[0], sensors[1].t2channels[1], sensors[1].t2channels[2], sensors[1].t2channels[3],

					enviromental.humidity, enviromental.pressure, enviromental.wind_speed, enviromental.wind_direction, rainState);
			//	@formatter:on
		}
	}
}

DataTask::~DataTask()
{
	// TODO Auto-generated destructor stub
}

bool DataTask::start()
{
	log = new Logger("Data");
	taskEvents = osEventFlagsNew(nullptr);

	osThreadAttr_t Task_attributes{0};
	Task_attributes.stack_size = 2048 * 4;
	Task_attributes.name = "DataTask";
	Task_attributes.priority = (osPriority_t) osPriorityNormal;
	sampleThreadID = osThreadNew(SamplingTask, this, &Task_attributes);
}

bool DataTask::kill()
{
}

HourlyData DataTask::generateProtoBuf(uint32_t unixTime)
{
	uint8_t newWriteIDX = (writeIDX + 1) % 2;
	uint8_t readIDX = writeIDX;

	banks[newWriteIDX].startTime = unixTime; // Set the timestamp on the new one to right now
	osEventFlagsSet(this->taskEvents, 0x01); // tell other task to switch banks
	osEventFlagsWait(this->taskEvents, 0x02, osFlagsWaitAny, osWaitForever); // wait for the ack


	sampleBuf *bank = &banks[readIDX];
	HourlyData data = HourlyData_init_zero;
	sensors.reserve(2);
	readings.reserve(16);
	int readingIdx = 0;
	for (int idx = 0; idx < 2; idx++)
	{
		sensors.push_back(HourlyData_SensorBoard_init_default);
		for (int idx2 = 0; idx2 < 8; idx2++)
		{
			readings.push_back(HourlyData_readings_init_default);
			readings[readingIdx].values = bank->channels[idx][idx2].data();
			readings[readingIdx++].values_count =
					bank->channels[idx][idx2].size();
		}
		sensors[idx].channels_count = 8;
	}
	sensors[0].channels = readings.data();
	sensors[1].channels = readings.data() + 8;

	data.sensors = sensors.data();
	data.sensors_count = sensors.size();

	data.bmeBoard.rain = reinterpret_cast<bool*>(bank->rain.data());
	data.bmeBoard.rain_count = bank->rain.size();

	data.bmeBoard.humidity.values = bank->hum.data();
	data.bmeBoard.humidity.values_count = bank->hum.size();
	data.bmeBoard.has_humidity = true;

	data.bmeBoard.pressure.values = bank->pres.data();
	data.bmeBoard.pressure.values_count = bank->pres.size();
	data.bmeBoard.has_pressure = true;

	data.bmeBoard.windSpeed.values = bank->windspd.data();
	data.bmeBoard.windSpeed.values_count = bank->windspd.size();
	data.bmeBoard.has_windSpeed = true;

	data.bmeBoard.windDirection.values = bank->winddir.data();
	data.bmeBoard.windDirection.values_count = bank->winddir.size();
	data.bmeBoard.has_windDirection = true;

	data.has_bmeBoard = true;

	data.dataStart.time.unixTime = bank->startTime;
	data.dataStart.which_time = TimeStamp_unixTime_tag;
	data.has_dataStart = true;

	data.duration = bank->rain.size();
	return data;
}

void DataTask::finishedSaving()
{
	banks[(writeIDX + 1) % 2].clearAll();
	osEventFlagsSet(this->taskEvents, 0x04); // let the other thread reserve max resources again
}
