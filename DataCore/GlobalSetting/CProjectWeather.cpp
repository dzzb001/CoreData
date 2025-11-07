#include "CProjectWeather.h"

// 天气
CProjectWeather::CProjectWeather() : IGlobalSetting() {
	return;
}

CProjectWeather::~CProjectWeather() {
	return;
}

void CProjectWeather::loadFromJson(cJSON *pJson) {
	return;
}

cJSON *CProjectWeather::toJson() {
	return cJSON_CreateObject();
}

//渲染效果
CVerseGraphicsConfig::CVerseGraphicsConfig() : IProjectTemplate() {
	return;
}

CVerseGraphicsConfig::~CVerseGraphicsConfig() {
	std::lock_guard<std::mutex> lock(m_mutexMap);
	for (auto [strKey, pJson] : m_mapGraphics)
	{
		if (pJson)
			cJSON_Delete(pJson);
	}
	return;
}

void CVerseGraphicsConfig::loadFromJson(cJSON* pJson) {
	std::lock_guard<std::mutex> lock(m_mutexMap);
	if (pJson) {
		cJSON_LoadFromObject(pJson, "realTimeLighting", m_bRealTimeLighting);
		cJSON_LoadFromObject(pJson, "currentVerse", m_currentVerse);

		cJSON_LoadFromObject(pJson, "enableTAA", enableTAA);
		cJSON_LoadFromObject(pJson, "beiJingTime", beiJingTime);
		cJSON* pArray = cJSON_GetObjectItem(pJson, "data");
		if (pArray) {
			int nSize = cJSON_GetArraySize(pArray);
			for (size_t i = 0; i < nSize; i++)
			{
				cJSON* pItem = cJSON_GetArrayItem(pArray, i);
				std::string id;
				cJSON_LoadFromObject(pItem, "id", id);

				m_mapGraphics[id] = cJSON_Duplicate(pItem, true);
			}
		}
	}
	return;
}

cJSON* CVerseGraphicsConfig::toJson() {
	std::lock_guard<std::mutex> lock(m_mutexMap);
	cJSON* pJson = cJSON_CreateObject();
	cJSON* pArray = cJSON_CreateArray();

	cJSON_AddStringToObject(pJson, "currentVerse", m_currentVerse.c_str());
	for (auto [strKey, pJson] : m_mapGraphics)
	{
		cJSON_AddItemToArray(pArray, cJSON_Duplicate(pJson, true));
	}

	cJSON_AddItemToObject(pJson, "data", pArray);
	cJSON_AddBoolToObject(pJson, "enableTAA", enableTAA);
	cJSON_AddBoolToObject(pJson, "realTimeLighting", m_bRealTimeLighting);
	cJSON_AddNumberToObject(pJson, "beiJingTime", beiJingTime);

	return pJson;
}

void CVerseGraphicsConfig::addFromJson(cJSON* pJson)
{

}

void CVerseGraphicsConfig::updateFromJson(cJSON* pJson)
{

}

void CVerseGraphicsConfig::deleteByResourceID(std::string strResourceID)
{

}

std::unordered_set<std::string> CVerseGraphicsConfig::getResourceIDList()
{
	std::lock_guard<std::mutex> lock(m_mutexMap);
	std::unordered_set<std::string> setResourceID;

	for (auto [strKey, pJson] : m_mapGraphics) {
		setResourceID.insert(strKey);
	}
	return setResourceID;
}

cJSON* CVerseGraphicsConfig::getByResourceID(std::string strResourceID)
{
	std::lock_guard<std::mutex> lock(m_mutexMap);
	auto itor = m_mapGraphics.find(strResourceID);
	if (itor != m_mapGraphics.end())
	{
		return cJSON_Duplicate(itor->second, true);
	}
	return nullptr;
}

cJSON* CVerseGraphicsConfig::getByName(std::string strResourceID, std::string strName)
{
	return nullptr;
}

int CVerseGraphicsConfig::getVerseGraphicsConfig(std::string key, std::string& data)
{
	std::lock_guard<std::mutex> lock(m_mutexMap);
	for (auto itor : m_mapGraphics)
	{
		bool bKey = false;
		cJSON_LoadFromObject(itor.second, key.c_str(), bKey);
		if (bKey)
		{
			cJSON_LoadFromObject(itor.second, "data", data);
			return 0;
		}
	}
	return -1;
}

int CVerseGraphicsConfig::changeCurVerseGraphicsConfig(std::string id, std::string &data)
{
	std::lock_guard<std::mutex> lock(m_mutexMap);
	auto itor = m_mapGraphics.find(id);

	if (itor != m_mapGraphics.end())
	{
		for (auto itor2 : m_mapGraphics)
		{
			cJSON* pTemp = itor2.second;
			cJSON* pisCurrent = cJSON_GetObjectItem(pTemp, "isCurrent");
			if (!id.compare(itor2.first)) {
				if(pisCurrent)
					cJSON_SetBoolValue(pisCurrent, true);
				cJSON_LoadFromObject(pTemp, "data", data);
			}
			else {
				if (pisCurrent)
					cJSON_SetBoolValue(pisCurrent, false);
			}
		}
	}

	m_currentVerse = data;
	
	return 0;
}

int CVerseGraphicsConfig::resetVerseGraphicsConfig(std::string& data)
{
	std::string id;
	{
		std::lock_guard<std::mutex> lock(m_mutexMap);
		//step1 获取默认设置的ID
		for (auto itor : m_mapGraphics)
		{
			bool bKey = false;
			cJSON_LoadFromObject(itor.second, "isDefault", bKey);
			if (bKey)
			{
				cJSON_LoadFromObject(itor.second, "id", id);
				break;
			}
		}
	}

	changeCurVerseGraphicsConfig(id, data);
	return 0;
}