
#include "DataManager.h"

#include "spdlog/spdlog.h"

static map<string, UserInfo> _users;
static mutex _lockuser;
static map <string, map<string, UserInfo>> _sysadmins;
static mutex _locksys;
static map<string, getGreetStateRep> _greets;
static mutex _lockgreet;
static TSysOrders _orders;
static mutex _lockorders;

static set<string> _finishOrders;
static mutex _lockfinishorders;

static DataManager _inst;

DataManager& DataManager::Inst()
{
	return _inst;
}

bool DataManager::addGreet(string& user, string& phone, getGreetStateRep& greet)
{
	bool bret = true;
	string key = user + "_" + phone;
	lock_guard<mutex> lock(_lockgreet);
	_greets[key] = greet;
	return bret;
}


getGreetStateRep DataManager::getGreet(string& user, string& phone)
{
	getGreetStateRep ret;
	string key = user + "_" + phone;
	lock_guard<mutex> lock(_lockgreet);
	auto it = _greets.find(key);
	if (it != _greets.end())
	{
		ret = it->second;
	}
	else
	{
		SPDLOG_WARN("get greet with key {} failed", key);
	}
	return ret;
}

bool DataManager::addOrder(string& user, string& phone, string& group, string& name, string& date, string& time, string& detail, int onum, string snum, int price)
{
	bool bret = false;
	lock_guard<mutex> lock(_lockorders);
	string key1 = user + "_" + phone;
	IT_TSysOrders it = _orders.find(key1);
	if (it == _orders.end())
	{
		_orders.insert({ key1, TGroupDayOrders() });
		it = _orders.find(key1);
	}
	string key2 = group;
	IT_TGroupDayOrders it2 = it->second.find(key2);
	if (it2 == it->second.end())
	{
		it->second.insert({ key2, TUserOrders() });
		it2 = it->second.find(key2);
	}
	auto it3 = it2->second.find(name);
	if (it3 == it2->second.end())
	{
		it2->second.insert({ name, UserDayOrders()});
		it3 = it2->second.find(name);
	}
	bool bexist = false;
	for (auto od = it3->second.orders.begin(); od != it3->second.orders.end(); ++od)
	{
		if (od->odate == date && od->orderContext == detail)
		{
			bexist = true;
			SPDLOG_WARN("user {} order {} already exist", od->sender, od->orderContext);
			break;
		}
	}
	if (!bexist)
	{
		OrderDetail od;
		od.orderContext = detail;
		od.sender = name;
		od.aprice = price;
		od.ordernum = onum;
		od.odate = date;
		od.sorderid = snum;
		it3->second.totalMoney += price;
		it3->second.orders.push_back(od);
		bret = true;
	}
	return bret;
}

tuple<string, string> DataManager::addPay(string& user, string& phone, string& group, string& sender, string& recver, string& date, string& sprice, int price)
{
	string bret;
	string bret2;
	lock_guard<mutex> lock(_lockorders);
	string key1 = user + "_" + phone;
	IT_TSysOrders it = _orders.find(key1);
	if (it == _orders.end())
	{
		_orders.insert({ key1, TGroupDayOrders() });
		it = _orders.find(key1);
	}
	string key2 = group ;
	IT_TGroupDayOrders it2 = it->second.find(key2);
	if (it2 == it->second.end())
	{
		it->second.insert({ key2, TUserOrders() });
		it2 = it->second.find(key2);
	}
	auto it3 = it2->second.find(sender);
	if (it3 != it2->second.end())
	{
		for (auto od = it3->second.orders.begin(); od != it3->second.orders.end(); ++od)
		{
			bret = od->sorderid;
			bret2 = od->odate;
			if (od->aprice != price)
			{
				SPDLOG_WARN("price in order {} != price in hb {}", od->aprice, price);
			}
			it3->second.orders.erase(od);
			break;
		}
		it3->second.payedMony += price;
		
	}
	return std::make_tuple(bret, bret2);
}

bool DataManager::isGroupDayOrderFinish(string& user, string& phone, string& group)
{
	bool bret = true;
	lock_guard<mutex> lock(_lockorders);
	string key1 = user + "_" + phone;
	IT_TSysOrders it = _orders.find(key1);
	if (it == _orders.end())
	{
		_orders.insert({ key1, TGroupDayOrders() });
		it = _orders.find(key1);
	}
	string key2 = group;
	IT_TGroupDayOrders it2 = it->second.find(key2);
	if (it2 == it->second.end())
	{
		it->second.insert({ key2, TUserOrders() });
		it2 = it->second.find(key2);
	}
	for (auto it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
	{
		if (it3->second.payedMony != it3->second.totalMoney)
		{
			bret = false;
		}
	}
	return bret;
}

bool DataManager::isOrderFinish(string& user, string& phone, string& group, string& date, string& name, string snum)
{
	bool bret = true;
	lock_guard<mutex> lock(_lockfinishorders);
	string key = fmt::format("{}_{}_{}_{}_{}_{}", user, phone, group, date, name, snum).c_str();
	bret = _finishOrders.find(key) != _finishOrders.end();
	return bret;
}

void DataManager::markOrderFinish(string& user, string& phone, string& group, string& date, string& name, string snum)
{
	lock_guard<mutex> lock(_lockfinishorders);
	string key = fmt::format("{}_{}_{}_{}_{}_{}", user, phone, group, date, name, snum).c_str();
	_finishOrders.insert(key);
}

vector<userOrderIndex> DataManager::getGroupUnpayedOrders(string& user, string& phone, string& group)
{
	vector<userOrderIndex> ret;
	lock_guard<mutex> lock(_lockorders);
	string key1 = user + "_" + phone;
	IT_TSysOrders it = _orders.find(key1);
	if (it == _orders.end())
	{
		_orders.insert({ key1, TGroupDayOrders() });
		it = _orders.find(key1);
	}
	string key2 = group;
	IT_TGroupDayOrders it2 = it->second.find(key2);
	for (auto uos = it2->second.begin(); uos != it2->second.end(); ++uos)
	{
		if (uos->second.payedMony != uos->second.totalMoney)
		{
			for (auto vv : uos->second.orders)
			{
				ret.push_back({ vv.sender, vv.ordernum });
			}
		}
	}
	return ret;
}
