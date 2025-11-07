#pragma once

#include "IProjectTemplate.h"

class CProjectPathBrowsing : public IProjectTemplate {
public:
	CProjectPathBrowsing();
	~CProjectPathBrowsing();

public:
	virtual void loadFromJson(cJSON */*pJson*/) override;
	virtual cJSON *toJson() override;

public:
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
	std::string getFromPMS();

private:
	std::unordered_map<std::string, StyleModel> m_mapPathBrowsing;
};

