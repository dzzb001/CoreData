#include "CProjectPOI.h"

// POI
CProjectPOI::CProjectPOI() : IProjectTemplate() {
	return ;
}

CProjectPOI::~CProjectPOI() {
	return ;
}

void CProjectPOI::loadFromJson(cJSON *pJson) {
	m_mapPOI.clear();

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
									cJSON *pJsonStylesItem = cJSON_GetArrayItem(pJsonStyles, j);
									if (pJsonStylesItem != nullptr) {
										if (cJSON_IsObject(pJsonStylesItem)) {
											this->addFromJson(pJsonStylesItem);
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

	return ;
}

cJSON *CProjectPOI::toJson() {
	cJSON *pJson = cJSON_CreateArray();
	if (pJson != nullptr) {
		cJSON *pJsonItem = cJSON_CreateObject();
		if (pJsonItem != nullptr) {
			cJSON *pJsonStyles = cJSON_CreateArray();
			if (pJsonStyles != nullptr) {
				for (auto [strKey, strValue] : m_mapPOI) {
					cJSON *pJsonStylesItem = cJSON_Parse(strValue.c_str());
					if (pJsonStylesItem != nullptr) {
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
}

void CProjectPOI::addFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			std::string strResourceID;
			cJSON_LoadFromObject(pJson, "guid", strResourceID);
			
			m_mapPOI[strResourceID] = cJSON_PrintFromJSON(pJson);

			//Poco::Logger::get("RteLogger").debug("addFromJson poi data %s", m_mapPOI[strResourceID]);
		}
	}

	return ;
}

void CProjectPOI::updateFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			std::string strResourceID;
			cJSON_LoadFromObject(pJson, "guid", strResourceID);

			auto iter = m_mapPOI.find(strResourceID);
			if (iter != m_mapPOI.end()) {
				iter->second = cJSON_PrintFromJSON(pJson);
				//Poco::Logger::get("RteLogger").debug("addFromJson poi data %s", iter->second);
			}
		}
	}

	return ;
}

void CProjectPOI::deleteByResourceID(std::string strResourceID) {
	auto iter = m_mapPOI.find(strResourceID);
	if (iter != m_mapPOI.end()) {
		m_mapPOI.erase(iter);
	}

	return ;
}

std::unordered_set<std::string> CProjectPOI::getResourceIDList() {
	std::unordered_set<std::string> setResourceID;

	for (auto [strKey, strValue] : m_mapPOI) {
		setResourceID.insert(strKey);
	}

	return setResourceID;
}

cJSON *CProjectPOI::getByResourceID(std::string strResourceID) {
	cJSON *pJson = nullptr;

	auto iter = m_mapPOI.find(strResourceID);
	if (iter != m_mapPOI.end()) {
		cJSON *pJsonItem = cJSON_Parse(iter->second.c_str());
		if (pJsonItem != nullptr) {
			if (cJSON_IsObject(pJsonItem)) {
				cJSON *pJsonElementStyle = cJSON_DetachItemFromObject(pJsonItem, "elementStyle");
				if (pJsonElementStyle != nullptr) {
					if (cJSON_IsObject(pJsonElementStyle)) {
						pJson = pJsonElementStyle;
					}
				}
			}

			cJSON_Delete(pJsonItem);
		}
	}

	return pJson;
}


cJSON *CProjectPOI::getByName(std::string strResourceID, std::string strName) {
	cJSON *pJson = nullptr;

	auto iter = m_mapPOI.find(strResourceID);
	if (iter != m_mapPOI.end()) {
		cJSON *pJsonItem = cJSON_Parse(iter->second.c_str());
		if (pJsonItem != nullptr) {
			if (cJSON_IsObject(pJsonItem)) {
				cJSON *pJsonName = cJSON_DetachItemFromObject(pJsonItem, strName.c_str());
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