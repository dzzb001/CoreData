#include "CTagsManager.h"
//#include "Poco/Logger.h"


/*******************************************************************************
* 函数名称：
* 功能描述：添加tag
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

int CTagsManager::addTag(std::string strTag, std::string id,int nType)
{
	std::lock_guard<std::mutex> lock(m_mutexTags);
	std::map<std::string, std::vector<std::pair<std::string, int>>>::iterator itor = m_mapTags.find(strTag);

	if (itor == m_mapTags.end())
	{
		std::vector <std::pair<std::string, int>> vecTag;
		vecTag.push_back(std::make_pair(id, nType));
		m_mapTags.emplace(strTag, vecTag);
	}
	else
	{
		itor->second.push_back(std::make_pair(id, nType));
	}
	return 0;
}


/*******************************************************************************
* 函数名称：
* 功能描述：根据tag id 删除tagmap对应数据
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

int CTagsManager::deleteTag(std::string strTag, std::string id)
{
	std::lock_guard<std::mutex> lock(m_mutexTags);
	std::map<std::string, std::vector<std::pair<std::string, int>>>::iterator itor = m_mapTags.find(strTag);
	if (itor == m_mapTags.end())
	{
		//Poco::Logger::get("RteLogger").warning("根据id[%s]删除tag，未找到对应的tag", id);
		return -1;
	}
	else
	{
		std::vector<std::pair<std::string, int>>::iterator itor2 = itor->second.begin();
		for (; itor2 != itor->second.end(); itor2++)
		{
			if (!(itor2->first).compare(id)) {
				itor->second.erase(itor2);
				break;
			}
		}

		if (itor->second.size() == 0) {
			m_mapTags.erase(itor);
		}
	}
	return 0;
}

int CTagsManager::getElementDatabyTag(std::string strTag, std::map<int, std::map<std::string, int>> &mapRet)
{
	//std::map<int, std::vector<std::string>> mapRet;
	std::lock_guard<std::mutex> lock(m_mutexTags);

	//根据tag查找到元素数组,如果为空，查找所有tags
	if (strTag.empty()) {
		std::map<std::string, std::vector<std::pair<std::string, int>>>::iterator itorFind = m_mapTags.begin();
		for (;itorFind != m_mapTags.end(); itorFind++)
		{
			//将查找到的元素根据类型分组
			std::vector<std::pair<std::string, int>>::iterator itorVec = itorFind->second.begin();
			for (; itorVec != itorFind->second.end(); itorVec++)
			{
				std::map<int, std::map<std::string, int>>::iterator itor2 = mapRet.find(itorVec->second); //根据类型查找
				if (itor2 == mapRet.end()) {
					std::map<std::string, int> mapElem; //元素map
					//vecTags.push_back(itorVec->first);
					mapElem[itorVec->first] = itorVec->second;  //元素id为key，元素类型为value
					mapRet[itorVec->second] = mapElem;          //元素类型为key,mapElem 为key存放在mapret
				}
				else {
					itor2->second[itorVec->first] = itorVec->second;
				}
			}
		}
	}
	else {
		std::map<std::string, std::vector<std::pair<std::string, int>>>::iterator itorFind = m_mapTags.find(strTag);
		if (itorFind != m_mapTags.end())
		{
			//将查找到的元素根据类型分组
			std::vector<std::pair<std::string, int>>::iterator itorVec = itorFind->second.begin();
			for (; itorVec != itorFind->second.end(); itorVec++)
			{
				std::map<int, std::map<std::string, int>>::iterator itor2 = mapRet.find(itorVec->second); //根据类型查找
				if (itor2 == mapRet.end()) {
					std::map<std::string, int> mapElem; //元素map
					//vecTags.push_back(itorVec->first);
					mapElem[itorVec->first] = itorVec->second;  //元素id为key，元素类型为value
					mapRet[itorVec->second] = mapElem;          //元素类型为key,mapElem 为key存放在mapret
				}
				else {
					itor2->second[itorVec->first] = itorVec->second;
				}
			}
		}
	}
	return 0;
}


/*******************************************************************************
* 函数名称：
* 功能描述：获取tags列表，需要根据元素类型分类
* 输入参数：
* 输出参数：
* 返 回 值：std::map<int, std::map<std::string,int>> first 元素类型， std::string,int 
			前一个为tag名称，int 无意义
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

std::map<int, std::map<std::string,int>> CTagsManager::getTagList()
{
	std::map<int, std::map<std::string,int>> mapRet;

	std::lock_guard<std::mutex> lock(m_mutexTags);
	std::map<std::string, std::vector<std::pair<std::string, int>>>::iterator itorTags = m_mapTags.begin();
	for (; itorTags != m_mapTags.end(); itorTags++)
	{
		std::vector<std::pair<std::string, int>>::iterator itorVec = itorTags->second.begin();
		for (; itorVec != itorTags->second.end(); itorVec++)
		{
			std::map<int, std::map<std::string, int>>::iterator itor2 = mapRet.find(itorVec->second);//根据int类型寻找
			if (itor2 == mapRet.end()) {
				std::map<std::string, int> vecTags;
				vecTags[itorTags->first] = 0; //存放tag
				mapRet[itorVec->second] = vecTags;
			}
			else {
				//使用map这里就不用去重了
				itor2->second[itorTags->first] = 0;
			}
		}
	}
	return mapRet;
}