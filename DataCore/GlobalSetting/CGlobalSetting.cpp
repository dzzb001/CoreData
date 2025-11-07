#include "CGlobalSetting.h"

CGlobalSetting CGlobalSetting::s_globalSetting;

// 全局设置
CGlobalSetting::CGlobalSetting()
	: m_pMainViewpoint(std::make_shared<CMainViewpoint>()),
	m_pAdvancedSetting(std::make_shared<CAdvancedSetting>()),
	m_pDataPannelGlobalSetting(std::make_shared<CDataPannelGlobalSetting>()),
	m_pProjectModel(std::make_shared<CProjectModel>()),
	m_projectEffect(std::make_shared<CProjectEffect>()),
	m_pProjectVehicleSimulation(std::make_shared<CProjectVehicleSimulation>()),
	m_pProjectVegetation(std::make_shared<CProjectVegetation>()),
	m_pProjectPOI(std::make_shared<CProjectPOI>()),
	m_pProjectSimplePOI(std::make_shared<CProjectSimplePOI>()),
	m_pProjectPathBrowsing(std::make_shared<CProjectPathBrowsing>()),
	m_pProjectAreaContour(std::make_shared<CProjectAreaContour>()),
	m_pProjectWeather(std::make_shared<CProjectWeather>()),
	m_pVerseGraphicsConfig(std::make_shared<CVerseGraphicsConfig>()),
	m_pProjectDataPannel(std::make_shared<CProjectDataPannel>()),
	m_strResourceID(""),
	m_strVersion(""),
	m_nLong(0),
	m_strName(""),
	m_strGuid(""),
	m_strURLList(""),
	m_nLat(0),
	m_strGlobalVersion(""),
	m_strFirstPersonManipultor(""),
	m_strVerseGraphicsConfig("")
{
	//m_pAdvancedSetting = nullptr;
	m_mapGlobalSetting["viewPoint"] = dynamic_cast<IGlobalSetting*>(m_pMainViewpoint.get());
	m_mapGlobalSetting["advancedSettings"] = dynamic_cast<IGlobalSetting*>(m_pAdvancedSetting.get());
	m_mapGlobalSetting["globalSetting"] = dynamic_cast<IGlobalSetting*>(m_pDataPannelGlobalSetting.get());
	m_mapGlobalSetting["projectModels"] = dynamic_cast<IGlobalSetting*>(m_pProjectModel.get());
	m_mapGlobalSetting["projectEffect"] = dynamic_cast<IGlobalSetting*>(m_projectEffect.get());
	m_mapGlobalSetting["projectVehicleSimulation"] = dynamic_cast<IGlobalSetting*>(m_pProjectVehicleSimulation.get());
	m_mapGlobalSetting["projectVegetations"] = dynamic_cast<IGlobalSetting*>(m_pProjectVegetation.get());
	m_mapGlobalSetting["projectPOI"] = dynamic_cast<IGlobalSetting*>(m_pProjectPOI.get());
	m_mapGlobalSetting["projectSimplePoi"] = dynamic_cast<IGlobalSetting*>(m_pProjectSimplePOI.get());
	m_mapGlobalSetting["projectWeather"] = dynamic_cast<IGlobalSetting*>(m_pProjectWeather.get());
	m_mapGlobalSetting["verseGraphicsConfig"] = dynamic_cast<IGlobalSetting*>(m_pVerseGraphicsConfig.get());
	m_mapGlobalSetting["projectDataPannel"] = dynamic_cast<CProjectDataPannel*>(m_pProjectDataPannel.get());
	m_mapProjectTemplate[RTE::SDT_ModelBase] = dynamic_cast<IProjectTemplate*>(m_pProjectModel.get());
	m_mapProjectTemplate[RTE::SDT_SceneEffects] = dynamic_cast<IProjectTemplate*>(m_projectEffect.get());
	m_mapProjectTemplate[RTE::SDT_VehicleSimulation] = dynamic_cast<IProjectTemplate*>(m_pProjectVehicleSimulation.get());
	m_mapProjectTemplate[RTE::SDT_Vegetation] = dynamic_cast<IProjectTemplate*>(m_pProjectVegetation.get());
	m_mapProjectTemplate[RTE::SDT_POI] = dynamic_cast<IProjectTemplate*>(m_pProjectPOI.get());
	m_mapProjectTemplate[RTE::SDT_SimplePoi] = dynamic_cast<IProjectTemplate*>(m_pProjectSimplePOI.get());
	m_mapProjectTemplate[RTE::SDT_PathBrowsing] = dynamic_cast<IProjectTemplate*>(m_pProjectPathBrowsing.get());
	m_mapProjectTemplate[RTE::SDT_AreaContour] = dynamic_cast<IProjectTemplate*>(m_pProjectAreaContour.get());
	m_mapProjectTemplate[RTE::SDT_verseGraphicsConfig] = dynamic_cast<IProjectTemplate*>(m_pVerseGraphicsConfig.get());
	m_mapProjectTemplate[RTE::SDT_DataPannel] = dynamic_cast<IProjectTemplate*>(m_pProjectDataPannel.get());
}

