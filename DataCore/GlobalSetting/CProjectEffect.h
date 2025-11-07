#pragma once

#include "IProjectTemplate.h"

// 特效
class CProjectEffect : public IProjectTemplate {
public:
	CProjectEffect();
	virtual ~CProjectEffect() override;

public:
	virtual void loadFromJson(cJSON *pJson) override;
	virtual cJSON *toJson() override;

public:
	virtual void addFromJson(cJSON *pJson) override;
	virtual void updateFromJson(cJSON *pJson) override;
	virtual void deleteByResourceID(std::string strResourceID) override;

public:
	virtual std::unordered_set<std::string> getResourceIDList() override;
	virtual cJSON *getByResourceID(std::string strResourceID) override;
	virtual cJSON *getByName(std::string strResourceID, std::string strName) override;

	virtual void addRef(std::string strResourceID);
	virtual void releaseRef(std::string strResourceID);

private:
	cJSON* m_pJson;
	std::unordered_map<std::string, cJSON*> m_mapEffect; //<特效id， 特效具体属性json,cJSON* 不能删除 附属与主json>
	std::unordered_map<std::string, std::string> m_mapEffect2; //<特效id， 特效具体属性json,cJSON* 不能删除 附属与主json>
};