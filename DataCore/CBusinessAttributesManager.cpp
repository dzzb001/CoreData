#include "CBusinessAttributesManager.h"
#include <iostream>
#include <set>
#include <algorithm>
#include <unordered_set>

CBusinessAttributesManager::CBusinessAttributesManager()
{

}

CBusinessAttributesManager::~CBusinessAttributesManager()
{

}

/*******************************************************************************
* 函数名称：
* 功能描述：添加外部属性索引
* 输入参数：strKey 外部属性名称
*			value  外部属性值，泛型支持 int、bool、dobule、char*四种类型
*			strId  元素的guid
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

int CBusinessAttributesManager::addBusAttr(std::string strKey, Attributes value, std::string strId)
{
	auto itorKey = m_mapAttributes.find(strKey);

	if (itorKey == m_mapAttributes.end())
	{
		std::unordered_map<Attributes, std::vector<std::string>, Attributes::Hash> tempMap;
		std::vector<std::string> ids;
		ids.push_back(strId);
		tempMap.emplace(value, ids);
		m_mapAttributes.emplace(strKey, tempMap);
	}
	else {
		std::unordered_map<Attributes, std::vector<std::string>, Attributes::Hash>::iterator itorValue = itorKey->second.find(value);
		if (itorValue == itorKey->second.end()) {
			std::vector<std::string> ids;
			ids.push_back(strId);
			itorKey->second.emplace(value, ids);
		}
		else {
			itorValue->second.push_back(strId);
		}
	}
	return 0;
}

/*******************************************************************************
* 函数名称：
* 功能描述：根据业务属性获取元素列表
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

int CBusinessAttributesManager::findIDsByAttri(std::string strKey, Attributes value, std::map<std::string, std::string>& retMap, bool reverse)
{
	if (!reverse) {
		auto itor = m_mapAttributes.find(strKey);
		if (itor != m_mapAttributes.end())
		{
			auto itor2 = itor->second.find(value);
			if (itor2 != itor->second.end()) {
				auto itor3 = itor2->second.begin();
				for (; itor3 != itor2->second.end(); itor3++)
				{
					retMap[*itor3] = *itor3;
				}
			}
		}
	}
	else { //废弃
		auto itor = m_mapAttributes.find(strKey);
		if (itor != m_mapAttributes.end())
		{
			for (const auto& pair : itor->second) {
				if (pair.first != value)
				{
					auto itor3 = pair.second.begin();
					for (; itor3 != pair.second.end(); itor3++)
					{
						retMap[*itor3] = *itor3;
					}
				}
			}
		}
	}
	return 0;
}

//两个vector去重求交集  //排序，时间复杂度高，实际测试这个函数快
std::vector<std::string> findIntersection(const std::vector<std::string>& vec1, const std::vector<std::string>& vec2) {
	// 将两个vector转换为set，自动去重并排序
	std::set<std::string> set1(vec1.begin(), vec1.end());
	std::set<std::string> set2(vec2.begin(), vec2.end());

	std::vector<std::string> result;
	// 计算交集，结果将自动排序
	std::set_intersection(
		set1.begin(), set1.end(),
		set2.begin(), set2.end(),
		std::back_inserter(result)
	);

	return result;
}

//不考虑排序
std::vector<std::string> findIntersection2(
	const std::vector<std::string>& vec1,
	const std::vector<std::string>& vec2
) {
	std::unordered_set<std::string> set2(vec2.begin(), vec2.end());
	std::unordered_set<std::string> seen;
	std::vector<std::string> result;

	for (const auto& s : vec1) {
		if (set2.count(s) && !seen.count(s)) {
			result.push_back(s);
			seen.insert(s);
		}
	}
	return result;
}

//多个条件取交集node
int CBusinessAttributesManager::findIDsByAttri(std::vector<std::pair<std::string, Attributes>> vecAttri, std::map<std::string, std::string>& retMap)
{
	std::vector<std::string> vecCurret;
	std::vector<std::string> result;

	for (size_t i = 0; i < vecAttri.size(); i++)
	{
		vecCurret.clear();
		auto itor = m_mapAttributes.find(vecAttri[i].first);
		if (itor != m_mapAttributes.end()) {
			auto itor2 = itor->second.find(vecAttri[i].second);

			if (itor2 != itor->second.end()) {
				if (i == 0)
				{
					result.insert(result.end(), itor2->second.begin(), itor2->second.end());
					continue;
				}
				else {
					vecCurret.insert(vecCurret.end(), itor2->second.begin(), itor2->second.end());

					std::vector<std::string> retTemp;
					retTemp = findIntersection(vecCurret, result);
					result.swap(retTemp);
					if (result.size() == 0)
						break;
				}
			}
		}
	}
	for (auto itor : result)
	{
		retMap[itor] = itor;
	}
	return 0;
}

/*******************************************************************************
* 函数名称：
* 功能描述：删除指定元素指定key，指定value的map数据
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/
void CBusinessAttributesManager::deleteAttri(std::string strKey, Attributes value, std::string id)
{
	auto itorFind = m_mapAttributes.find(strKey);
	if (itorFind != m_mapAttributes.end()) {
		auto itorFind2 = itorFind->second.find(value);
		if (itorFind2 != itorFind->second.end()) {
			auto& vecTemp = itorFind2->second;
			for (auto itor = vecTemp.begin(); itor != vecTemp.end(); itor++)
			{
				if (!id.compare(itor->c_str()))
				{
					vecTemp.erase(itor);
					break;
				}
			}

			if (vecTemp.size() == 0)
			{
				itorFind->second.erase(itorFind2);
			}

			if (itorFind->second.size() == 0) {
				m_mapAttributes.erase(itorFind);
			}
		}
	}
}
