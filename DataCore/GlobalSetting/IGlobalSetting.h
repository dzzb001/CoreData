#pragma once

#include "data.h"

class IGlobalSetting {
public:
	IGlobalSetting() {}
	virtual ~IGlobalSetting() {}

public:
	virtual void loadFromJson(cJSON *pJson) = 0;
	virtual cJSON *toJson() = 0;
};