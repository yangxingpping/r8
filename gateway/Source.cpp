
#include "IOContextManager.h"
#include "Configs.h"
#include "Https.h"
#include "networkinterface.h"
#include "database.h"
#include "myenum.h"
#include "Utils.h"
#include <thread>

using std::thread;

string confpath{ "r8/config/confcomm.toml" };
int main(int argc, char* argv[])
{
	if (CONFINST.Parse(confpath))
	{
		if (SMCONF::SInitLog("gateway.log", "gateway"))
		{
			SMUtils::cacheDBValuesNames<AppDB>();
			SMUtils::cacheMainValuesNames<AppMainCmd>();
			SMNetwork::initNetwork();
			SMHotupdate::IOContextManager::Init();
			SMDB::init();
			thread thhttp([]() {
				DEFHTTP.startServer();
				});
			thhttp.join();
		}
	}
	
	return 0;
}
