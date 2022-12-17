#include "GroupKeyword.h"
#include "myreqreps.h"
#include "enums.h"
#include "Utils.h"
#include "DBConnectManager.h"
#include "spdlog/spdlog.h"
#include "networkinterface.h"
#include "CoEvent.h"
#include "Utils.h"


#include <map>
#include <algorithm>
#include <set>

#include "date/date.h"
#include "date/tz.h"
#include "asio/awaitable.hpp"
#include "asio/co_spawn.hpp"

#include "date/date.h"
#include "date/tz.h"

#include "sqlpp11/sqlpp11.h"
#include "sqlpp11/update.h"
#include "sqlpp11/remove.h"
#include "sqlpp11/alias_provider.h"

#include "tinyutf8/tinyutf8.h"

#include "render.h"
#include "nlohmann/json.hpp"

#include "ctml.hpp"

#include <chrono>

#include "DataManager.h"

using string = std::string;


GroupKeyword::~GroupKeyword()
{

}




static shared_ptr<date::year_month_day> _calcDate;

void GroupKeyword::init(ServeMode mode)
{
	asio::co_spawn(IOCTX, [&]()->asio::awaitable<void> {
		while (true)
		{
			co_await updateEveryDay();
			auto _reconnTimer = make_shared<asio::steady_timer>(IOCTX);
			_reconnTimer->expires_after(std::chrono::seconds(300));
			asio::error_code ec;
			try
			{
				co_await _reconnTimer->async_wait(redirect_error(use_awaitable, ec));
			}
			catch (const asio::system_error& e)
			{
				SPDLOG_WARN("update every day timer out");
			}
		}
		co_return;
		}(), asio::detached);
}

awaitable<void> GroupKeyword::updateEveryDay()
{
	auto v2 = date::year_month_day{ date::floor<date::days>(std::chrono::system_clock::now()) };
	if (_calcDate == nullptr || v2.day() != _calcDate->day())
	{
	}
	if (_calcDate == nullptr)
	{
		_calcDate = make_shared<date::year_month_day>(date::floor<date::days>(std::chrono::system_clock::now()));
	}
	co_return;
}
