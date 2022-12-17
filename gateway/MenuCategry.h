#pragma once
/*****************************************************************//**
 * \file   MenuCategry.h
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

class MenuCategry :  public HttpCmdTag<MenuCategry, AppMainCmd>
{
public:
    BEGIN_ROUTER_MAP(AppMainCmd::CategryMenus);
    ROUTER_NEED_DB_JSON(addCategyReq, addCategyRep, R8AssGate::addCategy, AppDB::addCategy);
    ROUTER_NEED_DB_JSON(removeCategyReq, removeCategyRep, R8AssGate::removeCategy, AppDB::removeCategy);
    ROUTER_NEED_DB_JSON(addMenuReq, addMenuRep, R8AssGate::addMenu, AppDB::addMenu);
    ROUTER_NEED_DB_JSON(removeMenuReq, removeMenuRep, R8AssGate::removeMenu, AppDB::removeMenu);
    ROUTER_NEED_DB_JSON(getCategyReq, getCategyRep, R8AssGate::getCategy, AppDB::getCategy);
    ROUTER_NEED_DB_JSON(getMenuReq, getMenuRep, R8AssGate::getMenu, AppDB::getMenu);
    ROUTER_NEED_DB_JSON(updateMenuCategyReq, updateMenuCategyRep, R8AssGate::updateCategy, AppDB::updateMenuCategy);
    ROUTER_NEED_DB_JSON(deleteMenuCategyReq, deleteMenuCategyRep, R8AssGate::deleteCategy, AppDB::deleteMenuCategy);
    ROUTER_NEED_DB_JSON(updateMenuReq, updateMenuRep, R8AssGate::updateMenu, AppDB::updatMenu);
    END_ROUTER_MAP;
    virtual ~MenuCategry();


    void init(ServeMode mode) override;

    awaitable<void> updateEveryDay();

  
};

