#pragma once

#include "iguana/json.hpp"

#include <vector>
#include <string>
#include <map>

using TPrice = int;
using TOrderNum = int;

using std::vector;
using std::string;
using std::map;

struct uploadPayRecordItem
{
	string    nickname;
	string   recver;
	string    date;
	string  money;
	string attach;
};
REFLECTION(uploadPayRecordItem, nickname, recver, date, money, attach);

struct uploadPayReq
{
	string user;
	string phone;
	string token;
	string groupname;
	vector<uploadPayRecordItem> records;
};
REFLECTION(uploadPayReq, user, phone, token, groupname, records);

struct dbuploadPayRecordItem
{
	string    nickname;
	string   recver;
	string    date;
	int		money;
	string sorderid;
	int orderid;
	TOrderNum ordernum;
};
REFLECTION(dbuploadPayRecordItem, nickname, recver, date, money, sorderid, orderid, ordernum);

struct dbuploadPayReq
{
	string user;
	string phone;
	string token;
	string groupname;
	vector<dbuploadPayRecordItem> orders;
};
REFLECTION(dbuploadPayReq, user, phone, token, groupname, orders);

struct uploadOrderRecordItem
{
	string date;
	string nickname;
	string detail;
};
REFLECTION(uploadOrderRecordItem, date, nickname, detail);

struct uploadOrderReq
{
	string user;
	string phone;
	string token;
	string groupname;
	string date;
	vector<uploadOrderRecordItem> orders;
};
REFLECTION(uploadOrderReq, user, phone, token, groupname, date, orders);

struct dbuploadOrderRecordItem
{
	int ordernum;
	string sordernum;
	string date;
	string nickname;
	string detail;
	string formateddetail;
	int price;
};
REFLECTION(dbuploadOrderRecordItem, ordernum, sordernum, date, nickname, detail, formateddetail, price);

struct orderMenuItem
{
	string name;
	int price;
};
REFLECTION(orderMenuItem, name, price);

struct uniformOrderMenu
{
	int onum;
	vector<orderMenuItem> items;
	int price;
	int format;
};
REFLECTION(uniformOrderMenu, onum, items, price, format);

struct dbuploadOrderReq
{
	string user;
	string phone;
	string token;
	string groupname;
	int date;
	vector<dbuploadOrderRecordItem> orders;
};
REFLECTION(dbuploadOrderReq, user, phone, token, groupname, date, orders);

struct addGroupReq
{
	string user;
	string phone;
	string token;
	vector<string> groups;
};
REFLECTION(addGroupReq, user, phone, token, groups);

struct removeGroupReq
{
	string user;
	string phone;
	string token;
	vector<string> groups;
};
REFLECTION(removeGroupReq, user, phone, token, groups);

struct menuCategy
{
	string name;
	int price;
};
REFLECTION(menuCategy, name, price);

struct addCategyReq
{
	string user;
	string phone;
	string token;
	vector<menuCategy> categy;
};
REFLECTION(addCategyReq, user, phone, token, categy);

struct removeCategyReq
{
	string user;
	string phone;
	string token;
	vector<string> categy;
};
REFLECTION(removeCategyReq, user, phone, token, categy);

struct getCategyReq
{
	string user;
	string phone;
	string token;
};
REFLECTION(getCategyReq, user, phone, token);

struct addMenuItem
{
	string name;
	int categy;
	int price;
	string descx;
	bool defaultselect;
};
REFLECTION(addMenuItem, name, categy, price, descx, defaultselect);

struct addMenuReq
{
	string user;
	string phone;
	string token;
	vector<addMenuItem> menus;
};
REFLECTION(addMenuReq, user, phone, token, menus);

struct removeMenuItem
{
	int id;
};
REFLECTION(removeMenuItem, id);

struct removeMenuReq
{
	string user;
	string phone;
	string token;
	vector<removeMenuItem> menus;
};
REFLECTION(removeMenuReq, user, phone, token, menus);

