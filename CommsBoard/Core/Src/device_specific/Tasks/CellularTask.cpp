/*
 * CellularTask.cpp
 *
 *  Created on: Nov 8, 2021
 *      Author: reedt
 */

#include <Tasks/CellularTask.h>
#include "Cellular/Middlewares/ST/STM32_Cellular/Interface/Cellular_Ctrl/Inc/cellular_control_api.h"
#include "Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Inc/cellular_service_utils.h"
#include "Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Inc/cellular_service_os.h"
#include "HTTP/mbed-http/source/http_request.h"
#include <string>

#include "Tasks/MonitorTask.h"


extern MonitorTask * monitor;

CellularTask::CellularTask()
{
	// TODO Auto-generated constructor stub
	log = new Logger("Net");
}

CellularTask::~CellularTask()
{
	// TODO Auto-generated destructor stub
}

bool CellularTask::start()
{
	cellular_init();
	cellular_start();
	taskEvents = osEventFlagsNew(nullptr);
}

bool CellularTask::kill()
{
}

bool CellularTask::connect()
{
	if (connected)
		return true;

	osCDS_power_on();

	cellular_info_t info;

	auto ret = cellular_connect();
	do
	{
		cellular_get_cellular_info(&info);
		osDelay(500);
	} while (info.modem_state != CA_MODEM_STATE_DATAREADY);

	connected = true;
	return true;
}

bool CellularTask::disconnect()
{
	if (!connected)
		return true;

	cellular_disconnect();
	osCDS_power_off();
	connected = false;
	return true;
}

uint32_t CellularTask::getServerTime()
{
	auto prevState = connected;

	if(!connected)
	{
		connect();
	}

	uint32_t unixStartTime = 0;
	HttpRequest *req = new HttpRequest(HTTP_GET,
			"http://45.77.77.20:3000/utcnow");

	auto res = req->send(nullptr, 0);

	if (res && res->is_message_complete() && res->get_status_code() == 200)
	{
		unixStartTime = strtol(res->get_body_as_string().c_str(), nullptr, 10);
	}
	else
	{
		_HourlyData_SystemEvents event = HourlyData_SystemEvents_init_zero;
		event.type = HourlyData_SystemEvents_EventType_OTHER;
//		monitor->addEvent(event);

		log->error("[NET] Could not get time from server, code: %d", res ? res->get_status_code() : -1);
	}
	delete req;

	if(!prevState)
	{
		disconnect();
	}

	return unixStartTime;
}

bool CellularTask::uploadFile(uint32_t commsID,
		std::function<uint8_t* (uint32_t*)> bufferedReader)
{
	auto prevState = connected;

	if(!connected)
	{
		connect();
	}

	bool retval = true;
	char url[128];
	sprintf(url, "http://45.77.77.20:3000/radiometers/%#010lX", commsID);

	HttpRequest *req = new HttpRequest(HTTP_POST, url);
	req->set_header("Content-Type", "application/octet-stream");

	auto res = req->send(bufferedReader);
	if (!res || !res->is_message_complete() || res->get_status_code() != 200)
	{
		retval = false;
		_HourlyData_SystemEvents event = HourlyData_SystemEvents_init_zero;
		event.type = HourlyData_SystemEvents_EventType_OTHER;
//		monitor->addEvent(event);
	}
	delete req;

	if(!prevState)
	{
		disconnect();
	}

	return retval;
}

