/*
 * CellularTask.h
 *
 *  Created on: Nov 8, 2021
 *      Author: reedt
 */

#ifndef SRC_DEVICE_SPECIFIC_TASKS_CELLULARTASK_H_
#define SRC_DEVICE_SPECIFIC_TASKS_CELLULARTASK_H_
#include "TaskInterface.h"
#include <functional>

class CellularTask: public TaskInterface
{
private:
	bool connected = false;
	std::string host;

	int32_t sockfd;

	osTimerId_t watchdogTimer;

public:
	CellularTask(std::string hostname);
	~CellularTask();

	bool start() override;
	bool kill() override;

	bool connect();
	bool disconnect();

	uint32_t getServerTime();

	bool uploadFile(uint32_t commsID, std::function<uint8_t*(uint32_t *)> bufferedReader);

	void printNetworkInfo();
};

#endif /* SRC_DEVICE_SPECIFIC_TASKS_CELLULARTASK_H_ */