enum class OrderType
{
	UnPayOrder,
	FinishOrder,
	AllOrder,
};

struct getOrderReq
{
	string user;
	string phone;
	string token;
	OrderType type;
	string group;
	int date;
};
REFLECTION(getOrderReq, user, phone, token, type, group, date);

struct getWeixinGroupReq
{
	string user;
	string phone;
	string token;
};
REFLECTION(getWeixinGroupReq, user, phone, token);

struct getGroupKeywordReq
{
	string phone;
	string user;
	string token;
};
REFLECTION(getGroupKeywordReq, phone, user, token);

struct setGroupKeywordReq
{
	string phone;
	string user;
	string token;
	string keyword;
};
REFLECTION(setGroupKeywordReq, phone, user, token, keyword);

struct getGreetStateReq
{
	string user;
	string phone;
	string token;
};
REFLECTION(getGreetStateReq, user, phone, token);

struct setGreetStateReq
{
	string user;
	string phone;
	string hello;
	string deadline;
	string attention;
	string goodbye;
	string token;
};
REFLECTION(setGreetStateReq, user, phone, hello, deadline, attention, goodbye, token);

struct getMenuReq
{
	string user;
	string phone;
	string token;
};
REFLECTION(getMenuReq, user, phone, token);

struct updateMenuCategyReq
{
	string user;
	string phone;
	string token;
	int id;
	string name;
	int price;
	bool defaultselect;
};
REFLECTION(updateMenuCategyReq, user, phone, token, id, name, price, defaultselect);

struct deleteMenuCategyReq
{
	string user;
	string phone;
	string token;
	int id;
};
REFLECTION(deleteMenuCategyReq, user, phone, token, id);

struct updateMenuItem
{
	int id;
	string name;
	int categy;
	int price;
	string descx;
	bool defaultselect;
};
REFLECTION(updateMenuItem, id, name, price, descx, defaultselect);

struct updateMenuReq
{
	string user;
	string phone;
	string token;
	vector<updateMenuItem> menus;
};
REFLECTION(updateMenuReq, user, phone, token, menus);



struct getOrdersFromDBReq
{
	string user;
	string phone;
	string date;
};
REFLECTION(getOrdersFromDBReq, user, phone, date);

struct getOrderPrintReq
{
	string user;
	string phone;
	string token;
	string group;
	int day;
};
REFLECTION(getOrderPrintReq, user, phone, token, group, day);

using std::string;
using std::vector;

struct userOrderIndex
{
	string user;
	int index;
};
REFLECTION(userOrderIndex, user, index);

struct uploadPayRep
{
	int code = 20000;
	bool isFinish=false;
	string user;
	string phone;
	string group;
	string byebye;
	string attention;
	vector<userOrderIndex> nopay;
};
REFLECTION(uploadPayRep, code, user, phone, group, isFinish, byebye, attention, nopay);

struct uploadOrderRep
{
	int code = 20000;
};
REFLECTION(uploadOrderRep, code);

struct dbuploadOrderRep
{
	int code = 20000;
};
REFLECTION(dbuploadOrderRep, code);

struct addGroupRep
{
	int code = 20000;
};
REFLECTION(addGroupRep, code);

struct removeGroupRep
{
	int code = 20000;
};
REFLECTION(removeGroupRep, code);

struct addCategyRep
{
	int code = 20000;
};
REFLECTION(addCategyRep, code);

struct removeCategyRep
{
	int code = 20000;
};
REFLECTION(removeCategyRep, code);

struct addMenuRep
{
	int code = 20000;
};
REFLECTION(addMenuRep, code);

struct removeMenuRep
{
	int code = 20000;
};
REFLECTION(removeMenuRep, code);

