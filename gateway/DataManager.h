
#pragma once

#include <set>
#include <map>
#include <string>
#include <mutex>
#include <vector>
#include <tuple>

#include "myreqreps.h"

using std::vector;
using std::multimap;
using std::map;
using std::string;
using std::mutex;
using std::lock_guard;
using std::set;
using std::tuple;

typedef struct
{
	string user;
	string phone;
	string pass;
	int expire;
}UserInfo;

typedef struct
{
	int orderid;
	string sorderid;
	string orderContext;
	string sender;
	string recver;
	string date;
	string odate;
	string detail;
	string hbattach;
	string hbdt;
	TPrice aprice; //price parse from order;
	TPrice bprice; //payed parse from hongbao;
	TOrderNum ordernum;
	vector<orderMenuItem> details;
}OrderDetail;

typedef struct _UserDayOrders 
{
	int totalMoney;
	int payedMony;
	vector<OrderDetail> orders;
}UserDayOrders;

typedef map<string, UserDayOrders> TUserOrders; //key is user name
typedef TUserOrders::iterator IT_TUserOrders;
typedef map<string, TUserOrders> TGroupDayOrders;	//key is groupname
typedef TGroupDayOrders::iterator IT_TGroupDayOrders;
typedef map<string, TGroupDayOrders> TSysOrders;	//key is user + phone
typedef TSysOrders::iterator IT_TSysOrders;



class DataManager
{
public:
	static DataManager& Inst();
public:
	bool addUser(string& user, string& phone, string& pass, int expiredate);
	bool addGreet(string& user, string& phone, getGreetStateRep& greet);
	bool addOrder(string& user, string& phone, string& group, string& name, string& date, string& time, string& detail, int onum, string snum, int price);
	tuple<string,string> addPay(string& user, string& phone, string& group, string& sender, string& recver, string& date, string& sprice, int price);

	bool isGroupDayOrderFinish(string& user, string& phone, string& group);

	bool isOrderFinish(string& user, string& phone, string& group, string& date, string& name, string snum);
	void markOrderFinish(string& user, string& phone, string& group, string& date, string& name, string snum);

	vector<userOrderIndex> getGroupUnpayedOrders(string& user, string& phone, string& group);

	getGreetStateRep getGreet(string& user, string& phone);
};

#define DM DataManager::Inst()

