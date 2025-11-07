#include "CProjectEffect.h"

// 特效
CProjectEffect::CProjectEffect() : IProjectTemplate() {
	m_pJson = nullptr;
}

CProjectEffect::~CProjectEffect() {
	if (m_pJson) {
		cJSON_Delete(m_pJson);
		m_pJson = nullptr;
	}
}

void CProjectEffect::loadFromJson(cJSON *pJson) {
	if (m_pJson)
		cJSON_Delete(m_pJson);

	m_mapEffect.clear();
	m_pJson = cJSON_Duplicate(pJson, true);

	if (pJson != nullptr) {
		if (cJSON_IsArray(pJson)) {
			for (int i = 0; i < cJSON_GetArraySize(pJson); ++i) {
				cJSON *pJsonItem = cJSON_GetArrayItem(pJson, i);
				if (pJsonItem != nullptr) {
					if (cJSON_IsObject(pJsonItem)) {
						cJSON *pJsonStyles = cJSON_GetObjectItem(pJsonItem, "styles");
						if (pJsonStyles != nullptr) {
							if (cJSON_IsArray(pJsonStyles)) {
								for (int j = 0; j < cJSON_GetArraySize(pJsonStyles); ++j) {
									cJSON *pJsonTemplate = cJSON_GetArrayItem(pJsonStyles, j);
									if (pJsonTemplate != nullptr) {
										if (cJSON_IsObject(pJsonTemplate)) {
											this->addFromJson(pJsonTemplate);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return;
}

cJSON *CProjectEffect::toJson() {
#if 0
	//直接用加载时的json
	cJSON* pRet = nullptr;
	pRet = cJSON_Duplicate(m_pJson, true);
	return pRet;
#else
	cJSON* pJson = cJSON_CreateArray();
	if (pJson != nullptr) {
		cJSON* pJsonItem = cJSON_CreateObject();
		if (pJsonItem != nullptr) {
			cJSON* pJsonStyles = cJSON_CreateArray();
			if (pJsonStyles != nullptr) {
				for (auto [strKey, strValue] : m_mapEffect2) {
					cJSON* pJsonStylesItem = cJSON_Parse(strValue.c_str());
					if (pJsonStylesItem != nullptr) {
						//判断特效id是否被使用
						std::string resourceId;
						cJSON_LoadFromObject(pJsonStylesItem, "resourceId", resourceId);
						auto itor = m_mapRefCount.find(resourceId);
						if (itor != m_mapRefCount.end())
						{
							if (itor->second > 0) //小于0 为未使用，否则为使用
							{
								cJSON* isUsed = cJSON_GetObjectItem(pJsonStylesItem, "isUsed");
								if (isUsed)
									cJSON_SetBoolValue(isUsed, true);
								else
									cJSON_SetBoolValue(isUsed, false);
							}
						}

						if (cJSON_IsObject(pJsonStylesItem)) {
							cJSON_AddItemToArray(pJsonStyles, pJsonStylesItem);
						}
					}
				}

				cJSON_AddItemToObject(pJsonItem, "styles", pJsonStyles);
			}

			cJSON_AddItemToArray(pJson, pJsonItem);
		}
	}

	return pJson;
#endif
}

void CProjectEffect::addFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {

			std::string resourceId;
			cJSON* pResourceId = cJSON_GetObjectItem(pJson, "resourceId");

			if (pResourceId) {
				resourceId = pResourceId->valuestring;
			}
			m_mapEffect[resourceId] = pJson;
			char* pOut = cJSON_Print(pJson);

			m_mapEffect2[resourceId] = pOut;
			//Poco::Logger::get("RteLogger").error("resouce effect [%s]",std::string(pOut));
			free(pOut);

			m_mapRefCount[resourceId] = 0;
		}
	}
}

void CProjectEffect::updateFromJson(cJSON *pJson) {

	return;
}

void CProjectEffect::deleteByResourceID(std::string strResourceID) {

	return;
}

std::unordered_set<std::string> CProjectEffect::getResourceIDList() {
	std::unordered_set<std::string> setResourceID;

	for (auto [strKey, value] : m_mapEffect) {
		setResourceID.insert(strKey);
	}

	return setResourceID;
}

cJSON *CProjectEffect::getByResourceID(std::string strResourceID) {
	auto itorFind = m_mapEffect.find(strResourceID);
	if (itorFind != m_mapEffect.end())
	{
		cJSON* pRet = cJSON_Duplicate(itorFind->second, true);
		return pRet;
	}
	else
		return nullptr;
}

cJSON *CProjectEffect::getByName(std::string strResourceID, std::string strName) {
	cJSON* pJson = nullptr;

	auto iter = m_mapEffect2.find(strResourceID);
	if (iter != m_mapEffect2.end()) {
		cJSON* pJsonItem = cJSON_Parse(iter->second.c_str());
		if (pJsonItem != nullptr) {
			if (cJSON_IsObject(pJsonItem)) {
				cJSON* pJsonName = cJSON_DetachItemFromObject(pJsonItem, strName.c_str());
				if (pJsonName != nullptr) {
					//if (cJSON_IsObject(pJsonName)) 
					{
						pJson = pJsonName;
					}
				}
			}

			cJSON_Delete(pJsonItem);
		}
	}

	return pJson;
}

void CProjectEffect::addRef(std::string strResourceID)
{
	auto itorFind = m_mapRefCount.find(strResourceID);
	if (itorFind != m_mapRefCount.end())
	{
		itorFind->second++;
	}
}
void CProjectEffect::releaseRef(std::string strResourceID)
{
	auto itorFind = m_mapRefCount.find(strResourceID);
	if (itorFind != m_mapRefCount.end())
	{
		itorFind->second--;
	}
}