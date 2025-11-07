#include "CAdvancedSetting.h"

// 高级设置
CAdvancedSetting::CAdvancedSetting()
	: IGlobalSetting(),
	m_strVersion(""),
	m_strLightPoint(""),
	m_bSystemLonlatCopy(false),
	m_bSystemIsOpenSynergia(false),
	m_nSpaceMaxRange(0),
	m_bSpaceTroggle(false),
	m_nSpaceMinRange(0),
	m_strDisplayReflectImage(""),
	m_fDisplayBrightness(0),
	m_fDisplaySaturation(0.4),
	m_bDisplayTroggle(false),
	m_fDisplayAmibent(0.56),
	m_fFov(60),
	m_fDisplayContrast(1.0),
	m_nGlobalLodScale(25),
	m_bDisplayIsEnablePOIMaxScale(false) {
	return;
}

CAdvancedSetting::~CAdvancedSetting() {
	return ;
}

void CAdvancedSetting::loadFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			cJSON_LoadFromObject(pJson, "version", m_strVersion);

			cJSON *pJsonLightingSetting = cJSON_GetObjectItem(pJson, "场景灯光");
			if (pJsonLightingSetting != nullptr) {
				if (cJSON_IsObject(pJsonLightingSetting)) {
					this->loadLightingSettingFromJson(pJsonLightingSetting);
				}
			}

			cJSON* pShortcutKey = cJSON_GetObjectItem(pJson, "快捷键");
			if (pShortcutKey != nullptr) {
				if (cJSON_IsObject(pShortcutKey)) {
					this->loadShortcutKeysFromJson(pShortcutKey);
				}
			}


			cJSON* pEfficiencySettings = cJSON_GetObjectItem(pJson, "效率设置");
			if (pEfficiencySettings != nullptr) {
				if (cJSON_IsObject(pEfficiencySettings)) {
					this->loadEfficiencySettings(pEfficiencySettings);
				}
			}

			cJSON *pJsonSpaceSetting = cJSON_GetObjectItem(pJson, "空间设置");
			if (pJsonSpaceSetting != nullptr) {
				if (cJSON_IsObject(pJsonSpaceSetting)) {
					this->loadSpaceSettingFromJson(pJsonSpaceSetting);
				}
			}

			cJSON *pJsonDisplaySetting = cJSON_GetObjectItem(pJson, "显示设置");
			if (pJsonDisplaySetting != nullptr) {
				if (cJSON_IsObject(pJsonDisplaySetting)) {
					this->loadDisplaySettingFromJson(pJsonDisplaySetting);
				}
			}

			//自定义设置
			cJSON* pJsonConstomSetting = cJSON_GetObjectItem(pJson, "自定义设置");
			if (pJsonConstomSetting != nullptr)
			{
				if (cJSON_IsObject(pJsonConstomSetting)) {
					this->loadConstomSettingFromJson(pJsonDisplaySetting);
				}
			}
		}
	}

	return ;
}

cJSON *CAdvancedSetting::toJson() {
	cJSON *pJson = cJSON_CreateObject();
	if (pJson != nullptr) {
		cJSON_AddToObject(pJson, "version", m_strVersion);
		cJSON *pJsonLightingSetting = this->lightingSettingToJson();
		if (pJsonLightingSetting != nullptr) {
			cJSON_AddItemToObject(pJson, "场景灯光", pJsonLightingSetting);
		}

		cJSON *pShortcutKeys = this->ShortcutKeysToJson();
		if (pShortcutKeys != nullptr) {
			cJSON_AddItemToObject(pJson, "快捷键", pShortcutKeys);
		}

		cJSON* pEfficiencySettings = this->EfficiencySettingsToJson();
		if (pEfficiencySettings != nullptr) {
			cJSON_AddItemToObject(pJson, "效率设置", pEfficiencySettings);
		}

		cJSON *pJsonSpaceSetting = this->spaceSettingToJson();
		if (pJsonSpaceSetting != nullptr) {
			cJSON_AddItemToObject(pJson, "空间设置", pJsonSpaceSetting);
		}

		cJSON *pJsonDisplaySetting = this->displaySettingToJson();
		if (pJsonDisplaySetting != nullptr) {
			cJSON_AddItemToObject(pJson, "显示设置", pJsonDisplaySetting);
		}

		cJSON* pJsonConstomSetting = this->constomSettingToJson();
		if (pJsonConstomSetting != nullptr)
		{
			cJSON_AddItemToObject(pJson, "自定义设置", pJsonConstomSetting);
		}
	}

	return pJson;
}

//快捷键
void CAdvancedSetting::loadShortcutKeysFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			m_strShortcutKey = cJSON_PrintFromJSON(pJson);
		}
	}
}

cJSON *CAdvancedSetting::ShortcutKeysToJson() {
	cJSON *pJson = cJSON_Parse(m_strShortcutKey.c_str());
	return pJson;
}

//效率设置
void CAdvancedSetting::loadEfficiencySettings(cJSON* pJson)
{
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			m_strEfficiencySettings = cJSON_PrintFromJSON(pJson);
		}
	}
}

