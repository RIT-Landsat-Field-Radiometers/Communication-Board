/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "can.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "adc.h"
#include "rtc.h"

#include <string.h>
#include <cstdint>
#include <stdio.h>
#include <vector>
#include <ctime>

#include "bsp/DS28CM00ID/DS28CM00ID.h"
#include "bsp/LEDs/LEDManager.h"
#include "bsp/UART/UARTManager.h"

#include "Logging/Logger.h"
#include "protob/hourly.pb.h"

#include "Tasks/RTCTask.h"
#include "Tasks/CANOpenTask.h"
#include "Tasks/CellularTask.h"
#include "Tasks/DataTask.h"
#include "Tasks/FileSystemTask.h"
#include "Tasks/TaskInterface.h"
#include "Logging/UARTLogHandler.h"
//#include "Tasks/MonitorTask.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern DS28CM00_ID id1;
extern LEDManager leds;
extern UARTManager uartMan;
extern UARTLogHandler *handler;
extern Logger Log;

//MonitorTask * monitor(MonitorTask::getInstance());

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
osThreadAttr_t defaultTask_attributes{0};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartMainTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
//void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
//void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 4 */
extern "C" void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
	Log.error("stack overflow!!!!");
	osDelay(100);
	HAL_NVIC_SystemReset();
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
extern "C" void vApplicationMallocFailedHook(void)
{
	Log.error("malloc failed!!!!");
	osDelay(100);
	HAL_NVIC_SystemReset();
}

/* USER CODE END 5 */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
	/* USER CODE BEGIN Init */

	defaultTask_attributes.name = "MainTask";
	defaultTask_attributes.priority = (osPriority_t) osPriorityNormal;
	defaultTask_attributes.stack_size = 2048 * 4;

	/* USER CODE END Init */

	/* Create the thread(s) */
	/* creation of defaultTask */
	defaultTaskHandle = osThreadNew(StartMainTask, NULL,
			&defaultTask_attributes);

	/* END RTOS_THREADS */
	uartMan.start();
	leds.start(false);
//	Log.info("Application Started");
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */

uint64_t data_cnt = 0;
uint64_t save_cnt = 0;
uint64_t up_cnt = 0;
char recbuf[2] = {0};