struct OrderItemRep
{
	int id;
	int ordernum;
	string snum;
	string group;
	string nickname;
	string odatetime;
	string odate;
	string otime;
	string pdatetime;
	string orderdetail;
	string uniformdetail;
	int status;
	int price;
};
REFLECTION(OrderItemRep, id, ordernum, group, nickname, odatetime, pdatetime, orderdetail, status, price, otime, odate, snum)

struct getOrderRep
{
	int code = 20000;
	vector<OrderItemRep> orders;
};
REFLECTION(getOrderRep, code, orders)

struct weixinGroupRepItem
{
	int id;
	string name;
};
REFLECTION(weixinGroupRepItem, id, name);

struct getWeixinGroupRep
{
	int code = 20000;
	string user;
	string phone;
	vector<weixinGroupRepItem> groups;
};
REFLECTION(getWeixinGroupRep, code, user, phone, groups);

struct getGroupKeywordRep
{
	int code = 20000;
	string keyword;
};
REFLECTION(getGroupKeywordRep, code, keyword);

struct setGroupKeywordRep
{
	int code = 20000;
	string keyword;
};
REFLECTION(setGroupKeywordRep, code, keyword);

struct getGreetStateRep
{
	int code = 20000;
	string user;
	string phone;
	string hello;
	string endline;
	string attention;
	string byebye;
};
REFLECTION(getGreetStateRep, code, user, phone, hello, endline, attention, byebye);

struct setGreetStateRep
{
	int code = 20000;
	string user;
	string phone;
	string hello;
	string endline;
	string attention;
	string byebye;
};
REFLECTION(setGreetStateRep, code, user, phone, hello, endline, attention, byebye);

struct categyItem
{
	int id;
	string name;
	int price;
};
REFLECTION(categyItem, id, name, price);

struct getCategyRep
{
	int code = 20000;
	string user;
	string phone;
	string token;
	vector<categyItem> categy;
};
REFLECTION(getCategyRep, code, user, phone, token, categy);

struct getMenuItem
{
	int id;
	string name;
	string categy;
	int price;
	string descx;
	bool defaultselect;
};
REFLECTION(getMenuItem, id, name, categy, price, descx, defaultselect);

struct MenuCategyItems
{
	int id;
	string categy;
	vector<getMenuItem> items;
};
REFLECTION(MenuCategyItems, id, categy, items);

struct getMenuRep
{
	int code = 20000;
	string user;
	string phone;
	string title;
	vector<MenuCategyItems> menu;
};
REFLECTION(getMenuRep, code, user, phone, title, menu);

struct updateMenuCategyRep
{
	int code = 20000;
	string user;
	string phone;
};
REFLECTION(updateMenuCategyRep, code, user, phone);

struct deleteMenuCategyRep
{
	int code = 20000;
	string user;
	string phone;
};
REFLECTION(deleteMenuCategyRep, code, user, phone);

struct updateMenuRep
{
	int code = 20000;
	string user;
	string phone;
};
REFLECTION(updateMenuRep, code, user, phone);

struct getOrdersFromDBRep
{
	int code = 20000;
	string user;
	string phone;
	string date;
	vector<OrderItemRep> orders;
};
REFLECTION(getOrdersFromDBRep, code, user, phone, date, orders);

struct orderPrintItem
{
	int seq;
	string nickname;
	string detail;
};
REFLECTION(orderPrintItem, seq, nickname, detail);

struct getOrderPrintRep
{
	int code = 20000;
	string user;
	string phone;
	string group;
	vector<orderPrintItem> orders;
};
REFLECTION(getOrderPrintRep, code, user, phone, group, orders);

struct dayProfileItem
{
	int total;	//total money
	int count;	//order count
};
REFLECTION(dayProfileItem, total);

struct getDayProfileRep
{
	int code = 20000;
	string user;
	string phone;
	vector<dayProfileItem> items;
};
REFLECTION(getDayProfileRep, code, user, phone, items);

struct getDayProfileReq
{
	string user;
	string phone;
	string token;
	int daycount;
};
REFLECTION(getDayProfileReq, user, phone, token, daycount);
