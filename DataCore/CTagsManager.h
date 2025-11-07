#pragma once
#include <map>
#include <vector>
#include <string>
#include <mutex>
#include "cJSON.h"


class CTagsManager
{
public:
	CTagsManager() {};
	~CTagsManager() {};

	//添加tag
	int addTag(std::string strTag, std::string id,int nType);
	int deleteTag(std::string strTag, std::string id);

	//根据tag获取场景元素id
	int getElementDatabyTag(std::string strTag, std::map<int, std::map<std::string, int>> &mapRet);

	//获取所有tags数据，根据元素类型区分开，历史原因这样做，增加不必要的复杂度。
	//std::map<std::string,int> int 无用只是为了去重
	std::map<int, std::map<std::string,int>> getTagList();

	std::map<std::string, std::vector<std::pair<std::string, int>>> &getTagMap() { return m_mapTags; }
private:
	//tags map, 以tag为key将元素id数组作为value，tree manager主动同步m_mapTags
	std::map<std::string, std::vector<std::pair<std::string, int>>>		m_mapTags;
	std::mutex															m_mutexTags;
};

