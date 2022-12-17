#include "PayOrder.h"
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
#include "asio/co_spawn.hpp"

#include "date/date.h"
#include "date/tz.h"

#include "tinyutf8/tinyutf8.h"

#include "render.h"
#include "nlohmann/json.hpp"

#include "ctml.hpp"

#include <chrono>

#include "DataManager.h"

#include "parseOrder.h"

using string = std::string;

static CTML::Node layuijs("script");
static CTML::Node layuicss("link");
static CTML::Node nviewport("meta");
static CTML::Node myjs("script");

static CTML::Node btnOk("button", "copy");
static CTML::Node txtOrderDate("textarea");


PayOrder::~PayOrder()
{

}

dbuploadOrderReq PayOrder::uploaOrderdRecord(uploadOrderReq& req)
{
	dbuploadOrderReq dbreq;
	string key = req.user + "_" + req.phone;
	dbreq.user = req.user;
	dbreq.phone = req.phone;
	{
		std::istringstream ss{ req.date };
		std::chrono::sys_days tp;
#ifdef _WIN32
		ss >> std::chrono::parse("%F", tp);
#else
		ss >> date::parse("%F", tp);
#endif
		if (ss.bad())
		{
			SPDLOG_WARN("parse date {} failed for upload order", req.date);
		}
		dbreq.date = tp.time_since_epoch().count();
	}
	dbreq.groupname = req.groupname;

	for (auto it = req.orders.begin(); it != req.orders.end(); ++it)
	{
		auto fm = _parseUniformOrder(it->detail);
		if (DM.isOrderFinish(req.user, req.phone, req.groupname, req.date, it->nickname, std::to_string(fm.onum)))
		{
			continue;
		}
		if (DM.addOrder(req.user, req.phone, req.groupname, it->nickname, req.date, it->date, it->detail, fm.onum, std::to_string(fm.onum), fm.price))
		{
			dbuploadOrderRecordItem dbit;
			dbit.date = it->date;
			dbit.nickname = it->nickname;
			dbit.detail = it->detail;
			dbit.sordernum = std::to_string(dbit.ordernum);
			
			iguana::string_stream ss;
			iguana::json::to_json(ss, fm);
			dbit.formateddetail = ss.str();
			dbit.price = fm.price;
			dbit.ordernum = fm.onum;
			dbreq.orders.push_back(dbit);
		}
	}
	return dbreq;
}

dbuploadPayReq PayOrder::uploadPayRecord(uploadPayReq& req)
{

	string key = req.user + "_" + req.phone;
	dbuploadPayReq dbreq;
	dbreq.user = req.user;
	dbreq.phone = req.phone;
	dbreq.groupname = req.groupname;
	for (auto it = req.records.begin(); it != req.records.end(); ++it)
	{
		dbuploadPayRecordItem hh;
		hh.nickname = it->nickname;
		hh.money = _parsePriceFromHongBao(it->money);
		hh.ordernum = _parseIndexFromHBAttach(it->attach);
		hh.date = it->date;
		auto vv2 = DM.addPay(req.user, req.phone, req.groupname, it->nickname, it->recver, it->date, it->money, hh.money);
		string snum = std::get<0>(vv2);
		
		if (!snum.empty())
		{
			hh.sorderid = snum;
			dbreq.orders.push_back(hh);
			DM.markOrderFinish(req.user, req.phone, req.groupname, std::get<1>(vv2), it->nickname, snum);
		}
	}
	return dbreq;
}


void PayOrder::uploadPayRecordPost(uploadPayRep& rep)
{
	
}

