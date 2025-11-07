#pragma once

#include "IGlobalSetting.h"

//老板数据面片全局设置
// 数据面片全局设置
class CDataPannelGlobalSetting : public IGlobalSetting {
public:
	CDataPannelGlobalSetting();
	virtual ~CDataPannelGlobalSetting() override;

public:
	virtual void loadFromJson(cJSON *pJson) override;
	virtual cJSON *toJson() override;

public:
	void setDataPannelGlobalSetting();

private:
	bool m_bGlobalDataState;
	int m_nGlobalMinRange;
	int m_nGlobalMaxRange;
	bool m_bGlobalTowardsScreen;
};