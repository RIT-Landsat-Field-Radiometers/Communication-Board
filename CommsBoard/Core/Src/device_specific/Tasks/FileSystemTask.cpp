/*
 * FileSystemTask.cpp
 *
 *  Created on: Nov 8, 2021
 *      Author: reedt
 */

#include <Tasks/FileSystemTask.h>
#include <ctime>
#include "Tasks/MonitorTask.h"



FileSystemTask::FileSystemTask()
{
	// TODO Auto-generated constructor stub
	 log = new Logger("Files");
}

FileSystemTask::~FileSystemTask()
{
	// TODO Auto-generated destructor stub
}

bool FileSystemTask::start()
{
	taskEvents = osEventFlagsNew(nullptr);

	FRESULT fresult;  // result
	MX_FATFS_Init();
	const char *path = "/";
	fresult = f_mount(&fs, path, 1);	// mount partition

	if(fresult != FR_OK)
	{
		log->error("Could not mount SD card, code: %d", fresult);
		return false;
	}

	auto full = getFullCapacity();
	auto curr = getRemainingCapacity();

	log->info("Mounted SD card with %lld(GB) of %lld(GB) remaining (%4.2f%%)", curr >> 30, full >> 30, (curr * 100.0) / full);
	return fresult == FR_OK;
}

bool FileSystemTask::kill()
{
}

bool FileSystemTask::saveProtoBuf(HourlyData &data)
{
	FRESULT fresult;  // result
	char buffer[512];
	int size = 0;

	auto dataTime = gmtime((time_t*) &data.dataStart.time.unixTime);
	auto y = dataTime->tm_year + 1900;
	auto M = dataTime->tm_mon + 1; // For readability, 1-12
	auto d = dataTime->tm_mday;
	auto h = dataTime->tm_hour;

	size = sprintf(buffer, "/data");
	fresult = f_mkdir(buffer);
	if(!(fresult == FR_OK || fresult == FR_EXIST))
	{
		log->error("Could not create \"%s\" directory, code: %d", buffer, fresult);
		return false;
	}

	size = sprintf(buffer, "/data/%d", y);
	fresult = f_mkdir(buffer);
	if(!(fresult == FR_OK || fresult == FR_EXIST))
	{
		log->error("Could not create \"%s\" directory, code: %d", buffer, fresult);
		return false;
	}

	size = sprintf(buffer, "/data/%d/%d", y, M);
	fresult = f_mkdir(buffer);
	if(!(fresult == FR_OK || fresult == FR_EXIST))
	{
		log->error("Could not create \"%s\" directory, code: %d", buffer, fresult);
		return false;
	}

	size = sprintf(buffer, "/data/%d/%d/%d", y, M, d);
	fresult = f_mkdir(buffer);
	if(!(fresult == FR_OK || fresult == FR_EXIST))
	{
		log->error("Could not create \"%s\" directory, code: %d", buffer, fresult);
		return false;
	}

	size = sprintf(buffer, "/data/%d/%d/%d/%d_%d_%d_h%d.gz", y, M, d, y, M, d,
			h);

	FIL fp;
	fresult = f_open(&fp, buffer, FA_CREATE_ALWAYS | FA_WRITE);

	if(fresult != FR_OK)
	{
		log->error("Could not create \"%s\" file, code: %d", buffer, fresult);
		_HourlyData_SystemEvents event = HourlyData_SystemEvents_init_zero;
		event.type = HourlyData_SystemEvents_EventType_LOW_SD_CARD;
//		monitor->addEvent(event);
		return false;
	}

	CompressorWriteBuffer wb(&fp);
	pb_ostream_t stream;
	stream.state = (void*) &wb;
	decltype(stream.callback) cb =
			[](pb_ostream_t *stream, const pb_byte_t *buf,
					size_t count)
					{
						if (stream != nullptr && stream->state != nullptr)
						{
							CompressorWriteBuffer *out = (CompressorWriteBuffer*) stream->state;
							out->push(buf, count);
						}
						return true;
					};
	stream.callback = cb;
	stream.max_size = UINT32_MAX;
	stream.bytes_written = 0;
	stream.errmsg = NULL;

	auto res = pb_encode(&stream, HourlyData_fields, &data);
	wb.finish();
	fresult = f_close(&fp);
	if(!res)
	{
		log->error("Failed to encode Protobuf");
		return false;
	}
	if(fresult != FR_OK)
	{
		log->error("Failed to close file, code: %d", fresult);
		return false;
	}
	return true;
}

