#include "CProjectVehicleSimulation.h"

// ³µÁ¾
CProjectVehicleSimulation::CProjectVehicleSimulation() : IProjectTemplate() {
	return;
}

CProjectVehicleSimulation::~CProjectVehicleSimulation() {
	return;
}

void CProjectVehicleSimulation::loadFromJson(cJSON *pJson) {
	m_mapVehicleSimulation.clear();

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

cJSON *CProjectVehicleSimulation::toJson() {
	std::unordered_map<std::string, std::vector<TemplateModel> > mapVehicleSimulation;

	for (auto [strKey, value] : m_mapVehicleSimulation) {
		auto iter = mapVehicleSimulation.find(value.strDataType);
		if (iter == mapVehicleSimulation.end()) {
			std::vector<TemplateModel> vecTemplateModel;
			vecTemplateModel.push_back(value);
			mapVehicleSimulation[value.strDataType] = vecTemplateModel;
		} else {
			mapVehicleSimulation[value.strDataType].push_back(value);
		}
	}

	cJSON *pJson = cJSON_CreateArray();
	if (pJson != nullptr) {
		for (auto [strKey, vecValue] : mapVehicleSimulation) {
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

void CProjectVehicleSimulation::addFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			TemplateModel templateModel;
			templateModel.loadFromJson(pJson);

			m_mapVehicleSimulation[templateModel.strResourceId] = templateModel;
		}
	}

	return;
}

void CProjectVehicleSimulation::updateFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			TemplateModel templateModel;
			templateModel.loadFromJson(pJson);

			auto iter = m_mapVehicleSimulation.find(templateModel.strResourceId);
			if (iter != m_mapVehicleSimulation.end()) {
				iter->second = templateModel;
			}
		}
	}

	return;
}

void CProjectVehicleSimulation::deleteByResourceID(std::string strResourceID) {
	auto iter = m_mapVehicleSimulation.find(strResourceID);
	if (iter != m_mapVehicleSimulation.end()) {
		m_mapVehicleSimulation.erase(iter);
	}

	return;
}

std::unordered_set<std::string> CProjectVehicleSimulation::getResourceIDList() {
	std::unordered_set<std::string> setResourceID;

	for (auto [strKey, value] : m_mapVehicleSimulation) {
		setResourceID.insert(strKey);
	}

	return setResourceID;
}

cJSON *CProjectVehicleSimulation::getByResourceID(std::string strResourceID) {
	cJSON *pJson = nullptr;

	auto iter = m_mapVehicleSimulation.find(strResourceID);
	if (iter != m_mapVehicleSimulation.end()) {
		pJson = iter->second.toJson();
	}

	return pJson;
}

cJSON *CProjectVehicleSimulation::getByName(std::string strResourceID, std::string strName) {
	return cJSON_CreateObject();
}