
#pragma once

#include "enums.h"

enum class AppMainCmd
{
	R8Begin = to_underlying(MainCmd::MainCmdEnd)+1,
	UploadRecord,
	PayOrders,
	CategryMenus,
	GroupKeywords,
	GetRecord,
	R8End,
};

enum class AppDB
{
	AppDBBegin = to_underlying(AssDB::SysDBEnd) + 1,
	RedisBegin,
	getGroupKeyword,
	setGroupKeyword,
	RedisEnd,

	R8Begin,
	addGroup,
	removeGroup,
	getGroup,
	addCategy,
	removeCategy,
	getCategy,
	addMenu,
	updatMenu,
	removeMenu,
	upLoadOrderRecord,
	upLoadPayRecord,
	getOrder,
	getGreetState,
	setGreetState,
	getMenu,
	updateMenuCategy,
	deleteMenuCategy,
	getOrderPrint,
	getDayProfile,
	R8End,
	AppDBEnd,
};

enum class R8AssGate
{
    addGroup,
    removeGroup,
    getGroup,
    addCategy,
    removeCategy,
    addMenu,
    getCategy,
    updateCategy,
    deleteCategy,
    removeMenu,
    upLoadOrderRecord,
    upLoadPayRecord,
    getOrderRecord,
    getPayInfo,
    getGroupKeyword,
    setGroupKeyword,
    getGreetState,
    setGreetState,
    getMenu,
    updateMenu,
    getOrderPrint,
    getDayProfile,
    getAllUser,
    getOrderPage,
    getOrderPage2,
};

enum class R8StatusCode
{
    categryAlreadyExistWhenCreate = to_underlying(statusCode::MaxCommStatusCode) + 1,
    categryNotExistWhenUpdate,
};
