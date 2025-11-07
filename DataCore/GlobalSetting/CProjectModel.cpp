#include "CProjectModel.h"

// Ä£ÐÍ
CProjectModel::CProjectModel() : IProjectTemplate() {
	return;
}

CProjectModel::~CProjectModel() {
	return;
}

void CProjectModel::loadFromJson(cJSON *pJson) {
	m_mapModel.clear();

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

cJSON *CProjectModel::toJson() {
	std::unordered_map<std::string, std::vector<TemplateModel> > mapModel;

	for (auto [strKey, value] : m_mapModel) {
		auto iter = mapModel.find(value.strDataType);
		if (iter == mapModel.end()) {
			std::vector<TemplateModel> vecTemplateModel;
			vecTemplateModel.push_back(value);
			mapModel[value.strDataType] = vecTemplateModel;
		} else {
			mapModel[value.strDataType].push_back(value);
		}
	}

	cJSON *pJson = cJSON_CreateArray();
	if (pJson != nullptr) {
		for (auto [strKey, vecValue] : mapModel) {
			cJSON *pJsonItem = cJSON_CreateObject();
			if (pJsonItem != nullptr) {
				cJSON_AddToObject(pJsonItem, "name", strKey);

				cJSON *pJsonStyles = cJSON_CreateArray();
				if (pJsonStyles != nullptr) {
					for (auto templateModel : vecValue) {
						cJSON *pJsonTemplateModel = templateModel.toJson();
						if (pJsonTemplateModel != nullptr) {
							cJSON_AddItemToArray(pJsonStyles, pJsonTemplateModel);
						}
					}

					cJSON_AddItemToObject(pJsonItem, "styles", pJsonStyles);
				}

				cJSON_AddItemToArray(pJson, pJsonItem);
			}
		}
	}

	return pJson;
}

void CProjectModel::addFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			TemplateModel templateModel;
			templateModel.loadFromJson(pJson);

			m_mapModel[templateModel.strResourceId] = templateModel;
		}
	}

	return ;
}

void CProjectModel::updateFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			TemplateModel templateModel;
			templateModel.loadFromJson(pJson);

			auto iter = m_mapModel.find(templateModel.strResourceId);
			if (iter != m_mapModel.end()) {
				iter->second = templateModel;
			}
		}
	}

	return ;
}

void CProjectModel::deleteByResourceID(std::string strResourceID) {
	auto iter = m_mapModel.find(strResourceID);
	if (iter != m_mapModel.end()) {
		m_mapModel.erase(iter);
	}

	return ;
}

std::unordered_set<std::string> CProjectModel::getResourceIDList() {
	std::unordered_set<std::string> setResourceID;

	for (auto [strKey, value] : m_mapModel) {
		setResourceID.insert(strKey);
	}

	return setResourceID;
}

cJSON *CProjectModel::getByResourceID(std::string strResourceID) {
	cJSON *pJson = nullptr;

	auto iter = m_mapModel.find(strResourceID);
	if (iter != m_mapModel.end()) {
		pJson = iter->second.toJson();
	}

	return pJson;
}

cJSON *CProjectModel::getByName(std::string strResourceID, std::string strName) {
	return cJSON_CreateObject();
}