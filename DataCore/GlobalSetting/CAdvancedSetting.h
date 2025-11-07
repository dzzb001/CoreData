#pragma once

#include "IGlobalSetting.h"

// 高级设置
class CAdvancedSetting : public IGlobalSetting {
public:
	CAdvancedSetting();
	virtual ~CAdvancedSetting() override;

public:
	virtual void loadFromJson(cJSON *pJson) override;
	virtual cJSON *toJson() override;

public:
	// 快捷键
	void loadShortcutKeysFromJson(cJSON *pJson);
	cJSON * ShortcutKeysToJson();

	//效率设置
	void loadEfficiencySettings(cJSON* pJson);
	cJSON* EfficiencySettingsToJson();

	// 灯光设置
	void loadLightingSettingFromJson(cJSON *pJson);
	cJSON *lightingSettingToJson();

	// 空间设置
	void loadSpaceSettingFromJson(cJSON *pJson);
	cJSON *spaceSettingToJson();

	// 显示设置
	void loadDisplaySettingFromJson(cJSON *pJson);
	cJSON *displaySettingToJson();
		
	//自定义设置
	void loadConstomSettingFromJson(cJSON* pJson);
	cJSON* constomSettingToJson();

public:
	void setAdvancedSetting();
	int  getSettingValue(std::string type, std::string key, std::string& value);

	int getGlobalLodScale() { return m_nGlobalLodScale; }

	double getDisplayAmibent() { return m_fDisplayAmibent; }

private:
	void setSystemSetting();
	void setLightingSetting();
	void setSpaceSetting();
	void setDisplaySetting();

private:
	std::string m_strVersion;

	std::string m_strLightPoint;
	std::vector<std::string> m_vecLight;

	bool m_bSystemLonlatCopy;
	bool m_bSystemIsOpenSynergia;

	int m_nSpaceMaxRange;
	bool m_bSpaceTroggle;
	Point m_SpacePoint;
	int m_nSpaceMinRange;

	std::string m_strDisplayReflectImage;
	double m_fDisplayBrightness;
	double m_fDisplaySaturation;
	bool m_bDisplayTroggle;
	double m_fDisplayAmibent;
	double m_fDisplayContrast;
	int m_nGlobalLodScale;
	bool m_bDisplayIsEnablePOIMaxScale;
	double m_fPoiAggregationDistance;
	double m_fFov;

	std::string m_strShortcutKey;
	std::string m_strEfficiencySettings;

	//通用配置项
	std::string m_strConstomSettings;
	bool m_bSmoothPositioning = true;
};