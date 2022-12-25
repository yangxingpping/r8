
#include "IOContextManager.h"
#include "Configs.h"
#include "database.h"
#include "networkinterface.h"
#include "Utils.h"
#include "myenum.h"
#include <iostream>

#include "boost/callable_traits/args.hpp"

using std::string;
string confpath{ "r8/config/confcomm.toml" };

template<typename F>
void func(F func)
{
	using Args = boost::callable_traits::args_t<F>;

	Args arg;

	int i = 1;
}

int add(int a, int b)
{
	return a + b;
}


struct UserData
{
	int i = 0;
};

int main(int argc, char* argv[])
{
	func(&add);

	if (CONFINST.Parse(confpath))
	{
		//then init log module
		if (SMCONF::SInitLog("dbmgr.log", "dbmgr"))
		{
			SMUtils::cacheDBValuesNames<AppDB>();
			SMUtils::cacheMainValuesNames<AppMainCmd>();
			SMNetwork::initNetwork();
			shared_ptr<BeforeQueryFuncType> func = make_shared<BeforeQueryFuncType>(&SMNetwork::isjwttokenright);
			SMDB::addBeforeQuery(func);
			SMHotupdate::IOContextManager::Init();
			SMDB::init(true);
		}	
	}

	uWS::SocketContextOptions c;
	c.cert_file_name = "cert.pem";
	c.key_file_name = "key.pem";

	uWS::SSLApp(c).get("/hello", [](auto* res, auto* req) {

		/* You can efficiently stream huge files too */
		res->writeHeader("Content-Type", "text/html; charset=utf-8")->end("Hello HTTP!");

		}).ws<UserData>("/*", {

			/* Just a few of the available handlers */
			.open = [](auto* ws) {
				ws->subscribe("oh_interesting_subject");
			},
			.message = [](auto* ws, std::string_view message, uWS::OpCode opCode) {
				ws->send(message, opCode);
			}

			}).listen(9001, [](auto* listenSocket) {

				if (listenSocket) {
					std::cout << "Listening on port " << 9001 << std::endl;
				}
				else {
					std::cout << "Failed to load certs or to bind to port" << std::endl;
				}

				}).run();

	int i;
	std::cin >> i;
	return 0;
}

