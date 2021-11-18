/*
 * CANOpenTask.h
 *
 *  Created on: Nov 5, 2021
 *      Author: reedt
 */

#ifndef SRC_DEVICE_SPECIFIC_TASKS_CANOPENTASK_H_
#define SRC_DEVICE_SPECIFIC_TASKS_CANOPENTASK_H_

#include "TaskInterface.h"
#include <set>
#include <map>
#include <vector>
#include "can.h"
#include "tim.h"
#include "crc.h"
#include "CANopenNode/CANopen.h"


class CANOpenTask: public TaskInterface
{
private:
	std::map<uint8_t, CO_LSS_address_t> usedIDs;
	std::set<uint8_t> sensorIDs;
	std::set<uint8_t> bmeIDs;
	uint64_t fullSerial;
	bool syncActive = false;
	bool produceHB = false;
	CO_t *canOpenHandle;

	static CANOpenTask * singleton;

	void processTick(void);

	void CANOpenInit(void);

	osThreadId_t LSSTaskHandle;

	static void LSSTask(void * arg);

public:
	CANOpenTask(uint64_t fullSerial);
	~CANOpenTask();

	bool start() override;
	bool kill() override;

	void startLSSScan();
	void waitLSSComplete();

	void startAllDevices();
	void pauseAllDevices();
	void resetAllDevices();
	bool startPDOS();
	void resumeSYNC();
	void pauseSYNC();

	CO_LSS_address_t getOwnAddress();
	std::vector<CO_LSS_address_t> getSensorAddress();
	std::vector<CO_LSS_address_t> getBMEAddress();

	enum EVENTS : uint32_t
	{
		START_SCAN = 1 << 0,
		SCAN_DONE = 1 << 1
	};

	enum PRODUCT_CODES : uint32_t
	{
		COMMUNICATION = 1,
		SENSOR = 2,
		ENVIRONMENTAL = 3
	};

};

#endif /* SRC_DEVICE_SPECIFIC_TASKS_CANOPENTASK_H_ */
