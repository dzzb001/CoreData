
#include "GlobalResource.h"

CGlobalContent::CGlobalContent(cJSON* p) : CJson_base(p)
{
}

/*******************************************************************************
* 函数名称：
* 功能描述：根据指定名称获取json数据
* 输入参数：sType 字段名称
* 输出参数：
* 返 回 值：返回视点json字符串
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/
std::string CGlobalContent::GetJsonByName(const char* sType)
{
    std::string strJson;
    if (m_pJson)
    {
        cJSON* pItem = cJSON_GetObjectItem(m_pJson, sType);
        if (pItem)
        {
            char* pOut = cJSON_Print(pItem);
            strJson = pOut;
            free(pOut);
        }
    }
    return strJson;
}