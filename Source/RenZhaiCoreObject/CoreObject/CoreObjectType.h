#pragma once

enum ECoreObjectFlag
{
	PreInitialization,
	Initialization,
	PostInitialization,
	DeletedPeriod,//这个对象准备被删除
};