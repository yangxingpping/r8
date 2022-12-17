
#pragma once

#include "templatefuncs.h"
#include "TransCmdTag.h"
#include "database.h"
#include "enums.h"
#include "myreqreps.h"
#include "myenum.h"
#include "coros.h"
#include <string_view>
using std::string;
using std::string_view;
using asio::awaitable;

class DBProcess : public TransCmdTag<DBProcess, MainCmd>
{
    public:
    BEGIN_ROUTER_MAP_DB(MainCmd::DBQuery);
    ROUTER_DB3(&DBProcess::suploadOrderRecord, AppDB::upLoadOrderRecord);
      ROUTER_DB3(&DBProcess::saddGroup, AppDB::addGroup);
      ROUTER_DB3(&DBProcess::sremoveGroup, AppDB::removeGroup);
      ROUTER_DB3(&DBProcess::sgetOrder, AppDB::getOrder);
      ROUTER_DB3(&DBProcess::sgetGroup, AppDB::getGroup);
      ROUTER_DB_TRANS_NO_PRE_POST(&DBProcess::sgetGroupKeyword, getGroupKeywordReq, getGroupKeywordRep, AppDB::getGroupKeyword);
      ROUTER_DB3(&DBProcess::ssetGroupKeyword, AppDB::setGroupKeyword);
      ROUTER_DB3(&DBProcess::sgetGreetState, AppDB::getGreetState);
      ROUTER_DB3(&DBProcess::ssetGreetState, AppDB::setGreetState);
      ROUTER_DB3(&DBProcess::sgetCategy, AppDB::getCategy);
      ROUTER_DB3(&DBProcess::sgetMenu, AppDB::getMenu);
      ROUTER_DB3(&DBProcess::saddCategy, AppDB::addCategy);
      ROUTER_DB3(&DBProcess::saddMenu, AppDB::addMenu);
      ROUTER_DB3(&DBProcess::supdateMenuCategy, AppDB::updateMenuCategy);
      ROUTER_DB3(&DBProcess::sdeleteMenuCategy, AppDB::deleteMenuCategy);
      ROUTER_DB3(&DBProcess::supdatMenu, AppDB::updatMenu);
      ROUTER_DB3(&DBProcess::suploadPay, AppDB::upLoadPayRecord);
      ROUTER_DB3(&DBProcess::sgetDayProfile, AppDB::getDayProfile);
      ROUTER_DB_TRANS_NO_PRE_POST(&DBProcess::sgetOrderPrint, getOrderPrintReq, getOrderPrintRep, AppDB::getOrderPrint);
      ROUTER_DB3(&DBProcess::sremoveMenu, AppDB::removeMenu);
    END_ROUTER_MAP_DB;
    public:

	dbuploadOrderRep suploadOrderRecord(dbuploadOrderReq req);
	addGroupRep saddGroup(addGroupReq req);
	removeGroupRep sremoveGroup(removeGroupReq req);
	addMenuRep saddMenu(addMenuReq req);
	removeMenuRep sremoveMenu(removeMenuReq req);
	getOrderRep sgetOrder(getOrderReq req);
	getWeixinGroupRep sgetGroup(getWeixinGroupReq req);
	getGroupKeywordRep sgetGroupKeyword(getGroupKeywordReq req);
	setGroupKeywordRep ssetGroupKeyword(setGroupKeywordReq req);
	getGreetStateRep sgetGreetState(getGreetStateReq req);
	setGreetStateRep ssetGreetState(setGreetStateReq req);
	getCategyRep sgetCategy(getCategyReq req); 
	getMenuRep sgetMenu(getMenuReq req);
	addCategyRep saddCategy(addCategyReq req);
	updateMenuCategyRep supdateMenuCategy(updateMenuCategyReq req);
	deleteMenuCategyRep sdeleteMenuCategy(deleteMenuCategyReq req);
	updateMenuRep supdatMenu(updateMenuReq req);
	uploadPayRep suploadPay(dbuploadPayReq req);
	getOrderPrintRep sgetOrderPrint(getOrderPrintReq req);
    getDayProfileRep sgetDayProfile(getDayProfileReq req);
    virtual void init(ServeMode mode) override {}
private:
    int getGroupIdByName(string user, string phone, string name); 
};
