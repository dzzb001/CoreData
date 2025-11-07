#include "CProjectVegetation.h"

// Ö²±»
CProjectVegetation::CProjectVegetation() : IProjectTemplate() {
	return;
}

CProjectVegetation::~CProjectVegetation() {
	return;
}

void CProjectVegetation::loadFromJson(cJSON *pJson) {
	m_mapVegetation.clear();

	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			cJSON *pJsonStyles = cJSON_GetObjectItem(pJson, "styles");
			if (pJsonStyles != nullptr) {
				if (cJSON_IsArray(pJsonStyles)) {
					for (int i = 0; i < cJSON_GetArraySize(pJsonStyles); ++i) {
						cJSON *pJsonItem = cJSON_GetArrayItem(pJsonStyles, i);
						if (pJsonItem != nullptr) {
							if (cJSON_IsObject(pJsonItem)) {
								this->addFromJson(pJsonItem);
							}
						}
					}
				}
			}
		}
	}

	return;
}

cJSON *CProjectVegetation::toJson() {
	cJSON *pJson = cJSON_CreateObject();
	if (pJson != nullptr) {
		cJSON *pJsonStyles = cJSON_CreateArray();
		if (pJsonStyles != nullptr) {
			for (auto [strKey, value] : m_mapVegetation) {
				cJSON *pJsonItem = value.toJson();
				if (pJsonItem != nullptr) {
					cJSON_AddItemToArray(pJsonStyles, pJsonItem);
				}
			}

			cJSON_AddItemToObject(pJson, "styles", pJsonStyles);
		}
	}

	return pJson;
}

void CProjectVegetation::addFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			TemplateModel templateModel;
			templateModel.loadFromJson(pJson);

			m_mapVegetation[templateModel.strResourceId] = templateModel;
		}
	}

	return;
}

void CProjectVegetation::updateFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			TemplateModel templateModel;
			templateModel.loadFromJson(pJson);

			auto iter = m_mapVegetation.find(templateModel.strResourceId);
			if (iter != m_mapVegetation.end()) {
				iter->second = templateModel;
			}
		}
	}

	return;
}

void CProjectVegetation::deleteByResourceID(std::string strResourceID) {
	auto iter = m_mapVegetation.find(strResourceID);
	if (iter != m_mapVegetation.end()) {
		m_mapVegetation.erase(iter);
	}

	return;
}

std::unordered_set<std::string> CProjectVegetation::getResourceIDList() {
	std::unordered_set<std::string> setResourceID;

	for (auto [strKey, value] : m_mapVegetation) {
		setResourceID.insert(strKey);
	}

	return setResourceID;
}

cJSON *CProjectVegetation::getByResourceID(std::string strResourceID) {
	cJSON *pJson = nullptr;

	auto iter = m_mapVegetation.find(strResourceID);
	if (iter != m_mapVegetation.end()) {
		pJson = iter->second.toJson();
	}

	return pJson;
}

cJSON *CProjectVegetation::getByName(std::string strResourceID, std::string strName) {
	return cJSON_CreateObject();
}