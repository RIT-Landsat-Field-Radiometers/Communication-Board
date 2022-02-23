/*
 * CellularTask.cpp
 *
 *  Created on: Nov 8, 2021
 *      Author: reedt
 */

#include <Tasks/CellularTask.h>
#include "Cellular/Middlewares/ST/STM32_Cellular/Core/Rtosal/Inc/rtosal.h"
#include "Cellular/Middlewares/ST/STM32_Cellular/Core/Ipc/Inc/ipc_uart.h"

#include "Cellular/Middlewares/ST/STM32_Cellular/Interface/Cellular_Ctrl/Inc/cellular_control_api.h"
#include "Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Inc/cellular_service_utils.h"
#include "Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Inc/cellular_service_os.h"
#include "HTTP/mbed-http/source/http_request.h"
#include "Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Inc/com_sockets_ip_modem.h"


#include <string>

#include "Tasks/MonitorTask.h"

extern MonitorTask *monitor;

CellularTask::CellularTask(std::string hostname)
{
	// TODO Auto-generated constructor stub
	log = new Logger("Net");
	if (hostname.size() < 1)
	{
		log->error("Initalized Cellular Task with invalid hostname");
		return;
	}
	host.clear();

	if (hostname.rfind("http://", 0) == std::string::npos)
	{
		// Need to prepend "http://"
		host += "http://";
	}
	host += hostname;
	taskEvents = osEventFlagsNew(nullptr);
}

CellularTask::~CellularTask()
{
	// TODO Auto-generated destructor stub
}

bool CellularTask::start()
{
	cellular_init();
	cellular_start();

	return true;
}

bool CellularTask::kill()
{
	stopAllCelluar();

	extern socket_desc_t *socket_desc_list;
	auto iter = socket_desc_list;
	while(iter != nullptr)
	{
		auto next = iter->next;
		vPortFree(iter);
		iter = next;
	}

	connected = false;
	sockfd = -1;

	IPC_UART_deinit(0);


	char * message = "AT+QICLOSE\r\n";
	uint8_t size = strlen(message);

	HAL_UART_Transmit(&huart2, (uint8_t *) message, size, -1);

	return true;
}

bool CellularTask::connect()
{
	if (host.size() < 1)
	{
		log->error("Tried to connect with no set host!");
		return false;
	}

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

	sockfd = HttpRequest::make_socket(host.c_str());

	connected = true;
	return true;
}

bool CellularTask::disconnect()
{
	if (!connected)
		return true;

	if (sockfd >= 0)
	{
		osCDS_socket_close(sockfd, 1);
//		com_closesocket(sockfd);
		sockfd = -1;
	}

	cellular_disconnect();
	osCDS_power_off();
	connected = false;
	return true;
}

uint32_t CellularTask::getServerTime()
{
	auto prevState = connected;

	if (!connected)
	{
		if (!connect())
		{
			log->error("Connecting failed when retrieving UTC time");
			return false; // Didn't connect
		}
	}

	uint32_t unixStartTime = 0;

	std::string url = host;
	url += "/utcnow";

	HttpRequest *req;
	if (sockfd >= 0)
	{
		req = new HttpRequest(sockfd, HTTP_GET, url.c_str());
	}
	else
	{
		req = new HttpRequest(HTTP_GET, url.c_str());
	}

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

		log->error("Could not get time from server, code: %d",
				res ? res->get_status_code() : -1);
	}
	delete req;

	if (!prevState)
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

