#pragma once
/*****************************************************************//**
 * \file   DealRecord.h
 * \brief  gateway interfaces for http requests.
 * 
 * \author alqaz
 * \date   April 2022
 *********************************************************************/

#include "coros.h"
#include "database.h"
#include "wss.h"
#include "Utils.h"
#include "reqreps.h"
#include "myreqreps.h"
#include "myenum.h"
#include "HttpCmdTag.h"
#include "taskflow/taskflow.hpp"
#include <string>
#include <string_view>
#include <memory>

class DBManager;

class DealRecord :  public HttpCmdTag<DealRecord, AppMainCmd>
{
public:
    BEGIN_ROUTER_MAP(AppMainCmd::UploadRecord);
    ROUTER_NEED_DB_JSON(getGroupKeywordReq, getGroupKeywordRep, R8AssGate::getGroupKeyword, AppDB::getGroupKeyword);
    ROUTER_NEED_DB_JSON(getCategyReq, getCategyRep, R8AssGate::getCategy, AppDB::getCategy);
    ROUTER_NEED_DB_JSON(getMenuReq, getMenuRep, R8AssGate::getMenu, AppDB::getMenu);
    ROUTER_RETURN_STR_HTML(&DealRecord::getOrderPage, R8AssGate::getOrderPage);
    ROUTER_RETURN_STR_HTML(&DealRecord::getOrderPage2, R8AssGate::getOrderPage2);
    END_ROUTER_MAP;
    virtual ~DealRecord();

 
    bool  getGroupKeyword(getGroupKeywordReq& req);
    bool getGreetState(getGreetStateReq& req);
    bool getCategory(getCategyReq& req);
    bool getMenus(getMenuReq& req);
    awaitable<RouterFuncReturnType> getOrderPage(string req);
    awaitable<RouterFuncReturnType> getOrderPage2(string req);

   
    void init(ServeMode mode) override;
private:
    PackType _packtype;
};

