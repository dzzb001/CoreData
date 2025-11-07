#pragma once

#include "IProjectTemplate.h"

// ³µÁ¾
class CProjectVehicleSimulation : public IProjectTemplate {
public:
	CProjectVehicleSimulation();
	virtual ~CProjectVehicleSimulation() override;

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

	virtual void addRef(std::string strResourceID) {};
	virtual void releaseRef(std::string strResourceID) {};

private:
	std::unordered_map<std::string, TemplateModel> m_mapVehicleSimulation;
};