cJSON* CAdvancedSetting::EfficiencySettingsToJson()
{
	cJSON* pJson = cJSON_Parse(m_strEfficiencySettings.c_str());
	return pJson;
}


// 灯光设置
void CAdvancedSetting::loadLightingSettingFromJson(cJSON *pJson) {
	m_vecLight.clear();

	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			cJSON *pJsonLightPoint = cJSON_GetObjectItem(pJson, "lightPoint");
			if (pJsonLightPoint != nullptr) {
				if (cJSON_IsObject(pJsonLightPoint)) {
					m_strLightPoint = cJSON_PrintFromJSON(pJsonLightPoint);
				}
			}

			cJSON *pJsonLight = cJSON_GetObjectItem(pJson, "lights");
			if (pJsonLight != nullptr) {
				if (cJSON_IsArray(pJsonLight)) {
					for (int i = 0; i < cJSON_GetArraySize(pJsonLight); ++i) {
						cJSON *pJsonLightItem = cJSON_GetArrayItem(pJsonLight, i);
						if (pJsonLightItem != nullptr) {
							if (cJSON_IsObject(pJsonLightItem)) {
								m_vecLight.push_back(cJSON_PrintFromJSON(pJsonLightItem));
							}
						}
					}
				}
			}
		}
	}

	return ;
}

cJSON *CAdvancedSetting::lightingSettingToJson() {
	cJSON *pJson = cJSON_CreateObject();
	if (pJson != nullptr) {
		cJSON *pJsonLightPoint = cJSON_Parse(m_strLightPoint.c_str());
		if (pJsonLightPoint != nullptr) {
			if (cJSON_IsObject(pJsonLightPoint)) {
				cJSON_AddItemToObject(pJson, "lightPoint", pJsonLightPoint);
			}
		}

		cJSON *pJsonLight = cJSON_CreateArray();
		if (pJsonLight != nullptr) {
			for (auto strLight : m_vecLight) {
				cJSON *pJsonLightItem = cJSON_Parse(strLight.c_str());
				if (pJsonLightItem != nullptr) {
					if (cJSON_IsObject(pJsonLightItem)) {
						cJSON_AddItemToArray(pJsonLight, pJsonLightItem);
					}
				}
			}

			cJSON_AddItemToObject(pJson, "lights", pJsonLight);
		}
	}

	return pJson;
}

// 空间设置
void CAdvancedSetting::loadSpaceSettingFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			cJSON_LoadFromObject(pJson, "maxRange", m_nSpaceMaxRange);
			cJSON_LoadFromObject(pJson, "troggle", m_bSpaceTroggle);

			cJSON *pJsonPoint = cJSON_GetObjectItem(pJson, "point");
			if (pJsonPoint != nullptr) {
				if (cJSON_IsObject(pJsonPoint)) {
					m_SpacePoint.loadFromJson(pJsonPoint);
				}
			}

			cJSON_LoadFromObject(pJson, "minRange", m_nSpaceMinRange);
		}
	}

	return ;
}

cJSON *CAdvancedSetting::spaceSettingToJson() {
	cJSON *pJson = cJSON_CreateObject();
	if (pJson != nullptr) {
		cJSON_AddToObject(pJson, "maxRange", m_nSpaceMaxRange);
		cJSON_AddToObject(pJson, "troggle", m_bSpaceTroggle);

		cJSON *pJsonPoint = m_SpacePoint.toJson();
		if (pJsonPoint != nullptr) {
			cJSON_AddItemToObject(pJson, "point", pJsonPoint);
		}

		cJSON_AddToObject(pJson, "minRange", m_nSpaceMinRange);
	}

	return pJson;
}

// 显示设置
void CAdvancedSetting::loadDisplaySettingFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			cJSON_LoadFromObject(pJson, "ReflectImage", m_strDisplayReflectImage);
			cJSON_LoadFromObject(pJson, "Brightness", m_fDisplayBrightness);
			cJSON_LoadFromObject(pJson, "Saturation", m_fDisplaySaturation);
			cJSON_LoadFromObject(pJson, "Troggle", m_bDisplayTroggle);
			cJSON_LoadFromObject(pJson, "Amibent", m_fDisplayAmibent);
			cJSON_LoadFromObject(pJson, "Contrast", m_fDisplayContrast);
			cJSON_LoadFromObject(pJson, "GlobalLodScale", m_nGlobalLodScale);
			cJSON_LoadFromObject(pJson, "IsEnablePOIMaxScale", m_bDisplayIsEnablePOIMaxScale);
			cJSON_LoadFromObject(pJson, "poiAggregationDistance", m_fPoiAggregationDistance);
			cJSON_LoadFromObject(pJson, "Fov", m_fFov);
		}
	}

	return;
}

