#pragma once
/*****************************************************************//**
 * \file   PayOrder.h
 * \brief  gateway interfaces for http requests.
 * 
 * \author alqaz
 * \date   April 2022
 *********************************************************************/

#include "asio/awaitable.hpp"
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

class PayOrder :  public HttpCmdTag<PayOrder, AppMainCmd>
{
public:
    BEGIN_ROUTER_MAP(AppMainCmd::PayOrders);
    ROUTER_NEED_DBREQ_DB_JSON(&PayOrder::uploaOrderdRecord, uploadOrderReq, dbuploadOrderReq, uploadOrderRep, R8AssGate::upLoadOrderRecord, AppDB::upLoadOrderRecord);
    ROUTER_NEED_DBREQ_DB_DBREP_JSON(&PayOrder::uploadPayRecord, &PayOrder::uploadPayRecordPost, uploadPayReq, dbuploadPayReq, uploadPayRep, R8AssGate::upLoadPayRecord, AppDB::upLoadPayRecord);
    ROUTER_NEED_DB_JSON(getOrderReq, getOrderRep, R8AssGate::getOrderRecord, AppDB::getOrder);
    ROUTER_NEED_DB_JSON(getOrderPrintReq, getOrderPrintRep, R8AssGate::getOrderPrint, AppDB::getOrderPrint);
    ROUTER_NEED_DB_JSON(getDayProfileReq, getDayProfileRep, R8AssGate::getDayProfile, AppDB::getDayProfile);
	ROUTER_RETURN_STR_HTML(&PayOrder::getOrderPage, R8AssGate::getOrderPage);
    ROUTER_RETURN_STR_HTML(&PayOrder::getOrderPage2, R8AssGate::getOrderPage2);
    END_ROUTER_MAP;
    virtual ~PayOrder();

    dbuploadOrderReq uploaOrderdRecord(uploadOrderReq& req);
    dbuploadPayReq uploadPayRecord(uploadPayReq& req);
    void uploadPayRecordPost(uploadPayRep& rep);
    awaitable<RouterFuncReturnType> getOrderPage(string req);
    awaitable<RouterFuncReturnType> getOrderPage2(string req);

    void init(ServeMode mode) override;

    awaitable<void> updateEveryDay();

private:
    int _parseIndexFromHBAttach(string attach);
    int _parsePriceFromHongBao(string hongbaoDetail);
    int _parseIntAtStrHead(string str);
    int _parseIntAtStrHead2(string str);
    const uniformOrderMenu& _parseUniformOrder(string& orderDetail);
    awaitable<void> _loadAllOrdersFromDB(OrderType type=OrderType::AllOrder);
   
};

