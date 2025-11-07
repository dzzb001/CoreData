#pragma once

#include "IProjectTemplate.h"

// 天气
class CProjectWeather : public IGlobalSetting {
public:
	CProjectWeather();
	~CProjectWeather();

public:
	virtual void loadFromJson(cJSON *pJson) override;
	virtual cJSON *toJson() override;
};

//渲染效果
class CVerseGraphicsConfig : public IProjectTemplate {
public:
	CVerseGraphicsConfig();
	~CVerseGraphicsConfig();

public:
	virtual void loadFromJson(cJSON* pJson) override;
	virtual cJSON* toJson() override;

public:
	virtual void addFromJson(cJSON* pJson) override;
	virtual void updateFromJson(cJSON* pJson) override;
	virtual void deleteByResourceID(std::string strResourceID) override;

public:
	virtual std::unordered_set<std::string> getResourceIDList() override;
	virtual cJSON* getByResourceID(std::string strResourceID) override;
	virtual cJSON* getByName(std::string strResourceID, std::string strName) override;
	virtual void addRef(std::string strResourceID) {};
	virtual void releaseRef(std::string strResourceID) {};

	void getCurrentVerse(std::string& currentVerse) { currentVerse = m_currentVerse;}
	int getVerseGraphicsConfig(std::string key, std::string& data); // key { isDefault,isCurrent}
	int changeCurVerseGraphicsConfig(std::string id, std::string& data);

	int resetVerseGraphicsConfig(std::string& data);

	bool getRealTimeLighting() { return m_bRealTimeLighting; }
	void setRealTimeLighting(bool bSet) { m_bRealTimeLighting = bSet; }
private:
	std::unordered_map<std::string, cJSON*> m_mapGraphics; // <id, cjson*>>
	std::mutex								m_mutexMap;

	bool									m_bRealTimeLighting = false;

	std::string								m_currentVerse;
	bool									enableTAA;
	int										beiJingTime;
};
