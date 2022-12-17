
#include "DBProcess.h"
#include "DBManager.h"
#include "MessageSplitFuncs.h"
#include "Utils.h"

#include "r8.h"

#include "sqlpp11/sqlpp11.h"
#include "sqlpp11/update.h"
#include "sqlpp11/remove.h"
#include "sqlpp11/alias_provider.h"

#include "tinyutf8/tinyutf8.h"

#include "date/date.h"
#include "date/tz.h"

#include "CoEvent.h"
#include "spdlog/spdlog.h"
#include "fmt/format.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include "date/date.h"
#include "date/tz.h"

#include "myenum.h"


SQLPP_ALIAS_PROVIDER(left);
SQLPP_ALIAS_PROVIDER(right);
SQLPP_ALIAS_PROVIDER(categy);
SQLPP_ALIAS_PROVIDER(categyid);
SQLPP_ALIAS_PROVIDER(groupname);

using std::floor;


dbuploadOrderRep DBProcess::suploadOrderRecord(dbuploadOrderReq req)
{
	using namespace tiny_utf8;
	dbuploadOrderRep rep;
	r8::Orderrecord r;
	r.nickname = "";
	SMDB::DBInst db;
	auto gid = getGroupIdByName(req.user, req.phone, req.groupname);
	auto v2 = sqlpp::chrono::day_point(sqlpp::chrono::days(req.date));
	date::zoned_time t2{ SMDB::getDefaultTimeZone(), std::chrono::system_clock::now() };
	for (const auto& it : req.orders)
	{
		(*db)(insert_into(r).set(r.ordertime = it.date, r.sordernum = it.sordernum, r.gendatetime = v2, r.status = (int)(OrderType::UnPayOrder), r.price = it.price, r.ordernum = it.ordernum, r.formatdetail = it.formateddetail, r.orderdetail = it.detail, r.nickname = it.nickname, r.groupid = gid, r.user = req.user, r.phone = req.phone));
	}
	return rep;
}

addGroupRep DBProcess::saddGroup(addGroupReq req)
{
	addGroupRep rep;
	SMDB::DBInst db;
	r8::Wexingroup ca;
	ca.name = "";
	auto snow = std::chrono::system_clock::now();

	for (auto it = req.groups.begin(); it != req.groups.end(); ++it)
	{
		auto have = (*db)(select(count(ca.name)).from(ca).where(ca.name == *it and ca.phone == req.phone and ca.user == req.user)).front().count.value();
		if (have != 0)
		{
			SPDLOG_WARN("group {} already exist", *it);
		}
		else
		{
			(*db)(insert_into(ca).set(ca.name = *it, ca.user = req.user, ca.phone = req.phone));
		}
	}
	return rep;
}

removeGroupRep DBProcess::sremoveGroup(removeGroupReq req)
{
	removeGroupRep rep;
	SMDB::DBInst db;
	r8::Categy ca;
	ca.name = "";
	for (auto it = req.groups.begin(); it != req.groups.end(); ++it)
	{
		(*db)(remove_from(ca).where(ca.name == *it));
	}
	return rep;
}

addMenuRep DBProcess::saddMenu(addMenuReq req)
{
	addMenuRep rep;
	r8::Menu menu;
	menu.name = "";
	SMDB::DBInst db;
	for (auto it = req.menus.begin(); it != req.menus.end(); ++it)
	{
		auto have = (*db)(select(count(menu.name)).from(menu).where(menu.name == it->name)).front().count.value();
		if (have != 0)
		{
			SPDLOG_WARN("category {} already exist", it->name);
		}
		else
		{
			(*db)(insert_into(menu).set(menu.user = req.user, menu.phone = req.phone, menu.name = it->name, menu.categy = it->categy, menu.descx = it->descx, menu.price = it->price, menu.defaultselect = (int)(it->defaultselect)));
		}
	}
	return rep;
}

removeMenuRep DBProcess::sremoveMenu(removeMenuReq req)
{
	removeMenuRep rep;
	r8::Menu i;
	SMDB::DBInst db;
	i.name = "";
	for (auto it = req.menus.begin(); it != req.menus.end(); ++it)
	{
		(*db)(remove_from(i).where(i.id == it->id));
	}
	return rep;
}