void StartMainTask(void *argument)
{
	/* USER CODE BEGIN StartDefaultTask */

	Logger privLog("Main");
	privLog.info("Initializing Processes....");

	handler->setLevel(LogLevel::INFO_LEVEL);

	HAL_UART_RegisterCallback(&huart1, HAL_UART_RX_COMPLETE_CB_ID,
			[](UART_HandleTypeDef *huart)
			{
				if(huart->Instance == USART1)
				{
					switch(recbuf[0])
					{
						case 'a':
						case 'A':
						handler->setLevel(LogLevel::ALL_LEVEL);
						break;

						case 'd':
						case 'D':
						handler->setLevel(LogLevel::DEBUG_LEVEL);
						break;

						case 'i':
						case 'I':
						handler->setLevel(LogLevel::INFO_LEVEL);
						break;

						case 't':
						case 'T':
						handler->setLevel(LogLevel::TRACE_LEVEL);
						break;

						case 'w':
						case 'W':
						handler->setLevel(LogLevel::WARN_LEVEL);
						break;

						case 'e':
						case 'E':
						handler->setLevel(LogLevel::ERROR_LEVEL);
						break;

						case 'p':
						case 'P':
						handler->setLevel(LogLevel::PANIC_LEVEL);
						break;

						case 0:
						default:
						recbuf[0] = 0x00; // nop char
						break;
					}
				HAL_UART_Receive_IT(huart, (uint8_t *)recbuf, 1);
			}
		});

	HAL_UART_Receive_IT(&huart1, (uint8_t*) recbuf, 1);

	privLog.debug("Starting Monitor...");
	MonitorTask *mtask = MonitorTask::getInstance();
	mtask->start();
	/*
	 * START OF CELLULAR START UP
	 */
	privLog.debug("Starting Cellular...");
	CellularTask cellular;
	cellular.start();
	cellular.connect();
	auto unixStartTime = cellular.getServerTime();
	privLog.info("Raw epoch time: %" HEX_WORD, unixStartTime);
//	auto unixStartTime = 1635983941;
	/*
	 * END OF CELLULAR START UP
	 */
	/*
	 * START OF RTC START UP
	 */
	privLog.debug("Starting RTC...");
	RTCTask rtc;
	rtc.start();
	rtc.setUnixTime(unixStartTime);
	cellular.disconnect(); // Turn off cellular AFTER setting the clock since it takes a while
	/*
	 * END OF RTC START UP
	 */
	/*
	 * START OF FILESYSTEM START UP
	 */
	privLog.debug("Starting FileSystem...");
	FileSystemTask filesystem;
	if(!filesystem.start())
	{
		for (;;)
			osDelay(1000);
	}
	/*
	 * END OF FILESYSTEM START UP
	 */
	/*
	 * START OF CANOpen START UP
	 */
	privLog.debug("Starting CAN...");
	CANOpenTask cantask(id1.getID());
	cantask.start();
	cantask.startLSSScan();
	cantask.waitLSSComplete(); // this will block until the LSS scan is done
	auto sensors = cantask.getSensorAddress();
	auto bmes = cantask.getBMEAddress();
	int idx = 1;
	for(auto const & sen : sensors)
	{
		privLog.debug("Sensor[%d]- Vendor ID{%" HEX_WORD "}, Product Code{%" HEX_WORD "}, Revision Number{%" HEX_WORD "}, Serial Number{%" HEX_WORD "}",
				idx++, sen.identity.vendorID, sen.identity.productCode, sen.identity.revisionNumber, sen.identity.serialNumber);
	}
	for(auto const & bme : bmes)
	{
		privLog.debug("   BME[%d]- Vendor ID{%" HEX_WORD "}, Product Code{%" HEX_WORD "}, Revision Number{%" HEX_WORD "}, Serial Number{%" HEX_WORD "}",
				idx++, bme.identity.vendorID, bme.identity.productCode, bme.identity.revisionNumber, bme.identity.serialNumber);
	}

	cantask.startAllDevices();
	auto ok = cantask.startPDOS(); // Now that IDs are assigned, allow collection of data to the Object Dictionary
	if (!ok)
	{
		for (;;)
			osDelay(1000);
	}
	cantask.resumeSYNC(); // The board will now send SYNC messages, that trigger data collection;
	osDelay(1000); // wait a second so that there is valid data in the object dictionary before collecting it
	/*
	 * END OF CANOpen START UP
	 */
	/*
	 * START OF DATA START UP
	 */
	privLog.debug("Starting Data collection...");
	DataTask datatask;
	datatask.start();
	datatask.generateProtoBuf(rtc.getUnixTime()); // "Switch" databanks now, just so the timestamp is set
	datatask.finishedSaving(); 				// Let the current bank reserve size
	/*
	 * END OF DATA START UP
	 */

	privLog.info("Initialization Complete, Now Running");
	/* Infinite loop */
	for (;;)
	{
		int32_t flags = rtc.waitFlags(RTCTask::ALARMA | RTCTask::ALARMB, 10);

		if (flags > 0)
		{
			if (flags & RTCTask::ALARMA)
			{
				// Hourly alarm active
				privLog.info("Hourly Alarm triggered at: %s",
						rtc.getStringTime());
				auto data = datatask.generateProtoBuf(rtc.getUnixTime());
				data.commsSerial = cantask.getOwnAddress().identity.serialNumber;
				data.fwVersion = cantask.getOwnAddress().identity.revisionNumber;
				if (bmes.size() > 0 && data.has_bmeBoard)
				{
					data.bmeBoard.serialNumber = bmes[0].identity.serialNumber;
					data.bmeBoard.fwVersion = bmes[0].identity.revisionNumber;
				}
				int sensorIDX = 0;
				for (const auto &fp : sensors)
				{
					if (sensorIDX < data.sensors_count)
					{
						data.sensors[sensorIDX++].serialNumber = fp.identity.serialNumber;
						data.sensors[sensorIDX++].fwVersion = fp.identity.revisionNumber;
					}
				}

				auto events = mtask->getEvents(); // Get system events from last hour
				data.events = events.data();
				data.events_count = events.size();

				auto res = filesystem.saveProtoBuf(data);
				if (!res)
				{
					privLog.error("Failed to save data!!");
				}
				datatask.finishedSaving();
				mtask->clearEvents();
			}
			if (flags & RTCTask::ALARMB)
			{
				// Daily alarm active
				privLog.info("Daily Alarm triggered");
				cellular.connect(); // Doing many operations, so keep it on until finished
				auto curTime = rtc.getDateTime();
				char dirpath[256];
				sprintf(dirpath, "/data/%d/%d/%d", curTime.date.Year + 2000,
						curTime.date.Month, curTime.date.Date - 1); // Look at previous day
				auto files = filesystem.listDirectory(dirpath);
				for (const auto &fp : files)
				{
					privLog.info("Uploading file: %s", fp.c_str());
					auto reader = filesystem.createBufferedReader(fp);
					auto res = cellular.uploadFile(cantask.getOwnAddress().identity.serialNumber, reader);
					if(res)
						privLog.info("OK");
					else
						privLog.error("NOT OK");
				}

				unixStartTime = cellular.getServerTime(); // get time from server to correct drift
				rtc.setUnixTime(unixStartTime);

				cellular.disconnect(); // turn it off to save power
			}
		}

	}
	/* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
