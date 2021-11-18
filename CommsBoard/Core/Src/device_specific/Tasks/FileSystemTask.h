/*
 * FileSystemTask.h
 *
 *  Created on: Nov 8, 2021
 *      Author: reedt
 */

#ifndef SRC_DEVICE_SPECIFIC_TASKS_FILESYSTEMTASK_H_
#define SRC_DEVICE_SPECIFIC_TASKS_FILESYSTEMTASK_H_

#include "TaskInterface.h"
#include "zlib/zlib.h"
#include "fatfs.h"
#include "sdmmc.h"
#include "protob/hourly.pb.h"
#include "protob/pb_encode.h"
#include "protob/CompressorWriteBuffer.h"
#include <vector>
#include <string>
#include <functional>


class FileSystemTask: public TaskInterface
{
private:
	FATFS fs;  // file system


public:
	FileSystemTask();
	~FileSystemTask();

	bool start() override;
	bool kill() override;

	bool saveProtoBuf(HourlyData &data);

	std::vector<std::string> listDirectory(std::string path);

	std::function<uint8_t*(uint32_t *)> createBufferedReader(std::string path);

	uint64_t getFullCapacity();

	uint64_t getRemainingCapacity();
};

#endif /* SRC_DEVICE_SPECIFIC_TASKS_FILESYSTEMTASK_H_ */
