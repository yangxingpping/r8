#pragma once
/*****************************************************************//**
 * \file   GroupKeyword.h
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

class GroupKeyword :  public HttpCmdTag<GroupKeyword, AppMainCmd>
{
public:
    BEGIN_ROUTER_MAP(AppMainCmd::GroupKeywords);
    ROUTER_NEED_DB_JSON(addGroupReq, addGroupRep, R8AssGate::addGroup, AppDB::addGroup);
    ROUTER_NEED_DB_JSON(removeGroupReq, removeGroupRep, R8AssGate::removeGroup, AppDB::removeGroup);
    ROUTER_NEED_DB_JSON(getWeixinGroupReq, getWeixinGroupRep, R8AssGate::getGroup, AppDB::getGroup);
    ROUTER_NEED_DB_JSON(getGroupKeywordReq, getGroupKeywordRep, R8AssGate::getGroupKeyword, AppDB::getGroupKeyword);
    ROUTER_NEED_DB_JSON(setGroupKeywordReq, setGroupKeywordRep, R8AssGate::setGroupKeyword, AppDB::setGroupKeyword);
    ROUTER_NEED_DB_JSON(getGreetStateReq, getGreetStateRep, R8AssGate::getGreetState, AppDB::getGreetState);
    ROUTER_NEED_DB_JSON(setGreetStateReq, setGreetStateRep, R8AssGate::setGreetState, AppDB::setGreetState);
    END_ROUTER_MAP;
    virtual ~GroupKeyword();


   
    void init(ServeMode mode) override;

    awaitable<void> updateEveryDay();

};

