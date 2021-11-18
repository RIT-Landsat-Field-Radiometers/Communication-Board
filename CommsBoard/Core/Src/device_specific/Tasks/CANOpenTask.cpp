/*
 * CANOpenTask.cpp
 *
 *  Created on: Nov 5, 2021
 *      Author: reedt
 */

#include <Tasks/CANOpenTask.h>
#include "CANopenNode/301/CO_ODinterface.h"
#include "CANOpenNode/OD.h"
#include <functional>

#include "Tasks/MonitorTask.h"

#define NMT_CONTROL \
            CO_NMT_ERR_ON_ERR_REG \
          | CO_ERR_REG_GENERIC_ERR \
          | CO_ERR_REG_COMMUNICATION
#define FIRST_HB_TIME 500
#define SDO_SRV_TIMEOUT_TIME 1000
#define SDO_CLI_TIMEOUT_TIME 500
#define SDO_CLI_BLOCK false
#define OD_STATUS_BITS NULL

#define COMMSPC 1
#define SENSORSPC 2
#define BMEPC 3

CANOpenTask *CANOpenTask::singleton = nullptr;

CANOpenTask::CANOpenTask(uint64_t fullSerial) :
		fullSerial(fullSerial)
{
	log = new Logger("CAN");
}

CANOpenTask::~CANOpenTask()
{
	// TODO Auto-generated destructor stub
}

bool CANOpenTask::start()
{
	singleton = this;
	taskEvents = osEventFlagsNew(nullptr);
	CANOpenInit();
	CO_CANsetNormalMode(canOpenHandle->CANmodule);

	HAL_TIM_RegisterCallback(&htim15, HAL_TIM_PERIOD_ELAPSED_CB_ID,
			[](TIM_HandleTypeDef *htim)
			{
				if (htim->Instance == TIM15)
				{
					if(singleton != nullptr)
					{
						singleton->processTick();
					}
				}
			});

	HAL_TIM_Base_Start_IT(&htim15);
	HAL_TIM_Base_Start_IT(&htim16);
	__HAL_TIM_SET_COUNTER(&htim16, 0);

	osThreadAttr_t Task_attributes{0};
	Task_attributes.stack_size = 2048 * 4;
	Task_attributes.name = "CO_LSS";
	Task_attributes.priority = (osPriority_t) osPriorityNormal;
	LSSTaskHandle = osThreadNew(LSSTask, this, &Task_attributes);
	log->info("CANOpen started");
}

bool CANOpenTask::kill()
{
}

void CANOpenTask::startLSSScan()
{
	osEventFlagsSet(taskEvents, EVENTS::START_SCAN);
	log->info("LSS Fastscan started");
}

void CANOpenTask::waitLSSComplete()
{
	osEventFlagsWait(taskEvents, EVENTS::SCAN_DONE, osFlagsWaitAny, osWaitForever);
	log->info("LSS Fastscan completed");
}

CO_LSS_address_t CANOpenTask::getOwnAddress()
{
	auto iter = usedIDs.find(1); // comms SHOULD have ID 1
	if(iter != usedIDs.end())
	{
		return iter->second;
	}

	return {};
}

void CANOpenTask::processTick(void)
{
	uint32_t us = __HAL_TIM_GET_COUNTER(&htim16);
	auto reset = CO_RESET_NOT;

	uint32_t next = 0;
	reset = CO_process(canOpenHandle, false, us, &next);

	switch (reset)
	{
	/** 0, Normal return, no action */
	case CO_RESET_NOT:
		break;
		/** 1, Application must provide communication reset. */
	case CO_RESET_COMM:
		HAL_TIM_Base_Stop_IT(&htim15);
		CO_CANsetConfigurationMode((void*) &hcan1);
		CO_delete(canOpenHandle);
		CANOpenInit();
		CO_CANsetNormalMode(canOpenHandle->CANmodule);
		HAL_TIM_Base_Start_IT(&htim15);
		__HAL_TIM_SET_COUNTER(&htim16, 0);
		return;
		/** 2, Application must provide complete device reset */
	case CO_RESET_APP:
		HAL_NVIC_SystemReset(); // DOES NOT RETURN
		break;
		/** 3, Application must quit, no reset of microcontroller (command is not
		 * requested by the stack.) */
	case CO_RESET_QUIT:
		CO_CANsetConfigurationMode((void*) &hcan1);
		CO_delete(canOpenHandle);
		HAL_TIM_Base_Stop_IT(&htim15);
		return;
	default:
		Error_Handler();
	}

	bool syncWas = false;
#if (CO_CONFIG_SYNC) & CO_CONFIG_SYNC_ENABLE
	syncWas = CO_process_SYNC(canOpenHandle, us, NULL);
#endif
#if (CO_CONFIG_PDO) & CO_CONFIG_RPDO_ENABLE
	CO_process_RPDO(canOpenHandle, syncWas, us, NULL);
#endif
#if (CO_CONFIG_PDO) & CO_CONFIG_TPDO_ENABLE
            CO_process_TPDO(canOpenHandle, syncWas, timeDifference, NULL);
#endif

	__HAL_TIM_SET_COUNTER(&htim16, 0);
}

