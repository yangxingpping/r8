#include "DealRecord.h"
#include "myreqreps.h"
#include "enums.h"
#include "Utils.h"
#include "DBConnectManager.h"
#include "spdlog/spdlog.h"
#include "networkinterface.h"
#include "CoEvent.h"


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

static CTML::Node layuijs("script");
static CTML::Node layuicss("link");
static CTML::Node nviewport("meta");
static CTML::Node myjs("script");

static CTML::Node btnOk("button", "copy");
static CTML::Node txtOrderDate("textarea");

DealRecord::~DealRecord()
{

}



bool DealRecord::getGreetState(getGreetStateReq& req)
{
	bool bret = true;

	return bret;
}


bool DealRecord::getCategory(getCategyReq& req)
{
	bool bret = true;

	return bret;
}

bool DealRecord::getGroupKeyword(getGroupKeywordReq& req)
{
	bool bret = true;

	return bret;
}


bool DealRecord::getMenus(getMenuReq& req)
{
	bool bret = true;

	return bret;
}

awaitable<RouterFuncReturnType> DealRecord::getOrderPage(string req)
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
	
	ret = std::make_shared<string>(doc.ToString(CTML::ToStringOptions(CTML::StringFormatting::MULTIPLE_LINES)));
	co_return ret;
}

awaitable<RouterFuncReturnType> DealRecord::getOrderPage2(string req)
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

void DealRecord::init(ServeMode mode)
{
	SPDLOG_INFO("start as mode {}", magic_enum::enum_name(mode));
}
