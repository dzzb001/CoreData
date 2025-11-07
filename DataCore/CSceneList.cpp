#include "CSceneList.h"
#include <string>
#include "cJSON.h"
#include "common.h"

//#define USE_BEIHAI_VERSION 

CSceneList::CSceneList(cJSON* p) : CJson_base(p)
{
	if (cJSON_IsArray(m_pJson))
	{
		int nSize = cJSON_GetArraySize(m_pJson);
		for (size_t i = 0; i < nSize; i++)
		{
			cJSON* pItem = cJSON_GetArrayItem(m_pJson, i);

			std::string guid;
			cJSON_LoadFromObject(pItem, "guid", guid);
			m_mapSence[guid] = pItem;
			m_vecSence.push_back(guid);
			m_strCurSenceId = guid;
		}
	}
}

void CSceneList::getSceneList(std::string strRequest, std::string& strRespone)
{
	char* pOut = cJSON_Print(m_pJson);
	if (pOut) {
		strRespone = pOut;
		free(pOut);
	}
}

cJSON* CSceneList::getSceneViewpoint(std::string guid)
{
	cJSON* pRetJson = nullptr;
	std::map<std::string, cJSON*>::iterator itor = m_mapSence.find(guid);
	if (itor != m_mapSence.end())
	{
		if (itor->second) {
			/*
			* {
			*   实现逻辑
			*   1、判断是否设置默认主视点
			*	"viewPoint":{},如果viewPoint中的home==true 返回viewPoint json
			*   2、寻找是否定义了自定义视点
			*   "customViewPointList":[] //遍历customViewPointList数组，数组中哪个home==true，返回数组json
			*  }
			*/

			//step 1 判断是否设置默认主视点
#ifdef USE_BEIHAI_VERSION
			return cJSON_GetObjectItem(itor->second, "viewPoint");
#else
			pRetJson = cJSON_GetObjectItem(itor->second, "viewPoint");
			if (pRetJson) {
				bool bSelect = false;
				cJSON_LoadFromObject(pRetJson, "home", bSelect);

				if (bSelect)
					return pRetJson;
			}
#endif
			//step 2 判断是否定义了自定义视点，是返回当前自定义视点json
			cJSON* pcustomViewPointList = cJSON_GetObjectItem(itor->second, "customViewPointList");
			if (pcustomViewPointList)
			{
				if (cJSON_IsArray(pcustomViewPointList))
				{
					int nSize = cJSON_GetArraySize(pcustomViewPointList);
					for (size_t i = 0; i < nSize; i++)
					{
						cJSON* pItem = cJSON_GetArrayItem(pcustomViewPointList, i);

						bool bSelect = false;
						cJSON_LoadFromObject(pItem, "home", bSelect);

						if (bSelect)
							return pItem;
					}
				}
			}
		}
		return nullptr;
	}
	else
		return nullptr;
}