void CANOpenTask::CANOpenInit(void)
{
	CO_ReturnError_t err;
	canOpenHandle = CO_new( /*config*/nullptr, /*heap used*/nullptr);
	err = CO_CANinit(canOpenHandle, (void*) &hcan1, 1000);
	uint32_t *in = (uint32_t*) &fullSerial;
	uint32_t hashed = HAL_CRC_Calculate(&hcrc, in, 4);

	auto entry = OD_ENTRY_H1018_identity;
	OD_set_u32(entry, 1, 0xFFFFFFFF, true);	// Set device vendor id
	OD_set_u32(entry, 2, 0x00000001, true);	// Set product code (Comms)
	OD_set_u32(entry, 3, 0x00000001, true);	// Set revision number (Comms V1)
	OD_set_u32(entry, 4, hashed, true);		// Set device serial number

	uint8_t ID = 0x01; // Only 1 Comm per system, fixed ID of 1

	uint32_t errInf = 0;
	//	@formatter:off
		err = CO_CANopenInit(canOpenHandle,
				nullptr, /* alternate NMT */
				nullptr, /* alternate em */
				OD, /* Object dictionary */
				OD_STATUS_BITS, /* Optional OD_statusBits */
				(CO_NMT_control_t)(NMT_CONTROL),
				FIRST_HB_TIME, /* ??? firstHBTime_ms */
				SDO_SRV_TIMEOUT_TIME, /*??? SDOserverTimeoutTime_ms */
				SDO_CLI_TIMEOUT_TIME, /*??? SDOclientTimeoutTime_ms */
				SDO_CLI_BLOCK, /*??? SDOclientBlockTransfer */
				ID, /*??? CO_activeNodeId */
				&errInf /*errInfo*/);
	//	@formatter:on

	pauseSYNC();
}

static CO_LSSmaster_return_t blockingLSS(
		const std::function<CO_LSSmaster_return_t(uint32_t timeDiff)> &func)
{
	CO_LSSmaster_return_t result = func(0);
	while (result == CO_LSSmaster_WAIT_SLAVE)
	{
		osDelay(1);
		result = func(1000);
	}
	return result;
}