awaitable<RouterFuncReturnType> PayOrder::getOrderPage(string req)
{
	RouterFuncReturnType ret;
	CTML::Document doc;
	tiny_utf8::utf8_string vv = "欢迎光临";
	CTML::Node orderTitle("fieldset");
	orderTitle.SetAttribute("class", "layui-elem-field layui-field-title");
	orderTitle.SetAttribute("style", "margin-top: 20px;");
	
	auto vv2 = vv.cpp_str();
	string txtwelcome(vv2.begin(), vv2.end());

	getMenuRep menus;

	getMenuReq menureq;

	map<string, string> kvs = SMUtils::parseQueryString(req);
	menureq.user = kvs["user"];
	menureq.phone = kvs["phone"];
	//first get shop name
	getGroupKeywordReq kreq;
	kreq.user = menureq.user;
	kreq.phone = menureq.phone;
	getGroupKeywordRep krep;
	co_await DBCMGRPT->executeQuery(kreq, krep, magic_enum::enum_integer(AppDB::getGroupKeyword));

	txtwelcome += " ";
	txtwelcome += krep.keyword;

	CTML::Node txttitle("legend", txtwelcome.c_str());
	orderTitle.AppendChild(txttitle);
	CTML::Node nEncode("meta");
	nEncode.SetAttribute("charset", "utf-8");

	CTML::Node nForm("form");
	nForm.SetAttribute("class", "layui-form");
	nForm.SetAttribute("lay-filter", "myorder");
	doc.AppendNodeToHead(layuijs);
	doc.AppendNodeToHead(layuicss);
	doc.AppendNodeToBody(orderTitle);
	doc.AppendNodeToHead(nEncode);
	doc.AppendNodeToHead(nviewport);
	doc.AppendNodeToHead(myjs);
	
	if (!menureq.user.empty())
	{
		co_await DBCMGRREF.executeQuery(menureq, menus, magic_enum::enum_integer(AppDB::getMenu));
	}
	if (menus.menu.empty())
	{
		CTML::Node v("p", "no m");
	}
	else
	{
		map<string, map<int, getMenuItem>> ms;
		for (const auto& it : menus.menu)
		{
			if (it.items.empty())
			{
				continue;
			}
			for (auto& itc : it.items)
			{
				ms[itc.categy].insert({ itc.id, itc });
				
			}
		}
		for (const auto& it : ms)
		{
			CTML::Node cdiv("div");
			cdiv.SetAttribute("class", "layui-form-item");
			CTML::Node ct("label", it.first);
			ct.SetAttribute("class", "layui-form-label");
			CTML::Node cdiv2("div");
			cdiv2.SetAttribute("class", "layui-input-block");
			cdiv.AppendChild(ct);
			
			for (const auto& it2 : it.second)
			{
				CTML::Node i("input");
				i.SetAttribute("type", "checkbox");
				i.SetAttribute("title", it2.second.name);
				i.SetAttribute("value", std::to_string(it2.second.price));
				i.SetAttribute("class", "oitem");
				i.SetAttribute("lay-filter", "switchTest");
				if (it2.second.defaultselect)
				{
					i.SetAttribute("checked", "");
				}
				cdiv2.AppendChild(i);
			}
			cdiv.AppendChild(cdiv2);
			nForm.AppendChild(cdiv);
		}
		//add seq option
		{
			tiny_utf8::utf8_string u8seq = "序号";
			auto vv = u8seq.cpp_str();
			auto strseq = string(vv.begin(), vv.end());
			CTML::Node cdiv("div");
			cdiv.SetAttribute("class", "layui-form-item");
			CTML::Node ct("label", strseq);
			ct.SetAttribute("class", "layui-form-label");
			CTML::Node cdiv2("div");
			cdiv2.SetAttribute("class", "layui-input-block");
			cdiv.AppendChild(ct);
			CTML::Node cselect("select");
			cselect.SetAttribute("lay-filter", "selectseq");
			cselect.SetAttribute("id", "selectseq");
			for (int i = 1; i < 51; ++i)
			{
				CTML::Node cop("option", std::to_string(i));
				cop.SetAttribute("value", std::to_string(i));
				cselect.AppendChild(cop);
			}
			cdiv2.AppendChild(cselect);
			cdiv.AppendChild(cdiv2);
			nForm.AppendChild(cdiv);
		}
		doc.AppendNodeToBody(nForm);
	}

	
	doc.AppendNodeToBody(txtOrderDate);

	doc.AppendNodeToBody(btnOk);
	doc.body().SetAttribute("onload", "restoreToday()");
	
	ret = std::make_shared<std::string>(doc.ToString(CTML::ToStringOptions(CTML::StringFormatting::MULTIPLE_LINES)));
	co_return ret;
}