//bool CellularTask::uploadFile(uint32_t commsID,
//		std::function<uint8_t* (uint32_t*)> bufferedReader)
//{
//	auto prevState = connected;
//
//	if (!connected)
//	{
//		if (!connect())
//		{
//			log->error("Connecting failed when uploading file");
//			return false; // Didn't connect
//		}
//	}
//
//	bool retval = true;
//	char url[128];
//	sprintf(url, "%s/radiometers/%#010lX", this->host.c_str(), commsID);
//
//
//	HttpRequest *req;
//	if(sockfd >= 0)
//	{
//		req = new HttpRequest(this->sockfd, HTTP_POST, url);
//	}
//	else
//	{
//		req = new HttpRequest(HTTP_POST, url);
//	}
//
//	req->set_header("Content-Type", "application/octet-stream");
//
//
//
//	enum result : uint32_t
//	{
//		OK = 0x01,
//		FAIL = 0x02
//	};
//
//	typedef struct
//	{
//		std::function<uint8_t* (uint32_t*)> _bufferedReader;
//		osEventFlagsId_t _flags;
//		HttpRequest *_req; // Do this so it can be freed if the process dies
//	} funcArgs;
//
//	static osEventFlagsId_t flags = osEventFlagsNew(nullptr);
//	osEventFlagsClear(flags, 0xFFFFFFFF);
//
//	funcArgs args
//	{bufferedReader, flags, req};
//
//	osThreadAttr_t Task_attributes
//	{ 0 };
//	Task_attributes.stack_size = 4096 * 4;
//	Task_attributes.name = "CELL_SUB";
//	Task_attributes.priority = (osPriority_t) osPriorityNormal;
//	osThreadId_t runner =
//			osThreadNew(
//					[](
//							void *arg)
//							{
//								Logger log("CELL_SUB");
//								if(arg == nullptr)
//								{
//									log.error("Null arg, aborting");
//									osThreadExit();
//								}
//
//								funcArgs * fargs = (funcArgs *) arg;
//
//								auto res = fargs->_req->send(fargs->_bufferedReader);
//
//								if(!res)
//								{
//									log.error("Response null");
//								}
//								else if(!res->is_message_complete())
//								{
//									log.error("Message not complete");
//								}
//								else if(res->get_status_code() != 200)
//								{
//									log.error("Status code not 200, code= %d", res->get_status_code());
//								}
//
//								if (!res || !res->is_message_complete() || res->get_status_code() != 200)
//								{
////									_HourlyData_SystemEvents event = HourlyData_SystemEvents_init_zero;
////									event.type = HourlyData_SystemEvents_EventType_OTHER;
//									osEventFlagsSet(fargs->_flags, FAIL);
//									//		monitor->addEvent(event);
//								}
//								else
//								{
//									osEventFlagsSet(fargs->_flags, OK);
//								}
////								delete fargs->_req;
////								fargs->_req = nullptr;
//								osThreadExit();
//							}, &args, &Task_attributes);
//
//	bool ok = true;
//	int32_t res = osEventFlagsWait(flags, OK | FAIL, osFlagsWaitAny, 60 * 1000); // Wait 60 seconds for the operation
//	if(res < 0)
//	{
//		// Thread did not exit...
//		osThreadTerminate(runner);
//		log->error("Modem timed-out during file upload");
//		osDelay(50);
////		disconnect(); // Stop the modem to clean up anything left behind
//		kill();
//		start();
//
//		if(prevState)
//		{
//			connect();
//		}
//		ok = false;
//	}
//	else if(res & FAIL)
//	{
//		log->error("Error in file upload, reconnecting");
//		disconnect(); // Stop the modem to clean up anything left behind
//
////		kill();
////		start();
//
//		if(prevState)
//		{
//			connect();
//		}
//		ok = false;
//	}
//
//	uint32_t remBytes = 0;
//
//	while(bufferedReader(&remBytes) != nullptr); // Clean out the reader
//
//	delete req;
//
//	if(ok)
//	{
//		ok = res & OK; // process worked
//	}
//
//	if(!prevState)
//	{
//		disconnect();
//	}
//
//	return ok;
//}

