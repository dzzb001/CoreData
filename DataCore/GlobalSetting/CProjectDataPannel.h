#pragma once

#include "IProjectTemplate.h"

// POI
class CProjectDataPannel : public IProjectTemplate {
public:
	CProjectDataPannel();
	~CProjectDataPannel();

public:
	virtual void loadFromJson(cJSON *pJson) override;
	virtual cJSON *toJson() override;

public:
	//pac目前所有模板类只提供查询，以下几个接口可以不用实现。
	virtual void addFromJson(cJSON *pJson) override;
	virtual void updateFromJson(cJSON *pJson) override;
	virtual void deleteByResourceID(std::string strResourceID) override;

public:
	virtual std::unordered_set<std::string> getResourceIDList() override;
	virtual cJSON *getByResourceID(std::string strResourceID) override;
	virtual cJSON *getByName(std::string strResourceID, std::string strName) override;
	virtual void addRef(std::string strResourceID) {};
	virtual void releaseRef(std::string strResourceID) {};

private:
	void cleanResouce();
	std::unordered_map<std::string, cJSON*> m_mapDataPannel;
};