cJSON *CAdvancedSetting::displaySettingToJson() {
	cJSON *pJson = cJSON_CreateObject();
	if (pJson != nullptr) {
		cJSON_AddToObject(pJson, "ReflectImage", m_strDisplayReflectImage);
		cJSON_AddToObject(pJson, "Brightness", m_fDisplayBrightness);
		cJSON_AddToObject(pJson, "Saturation", m_fDisplaySaturation);
		cJSON_AddToObject(pJson, "Troggle", m_bDisplayTroggle);
		cJSON_AddToObject(pJson, "Amibent", m_fDisplayAmibent);
		cJSON_AddToObject(pJson, "Contrast", m_fDisplayContrast);
		cJSON_AddToObject(pJson, "GlobalLodScale", m_nGlobalLodScale);
		cJSON_AddToObject(pJson, "IsEnablePOIMaxScale", m_bDisplayIsEnablePOIMaxScale);
		cJSON_AddToObject(pJson, "poiAggregationDistance", m_fPoiAggregationDistance);
		cJSON_AddToObject(pJson, "Fov", m_fFov);
	}

	return pJson;
}

//自定义设置
void CAdvancedSetting::loadConstomSettingFromJson(cJSON* pJson)
{
	if (pJson != nullptr) {
		m_strConstomSettings = cJSON_PrintFromJSON(pJson);
		if (cJSON_IsObject(pJson)) {
			cJSON_LoadFromObject(pJson, "smoothPositioning", m_bSmoothPositioning);
		}
	}
}

cJSON* CAdvancedSetting::constomSettingToJson()
{
	return cJSON_Parse(m_strConstomSettings.c_str());
}

void CAdvancedSetting::setAdvancedSetting() {
	this->setSystemSetting();
	this->setLightingSetting();
	this->setSpaceSetting();
	this->setDisplaySetting();

	return ;
}

int  CAdvancedSetting::getSettingValue(std::string type, std::string key, std::string& value)
{
	if (!type.compare("自定义设置"))
	{
		if (!key.compare("smoothPositioning"))
		{
			value = m_bSmoothPositioning ? "1" : "0";
		}
	}
	else {

	}
	return 0;
}
void CAdvancedSetting::setSystemSetting() {
	return ;
}

void CAdvancedSetting::setLightingSetting() {
	std::vector<CBaseData *> vecBaseData;

	cJSON *pJsonLightPoint = cJSON_Parse(m_strLightPoint.c_str());
	if (pJsonLightPoint != nullptr) {
		if (cJSON_IsObject(pJsonLightPoint)) {
			cJSON *pJsonElementData = cJSON_GetObjectItem(pJsonLightPoint, "elementData");
			if (pJsonElementData != nullptr) {
				if (cJSON_IsObject(pJsonElementData)) {
					CPointData *pointData = new CPointData();
					pointData->loadJson(cJSON_PrintFromJSON(pJsonElementData));
					pointData->_isNewAdd = false;

					vecBaseData.push_back(pointData);
				}
			}
		}

		cJSON_Delete(pJsonLightPoint);
	}

	for (auto strLight : m_vecLight) {
		cJSON *pJsonLight = cJSON_Parse(strLight.c_str());
		if (pJsonLight != nullptr) {
			if (cJSON_IsObject(pJsonLight)) {
				cJSON *pJsonElementData = cJSON_GetObjectItem(pJsonLight, "elementData");
				if (pJsonElementData != nullptr) {
					if (cJSON_IsObject(pJsonElementData)) {
						CLightData *lightData = new CLightData();
						lightData->loadJson(cJSON_PrintFromJSON(pJsonElementData));
						lightData->_isNewAdd = false;

						vecBaseData.push_back(lightData);
					}
				}
			}

			cJSON_Delete(pJsonLight);
		}
	}

	Interface::addModelBase(vecBaseData);

	return ;
}

void CAdvancedSetting::setSpaceSetting() {
	Interface::saveCameraRange(YZPoint2d(m_nSpaceMinRange, m_nSpaceMaxRange));
	Interface::saveCameraExtent(YZPoint(m_SpacePoint.fX, m_SpacePoint.fY,
										m_SpacePoint.fZ, m_SpacePoint.fW));
	Interface::troggleCameraRange(m_bSpaceTroggle);
	return ;
}

void CAdvancedSetting::setDisplaySetting() {
	Interface::setVerseSceneTroggleSaturation(m_bDisplayTroggle);

	Interface::setVerseSceneContrast(m_fDisplayContrast);
	Interface::setVerseSceneSaturation(m_fDisplaySaturation);
	Interface::setVerseSceneBrightness(m_fDisplayBrightness);
	Interface::setVerseSceneAmibentWidget(m_fDisplayAmibent);
	Interface::setMainCameraFovy(m_fFov);

	//Interface::setVerseSceneReflectImage(m_strDisplayReflectImage); //环境纹理不需要在这里设置
	Interface::setGlobalLodScale(m_nGlobalLodScale);
	//如果开启聚合，设置聚合距离
	if (m_bDisplayIsEnablePOIMaxScale)
	{
		Interface::setPOICluster(m_fPoiAggregationDistance); //5.2.4版本支持此接口。
	}
	return ;
}