void CellularTask::printNetworkInfo()
{
	const char mdm_state[][19] =
	{ "POWERED_ON", "SIM_CONNECTED", "NETWORK_SEARCHING", "NETWORK_REGISTERED",
			"DATAREADY", "FLIGHTMODE", "REBOOTING", "FOTA_INPROGRESS",
			"POWER_OFF", };
	const char network_type[][8] =
	{ "GSM", "LTE.M1", "LTE.NB1", "??" };
	const char sim_select[][15] =
	{ "REMOVABLE", "EXTERNAL_MODEM", "INTERNAL_MODEM", "NON_EXISTING", };
	const char sim_status[][19] =
	{ "READY", "UNKNOWN", "CONNECTION_ONGOING", "PIN_OR_PUK_LOCKED",
			"INCORRECT_PIN", "BUSY", "ERROR", "NOT_INSERTED", "NOT_USED",
			"NOT_IMPLEMENTED" };

	std::string output;
	output += "\r\n====Cellular Info====\r\n";

	cellular_info_t info;
	cellular_get_cellular_info(&info);

	output += "\tState: ";
	output += mdm_state[std::min(info.modem_state - 1, 8)];
	output += "\r\n====Modem Info===\r\n";

	output += "\tManufacturer: ";
	output +=
			(info.identity.manufacturer_id.len > 0) ?
					(char*) info.identity.manufacturer_id.value : "??";

	output += "\r\n\tModel: ";
	output +=
			(info.identity.model_id.len > 0) ?
					(char*) info.identity.model_id.value : "??";

	output += "\r\n\tRevision: ";
	output +=
			(info.identity.revision_id.len > 0) ?
					(char*) info.identity.revision_id.value : "??";

	output += "\r\n\tSerial: ";
	output +=
			(info.identity.serial_number_id.len > 0) ?
					(char*) info.identity.serial_number_id.value : "??";

	output += "\r\n\tIMEI: ";
	output += (info.imei.len > 0) ? (char*) info.imei.value : "??";

	output += "\r\n\tNetwork: ";
	output += (info.mno_name.len > 0) ? (char*) info.mno_name.value : "??";

	output += "\r\n\tIP: " + std::to_string((info.ip_addr.addr >> 24) & 0xFF)
			+ "." + std::to_string((info.ip_addr.addr >> 16) & 0xFF) + "."
			+ std::to_string((info.ip_addr.addr >> 8) & 0xFF) + "."
			+ std::to_string((info.ip_addr.addr >> 0) & 0xFF) + "\r\n";

	cellular_signal_info_t info2;
	cellular_get_signal_info(&info2);
	output += "\tSignal Strength: "
			+ std::to_string(info2.signal_strength.db_value) + "db ("
			+ std::to_string(info2.signal_strength.raw_value) + ")\r\n";
	output += "\tConnection Type: ";
	output += network_type[std::min(info2.access_techno >> 2, 3)];

	cellular_sim_info_t info3;
	cellular_get_sim_info(&info3);
	output += "\r\n\tICCID: ";
	output += (info3.iccid.len > 0) ? (char*) info3.iccid.value : "??";

	output += "\r\n\tIMSI: ";
	output += (info3.imsi.len > 0) ? (char*) info3.imsi.value : "??";

	output += "\r\n\tSIM Used: ";
	output +=
			sim_select[std::min((uint8_t) info3.sim_slot_type[0], (uint8_t) 3)];

	output += "\r\n\tSIM State: ";
	output += sim_status[std::min((uint8_t) info3.sim_status[0], (uint8_t) 9)];

	output += "\r\n====PDN====\r\n";
	output += "\tSend to Modem: ";
	output += (info3.pdn[0].apn_send_to_modem == 1) ? "YES\r\n" : "NO\r\n";

	output += "\tCID: " + std::to_string(info3.pdn[0].cid);

	output += "\r\n\tAPN: ";
	output +=
			(info3.pdn[0].apn.len > 0) ? (char*) info3.pdn[0].apn.value : "??";

	output += "\r\n\tUser: ";
	output +=
			(info3.pdn[0].username.len > 0) ?
					(char*) info3.pdn[0].username.value : "??";

	output += "\r\n\tPass: ";
	output +=
			(info3.pdn[0].password.len > 0) ?
					(char*) info3.pdn[0].password.value : "??";

	log->info("%s", output.c_str());
}
