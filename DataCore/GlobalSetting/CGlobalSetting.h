#pragma once

#include <thread>
#include "CMainViewpoint.h"
#include "CAdvancedSetting.h"
#include "CDataPannelGlobalSetting.h"
#include "CProjectModel.h"
#include "CProjectEffect.h"
#include "CProjectVehicleSimulation.h"
#include "CProjectVegetation.h"
#include "CProjectPOI.h"
#include "CProjectSimplePOI.h"
#include "CProjectPathBrowsing.h"
#include "CProjectAreaContour.h"
#include "CProjectWeather.h"
#include "CProjectDataPannel.h"

#define GetGlobalSettingInstance() CGlobalSetting::getInstance()

// 全局设置
class CGlobalSetting {
public:
	CGlobalSetting();
	~CGlobalSetting();

public:
	static CGlobalSetting &getInstance();

	static CGlobalSetting s_globalSetting;

public:
	// 重新加载所有全局数据
	void loadFromJson(cJSON *pJson);
	// 获取所有全局数据
	cJSON *toJson();

public:
	// 设置全局数据，需在Viewer初始化后使用
	void setGlobalSetting();

public:
	cJSON *mainViewpointToJson(cJSON *pJson);
	void updateMainViewpointFromJson(cJSON *pJson);

public:
	cJSON *projectTemplateToJson(RTE::SceneDataType sceneDataType);

	void addProjectTemplateFromJson(RTE::SceneDataType sceneDataType, cJSON *pJson);
	void updateProjectTemplateFromJson(RTE::SceneDataType sceneDataType, cJSON *pJson);
	void deleteProjectTemplateByResourceID(RTE::SceneDataType sceneDataType, std::string strResourceID);

	//修改第一人称数据
	void resetFirstPersonManipultor(std::string strData) { m_strFirstPersonManipultor = strData; }
	std::string getFirstPersonManipultor() { return m_strFirstPersonManipultor; }
	std::string getVerseGraphicsConfig() { return m_strVerseGraphicsConfig; } //废弃
	
	//获取环境光设置,可以获取当前的或者默认的配置
	int getVerseGraphicsConfig(std::string key, std::string& data);

	int getCurrentVerse(std::string& data);

	//切换环境光配置
	int changeVerseGraphicsConfig(std::string id, std::string& data);

	//还原初始配置
	int resetVerseGraphicsConfig(std::string& data);

	//获取实时光影状态
	bool getRealTimeLighting();
	//设置实时光影状态
	void setRealTimeLighting(bool bSet);

	//获取元素默认可视距参数
	int getGlobalLoadScale();
public:
	// 获取模板resourceId列表
	std::unordered_set<std::string> getResourceIDList(RTE::SceneDataType sceneDataType);
	// 获取模板信息，需要主动释放cJSON指针内容
	cJSON *getByResourceID(RTE::SceneDataType sceneDataType, std::string strResourceID);

	void  addRef(RTE::SceneDataType sceneDataType, std::string strResourceID); //模板资源引用加一
	void  releaseRef(RTE::SceneDataType sceneDataType, std::string strResourceID); //模板资源引用减一

	// 获取模板指定字段信息，需要主动释放cJSON指针内容
	cJSON *getByName(RTE::SceneDataType sceneDataType, std::string strResourceID, std::string strName);

public:
	std::shared_ptr <CAdvancedSetting> m_pAdvancedSetting;
private:
	std::shared_ptr<CMainViewpoint> m_pMainViewpoint;
	std::shared_ptr <CDataPannelGlobalSetting> m_pDataPannelGlobalSetting;

	std::shared_ptr <CProjectModel> m_pProjectModel;
	std::shared_ptr <CProjectEffect> m_projectEffect;
	std::shared_ptr <CProjectVehicleSimulation> m_pProjectVehicleSimulation;

	std::shared_ptr <CProjectVegetation> m_pProjectVegetation;

	std::shared_ptr <CProjectPOI> m_pProjectPOI;
	std::shared_ptr <CProjectSimplePOI> m_pProjectSimplePOI;

	std::shared_ptr <CProjectPathBrowsing> m_pProjectPathBrowsing;
	std::shared_ptr <CProjectAreaContour> m_pProjectAreaContour;

	std::shared_ptr <CProjectWeather> m_pProjectWeather;

	std::shared_ptr<CVerseGraphicsConfig> m_pVerseGraphicsConfig;
	std::shared_ptr <CProjectDataPannel> m_pProjectDataPannel; //新版数据面片模板数据

private:
	std::unordered_map<std::string, /*std::shared_ptr<IGlobalSetting>*/IGlobalSetting*> m_mapGlobalSetting;
	std::unordered_map<RTE::SceneDataType, IProjectTemplate*> m_mapProjectTemplate;

private:
	std::string m_strResourceID;
	std::string m_strVersion;
	std::vector<int> m_vecVersion;
	int m_nLong;
	std::string m_strName;
	std::string m_strGuid;
	std::string m_strURLList;
	int m_nLat;
	std::string m_strGlobalVersion;
	std::string m_strFirstPersonManipultor;
	std::string m_strVerseGraphicsConfig;
};