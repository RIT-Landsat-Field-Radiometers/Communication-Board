/*
 * DataTask.h
 *
 *  Created on: Nov 7, 2021
 *      Author: reedt
 */

#ifndef SRC_DEVICE_SPECIFIC_TASKS_DATATASK_H_
#define SRC_DEVICE_SPECIFIC_TASKS_DATATASK_H_

#include "TaskInterface.h"
#include "CANOpenNode/OD.h"
#include "301/CO_ODinterface.h"
#include "protob/hourly.pb.h"
#include "protob/pb_encode.h"
#include "cmsis_os.h"
#include <vector>
#include <cmath>

class DataTask : public TaskInterface
{
private:
	static void SamplingTask(void * arg);
	osThreadId_t sampleThreadID;

	std::vector<HourlyData_SensorBoard> sensors;
	std::vector<_HourlyData_readings> readings;
public:
	typedef struct
	{
		std::vector<std::vector<std::vector<float>> > channels;
		std::vector<float> hum, pres, windspd, winddir;
		std::vector<uint8_t> rain;
		uint32_t startTime;

		void initChannels(size_t sensors, size_t channels_per_sensor)
		{
			channels.reserve(sensors);
			for (uint8_t idx = 0; idx < sensors; idx++)
			{
				channels.emplace_back();
				channels[idx].reserve(channels_per_sensor);
				for (uint8_t cidx = 0; cidx < channels_per_sensor; cidx++)
				{
					channels[idx].emplace_back();
				}
			}
		}

		void reserveAll(size_t size)
		{
			for (auto &sensor : channels)
			{
				for (auto &chan : sensor)
				{
					chan.reserve(size);
				}
			}
			hum.reserve(size);
			pres.reserve(size);
			windspd.reserve(size);
			winddir.reserve(size);
			rain.reserve(size);
		}

		void clearAll()
		{
			hum.clear();
			std::vector<float>().swap(hum);
			pres.clear();
			std::vector<float>().swap(pres);
			windspd.clear();
			std::vector<float>().swap(windspd);
			winddir.clear();
			std::vector<float>().swap(winddir);
			channels.clear();
			std::vector<std::vector<std::vector<float>> >().swap(channels);
			rain.clear();
			std::vector<uint8_t>().swap(rain);
		}
	} sampleBuf;

	typedef struct
	{
		float t1channels[4];
		float t2channels[4];
	} SensorReadings_t;

	typedef struct
	{
		float humidity;
		float pressure;
		float wind_direction;
		float wind_speed;
		int8_t rain_detected;
	} EnviromentalReading_t;

	DataTask();
	~DataTask();

	bool start() override;
	bool kill() override;

	HourlyData generateProtoBuf(uint32_t unixTime);

	void finishedSaving();

	static bool readSensor(SensorReadings_t *out, int which)
	{
		bool allNAN = true;
		OD_entry_t *sensorEntry;
		if (which == 1)
		{
			sensorEntry = OD_ENTRY_H6000_sensor1Data;
		}
		else
		{
			sensorEntry = OD_ENTRY_H6001_sensor2Data;
		}

		for (int channel = 0; channel < 8; channel++)
		{
			if (channel < 4)
			{
				OD_get_f32(sensorEntry, channel + 1, &out->t1channels[channel],
						true);
				allNAN &= std::isnan(out->t1channels[channel]);
			}
			else
			{
				OD_get_f32(sensorEntry, channel + 1, &out->t2channels[channel - 4],
						true);
				allNAN &= std::isnan(out->t2channels[channel - 4]);
			}
//			OD_set_f32(sensorEntry, channel + 1, NAN, true);
		}
		return !allNAN;
	}

	static bool readEnviromental(EnviromentalReading_t *out)
	{
		bool allNAN = true;
		OD_entry_t *BMEEntry = OD_ENTRY_H6003_BMEData;

		OD_get_f32(BMEEntry, 1, &out->humidity, true);
//		OD_set_f32(BMEEntry, 1, NAN, true);
		allNAN &= std::isnan(out->humidity);

		OD_get_f32(BMEEntry, 2, &out->pressure, true);
//		OD_set_f32(BMEEntry, 2, NAN, true);
		allNAN &= std::isnan(out->pressure);

		OD_get_f32(BMEEntry, 4, &out->wind_direction, true);
//		OD_set_f32(BMEEntry, 4, NAN, true);
		allNAN &= std::isnan(out->wind_direction);

		OD_get_f32(BMEEntry, 3, &out->wind_speed, true);
//		OD_set_f32(BMEEntry, 3, NAN, true);
		allNAN &= std::isnan(out->wind_speed);

		OD_get_i8(BMEEntry, 5, &out->rain_detected, true);
//		OD_set_i8(BMEEntry, 5, 0xff, true);
		allNAN &= (out->rain_detected == 0xff);

		return !allNAN;
	}

private:

	uint8_t writeIDX = 0;
	sampleBuf banks[2];

};

#endif /* SRC_DEVICE_SPECIFIC_TASKS_DATATASK_H_ */