void CANOpenTask::LSSTask(void *arg)
{
	if (arg == nullptr)
		osThreadExit();
	CANOpenTask *_this = (CANOpenTask*) arg;

	auto entry = OD_ENTRY_H1018_identity;

	CO_LSS_address_t commsAddr;
	OD_get_u32(entry, 1, &commsAddr.identity.vendorID, true);
	OD_get_u32(entry, 2, &commsAddr.identity.productCode, true);
	OD_get_u32(entry, 3, &commsAddr.identity.revisionNumber, true);
	OD_get_u32(entry, 4, &commsAddr.identity.serialNumber, true);


	_this->usedIDs.insert(
	{ 1, commsAddr }); // ID 1 is reserved for the only comms board
	uint8_t nextID = 2;
	auto lss = _this->canOpenHandle->LSSmaster;

	for (;;)
	{
		int32_t flags = osEventFlagsWait(_this->taskEvents, EVENTS::START_SCAN,
		osFlagsWaitAll, osWaitForever);
		if (flags > 0)
		{
			if (flags == EVENTS::START_SCAN)
			{
				CO_LSSmaster_fastscan_t fastscan;
				fastscan.scan[CO_LSS_FASTSCAN_VENDOR_ID] =
						CO_LSSmaster_FS_MATCH;
				fastscan.match.identity.vendorID = 0xFFFFFFFF; // UNOFFICIAL vendor ID for this project
				fastscan.scan[CO_LSS_FASTSCAN_PRODUCT] = CO_LSSmaster_FS_MATCH;
				fastscan.match.identity.productCode = PRODUCT_CODES::SENSOR;
				fastscan.scan[CO_LSS_FASTSCAN_REV] = CO_LSSmaster_FS_SKIP;
				fastscan.scan[CO_LSS_FASTSCAN_SERIAL] = CO_LSSmaster_FS_SCAN;

				for (;;)
				{
					auto resp =
							blockingLSS(
									[&lss, &fastscan](
											uint32_t timediff)
											{
												return CO_LSSmaster_IdentifyFastscan(lss, timediff, &fastscan);
											});

					if (resp == CO_LSSmaster_SCAN_FINISHED)
					{
						// Selected a node
						resp =
								blockingLSS(
										[&lss, &nextID](
												uint32_t timediff)
												{
													return CO_LSSmaster_configureNodeId(lss, timediff, nextID);
												});

						if (resp == CO_LSSmaster_OK)
						{

							_HourlyData_SystemEvents event = HourlyData_SystemEvents_init_zero;
							event.type = HourlyData_SystemEvents_EventType_DEVICE_CONNECT;
							event.details = (char *) pvPortMalloc(64);

							// Node-ID updated
							if (fastscan.found.identity.productCode
									== PRODUCT_CODES::SENSOR)
							{
								// assigned a sensor
								_this->sensorIDs.insert(nextID);
								sprintf(event.details, "Sensor[%d] Added{%" HEX_WORD "}", nextID, fastscan.found.identity.serialNumber);
							}
							else if (fastscan.found.identity.productCode
									== PRODUCT_CODES::ENVIRONMENTAL)
							{
								// assigned a BME
								_this->bmeIDs.insert(nextID);
								sprintf(event.details, "BME[%d] Added{%" HEX_WORD "}", nextID, fastscan.found.identity.serialNumber);
							}
							else
							{
								event.type = HourlyData_SystemEvents_EventType_OTHER;
								_this->log->error("Assigned ID(%d) to unknown device type(%ld)", nextID, fastscan.found.identity.productCode);
								sprintf(event.details, "UNKNOWN(%d)[%d] Added{%" HEX_WORD "}", fastscan.found.identity.productCode, nextID, fastscan.found.identity.serialNumber);
							}

							_this->monitor->addEvent(event);
							_this->usedIDs.insert(
							{ nextID, fastscan.found});
							nextID++;
						}
						else
						{
							_HourlyData_SystemEvents event = HourlyData_SystemEvents_init_zero;
							event.type = HourlyData_SystemEvents_EventType_DEVICE_CONNECT;
							event.details = (char *) pvPortMalloc(100);
							sprintf(event.details, "Could not assign ID(%d) to {%" HEX_WORD "} of type(%d): code(%d)", nextID, fastscan.found.identity.serialNumber, fastscan.found.identity.productCode, resp);
							_this->monitor->addEvent(event);

							_this->log->error("Could not assign ID(%d), code: %d", nextID, resp);
						}
						CO_LSSmaster_switchStateDeselect(lss);
						continue; // Go on to next device
					}

					if (resp == CO_LSSmaster_SCAN_NOACK)
					{
						// No more devices of the current type
						if (fastscan.match.identity.productCode == PRODUCT_CODES::SENSOR)
						{
							fastscan.match.identity.productCode = PRODUCT_CODES::ENVIRONMENTAL;
							nextID = 4;
						}
						else
						{
							CO_LSSmaster_switchStateDeselect(lss);
							break; // Finished assignment
						}
					}
				}
				osEventFlagsSet(_this->taskEvents, EVENTS::SCAN_DONE);
			}
		}
	}
}

bool CANOpenTask::startPDOS()
{
	uint32_t errInf = 0;
	CO_CANopenInitPDO(canOpenHandle, canOpenHandle->em, OD, 0x01, &errInf);

	if(errInf == CO_ERROR_NO)
	{
		log->info("PDOs initialized");
		return true;
	}
	else
	{
		log->error("Failed to initialize PDOs, code: %ld", errInf);
		return false;
	}

}

void CANOpenTask::resumeSYNC()
{
	canOpenHandle->SYNC->isProducer = true;
	log->info("SYNC started");
}

void CANOpenTask::pauseSYNC()
{
	canOpenHandle->SYNC->isProducer = false;
	log->info("SYNC stopped");
}

std::vector<CO_LSS_address_t> CANOpenTask::getBMEAddress()
{
	std::vector<CO_LSS_address_t> addrs;

	for(const auto &id : bmeIDs)
	{
		auto iter = usedIDs.find(id);
		if(iter != usedIDs.end())
		{
			addrs.emplace_back(iter->second);
		}
	}

	return addrs;
}

std::vector<CO_LSS_address_t> CANOpenTask::getSensorAddress()
{
	std::vector<CO_LSS_address_t> addrs;

	for(const auto &id : sensorIDs)
	{
		auto iter = usedIDs.find(id);
		if(iter != usedIDs.end())
		{
			addrs.emplace_back(iter->second);
		}
	}

	return addrs;
}

void CANOpenTask::startAllDevices()
{
	CO_NMT_sendCommand(this->canOpenHandle->NMT, CO_NMT_ENTER_OPERATIONAL, 0);
	log->info("Devices entering operational");
}

void CANOpenTask::pauseAllDevices()
{
	CO_NMT_sendCommand(this->canOpenHandle->NMT, CO_NMT_ENTER_PRE_OPERATIONAL, 0);
	log->info("Devices entering pre-operational");
}