getOrderRep DBProcess::sgetOrder(getOrderReq req)
{
	getOrderRep rep;
	SMDB::DBInst db;
	r8::Orderrecord orx;
	r8::Wexingroup g;
	auto l = orx.as(left);
	auto r = g.as(right);
	auto sss = sqlpp::dynamic_select(*db).dynamic_columns(orx.id, g.name.as(groupname), orx.sordernum, orx.ordernum, orx.nickname, orx.ordertime, orx.gendatetime, orx.paytime, orx.formatdetail, orx.orderdetail, orx.status, orx.price).from(orx.join(g).on(orx.groupid == g.id));
	auto s = sss.dynamic_where(orx.user == req.user and orx.phone == req.phone);
	date::zoned_time t2{ SMDB::getDefaultTimeZone(), std::chrono::system_clock::now() };
	auto vv2 = sqlpp::chrono::day_point(sqlpp::chrono::floor<sqlpp::chrono::days>(t2.get_local_time().time_since_epoch()));
	if (req.type != OrderType::AllOrder)
	{
		s.where.add(orx.status == (int)(req.type));
	}
	if (req.date != 0)
	{
		auto curdt = sqlpp::chrono::floor<sqlpp::chrono::days>(std::chrono::system_clock::now());
		auto dt = sqlpp::chrono::day_point(sqlpp::chrono::days(curdt.time_since_epoch().count()-req.date));
		s.where.add(orx.gendatetime > dt and orx.gendatetime <= vv2);
	}
	for (const auto& row : (*db)(s))
	{
		OrderItemRep item;
		item.id = row.id;
		item.group = row.groupname;
		item.nickname = row.nickname;
		auto ymd = sqlpp::chrono::floor<sqlpp::chrono::days>(row.gendatetime.value());
		std::ostringstream oss;
		//oss << ymd.time_since_epoch();
		item.odate = oss.str();
		oss << " " << row.ordertime;
		item.odatetime = oss.str();
		item.otime = row.ordertime;
		item.pdatetime = row.paytime;
		item.orderdetail = row.orderdetail;
		item.status = row.status;
		item.price = row.price;
		item.ordernum = row.ordernum;
		item.snum = row.sordernum;
		item.uniformdetail = row.formatdetail;
		rep.orders.push_back(item);
	}
	return rep;
}

getWeixinGroupRep DBProcess::sgetGroup(getWeixinGroupReq req)
{
	getWeixinGroupRep rep;
	r8::Wexingroup group;
	SMDB::DBInst db;
	group.name = "";
	for (const auto& row : (*db)(select(all_of(group)).from(group).where(group.phone == req.phone and group.user == req.user)))
	{
		weixinGroupRepItem item;
		item.id = row.id.value();
		item.name = row.name.value();
		rep.groups.push_back(item);
	}
	rep.user = req.user;
	rep.phone = req.phone;
	return rep;
}

getGroupKeywordRep DBProcess::sgetGroupKeyword(getGroupKeywordReq req)
{
	getGroupKeywordRep rep;
	SMDB::DBInst db;
	r8::Groupkeyword gk;
	gk.phone = "";
	for (const auto& row : (*db)(select(all_of(gk)).from(gk).where(gk.user == req.user and gk.phone == req.phone)))
	{
		rep.keyword = row.keyword;
		break;
	}
	return rep;
}

setGroupKeywordRep DBProcess::ssetGroupKeyword(setGroupKeywordReq req)
{
	setGroupKeywordRep rep;
	SMDB::DBInst db;
	r8::Groupkeyword gk;
	gk.phone = "";
	auto have = (*db)(select(count(gk.user)).from(gk).where(gk.user == req.user and gk.phone == req.phone)).front().count.value();
	if (have != 0) //update 
	{
		(*db)(update(gk).set(gk.keyword = req.keyword).where(gk.user == req.user and gk.phone == req.phone));
	}
	else //insert
	{
		(*db)(insert_into(gk).set(gk.user = req.user, gk.phone = req.phone, gk.keyword = req.keyword));
	}
	rep.keyword = req.keyword;
	return rep;
}

getGreetStateRep DBProcess::sgetGreetState(getGreetStateReq req)
{
	getGreetStateRep rep;
	SMDB::DBInst db;
	r8::GreetState st;
	st.user = req.user;
	rep.user = req.user;
	rep.phone = req.phone;
	for (const auto& row : (*db)(select(all_of(st)).from(st).where(st.user == req.user and st.phone == req.phone)))
	{
		rep.hello = row.hello;
		rep.endline = row.endtime;
		rep.attention = row.attention;
		rep.byebye = row.goodbye;
		break;
	}
	return rep;
}

