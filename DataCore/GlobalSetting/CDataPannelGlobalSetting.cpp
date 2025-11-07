#include "CDataPannelGlobalSetting.h"

// 数据面片全局设置
CDataPannelGlobalSetting::CDataPannelGlobalSetting()
	: IGlobalSetting(),
	m_bGlobalDataState(false),
	m_nGlobalMinRange(0),
	m_nGlobalMaxRange(0),
	m_bGlobalTowardsScreen(false) {
	return;
}

CDataPannelGlobalSetting::~CDataPannelGlobalSetting() {
	return;
}

void CDataPannelGlobalSetting::loadFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			cJSON_LoadFromObject(pJson, "globalDataState", m_bGlobalDataState);
			cJSON_LoadFromObject(pJson, "globalMinRange", m_nGlobalMinRange);
			cJSON_LoadFromObject(pJson, "globalMaxRange", m_nGlobalMaxRange);
			cJSON_LoadFromObject(pJson, "globalTowardsScreen", m_bGlobalTowardsScreen);
		}
	}

	return;
}

cJSON *CDataPannelGlobalSetting::toJson() {
	cJSON *pJson = cJSON_CreateObject();
	if (pJson != nullptr) {
		cJSON_AddToObject(pJson, "globalDataState", m_bGlobalDataState);
		cJSON_AddToObject(pJson, "globalMinRange", m_nGlobalMinRange);
		cJSON_AddToObject(pJson, "globalMaxRange", m_nGlobalMaxRange);
		cJSON_AddToObject(pJson, "globalTowardsScreen", m_bGlobalTowardsScreen);
	}

	return pJson;
}

void CDataPannelGlobalSetting::setDataPannelGlobalSetting() {
	Interface::setGlobalDataModelState(m_bGlobalDataState);
	Interface::setGlobalDataModelBillBoard(m_bGlobalTowardsScreen);
	Interface::setGlobalDataModelRange(m_nGlobalMinRange, m_nGlobalMaxRange);

	return ;
}