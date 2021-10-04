/*
 * CANOpenTask.cpp
 *
 *  Created on: Oct 1, 2021
 *      Author: reedt
 */

#include "CANopenNode/CANopen.h"
#include "CANopenNode/301/CO_ODinterface.h"
#include "bsp/DS28CM00ID/DS28CM00ID.h"
#include "bsp/LEDs/LEDManager.h"
#include "can.h"
#include "tim.h"
#include "CANOpenNode/OD.h"
#include "Logging/Logger.h"
#include "cmsis_os.h"
#include <map>
#include <set>
#include <functional>


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



CO_t *canOpenHandle;
extern LEDManager leds;
extern DS28CM00_ID id1;



void canopen_start(void)
{
	CO_ReturnError_t err;
	auto co = CO_new( /*config*/nullptr, /*heap used*/nullptr);
	err = CO_CANinit(co, (void*) &hcan1, 1000);
	uint64_t serNum = id1.getID();

	auto entry = OD_ENTRY_H1018_identity;
	OD_set_u32(entry, 1, 0xFFFFFFFF, true);	// Set device vendor id
	OD_set_u32(entry, 2, 0x00000001, true);	// Set product code (Comms)
	OD_set_u32(entry, 3, 0x00000001, true);	// Set revision number (Comms V1)
	OD_set_u32(entry, 4, serNum, true);		// Set device serial number

	uint8_t ID = 0x01; // Only 1 Comm per system, fixed ID of 1

	uint32_t errInf = 0;
	//	@formatter:off
		err = CO_CANopenInit(co,
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

//	err = CO_CANopenInitPDO(co, co->em, OD, ID, &errInf);	// Delay until after Node-ID assignment

	CO_CANsetNormalMode(co->CANmodule);

	canOpenHandle = co;

	HAL_TIM_Base_Start_IT(&htim15);
	HAL_TIM_Base_Start_IT(&htim16);
	__HAL_TIM_SET_COUNTER(&htim16, 0);
}


void canopen_oneMs(void)
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
		canopen_start();
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

	auto LED_red = CO_LED_RED(canOpenHandle->LEDs, CO_LED_CANopen);
	auto LED_green = CO_LED_GREEN(canOpenHandle->LEDs, CO_LED_CANopen);
	if (LED_red != 0)
	{
		leds.turnOn(color::RED);
	}
	else
	{
		leds.turnOff(color::RED);
	}
	if (LED_green != 0)
	{
		leds.turnOn(color::GREEN);
	}
	else
	{
		leds.turnOff(color::GREEN);
	}
	__HAL_TIM_SET_COUNTER(&htim16, 0);
}


CO_LSSmaster_return_t blockingLSS(
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



void startLSS(void *argument)
{
	/* USER CODE BEGIN startLSS */

	Logger ilog("CO_LSS");

	auto hbTime = canOpenHandle->NMT->HBproducerTime_us;
	canOpenHandle->NMT->HBproducerTime_us = 0; // Disable HB during assignment to reduce traffic

	auto syncProd = canOpenHandle->SYNC->isProducer;
	canOpenHandle->SYNC->isProducer = false;	// Disable SYNC during assignment to reduce traffic

	osDelay(10000);
	ilog.info("Starting ID assignment");
	auto lss = canOpenHandle->LSSmaster;

	CO_LSSmaster_fastscan_t fastscan;
	fastscan.scan[CO_LSS_FASTSCAN_VENDOR_ID] = CO_LSSmaster_FS_MATCH;
	fastscan.match.identity.vendorID = 0xFFFFFFFF; // UNOFFICIAL vendor ID for this project
	fastscan.scan[CO_LSS_FASTSCAN_PRODUCT] = CO_LSSmaster_FS_MATCH;
	fastscan.match.identity.productCode = SENSORSPC;
	fastscan.scan[CO_LSS_FASTSCAN_REV] = CO_LSSmaster_FS_SKIP;
	fastscan.scan[CO_LSS_FASTSCAN_SERIAL] = CO_LSSmaster_FS_SCAN;
	std::map<uint8_t, uint32_t> usedIDs;
	std::set<uint8_t> sensorIDs;
	std::set<uint8_t> bmeIDs;

	auto entry = OD_ENTRY_H1018_identity;
	uint32_t serNum;
	OD_get_u32(entry, 4, &serNum, true);		// Set device serial number

	usedIDs.insert(
	{ 1, serNum }); // ID 1 is reserved for the only comms board
	uint8_t nextID = 2;

	for (;;)
	{
		auto resp = blockingLSS([&lss, &fastscan](uint32_t timediff)
		{
			return CO_LSSmaster_IdentifyFastscan(lss, timediff, &fastscan);
		});
		ilog.info("FS res= %d", resp);
		if (resp == CO_LSSmaster_SCAN_FINISHED)
		{
			// Selected a node
			resp = blockingLSS([&lss, &nextID](uint32_t timediff)
			{
				return CO_LSSmaster_configureNodeId(lss, timediff, nextID);
			});
			ilog.info("cfg_nid res= %d", resp);

			if (resp == CO_LSSmaster_OK)
			{
				// Node-ID updated
				if (fastscan.found.identity.productCode == SENSORSPC)
				{
					// assigned a sensor
					sensorIDs.insert(nextID);
					ilog.info("Assigned node ID(%" HEX_BYTE ") to a sensor board with serial number: %" HEX_WORD, nextID, fastscan.found.identity.serialNumber);
				}
				else if (fastscan.found.identity.productCode == BMEPC)
				{
					// assigned a BME
					bmeIDs.insert(nextID);
					ilog.info("Assigned node ID(%" HEX_BYTE ") to a BME board with serial number: %" HEX_WORD, nextID, fastscan.found.identity.serialNumber);
				}
				else
				{
					ilog.warn("Assigned ID to unknown device type: %" HEX_WORD, fastscan.found.identity.productCode);
				}
				usedIDs.insert({ nextID, fastscan.found.identity.serialNumber });
				nextID++;
			}
			else
			{
				ilog.error("Failed to set node ID for device type: %" HEX_WORD ", with serial number: %" HEX_WORD", error: %d", fastscan.found.identity.productCode, fastscan.found.identity.serialNumber, resp);
			}
			CO_LSSmaster_switchStateDeselect(lss);
			continue; // Go on to next device
		}

		if(resp == CO_LSSmaster_SCAN_NOACK)
		{
			// No more devices of the current type
			if(fastscan.match.identity.productCode == SENSORSPC)
			{
				fastscan.match.identity.productCode = BMEPC;
				nextID = 3;
			}
			else
			{
				ilog.info("All nodes assigned, LSS config done");
				CO_LSSmaster_switchStateDeselect(lss);
				CO_NMT_sendCommand(canOpenHandle->NMT, CO_NMT_ENTER_OPERATIONAL, 0);

				canOpenHandle->NMT->HBproducerTime_us = hbTime; // Assignment done, may resume periodic transmissions
				canOpenHandle->SYNC->isProducer = syncProd;

				uint32_t errInf = 0;
				CO_CANopenInitPDO(canOpenHandle, canOpenHandle->em, OD, 0x01, &errInf);	// Delay until after Node-ID assignment

				for(;;)osDelay(1000000);
			}
		}

	}
	/* USER CODE END startLSS */
}
