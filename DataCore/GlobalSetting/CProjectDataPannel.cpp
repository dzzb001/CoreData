#include "CProjectDataPannel.h"

// POI
CProjectDataPannel::CProjectDataPannel() : IProjectTemplate() {
	return ;
}

CProjectDataPannel::~CProjectDataPannel() {
	cleanResouce();
	return ;
}

void CProjectDataPannel::cleanResouce()
{
	std::unordered_map<std::string, cJSON*>::iterator itor = m_mapDataPannel.begin();
	while (itor!=m_mapDataPannel.end())
	{
		if (itor->second)
			cJSON_free(itor->second);
		itor = m_mapDataPannel.erase(itor);
	}
}

void CProjectDataPannel::loadFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsArray(pJson)) {
			for (int i = 0; i < cJSON_GetArraySize(pJson); ++i) {
				cJSON *pJsonItem = cJSON_GetArrayItem(pJson, i);
				if (pJsonItem != nullptr) {
					std::string templateID;
					cJSON_LoadFromObject(pJsonItem, "templateId", templateID);
					if(templateID.compare(""))
						m_mapDataPannel[templateID] = cJSON_Duplicate(pJsonItem, true);
				}
			}
		}
	}
	return ;
}

cJSON *CProjectDataPannel::toJson() {
	cJSON *pJson = cJSON_CreateArray();
	if (pJson != nullptr) {
		std::unordered_map<std::string, cJSON*>::iterator itor = m_mapDataPannel.begin();
		for (; itor != m_mapDataPannel.end(); ++itor)
		{
			cJSON* pJsonItem = nullptr;
			if (itor->second)
			{
				pJsonItem = cJSON_Duplicate(itor->second, true);
				cJSON_AddItemToArray(pJson, pJsonItem);
			}
		}
	}

	return pJson;
}

void CProjectDataPannel::addFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			std::string strTemplateId;
			cJSON_LoadFromObject(pJson, "templateId", strTemplateId);
			m_mapDataPannel[strTemplateId] = cJSON_Duplicate(pJson,true);
		}
	}

	return ;
}

void CProjectDataPannel::updateFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			std::string strResourceID;
			cJSON_LoadFromObject(pJson, "templateId", strResourceID);

			auto iter = m_mapDataPannel.find(strResourceID);
			if (iter != m_mapDataPannel.end()) {
				if (iter->second)
					cJSON_free(iter->second);
				iter->second = cJSON_Duplicate(pJson,true);
			}
		}
	}

	return ;
}

void CProjectDataPannel::deleteByResourceID(std::string strResourceID) {
	auto iter = m_mapDataPannel.find(strResourceID);
	if (iter != m_mapDataPannel.end()) {
		if (iter->second)
			cJSON_free(iter->second);
		m_mapDataPannel.erase(iter);
	}

	return ;
}

std::unordered_set<std::string> CProjectDataPannel::getResourceIDList() {
	std::unordered_set<std::string> setResourceID;

	for (auto [strKey, strValue] : m_mapDataPannel) {
		setResourceID.insert(strKey);
	}

	return setResourceID;
}

cJSON *CProjectDataPannel::getByResourceID(std::string strResourceID) {
	cJSON *pJson = nullptr;

	auto iter = m_mapDataPannel.find(strResourceID);
	if (iter != m_mapDataPannel.end()) {
		cJSON *pJsonItem = iter->second;
		if (pJsonItem != nullptr) {
			if (cJSON_IsObject(pJsonItem)) {
				pJson = cJSON_Duplicate(pJsonItem, true);
			}
		}
	}

	return pJson;
}

cJSON *CProjectDataPannel::getByName(std::string strResourceID, std::string strName) {
	cJSON *pJson = nullptr;

	auto iter = m_mapDataPannel.find(strResourceID);
	if (iter != m_mapDataPannel.end()) {
		cJSON* pJsonItem = iter->second;
		if (pJsonItem != nullptr) {
			if (cJSON_IsObject(pJsonItem)) {
				cJSON* attributeList = cJSON_GetObjectItem(pJsonItem, "attributeList");
				pJson = cJSON_Duplicate(attributeList, true);
			}
		}
	}
	return pJson;
}