setGreetStateRep DBProcess::ssetGreetState(setGreetStateReq req)
{
	setGreetStateRep rep;
	SMDB::DBInst db;
	r8::GreetState st;
	st.user = req.user;
	rep.user = req.user;
	rep.phone = req.phone;
	rep.hello = req.hello;
	rep.endline = req.deadline;
	rep.attention = req.attention;
	rep.byebye = req.goodbye;
	auto have = (*db)(select(count(st.user)).from(st).where(st.user == req.user and st.phone == req.phone)).front().count.value();
	if (have != 0) //update 
	{
		(*db)(update(st).set(st.hello = req.hello, st.endtime = req.deadline, st.attention = req.attention, st.goodbye = req.goodbye).where(st.user == req.user and st.phone == req.phone));
	}
	else //insert
	{
		(*db)(insert_into(st).set(st.user = req.user, st.phone = req.phone, st.hello = req.hello, st.endtime = req.deadline, st.attention = req.attention, st.goodbye = req.goodbye));
	}
	return rep;
}

getCategyRep DBProcess::sgetCategy(getCategyReq req)
{
	getCategyRep rep;
	SMDB::DBInst db;
	r8::Categy ct;
	ct.id = 0;
	rep.user = req.user;
	rep.phone = req.phone;
	for (const auto& row : (*db)(select(all_of(ct)).from(ct).where(ct.user == req.user and ct.phone == req.phone)))
	{
		categyItem it;
		it.id = row.id;
		it.name = row.name;
		it.price = row.price;
		rep.categy.push_back(it);
	}
	return rep;
}

getMenuRep DBProcess::sgetMenu(getMenuReq req)
{
	getMenuRep rep;
	SMDB::DBInst db;
	r8::Menu ct;
	r8::Categy ca;

	ct.id = 0;
	rep.user = req.user;
	rep.phone = req.phone;

	
	for (const auto& row : (*db)(select(ct.id, ct.name, ca.name.as(categy), ct.categy.as(categyid), ct.descx, ct.price, ct.defaultselect).from(ct.join(ca).on(ca.user == ct.user and ca.phone == ct.phone and ca.id == ct.categy)).where(ct.user == req.user and ct.phone == req.phone).order_by(ct.categy.asc())))
	{
		getMenuItem it;
		it.id = row.id;
		it.name = row.name;
		it.categy = row.categy;
		it.descx = row.descx;
		it.price = row.price;
		it.defaultselect = (bool)(row.defaultselect);
		if (rep.menu.empty() || rep.menu.rbegin()->id != row.categyid.value()) //create a new one
		{
			MenuCategyItems vv;
			vv.id = row.categyid.value();
			vv.categy = row.categy;
			rep.menu.push_back(vv);
		}
		auto catmenus = rep.menu.rbegin();
		catmenus->items.push_back(it);
	}
	return rep;
}

addCategyRep DBProcess::saddCategy(addCategyReq req)
{
	addCategyRep rep;
	SMDB::DBInst db;
	r8::Categy st;
	st.id = 1;

	for (auto& it : req.categy)
	{
		auto have = (*db)(select(count(st.id)).from(st).where(st.phone == req.phone and st.user == req.user and st.name == it.name)).front().count.value();
		if (have != 0) //update 
		{
			SPDLOG_WARN("categy name {} already exist", it.name);
			rep.code = to_underlying(R8StatusCode::categryAlreadyExistWhenCreate);
		}
		else //insert
		{
			(*db)(insert_into(st).set(st.name = it.name, st.price = it.price, st.user = req.user, st.phone = req.phone));
		}
	}

	return rep;
}

updateMenuCategyRep DBProcess::supdateMenuCategy(updateMenuCategyReq req)
{
	updateMenuCategyRep rep;
	SMDB::DBInst db;
	r8::Categy st;
	rep.user = req.user;
	rep.phone = req.phone;

	st.id = 1;
	auto have = (*db)(select(count(st.id)).from(st).where(st.phone == req.phone and st.user == req.user and st.id == req.id)).front().count.value();
	if (have != 0) //update 
	{
		(*db)(update(st).set(st.name = req.name, st.price = req.price).where(st.phone == req.phone and st.user == req.user and st.id == req.id));
	}
	else //insert
	{
		rep.code = to_underlying(R8StatusCode::categryNotExistWhenUpdate);
	}
	return rep;
}

