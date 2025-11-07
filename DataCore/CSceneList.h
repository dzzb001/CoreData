#pragma once
#include <string>
#include "CJsonBase.h"
#include <map>
#include <vector>

class CSceneList : public CJson_base
{
public:
	CSceneList(cJSON* p);

	void getSceneList(std::string strRequest, std::string& strRespone);	
	cJSON* toJson() { return cJSON_Duplicate(m_pJson, true); }

	std::string getCurSenceId() { return m_strCurSenceId; }

	cJSON* getSceneViewpoint(std::string guid);
public:
	std::string m_strCurSenceId;
	std::vector<std::string> m_vecSence;
	std::map<std::string, cJSON *> m_mapSence; // guid ,scene json
};