std::vector<std::string> FileSystemTask::listDirectory(std::string path)
{
	std::vector<std::string> retval;

	FRESULT fresult;
	DIR directory;
	char buffer[256];
	fresult = f_opendir(&directory, path.c_str());
	if (fresult == FR_OK)
	{
		FILINFO fno;
		for (fresult = f_readdir(&directory, &fno);
				fresult == FR_OK && fno.fname[0] != 0;
				fresult = f_readdir(&directory, &fno))
		{
			// Go through each file
			sprintf(buffer, "%s/%s", path.c_str(), fno.fname);
			retval.emplace_back(buffer);
		}
		f_closedir(&directory);
	}
	else
	{
		_HourlyData_SystemEvents event = HourlyData_SystemEvents_init_zero;
		event.type = HourlyData_SystemEvents_EventType_LOW_SD_CARD;
//		monitor->addEvent(event);
	}

	return retval;
}

std::function<uint8_t* (uint32_t*)> FileSystemTask::createBufferedReader(
		std::string path)
{
	static FIL fin;
	static bool opened = false;
	FRESULT fresult;  // result

	if(opened)
	{
		f_close(&fin);
		opened = false;
	}
	fresult = f_open(&fin, path.c_str(), FA_READ);

	if (fresult != FR_OK)
	{
		_HourlyData_SystemEvents event = HourlyData_SystemEvents_init_zero;
		event.type = HourlyData_SystemEvents_EventType_LOW_SD_CARD;
//		monitor->addEvent(event);

		// Can't open file, return empty functor
		return [](uint32_t *size)
		{
			*size = 0;
			return nullptr;
		};
	}
	opened = true;

	return [&fin, &opened](uint32_t *size)
	{
		static uint8_t buffer[512];
		UINT read = 0;
		FRESULT fr = f_read(&fin, buffer, 512, &read);

		if (fr != FR_OK || read < 1)
		{
			f_close(&fin);
			opened = false;
			*size = 0;
			return (uint8_t *)  nullptr;
		}
		else
		{
//			osDelay(50); // Don't like it but want to reduce modem buffer overruns.
		}

		*size = read;
		return buffer;
	};

}

uint64_t FileSystemTask::getFullCapacity()
{
	return (fs.n_fatent -2) * (uint64_t)fs.csize * _MAX_SS;
}

uint64_t FileSystemTask::getRemainingCapacity()
{
    FATFS *fs;
    DWORD fre_clust, fre_sect, tot_sect;
	const char *path = "/";

	FRESULT res = f_getfree(path, &fre_clust, &fs);

	if(res != FR_OK)
	{
		return UINT64_MAX;
	}

	uint64_t rem = fre_clust * (uint64_t)fs->csize * _MAX_SS;

	if( (rem >> 30) < 1)
	{
		_HourlyData_SystemEvents event = HourlyData_SystemEvents_init_zero;
		event.type = HourlyData_SystemEvents_EventType_LOW_SD_CARD;
//		monitor->addEvent(event);
	}

	return rem; // returns number of bytes
}

#include "JSON/Parser/tiny-json.h"

std::string FileSystemTask::getHostnameFromConfig()
{
	static const char * path = "/config/connection.json";

	std::string contents;
	auto reader = createBufferedReader(path);
	uint32_t read = 0;
	uint32_t totalRead = 0;
	uint8_t * data = reader(&read);
	while(read > 0)
	{
		contents += std::string((char *)data, read);
		totalRead+= read;
		data = reader(&read);
	}

	if(totalRead < 1)
	{
		// Didn't read anything
		log->error("Failed to read connection config!");
		contents.clear();
		return contents;
	}

	json_t pool[16];
	char * start =  (char *)contents.c_str(); // Json libary increments the pointer, so make a copy to the first for it to play with

	json_t const* parent = json_create(start, pool, 16 );
	if ( parent == nullptr )
	{
		log->error("Failed to parse connection config!");
		contents.clear();
		return contents;
	}

    json_t const* host = json_getProperty( parent, "host" );
    if(host == nullptr || JSON_TEXT != json_getType( host ))
    {
		log->error("Could not find host in connection config!");
		contents.clear();
		return contents;
    }

    return std::string(json_getValue(host));
}
