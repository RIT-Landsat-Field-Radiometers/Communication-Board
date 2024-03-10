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
#include "Tasks/FileSystemTask.h"

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
	bool getMissingFiles(uint32_t commsID);
	bool uploadMissingFiles(uint32_t commsID, FileSystemTask *filesystem);
	bool fileDNE(uint32_t commsID, char *filePath);
	bool isResetNeeded(uint32_t commsID);
	bool sendBootup(uint32_t commsID);

	void printNetworkInfo();
};

#endif /* SRC_DEVICE_SPECIFIC_TASKS_CELLULARTASK_H_ */