CGlobalSetting::~CGlobalSetting() {
}

CGlobalSetting &CGlobalSetting::getInstance() {
	return s_globalSetting;
}

void CGlobalSetting::loadFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			cJSON_LoadFromObject(pJson, "resourceId", m_strResourceID);
			cJSON_LoadFromObject(pJson, "version", m_strVersion);

			/*Poco::StringTokenizer tokenizer(m_strVersion, ".", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);

			for (const auto& token : tokenizer) {
				std::cout << token << std::endl;
			}*/

			cJSON_LoadFromObject(pJson, "long", m_nLong);
			cJSON_LoadFromObject(pJson, "name", m_strName);
			cJSON_LoadFromObject(pJson, "guid", m_strGuid);
			cJSON_LoadFromObject(pJson, "urlList", m_strURLList);
			cJSON_LoadFromObject(pJson, "lat", m_nLat);
			cJSON_LoadFromObject(pJson, "globalVersion", m_strGlobalVersion);
			cJSON_LoadFromObject(pJson, "firstPersonManipultor", m_strFirstPersonManipultor);
			//cJSON_LoadFromObject(pJson, "verseGraphicsConfig", m_strVerseGraphicsConfig);

			for (auto [strKey, pValue] : m_mapGlobalSetting) 
			{
				cJSON *pJsonGlobalSetting = cJSON_GetObjectItem(pJson, strKey.c_str());

				//test CVerseGraphicsConfig begin
				if (!strKey.compare("verseGraphicsConfig"))
				{
					if (pJsonGlobalSetting != nullptr) {
						if (cJSON_IsObject(pJsonGlobalSetting) || cJSON_IsArray(pJsonGlobalSetting)) {
							pValue->loadFromJson(pJsonGlobalSetting);
						}
					}
					continue;
				}

				//
				if (pJsonGlobalSetting != nullptr) 
				{
					if (cJSON_IsObject(pJsonGlobalSetting) || cJSON_IsArray(pJsonGlobalSetting)) {
						pValue->loadFromJson(pJsonGlobalSetting);
					}
				}
			}

			m_pProjectPathBrowsing->loadFromJson(nullptr);
			m_pProjectAreaContour->loadFromJson(nullptr);
		}
	}

	return ;
}

cJSON *CGlobalSetting::toJson() {
	cJSON *pJson = cJSON_CreateObject();
	if (pJson != nullptr) {
		cJSON_AddToObject(pJson, "resourceId", m_strResourceID);
		cJSON_AddToObject(pJson, "version", m_strVersion);
		cJSON_AddToObject(pJson, "long", m_nLong);
		cJSON_AddToObject(pJson, "name", m_strName);
		cJSON_AddToObject(pJson, "guid", m_strGuid);
		cJSON_AddToObject(pJson, "urlList", m_strURLList);
		cJSON_AddToObject(pJson, "lat", m_nLat);
		cJSON_AddToObject(pJson, "globalVersion", m_strGlobalVersion);
		cJSON_AddToObject(pJson, "firstPersonManipultor", m_strFirstPersonManipultor);
		//cJSON_AddToObject(pJson, "verseGraphicsConfig", m_strVerseGraphicsConfig);

		for (auto [strKey, pValue] : m_mapGlobalSetting) {
			cJSON *pJsonGlobalSetting = pValue->toJson();
			if (pJsonGlobalSetting != nullptr) {
				cJSON_AddItemToObject(pJson, strKey.c_str(), pJsonGlobalSetting);
			}
		}
	}

	return pJson;
}

void CGlobalSetting::setGlobalSetting() {
	m_pMainViewpoint->setMainViewpoint();
	if(m_pAdvancedSetting)
		m_pAdvancedSetting->setAdvancedSetting();
	m_pDataPannelGlobalSetting->setDataPannelGlobalSetting();

	return ;
}

cJSON *CGlobalSetting::mainViewpointToJson(cJSON *pJson) {
	return m_pMainViewpoint->mainViewpointToJson(pJson);
}

void CGlobalSetting::updateMainViewpointFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			m_pMainViewpoint->updateMainViewpointFromJson(pJson);
		}
	}

	return ;
}

cJSON *CGlobalSetting::projectTemplateToJson(RTE::SceneDataType sceneDataType) {
	cJSON *pJson = nullptr;

	auto iter = m_mapProjectTemplate.find(sceneDataType);
	if (iter != m_mapProjectTemplate.end()) {
		pJson = iter->second->toJson();
	}

	return pJson;
}

