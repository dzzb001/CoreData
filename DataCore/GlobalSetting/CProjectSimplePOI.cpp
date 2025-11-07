#include "CProjectSimplePOI.h"

// ¾ÛºÏPOI
CProjectSimplePOI::CProjectSimplePOI() : IProjectTemplate() {
	return;
}

CProjectSimplePOI::~CProjectSimplePOI() {
	return;
}

void CProjectSimplePOI::loadFromJson(cJSON *pJson) {
	m_mapSimplePOI.clear();

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

	return;
}

cJSON *CProjectSimplePOI::toJson() {
	cJSON *pJson = cJSON_CreateArray();
	if (pJson != nullptr) {
		cJSON *pJsonItem = cJSON_CreateObject();
		if (pJsonItem != nullptr) {
			cJSON *pJsonStyles = cJSON_CreateArray();
			if (pJsonStyles != nullptr) {
				for (auto [strKey, strValue] : m_mapSimplePOI) {
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

void CProjectSimplePOI::addFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			std::string strResourceID;
			cJSON_LoadFromObject(pJson, "guid", strResourceID);

			m_mapSimplePOI[strResourceID] = cJSON_PrintFromJSON(pJson);
		}
	}

	return;
}

void CProjectSimplePOI::updateFromJson(cJSON *pJson) {
	return;
}

void CProjectSimplePOI::deleteByResourceID(std::string strResourceID) {
	return;
}

std::unordered_set<std::string> CProjectSimplePOI::getResourceIDList() {
	return std::unordered_set<std::string>();
}

cJSON *CProjectSimplePOI::getByResourceID(std::string strResourceID) {
	return cJSON_CreateObject();
}

cJSON *CProjectSimplePOI::getByName(std::string strResourceID, std::string strName) {
	return cJSON_CreateObject();
}