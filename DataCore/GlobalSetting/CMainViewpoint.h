#pragma once

#include "IGlobalSetting.h"

// Ö÷ÊÓµã
class CMainViewpoint : public IGlobalSetting {
public:
	CMainViewpoint();
	virtual ~CMainViewpoint() override;

public:
	virtual void loadFromJson(cJSON *pJson) override;
	virtual cJSON *toJson() override;

public:
	void setMainViewpoint();

public:
	cJSON *mainViewpointToJson(cJSON *pJson);
	void updateMainViewpointFromJson(cJSON *pJson);

private:
	Viewpoint m_viewpoint;
};