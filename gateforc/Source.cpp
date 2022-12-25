
#include "networkinterface.h"
#include "IOContextManager.h"
#include "FilePathMonitor.h"
#include "coros.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "Configs.h"
#include "Apps.h"
#include "DBs.h"
#include "database.h"

#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::string;
using std::thread;

static std::mutex _lockdb;

int main(int argc, char* argv[])
{
	SPDLOG_INFO("server gateforc starting up...");
	//first set config name
	string confpath{ "r8/config/confcomm.toml" };
	if (CONFINST.Parse(confpath))
	{
		CONFINST.getLogConfig()._context._file = "gateforc.log";
		//then init log module
		if (SMCONF::SInitLog())
		{

			SPDLOG_INFO("init log module success");
			SMNetwork::initNetwork();
			SMHotupdate::IOContextManager::Init();
			SMDB::init();
			//start http routers
			auto& hconf = CONFINST.getHttpConfig();
			hconf._port = 8081;
			thread thhttp([&]() {
				DEFHTTP.startServer();
			SPDLOG_INFO("finish http router");
				});
			thhttp.join();
		}
	}

	return 0;
}