awaitable<RouterFuncReturnType> PayOrder::getOrderPage2(string req)
{
	RouterFuncReturnType ret;
	getMenuRep menus;

	getMenuReq menureq;
	map<string, string> kvs = SMUtils::parseQueryString(req);
	menureq.user = kvs["user"];
	menureq.phone = kvs["phone"];
	//first get shop name
	getGroupKeywordReq kreq;
	kreq.user = menureq.user;
	kreq.phone = menureq.phone;
	getGroupKeywordRep krep;
	co_await DBCMGRREF.executeQuery(kreq, krep, magic_enum::enum_integer(AppDB::getGroupKeyword));
	if (!menureq.user.empty())
	{
		co_await DBCMGRREF.executeQuery(menureq, menus, magic_enum::enum_integer(AppDB::getMenu));
	}
	menus.title = krep.keyword;
	iguana::string_stream ss;
	iguana::json::to_json(ss, menus);
	auto stringv = ss.str();
	nlohmann::json j = nlohmann::json::parse(ss.str());
	ret = std::make_shared<string>(render::render_file("dist/template/orderpage.html", j));
	co_return ret;
}


static shared_ptr<date::year_month_day> _calcDate;

void PayOrder::init(ServeMode mode)
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

	myjs.SetAttribute("src", "myjs.js");
	layuijs.SetAttribute("src", "https://cdn.staticfile.org/layui/2.7.2/layui.js");
	layuicss.SetAttribute("href", "https://cdn.staticfile.org/layui/2.7.2/css/layui.css");
	layuicss.SetAttribute("rel", "stylesheet");
	nviewport.SetAttribute("name", "viewport");
	nviewport.SetAttribute("content", "width=device-width, initial-scale=1");

	txtOrderDate.SetAttribute("class", "layui-textarea");
	txtOrderDate.SetAttribute("id", "txtalldata");
	btnOk.SetAttribute("onclick", "test()");
	btnOk.SetAttribute("class", "layui-btn layui-btn-primary layui-border-blue");

	
}

awaitable<void> PayOrder::updateEveryDay()
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

int PayOrder::_parseIndexFromHBAttach(string attach)
{
	int ret = 0;
	ret = _parseIntAtStrHead(attach);
	return ret;
}

int PayOrder::_parsePriceFromHongBao(string o)
{
	int ret = 0;
	ret = _parseIntAtStrHead2(o);
	return ret;
}

int PayOrder::_parseIntAtStrHead2(string o)
{
	int ret = 0;
	auto v = std::remove_if(o.begin(), o.end(), [](unsigned char x) {return std::isspace(x); });
	o.erase(v, o.end());
	auto it = o.begin();
	int decimaicount = 0;
	bool bdecimail = 0;
	while (it != o.end())
	{
		if (*it >= '0' && *it <= '9')
		{
			ret *= 10;
			ret += *it - '0';
			if (bdecimail)
			{
				++decimaicount;
			}
		}
		else if (*it == '.')
		{
			bdecimail = true;
			++it;
			continue;
		}
		else
		{
			break;
		}
		++it;

	}
	while (decimaicount < 2)
	{
		ret *= 10;
		++decimaicount;
	}
	return ret;
}

const uniformOrderMenu& PayOrder::_parseUniformOrder(string& orderDetail)
{
	thread_local uniformOrderMenu ret;
	ret.items.clear();
	ret.onum = 0;
	ret.price = 0;
	auto parseResult = parse_plc_resp_str_(orderDetail.data(), (int)(orderDetail.length()));
	SPDLOG_INFO("parse order[{}] result {}", orderDetail, magic_enum::enum_name(parseResult));
	if(parseResult==orderFormat::invalidFormat)
	{
		SPDLOG_WARN("parse order [{}] failed", orderDetail);
	}
	ret.onum = _parse_order_num;
	ret.price = _total_price;
	for(int i=0; i< _menu_count;++i)
	{
		orderMenuItem v;
		v.name = _menus[i].name;
		v.price = _menus[i].price;
		ret.items.push_back(v);
	}
	ret.format = to_underlying(parseResult);
	return ret;
}

int PayOrder::_parseIntAtStrHead(string o)
{
	int ret = 0;
	auto v = std::remove_if(o.begin(), o.end(), [](unsigned char x) {return std::isspace(x); });
	o.erase(v, o.end());
	auto it = o.begin();
	while (it != o.end())
	{
		if (*it >= '0' && *it <= '9')
		{
			ret *= 10;
			ret += *it - '0';
		}
		else
		{
			break;
		}
		++it;

	}
	return ret;
}