deleteMenuCategyRep DBProcess::sdeleteMenuCategy(deleteMenuCategyReq req)
{
	deleteMenuCategyRep rep;
	SMDB::DBInst db;
	r8::Categy st;
	r8::Menu mu;
	rep.user = req.user;
	rep.phone = req.phone;
	mu.categy = 0;
	st.id = 1;
	(*db)(remove_from(st).where(st.phone == req.phone and st.user == req.user and st.id == req.id));
	
	return rep;
}

updateMenuRep DBProcess::supdatMenu(updateMenuReq req)
{
	updateMenuRep rep;
	r8::Menu menu;
	menu.name = "";
	rep.user = req.user;
	rep.phone = req.phone;
	SMDB::DBInst db;
	for (auto it = req.menus.begin(); it != req.menus.end(); ++it)
	{
		(*db)(update(menu).set(menu.defaultselect = (int)(it->defaultselect), menu.name = it->name, menu.descx = it->descx, menu.price = it->price).where(menu.user == req.user and menu.phone == req.phone and menu.id == it->id));
	}
	return rep;
}

uploadPayRep DBProcess::suploadPay(dbuploadPayReq req)
{
	uploadPayRep rep;
	rep.user = req.user;
	rep.phone = req.phone;
	rep.group = req.groupname;
	r8::Orderrecord orx;
	SMDB::DBInst db;
	orx.nickname = "";
	for (auto& i : req.orders)
	{
		(*db)(update(orx).set(orx.paytime = i.date, orx.status = int(OrderType::FinishOrder)).where(orx.user == req.user and orx.phone == req.phone and orx.nickname == i.nickname and orx.status == int(OrderType::UnPayOrder) and orx.sordernum == i.sorderid));
	}
	return rep;
}

getOrderPrintRep DBProcess::sgetOrderPrint(getOrderPrintReq req)
{
	getOrderPrintRep rep;
	r8::Orderrecord orx;
	SMDB::DBInst db;
	orx.nickname = "";
	rep.user = req.user;
	rep.phone = req.phone;


	auto curdt = sqlpp::chrono::floor<sqlpp::chrono::days>(std::chrono::system_clock::now()); //write date

	date::zoned_time t2{ SMDB::getDefaultTimeZone(), std::chrono::system_clock::now() };
	auto vv2 = sqlpp::chrono::day_point(sqlpp::chrono::floor<sqlpp::chrono::days>(t2.get_local_time().time_since_epoch()));
	if (req.day < 365 && req.day>0)
	{
		curdt = sqlpp::chrono::day_point(sqlpp::chrono::days(vv2.time_since_epoch().count() - req.day));
	}

	for (const auto& row : (*db)(select(orx.nickname, orx.ordernum, orx.orderdetail).from(orx).where(orx.user == req.user and orx.phone == req.phone and orx.gendatetime >= curdt and orx.groupid == getGroupIdByName(req.user, req.phone, req.group))))
	{
		orderPrintItem it;
		it.seq = row.ordernum;
		it.nickname = row.nickname;
		it.detail = row.orderdetail;
		rep.orders.push_back(it);
	}
	return rep;
}

getDayProfileRep DBProcess::sgetDayProfile(getDayProfileReq req)
{
	getDayProfileRep rep;
	rep.user = req.user;
	rep.phone = req.phone;
	r8::Orderrecord orx;
	orx.user = req.user;
	SMDB::DBInst db;
	for(const auto& row : (*db)(select(count(orx.id), sum(orx.price), orx.gendatetime).from(orx).where(orx.user==req.user and orx.phone==req.phone).group_by(orx.gendatetime).order_by(orx.gendatetime.asc()).limit((size_t)(req.daycount))))
	{
		dayProfileItem v;
		v.count = row.count;
		v.total = row.sum;
		rep.items.push_back(v);
	}
	return rep;
}

int DBProcess::getGroupIdByName(string user, string phone, string name)
{
	int ret = 0;
	r8::Wexingroup g;
	SMDB::DBInst db;
	g.id = 0;
	for (const auto& row : (*db)(select(g.id).from(g).where(g.user == user and g.phone == phone and g.name == name)))
	{
		ret = row.id;
		break;
	}
	return ret;
}
