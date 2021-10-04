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
#include <string.h>
#include <cstdint>
#include <stdio.h>
#include "adc.h"

#include "bsp/DS28CM00ID/DS28CM00ID.h"
#include "bsp/LEDs/LEDManager.h"
#include "bsp/UART/UARTManager.h"
#include "Logging/Logger.h"
#include "CANOpenNode/OD.h"
#include "301/CO_ODinterface.h"

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
extern Logger Log;



/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
osThreadAttr_t defaultTask_attributes;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

extern void canopen_start(void);
extern void startLSS(void *argument);



/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
void vApplicationMallocFailedHook(void)
{
   /* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created. It is also called by various parts of the
   demo application. If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
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

	defaultTask_attributes.name = "defaultTask";
	defaultTask_attributes.priority = (osPriority_t) osPriorityNormal;
	defaultTask_attributes.stack_size = 2048 * 4;

	/* USER CODE END Init */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of defaultTask */
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL,
			&defaultTask_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	osThreadNew(startLSS, nullptr, nullptr);

	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */

	uartMan.start();
	leds.start(false);
	Log.info("Application Started");
	canopen_start();
	/* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
typedef struct
{
	float t1channels[4];
	float t2channels[4];
}SensorReadings_t;

typedef struct
{
	float humidity;
	float pressure;
	float wind_direction;
	float wind_speed;
	int8_t rain_detected;
}EnviromentalReading_t;

void readSensor(SensorReadings_t * out, int which)
{
	OD_entry_t * sensorEntry;
	if(which == 1)
	{
		sensorEntry = OD_ENTRY_H6000_sensor1Data;
	}
	else
	{
		sensorEntry = OD_ENTRY_H6001_sensor2Data;
	}

	for(int channel = 0; channel < 8; channel++)
	{
		if(channel < 4)
		{
			OD_get_f32(sensorEntry, channel+1, &out->t1channels[channel], true);
		}
		else
		{
			OD_get_f32(sensorEntry, channel+1, &out->t2channels[channel-4], true);
		}
	}
}

void readEnviromental(EnviromentalReading_t * out)
{
	OD_entry_t * BMEEntry = OD_ENTRY_H6003_BMEData;
	OD_get_f32(BMEEntry, 1, &out->humidity, true);
	OD_get_f32(BMEEntry, 2, &out->pressure, true);
	OD_get_f32(BMEEntry, 4, &out->wind_direction, true);
	OD_get_f32(BMEEntry, 3, &out->wind_speed, true);
	OD_get_i8(BMEEntry, 5, &out->rain_detected, true);
}


void StartDefaultTask(void *argument)
{
	/* USER CODE BEGIN StartDefaultTask */
	Logger privLog("SystemMonitor");
	auto family = id1.getFamily();
	auto id = id1.getID();
	auto dev_mode = id1.getMode();
	privLog.info("Family=%hhx, id=%llx, mode=%hhx", family, id, dev_mode);

	uint32_t samples[2];

	HAL_ADC_Start_DMA(&hadc1, samples, 2);


//	leds.slowFlash(WHITE);

	SensorReadings_t sensors[2];
	EnviromentalReading_t enviromental;




	/* Infinite loop */
	for (;;)
	{
		osDelay(1000);
		// get the data from the CANOpen object dictionary
		readSensor(&sensors[0], 1);
		readSensor(&sensors[1], 2);
		readEnviromental(&enviromental);

		//	@formatter:off
		privLog.info("\r\n"
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
				sensors[0].t1channels[0], sensors[0].t1channels[1], sensors[0].t1channels[2], sensors[0].t1channels[3],
				sensors[0].t2channels[0], sensors[0].t2channels[1], sensors[0].t2channels[2], sensors[0].t2channels[3],

				sensors[1].t1channels[0], sensors[1].t1channels[1], sensors[1].t1channels[2], sensors[1].t1channels[3],
				sensors[1].t2channels[0], sensors[1].t2channels[1], sensors[1].t2channels[2], sensors[1].t2channels[3],

				enviromental.humidity, enviromental.pressure, enviromental.wind_speed, enviromental.wind_direction, enviromental.rain_detected != 0 ? "Yes" : "No");
		//	@formatter:on

		uint16_t raw = samples[0] & 0xfff;
		double voltage = (raw / (4096.0)) * 3.3 * 11.0;//38.4115; // TODO: Why is this off?????


		privLog.info("Battery voltage: %4.4f", voltage);

		size_t freeHeap = xPortGetFreeHeapSize();
		if (freeHeap < 2000)
		{
			privLog.error("LOW HEAP, %u bytes remaining", freeHeap);
		}
		if (voltage < 11.8)
		{
			privLog.warn("LOW BATTERY, %4.4f", voltage);
		}
	}
	/* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
