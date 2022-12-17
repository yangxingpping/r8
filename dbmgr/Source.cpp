
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
	int i;
	std::cin >> i;
	return 0;
}

