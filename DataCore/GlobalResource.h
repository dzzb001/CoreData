#pragma once
#include "CJsonBase.h"
#include <string>


//提供增删改查接口
class CGlobalContent : public CJson_base
{
public:
	explicit CGlobalContent(cJSON* p);
public:
	//获取场景视点
	std::string GetJsonByName(const char *sType);

	/*
	* viewPoint
	* projectPOI 模板
	* projectVehicleSimulation
	* projectWeather
	* projectSimplePoi
	* advancedSettings
	* name
	* globalSetting
	* projectEffect
	* projectVegetations
	* urlList 所有模型url地址
	* projectModels
	*/
};

