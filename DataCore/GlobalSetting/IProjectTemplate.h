#pragma once

#include "IGlobalSetting.h"

class IProjectTemplate : public IGlobalSetting {
public:
	IProjectTemplate() : IGlobalSetting() {}
	virtual ~IProjectTemplate() {}

public:
	/*virtual void loadFromJson(cJSON *pJson) override = 0;
	virtual cJSON *toJson() override = 0;*/

public:
	virtual void addFromJson(cJSON *pJson) = 0;
	virtual void updateFromJson(cJSON *pJson) = 0;
	virtual void deleteByResourceID(std::string strResourceID) = 0;

public:
	virtual std::unordered_set<std::string> getResourceIDList() = 0;
	virtual cJSON *getByResourceID(std::string strResourceID) = 0;
	virtual cJSON *getByName(std::string strResourceID, std::string strName) = 0;
	virtual void addRef(std::string strResourceID) = 0;
	virtual void releaseRef(std::string strResourceID) = 0;

public:
	std::map<std::string, int> m_mapRefCount;
	//int m_nRefCount = 0;
};