void CGlobalSetting::addProjectTemplateFromJson(RTE::SceneDataType sceneDataType, cJSON *pJson) {
	auto iter = m_mapProjectTemplate.find(sceneDataType);
	if (iter != m_mapProjectTemplate.end()) {
		if (pJson != nullptr) {
			if (cJSON_IsObject(pJson)) {
				iter->second->addFromJson(pJson);
			}
		}
	}

	return;
}

void CGlobalSetting::updateProjectTemplateFromJson(RTE::SceneDataType sceneDataType, cJSON *pJson) {
	auto iter = m_mapProjectTemplate.find(sceneDataType);
	if (iter != m_mapProjectTemplate.end()) {
		if (pJson != nullptr) {
			if (cJSON_IsObject(pJson)) {
				iter->second->updateFromJson(pJson);
			}
		}
	}

	return ;
}

void CGlobalSetting::deleteProjectTemplateByResourceID(RTE::SceneDataType sceneDataType, std::string strResourceID) {
	auto iter = m_mapProjectTemplate.find(sceneDataType);
	if (iter != m_mapProjectTemplate.end()) {
		iter->second->deleteByResourceID(strResourceID);
	}

	return ;
}

std::unordered_set<std::string> CGlobalSetting::getResourceIDList(RTE::SceneDataType sceneDataType) {
	std::unordered_set<std::string> setResourceID;

	auto iter = m_mapProjectTemplate.find(sceneDataType);
	if (iter != m_mapProjectTemplate.end()) {
		setResourceID = iter->second->getResourceIDList();
	}

	return setResourceID;
}

cJSON *CGlobalSetting::getByResourceID(RTE::SceneDataType sceneDataType, std::string strResourceID) {
	cJSON *pJson = nullptr;

	auto iter = m_mapProjectTemplate.find(sceneDataType);
	if (iter != m_mapProjectTemplate.end()) {
		pJson = iter->second->getByResourceID(strResourceID);
	}

	return pJson;
}

void CGlobalSetting::addRef(RTE::SceneDataType sceneDataType, std::string strResourceID)
{
	auto iter = m_mapProjectTemplate.find(sceneDataType);
	if (iter != m_mapProjectTemplate.end()) {
		iter->second->addRef(strResourceID);
	}
}

void CGlobalSetting::releaseRef(RTE::SceneDataType sceneDataType, std::string strResourceID)
{
	auto iter = m_mapProjectTemplate.find(sceneDataType);
	if (iter != m_mapProjectTemplate.end()) {
		iter->second->releaseRef(strResourceID);
	}
}

cJSON *CGlobalSetting::getByName(RTE::SceneDataType sceneDataType, std::string strResourceID, std::string strName) {
	cJSON *pJson = nullptr;

	auto iter = m_mapProjectTemplate.find(sceneDataType);
	if (iter != m_mapProjectTemplate.end()) {
		pJson = iter->second->getByName(strResourceID, strName);
	}

	return pJson;
}

//获取指定模板环境光设置
int CGlobalSetting::getVerseGraphicsConfig(std::string key, std::string& data)
{
	if (m_pVerseGraphicsConfig) {
		return m_pVerseGraphicsConfig->getVerseGraphicsConfig(key, data);
	}
	return -1;
}

//获取指定当前环境光设置
int CGlobalSetting::getCurrentVerse( std::string& data)
{
	if (m_pVerseGraphicsConfig) {
		m_pVerseGraphicsConfig->getCurrentVerse(data);
		return 0;
	}
	return -1;
}

//切换环境光配置
int CGlobalSetting::changeVerseGraphicsConfig(std::string id, std::string& data)
{
	if (m_pVerseGraphicsConfig) {
		return m_pVerseGraphicsConfig->changeCurVerseGraphicsConfig(id,data);
	}
	else
		return -1;
}

int CGlobalSetting::resetVerseGraphicsConfig(std::string& data)
{
	if (m_pVerseGraphicsConfig) {
		return m_pVerseGraphicsConfig->resetVerseGraphicsConfig(data);
	}
	else
		return -1;
}

//获取实时光影状态
bool CGlobalSetting::getRealTimeLighting()
{
	if (m_pVerseGraphicsConfig) {
		return m_pVerseGraphicsConfig->getRealTimeLighting();
	}
	else
		return false;
}

//设置实时光影状态
void CGlobalSetting::setRealTimeLighting(bool bSet)
{
	if (m_pVerseGraphicsConfig) {
			m_pVerseGraphicsConfig->setRealTimeLighting(bSet);
	}
}

//获取元素默认可视距参数
int CGlobalSetting::getGlobalLoadScale()
{
	if (m_pAdvancedSetting)
		return m_pAdvancedSetting->getGlobalLodScale();
	else
		return 25;
}