void CellularTask::printNetworkInfo()
{
	const char mdm_state[][19]  =
	{
			 "POWERED_ON",
			 "SIM_CONNECTED",
			 "NETWORK_SEARCHING",
			 "NETWORK_REGISTERED",
			 "DATAREADY",
			 "FLIGHTMODE",
			 "REBOOTING",
			 "FOTA_INPROGRESS",
			 "POWER_OFF",
	};
	const char network_type[][8]=
	{
			"GSM",
			"LTE.M1",
			"LTE.NB1",
			"??"
	};
	const char sim_select[][15]=
	{
			"REMOVABLE",
			"EXTERNAL_MODEM",
			"INTERNAL_MODEM",
			"NON_EXISTING",
	};
	const char sim_status[][19] =
	{
			"READY",
			"UNKNOWN",
			"CONNECTION_ONGOING",
			"PIN_OR_PUK_LOCKED",
			"INCORRECT_PIN",
			"BUSY",
			"ERROR",
			"NOT_INSERTED",
			"NOT_USED",
			"NOT_IMPLEMENTED"
	};

	std::string output;
	output += "\r\n====Cellular Info====\r\n";

	cellular_info_t info;
	cellular_get_cellular_info(&info);

	output += "\tState: ";
	output += mdm_state[ std::min(info.modem_state - 1, 8)];
	output += "\r\n====Modem Info===\r\n";

	output += "\tManufacturer: ";
	output += (info.identity.manufacturer_id.len > 0) ? (char *) info.identity.manufacturer_id.value : "??";

	output += "\r\n\tModel: ";
	output += (info.identity.model_id.len > 0) ? (char *) info.identity.model_id.value: "??";

	output += "\r\n\tRevision: ";
	output += (info.identity.revision_id.len > 0) ? (char *) info.identity.revision_id.value : "??";

	output += "\r\n\tSerial: ";
	output += (info.identity.serial_number_id.len > 0) ? (char *) info.identity.serial_number_id.value : "??";

	output += "\r\n\tIMEI: ";
	output += (info.imei.len > 0) ? (char *) info.imei.value : "??";

	output += "\r\n\tNetwork: ";
	output += (info.mno_name.len > 0) ? (char *) info.mno_name.value : "??";

	output += "\r\n\tIP: " + std::to_string( (info.ip_addr.addr >> 24) & 0xFF) + "." + std::to_string( (info.ip_addr.addr >> 16) & 0xFF) + "." + std::to_string( (info.ip_addr.addr >> 8) & 0xFF) + "." + std::to_string( (info.ip_addr.addr >> 0) & 0xFF) + "\r\n";



	cellular_signal_info_t info2;
	cellular_get_signal_info(&info2);
	output += "\tSignal Strength: " + std::to_string(info2.signal_strength.db_value) + "db (" + std::to_string(info2.signal_strength.raw_value) + ")\r\n";
	output += "\tConnection Type: ";
	output += network_type[ std::min(info2.access_techno >> 2, 3) ];




	cellular_sim_info_t info3;
	cellular_get_sim_info(&info3);
	output += "\r\n\tICCID: ";
	output += (info3.iccid.len > 0) ? (char *) info3.iccid.value : "??";

	output += "\r\n\tIMSI: ";
	output += (info3.imsi.len > 0) ? (char *) info3.imsi.value : "??";

	output += "\r\n\tSIM Used: ";
	output += sim_select[std::min((uint8_t)info3.sim_slot_type[0], (uint8_t)3)];

	output += "\r\n\tSIM State: ";
	output += sim_status[std::min((uint8_t)info3.sim_status[0], (uint8_t)9)];

	output += "\r\n====PDN====\r\n";
	output += "\tSend to Modem: ";
	output += (info3.pdn[0].apn_send_to_modem == 1) ? "YES\r\n" : "NO\r\n";

	output += "\tCID: " + std::to_string(info3.pdn[0].cid);

	output += "\r\n\tAPN: ";
	output += (info3.pdn[0].apn.len > 0) ? (char *) info3.pdn[0].apn.value : "??";

	output += "\r\n\tUser: ";
	output += (info3.pdn[0].username.len > 0) ? (char *) info3.pdn[0].username.value : "??";

	output += "\r\n\tPass: ";
	output += (info3.pdn[0].password.len > 0) ? (char *) info3.pdn[0].password.value : "??";

	log->info("%s", output.c_str());
}
