#include "stdafx.h"
#include "DataManager.h"
#include "Tool/cJSON.h"
#include "SceneDataType.h"
#include "config.h"

#define Log LogN(1101)
using namespace RTE;

CDataManager::CDataManager() : m_nLoadMode(0),
m_pmsData(nullptr)
{
    m_bLoadComplite.store(false);
}

CDataManager::~CDataManager()
{
    if (m_pmsData)
        cJSON_Delete(m_pmsData);
}

void CDataManager::InitMapCallback()
{
    m_mapCallbackSubscribe["area"] = false;
    m_mapCallbackSubscribe["heatmap"] = false;
    m_mapCallbackSubscribe["coordinate"] = false;
    m_mapCallbackSubscribe["data"] = false;
    m_mapCallbackSubscribe["vegetation"] = false;
    m_mapCallbackSubscribe["video"] = false;
    m_mapCallbackSubscribe["poi"] = false;
    m_mapCallbackSubscribe["contour"] = false;
    m_mapCallbackSubscribe["path"] = false;
    m_mapCallbackSubscribe["pixelPoint"] = false;
    m_mapCallbackSubscribe["effect"] = false;
    m_mapCallbackSubscribe["model"] = false;
    m_mapCallbackSubscribe["river"] = false;
    m_mapCallbackSubscribe["fence"] = false; //空间触发器
    m_mapCallbackSubscribe["simplePoi"] = false;
    m_mapCallbackSubscribe["lonlatData"] = false;
    m_mapCallbackSubscribe["elementPosition"] = false;

    //新增开关,都对应redis状态频道
    m_mapCallbackSubscribe["modelPathFly"] = false;
    m_mapCallbackSubscribe["cameraPathFly"] = false;
    m_mapCallbackSubscribe["cameraVPFly"] = false;
    m_mapCallbackSubscribe["pathFly"] = false;
    m_mapCallbackSubscribe["virtualCamera"] = false;
    m_mapCallbackSubscribe["videoTimestamp"] = false;
}

bool CDataManager::GetCallbackState(std::string strEvent)
{
    auto itor = m_mapCallbackSubscribe.find(strEvent);
    if (itor != m_mapCallbackSubscribe.end()) {
        return itor->second;
    }
    return false;
}

//解析大json，然后结构化数据
int CDataManager::ParsePmsJson(const char* pSrcData, const char * ProjectId, const char* SenceId)
{
    bool bError = true;
    if (!m_bLoad) 
    {
        m_strProjectId = ProjectId;
        m_strSenceId = SenceId;

        if (m_pmsData)
        {
            cJSON_Delete(m_pmsData);
            m_pmsData = nullptr;
        }
        m_pmsData = cJSON_Parse(pSrcData);

        do
        {
            cJSON* pData = cJSON_GetObjectItem(m_pmsData, "data");
            if (!pData)
                break;

            //把第一个 content Detach出来
            cJSON* pFirstContent = cJSON_DetachItemFromObject(pData, "content");
            if (!pFirstContent)
                break;

            // 加载全局数据
            GetGlobalSettingInstance().loadFromJson(pFirstContent);

            //把第二个 content Detach出来
            cJSON* pSecondContent = cJSON_DetachItemFromObject(pFirstContent, "content");
            if (!pSecondContent)
                break;

            cJSON* pSceneList = cJSON_DetachItemFromObject(pSecondContent, "sceneList");
            cJSON* pTreeItem = cJSON_DetachItemFromObject(pSecondContent, "treeItem");

            //SecondContent 目前没有任何数据可以删除了。 
            cJSON_Delete(pSecondContent);

            //把第一个的json指针保存到CGlobalContent中，注意不包含第二个content
            m_pGlobal = std::make_shared<CGlobalContent>(pFirstContent);

            //将数据转移到scence类中待处理
            m_pScenceList = std::make_shared<CSceneList>(pSceneList); //会在基类析构时删除pSceneList

            //将数据转移到treeitem类中待处理
            {
                std::unique_lock lock(m_mutexTree2);
                m_pTreeItem = std::make_shared<CTreeItem>(pTreeItem, m_strSenceId);
                m_pTreeItem->setCurSenceId(m_strSenceId);
                m_pTreeItem->setSenceList(m_pScenceList->m_vecSence);
            }
            bError = false;
        } while (0);

        if (bError) {
            //Poco::Logger::get("RteLogger").error("get sence recive data invalud");
            return -1;
        }

        m_bLoad = true;

        if(m_bLoadComplite)
        {
            Interface::close();
            m_condCloseCallback.wait_for("123", 10);
            m_bLoadComplite.store(false);
            if (m_callback)
                m_callback("LOADPROJECT_COMPLITE", "false", m_pUser);
        }

        m_nLoadMode = 0;
    }

    return 0;
}

/*******************************************************************************
* 函数名称：
* 功能描述：获取tag列表
* 输入参数：request 没有实际意义
*           strRespone 返回的数据 只返回data数据，web返回在外部拼接
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

void CDataManager::getTagList(std::string request, std::string& strRespone)
{
    //std::lock_guard<std::mutex> lock(m_mutexTree);
    std::shared_lock lock(m_mutexTree2);
    if (m_pTreeItem)
    {
        strRespone = "Server not load resource";
        return;
    }

    std::map<int, std::map<std::string, int>> retMap = m_pTreeItem->m_tagManager.getTagList();

    cJSON* pRoot = cJSON_CreateObject();
    std::map<int, std::map<std::string, int>>::iterator itor = retMap.begin();
    for (; itor != retMap.end(); itor++)
    {
        //创建数组
        std::string strArrayName = RTE::getModelStrTypeByType(itor->first);
        cJSON* pArray = cJSON_CreateArray();
        std::map<std::string, int>::iterator itorSecond = itor->second.begin();
        for (; itorSecond != itor->second.end(); itorSecond++)
        {
            cJSON_AddItemToArray(pArray, cJSON_CreateString(itorSecond->first.c_str()));
        }
        cJSON_AddItemToObject(pRoot, strArrayName.c_str(), pArray);
    }

    char* pOut = cJSON_Print(pRoot);
    strRespone = pOut;
    std::cout << pOut << std::endl;

    free(pOut);
    cJSON_Delete(pRoot);
}

/*******************************************************************************
* 函数名称：
* 功能描述：根据tag获取场景元素
* 输入参数：
*           {
                "tags":["path","path2"]
            }
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

void CDataManager::getTag(std::string request, std::string& strRespone)
{
    //std::lock_guard<std::mutex> lock(m_mutexTree);
    std::shared_lock lock(m_mutexTree2);
    if (m_pTreeItem)
    {
        strRespone = "Server not load resource";
        return;
    }
    //step1 解析json，并拆出tags数组  Invalid parameter
    cJSON* pRoot = cJSON_Parse(request.c_str());
    if (!pRoot)
    {
        strRespone = "Invalid parameter";
        return;
    }

    cJSON* pAarry = cJSON_GetObjectItem(pRoot, "tags");
    if (!pAarry)
    {
        strRespone = "Invalid parameter";
        return;
    }

    int nCount = cJSON_GetArraySize(pAarry);

    //step1 根据tag获取元素id
    std::map<int, std::map<std::string, int>> mapRet;
    if (nCount == 0)
    {
        //0 代表查找所有tag
        m_pTreeItem->m_tagManager.getElementDatabyTag("", mapRet);
    }
    else {
        //指定tag查找，数组中的tag不能为空，为空遍历所有。
        for (size_t i = 0; i < nCount; i++)
        {
            cJSON* pArrayItem = cJSON_GetArrayItem(pAarry, i);
            if (pArrayItem) {
                if (pArrayItem->valuestring && std::string(pArrayItem->valuestring).compare("")) {
                    m_pTreeItem->m_tagManager.getElementDatabyTag(pArrayItem->valuestring, mapRet);
                }
            }
        }
    }

    //step2 根据元素id获取元素详情
    cJSON* pResponeJson = cJSON_CreateObject();
    std::map<int, std::map<std::string, int>>::iterator itor = mapRet.begin();
    for (; itor != mapRet.end(); itor++)
    {
        std::string name;
        name = RTE::getModelStrTypeByType(itor->first);

        //创建数组
        cJSON* pAarry2 = cJSON_AddArrayToObject(pResponeJson, name.c_str());
        std::map<std::string, int>::iterator itorElement = itor->second.begin();
        for (; itorElement != itor->second.end(); itorElement++)
        {
            if (m_pTreeItem) {
                cJSON* pElement = m_pTreeItem->getElementJson(itorElement->first);
                if (pElement)
                    cJSON_AddItemToArray(pAarry2, pElement);
            }
        }
    }
    //step3 将组装好的json填充到strRespone中
    char* pOut = cJSON_Print(pResponeJson);
    if (pOut)
    {
        strRespone = pOut;
        free(pOut);
    }
    cJSON_Delete(pResponeJson);
    cJSON_Delete(pRoot);
}

int CDataManager::updateTag(std::string request, std::string& strRespone)
{
    //std::lock_guard<std::mutex> lock(m_mutexTree);
    std::unique_lock lock(m_mutexTree2);
    if (!m_pTreeItem)
    {
        strRespone = "Server not load resource";
        return -1;
    }
    //step1 解析json，并拆出tags数组  Invalid parameter
    cJSON* pRoot = cJSON_Parse(request.c_str());
    if (!pRoot)
    {
        strRespone = "Invalid parameter";
        return -1;
    }

    if (cJSON_IsArray(pRoot))
    {
        int nCount = cJSON_GetArraySize(pRoot);
        //遍历数组每一个元素进行修改
        for (size_t i = 0; i < nCount; i++)
        {
            cJSON* pArrayItem = cJSON_GetArrayItem(pRoot, i);
            if (pArrayItem) {
                cJSON* pJsonTags = nullptr;
                cJSON* pJsonId = nullptr;

                pJsonId = cJSON_GetObjectItem(pArrayItem, "id");
                if (pJsonId && pJsonId->valuestring)
                {
                    if (cJSON_HasObjectItem(pArrayItem, "tags")) {
                        pJsonTags = cJSON_GetObjectItem(pArrayItem, "tags");
                        if (!pJsonId || !pJsonTags) {
                            //是否记录错误返回给web
                            continue;
                        }
                        //调用底层数据进行更新
                        std::string strRet;
                        m_pTreeItem->updateTagsByElementId(pJsonId->valuestring, pJsonTags, "", strRet);
                    }
                    else if (cJSON_HasObjectItem(pArrayItem, "tags+")) {
                        pJsonTags = cJSON_GetObjectItem(pArrayItem, "tags+");
                        if (!pJsonId || !pJsonTags) {
                            //是否记录错误返回给web
                            continue;
                        }
                        //调用底层数据进行更新
                        std::string strRet;
                        m_pTreeItem->updateTagsByElementId(pJsonId->valuestring, pJsonTags, "+", strRet);
                    }
                    else if (cJSON_HasObjectItem(pArrayItem, "tags-")) {
                        pJsonTags = cJSON_GetObjectItem(pArrayItem, "tags-");
                        if (!pJsonId || !pJsonTags) {
                            //是否记录错误返回给web
                            continue;
                        }
                        //调用底层数据进行更新
                        std::string strRet;
                        m_pTreeItem->updateTagsByElementId(pJsonId->valuestring, pJsonTags, "-", strRet);
                    }
                }
                else {
                    continue;
                }
            }
        }
    }
    else {
        strRespone = "Invalid parameter [Tag is not array]";
        return -1;
    }
    cJSON_Delete(pRoot);

    return 0;
}

//元素相关接口
void CDataManager::getElement(std::string request, std::string& strRespone)
{
    std::shared_lock lock(m_mutexTree2);
    if (m_pTreeItem)
    {
        m_pTreeItem->getElement(request, strRespone);
    }
}


/*
*
*
{
    "method": "update",
    "Element": {
        "id": ["e0498830-0870-41bf-abbe-6ca7a52b1051","e0498830-0870-41bf-abbe-6ca7a52b1050"],
        "type": 29,
        "elementData": {
            "_polygonColor": {
                "w": 0.5,
                "x": 0.0,
                "y": 1.0,
                "z": 0.0
            }
        },
    "foreignKey":	[],
    }
}
id、type 前置条件必填，其他为修改参数
*/
/*******************************************************************************
* 函数名称：
* 功能描述：更新元素数据
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

int CDataManager::updateElement(std::string request, std::string& strRespone)
{
    std::unique_lock lock(m_mutexTree2);
    std::string strMessage;
    if (m_pTreeItem)
    {
        return m_pTreeItem->updateElement(request, strRespone, strMessage);
    }
    return -1;
}

int CDataManager::addElement(std::string request, std::string& strRespone)
{
    std::unique_lock lock(m_mutexTree2);
    if (m_pTreeItem)
    {
        return m_pTreeItem->addElement(request, strRespone);
        //return m_pTreeItem->addElementArrary(request, strRespone);
    }
    return -1;
}


int CDataManager::deleteElement(std::string request, std::string& strRespone)
{
    std::unique_lock lock(m_mutexTree2);
    if (m_pTreeItem)
    {
        return m_pTreeItem->deleteElement(request, strRespone);
    }
    return -1;
}

/*******************************************************************************
* 函数名称：
* 功能描述：元素控制
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

int CDataManager::controlElement(std::string request, std::string& strRespone)
{
    std::shared_lock lock(m_mutexTree2);
    if (m_pTreeItem)
    {
        return m_pTreeItem->controlElement(request, strRespone);
    }

    return -1;
}

//透传接口 element handle 
int CDataManager::GeneralInterface(std::string request, std::string& strRespone)
{
    std::shared_lock lock(m_mutexTree2);
    if (m_pTreeItem)
    {
        return m_pTreeItem->GeneralInterface(request, strRespone);
    }

    return -1;
}

//透传接口 event handle 
int CDataManager::EventGeneralInterface(std::string request, std::string& strRespone)
{
    std::shared_lock lock(m_mutexTree2);
    if (m_pTreeItem)
    {
        return m_pTreeItem->EventGeneralInterface(request, strRespone);
    }

    return -1;
}


//获取元素结构树接口，根据元素guid 获取元素的树形结构

/*******************************************************************************
* 函数名称：
* 功能描述：
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* {
    "method": "get",
    "ElementTree": {
        "guid":"ce0b08af-215f-41c1-8ad1-f22a1a011ec1"
    }
}
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

void CDataManager::getElementTree(std::string request, std::string& strRespone)
{
    std::shared_lock lock(m_mutexTree2);
    if (m_pTreeItem)
    {
        m_pTreeItem->getElementTree(request, strRespone);
    }
}

/*******************************************************************************
* 函数名称：
* 功能描述：element一些特殊处理接口，不需要调用3d底层
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 示例
{
    "method": "get",
    "ElementGeneralInterface": {
        "event": "getElementBuildID",
        "data": {
            "guid": "1212121"
        }
    }
}
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

void CDataManager::getElementGeneralInterface(std::string request, std::string& strRespone)
{
    strRespone = "Invalid parameter";
    std::shared_lock lock(m_mutexTree2);
    if (m_pTreeItem)
    {
        cJSON* pRoot = cJSON_Parse(request.c_str());
        if (pRoot) {
            std::string strEvent;
            cJSON_LoadFromObject(pRoot, "event", strEvent);

            if (!strEvent.compare("getElementBuildID"))
                m_pTreeItem->getElementBuildID(request, strRespone);
            else if (!strEvent.compare("getElementTree"))
            {
                m_pTreeItem->getElementTree2(request, strRespone);
            }
            else if (!strEvent.compare("open2DWnd"))
            {
                //open2DWnd(strRespone);
            }
            else if (!strEvent.compare("close2DWnd"))
            {
                //close2DWnd(strRespone);
            }
            else if (!strEvent.compare("frameRate"))
            {
                //getViewFrameRate(strRespone);
            }
            else if (!strEvent.compare("setMainViewPoint"))
            {
                setMainViewPointByInterface(strRespone);
            }
            cJSON_Delete(pRoot);
        }
    }
}

/*******************************************************************************
* 函数名称：
* 功能描述：球机追视
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/
int CDataManager::controlVideoTrackPara(std::string request, std::string& strRespone)
{
    cJSON* pTemp = cJSON_Parse(request.c_str());
    if (pTemp) {

        bool ptzTracking = cJSON_IsTrue(cJSON_GetObjectItem(pTemp, "ptzTracking"));
        int  getPositionMethod = cJSON_GetNumberValue(cJSON_GetObjectItem(pTemp, "getPositionMethod"));

        cJSON* pRequestJson = cJSON_CreateObject();
        cJSON_AddStringToObject(pRequestJson, "method", "control");

        cJSON* operate = cJSON_CreateObject();
        cJSON_AddItemToObject(operate, "VideoTrackPara", pTemp);

        cJSON_AddItemToObject(pRequestJson, "operate", operate);

        char* pOut = cJSON_Print(pRequestJson);
        std::string strInput = pOut;
        free(pOut);

        std::string strOutput;
        int nRet = Interface::elementHandle(strInput, strOutput);
        if (!nRet) {
            strRespone = R"(
            {
             })";
            //解析收到的ids //成功返回guid数组
            cJSON* pRetJson = cJSON_Parse(strOutput.c_str());
            if (pRetJson)
            {
                double code = cJSON_GetNumberValue(cJSON_GetObjectItem(pRetJson, "code"));
                if (code == 0)
                {
                    std::vector<std::pair<std::string, std::string>> ids;
                    cJSON* data = cJSON_GetObjectItem(pRetJson, "data");
                    if (cJSON_IsArray(data))
                    {
                        //std::lock_guard<std::mutex> lock(m_mutexTree);
                        std::shared_lock lock(m_mutexTree2);
                        int nSize = cJSON_GetArraySize(data);
                        for (size_t i = 0; i < nSize; i++)
                        {
                            std::string strId, strForeignKey;
                            cJSON* pItem = cJSON_GetArrayItem(data, i);
                            strId = cJSON_GetStringValue(pItem) ? cJSON_GetStringValue(pItem) : "";
                            std::shared_ptr<Node> pNode = m_pTreeItem->getElementNode(strId);
                            if (pNode && cJSON_IsArray(pNode->foreignKey.get()))
                            {
                                int nSize2 = cJSON_GetArraySize(pNode->foreignKey.get());
                                if (nSize2 > 0)
                                {
                                    cJSON* pForeignKeyItem = cJSON_GetArrayItem(pNode->foreignKey.get(), 0);
                                    strForeignKey = cJSON_GetStringValue(pForeignKeyItem) ? cJSON_GetStringValue(pForeignKeyItem) : "";
                                }
                            }

                            ids.push_back(std::pair<std::string, std::string>(strId, strForeignKey));
                        }
                    }

                    //将查询到的ids 发送给reids
                    {
                        cJSON* pRoot = cJSON_CreateObject();
                        cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());

                        cJSON* idsJson = cJSON_CreateArray();
                        for (size_t i = 0; i < ids.size(); i++)
                        {
                            cJSON* idItem = cJSON_CreateObject();
                            cJSON_AddStringToObject(idItem, "id", ids[i].first.c_str());
                            cJSON_AddStringToObject(idItem, "foreignKey", ids[i].second.c_str());

                            cJSON_AddItemToArray(idsJson, idItem);
                        }
                        cJSON_AddItemToObject(pRoot, "ids", idsJson);

                        cJSON_AddBoolToObject(pRoot, "ptzTracking", ptzTracking);
                        cJSON_AddNumberToObject(pRoot, "getPositionMethod", getPositionMethod);
                        cJSON_AddStringToObject(pRoot, "click", "single");

                        char* pOut2 = cJSON_Print(pRoot);
                        if(m_callback)
                        {
                            m_callback("VIDEO_PIXEL", pOut2, m_pUser);
                        }
                        Log(Tool::Debug, "%s", pOut2);
                        free(pOut2);
                        cJSON_Delete(pRoot);
                    }
                }
                cJSON_Delete(pRetJson);
            }
        }
        else {
            strRespone = strOutput;
        }
        cJSON_Delete(pRequestJson);
        return nRet;
    }
    else {
        strRespone = "Invalid parameter";
    }
    return -1;
}

// 全局数据相关接口
// 主视点
void CDataManager::getSceneViewPoint(std::string strRequest, std::string& strRespone) {
    cJSON* pJson = cJSON_Parse(strRequest.c_str());
    if (pJson != nullptr) {
        if (cJSON_IsObject(pJson)) {
            cJSON* pJsonMainViewpoint = GetGlobalSettingInstance().mainViewpointToJson(pJson);
            if (pJsonMainViewpoint != nullptr) {
                strRespone = cJSON_PrintFromJSON(pJsonMainViewpoint);
                cJSON_Delete(pJsonMainViewpoint);
            }
        }

        cJSON_Delete(pJson);
    }
    return;
}

int CDataManager::updateSceneViewPoint(std::string strRequest, std::string& strRespone) {
    cJSON* pJson = cJSON_Parse(strRequest.c_str());
    if (pJson != nullptr) {
        if (cJSON_IsObject(pJson)) {
            GetGlobalSettingInstance().updateMainViewpointFromJson(pJson);

            cJSON* pJsonMainViewpoint = GetGlobalSettingInstance().mainViewpointToJson(pJson);
            if (pJsonMainViewpoint != nullptr) {
                strRespone = cJSON_PrintFromJSON(pJsonMainViewpoint);

                cJSON_Delete(pJsonMainViewpoint);
            }
        }

        cJSON_Delete(pJson);
    }

    return 0;
}

// 模板
void CDataManager::getProjectTemplate(std::string strRequest, std::string& strRespone) {
    cJSON* pJson = cJSON_Parse(strRequest.c_str());
    if (pJson != nullptr) {
        if (cJSON_IsObject(pJson)) {
            int nType;
            cJSON_LoadFromObject(pJson, "type", nType);
            RTE::SceneDataType sceneDataType = (RTE::SceneDataType)nType;

            cJSON* pJsonTemplate = GetGlobalSettingInstance().projectTemplateToJson(sceneDataType);
            if (pJsonTemplate != nullptr) {
                strRespone = cJSON_PrintFromJSON(pJsonTemplate);

                cJSON_Delete(pJsonTemplate);
            }
        }

        cJSON_Delete(pJson);
    }

    return;
}

int CDataManager::addProjectTemplate(std::string strRequest, std::string& strRespone) {
    cJSON* pJson = cJSON_Parse(strRequest.c_str());
    if (pJson != nullptr) {
        if (cJSON_IsObject(pJson)) {
            int nType;
            cJSON_LoadFromObject(pJson, "type", nType);
            RTE::SceneDataType sceneDataType = (RTE::SceneDataType)nType;

            GetGlobalSettingInstance().addProjectTemplateFromJson(sceneDataType, pJson);

            cJSON* pJsonTemplate = GetGlobalSettingInstance().projectTemplateToJson(sceneDataType);
            if (pJsonTemplate != nullptr) {
                strRespone = cJSON_PrintFromJSON(pJsonTemplate);

                cJSON_Delete(pJsonTemplate);
            }
        }

        cJSON_Delete(pJson);
    }

    return 0;
}

int CDataManager::updateProjectTemplate(std::string strRequest, std::string& strRespone) {
    cJSON* pJson = cJSON_Parse(strRequest.c_str());
    if (pJson != nullptr) {
        if (cJSON_IsObject(pJson)) {
            int nType;
            cJSON_LoadFromObject(pJson, "type", nType);
            RTE::SceneDataType sceneDataType = (RTE::SceneDataType)nType;

            GetGlobalSettingInstance().updateProjectTemplateFromJson(sceneDataType, pJson);

            cJSON* pJsonTemplate = GetGlobalSettingInstance().projectTemplateToJson(sceneDataType);
            if (pJsonTemplate != nullptr) {
                strRespone = cJSON_PrintFromJSON(pJsonTemplate);

                cJSON_Delete(pJsonTemplate);
            }
        }

        cJSON_Delete(pJson);
    }

    return 0;
}

int CDataManager::deleteProjectTemplate(std::string strRequest, std::string& strRespone) {
    cJSON* pJson = cJSON_Parse(strRequest.c_str());
    if (pJson != nullptr) {
        if (cJSON_IsObject(pJson)) {
            int nType;
            cJSON_LoadFromObject(pJson, "type", nType);
            RTE::SceneDataType sceneDataType = (RTE::SceneDataType)nType;

            cJSON* pJsonResourceID = cJSON_GetObjectItem(pJson, "resourceId");
            if (pJsonResourceID != nullptr) {
                if (cJSON_IsArray(pJsonResourceID)) {
                    for (int i = 0; i < cJSON_GetArraySize(pJsonResourceID); ++i) {
                        cJSON* pJsonResourceIDItem = cJSON_GetArrayItem(pJsonResourceID, i);
                        if (pJsonResourceIDItem != nullptr) {
                            if (cJSON_IsString(pJsonResourceIDItem)) {
                                std::string strResourceID;
                                cJSON_LoadFromValue(pJsonResourceIDItem, strResourceID);

                                GetGlobalSettingInstance().deleteProjectTemplateByResourceID(sceneDataType, strResourceID);
                            }
                        }
                    }
                }
            }

            cJSON* pJsonTemplate = GetGlobalSettingInstance().projectTemplateToJson(sceneDataType);
            if (pJsonTemplate != nullptr) {
                strRespone = cJSON_PrintFromJSON(pJsonTemplate);
                cJSON_Delete(pJsonTemplate);
            }
        }

        cJSON_Delete(pJson);
    }

    return 0;
}

void CDataManager::ProjectTemplateGeneralInterface(std::string strRequest, std::string& strRespone)
{
    cJSON* pJson = cJSON_Parse(strRequest.c_str());
    if (pJson != nullptr) {
        if (cJSON_IsObject(pJson)) {
            int nType;
            cJSON_LoadFromObject(pJson, "type", nType);
            RTE::SceneDataType sceneDataType = (RTE::SceneDataType)nType;

            if (sceneDataType == 200) {
                std::string configData;
                std::string strEvent;
                cJSON_LoadFromObject(pJson, "event", strEvent);
                if (!strEvent.compare("switchVerseGraphicsConfig"))
                {
                    cJSON* data = cJSON_GetObjectItem(pJson, "data");

                    if (data) {
                        std::string id;
                        cJSON_LoadFromObject(data, "id", id);

                        //获取指定id的配置
                        cJSON* pConfig = GetGlobalSettingInstance().getByResourceID(RTE::SDT_verseGraphicsConfig, id);
                        if (pConfig) {
                            cJSON_LoadFromObject(pConfig, "data", configData);
                            if (!LoadGraphicsConfig(configData, strRespone))
                            {
                                //3d 加载成功之后改变全局配置
                                GetGlobalSettingInstance().changeVerseGraphicsConfig(id, configData);
                            }
                            cJSON_Delete(pConfig);
                        }
                    }
                }
                else if (!strEvent.compare("resetVerseGraphicsConfig"))
                {
                    GetGlobalSettingInstance().getVerseGraphicsConfig("isDefault", configData);
                    if (!LoadGraphicsConfig(configData, strRespone))
                        GetGlobalSettingInstance().resetVerseGraphicsConfig(configData);
                }
                else if (!strEvent.compare("switchRealTimeLighting"))
                {
                    cJSON* data = cJSON_GetObjectItem(pJson, "data");

                    if (data) {
                        bool realTimeLighting;
                        cJSON_LoadFromObject(data, "realTimeLighting", realTimeLighting);

                        if (!switchRealTimeLighting(realTimeLighting, strRespone))
                            GetGlobalSettingInstance().setRealTimeLighting(realTimeLighting);
                    }
                }
            }
        }

        cJSON_Delete(pJson);
    }
}

void CDataManager::getSceneList(std::string strRequest, std::string& strRespone)
{
    if (m_pScenceList)
    {
        m_pScenceList->getSceneList(strRequest, strRespone);
    }
}

//拿到大json是一个比较耗时的请求，先判断下是否可以切换。
bool CDataManager::canSwitchScence(std::string strRequest, std::string& strRespone)
{
    strRespone = "Invalid parameter";
    cJSON* pRoot = cJSON_Parse(strRequest.c_str());

    if (pRoot)
    {
        std::string projectId, sceneId;
        cJSON_LoadFromObject(pRoot, "projectId", projectId);
        cJSON_LoadFromObject(pRoot, "sceneId", sceneId);
        if (!m_strProjectId.compare(projectId.c_str()))//同一个项目
        {
            //同一个项目不同场景可以切换，但是需要重新加载模型
            if (!sceneId.compare(m_strSenceId.c_str())) {
                strRespone = "{ \"message:\" \"projectId sceneId is same.\"}";
                return false;
            }
            strRespone = "{}";
            return true;
        }
        else { //不同项目可能可以切换
            if (!sceneId.compare("")) {
                strRespone = "{ \"message:\" \"sceneId is null\"}";
                return false;
            }
            strRespone = "{}";
            return true;
        }
    }
    return false;
}

//切换场景（支持不同项目场景切换，也支持同一项目不同场景切换）
/*
* strTreeJson:左侧树大json
* strRequest：{
                "projectId":"",
                "sceneId": ""
	           }
*/
int CDataManager::switchScence(std::string strTreeJson, std::string strRequest, std::string& strRespone)
{
    strRespone = "Invalid parameter";
    cJSON* pRoot = cJSON_Parse(strRequest.c_str());
    if (pRoot)
    {
        std::string projectId, sceneId;
        cJSON_LoadFromObject(pRoot, "projectId", projectId);
        cJSON_LoadFromObject(pRoot, "sceneId", sceneId);
        if (!m_strProjectId.compare(projectId.c_str()))//同一个项目
        {
            //同一个项目不同场景可以切换，但是需要重新加载模型
            if (!sceneId.compare(m_strSenceId.c_str())) {
                strRespone = "{ \"message:\" \"projectId sceneId is same.\"}";
                return -1;
            }
            strRespone = "{}";
            m_bLoad = false;
            return ParsePmsJson(strTreeJson.c_str(), projectId.c_str(), sceneId.c_str());
        }
        else { //不同项目可能可以切换
            if (!sceneId.compare("")) {
                strRespone = "{ \"message:\" \"sceneId is null\"}";
                return -1;
            }
            strRespone = "{}";

            m_bLoad = false;
            return ParsePmsJson(strTreeJson.c_str(), projectId.c_str(), sceneId.c_str());
        }
    }
    return -1;
}

//设置主视点
void CDataManager::SetMainViewPoint()
{
    cJSON* pViewPoint = nullptr;
    //首先从场景列表中查找当前场景主视点，如果为空再从上一层调用当前大json的主视点。

    if (m_pScenceList)
    {
        pViewPoint = m_pScenceList->getSceneViewpoint(m_strSenceId);
    }

    if (!pViewPoint) {
        Log(Tool::Error, "view point is null");
        return;
    }

    if (!m_pGlobal) {
        Log(Tool::Error, "global point is null");
        return;
    }

    if (!pViewPoint->child) {
        std::string strViewPoint = m_pGlobal->GetJsonByName("viewPoint");
        pViewPoint = cJSON_Parse(strViewPoint.c_str());
    }

    YZViewPoint pot;

    cJSON* plongitude = cJSON_GetObjectItem(pViewPoint, "longitude");

    if (plongitude)
        pot._center.x = plongitude->valuedouble;

    cJSON* platitude = cJSON_GetObjectItem(pViewPoint, "latitude");
    if (platitude)
        pot._center.y = platitude->valuedouble;

    cJSON* pheight = cJSON_GetObjectItem(pViewPoint, "height");
    if (pheight)
        pot._center.z = pheight->valuedouble;

    cJSON* pheading = cJSON_GetObjectItem(pViewPoint, "heading");
    if (pheading)
        pot._heading = pheading->valuedouble;

    cJSON* ppitch = cJSON_GetObjectItem(pViewPoint, "pitch");
    if (ppitch)
        pot._pitch = ppitch->valuedouble;

    cJSON* range = cJSON_GetObjectItem(pViewPoint, "range");
    if (range)
        pot._range = range->valuedouble;

    cJSON* staytime = cJSON_GetObjectItem(pViewPoint, "staytime");
    if (staytime)
        pot._stay_s = staytime->valueint;

    cJSON* vpImageURl = cJSON_GetObjectItem(pViewPoint, "vpImageURl");
    if (vpImageURl && vpImageURl->valuestring)
        pot._vpImageURl = vpImageURl->valuestring;

    cJSON* vaild = cJSON_GetObjectItem(pViewPoint, "vaild");
    if (vaild)
        pot._isValid = vaild->type == cJSON_True ? true : false;

    cJSON* relPosX = cJSON_GetObjectItem(pViewPoint, "relPosX");
    if (relPosX)
        pot._relPos.x = relPosX->valueint;

    cJSON* relPosY = cJSON_GetObjectItem(pViewPoint, "relPosY");
    if (relPosY)
        pot._relPos.y = relPosY->valueint;

    cJSON* relPosZ = cJSON_GetObjectItem(pViewPoint, "relPosZ");
    if (relPosZ)
        pot._relPos.z = relPosZ->valueint;

    cJSON* home = cJSON_GetObjectItem(pViewPoint, "home");
    if (home)
        pot._isDefaultVP = home->type == cJSON_True ? true : false;

    Interface::setViewPoint(pot, true);
}


int CDataManager::GetModelCount()
{
    std::shared_lock lock(m_mutexTree2);
    if (m_pTreeItem) {
        //+1 是因为添加了默认地图 AddProiMageMap()
        int nSum = m_pTreeItem->getModeCount() + m_pTreeItem->getLayConut() + 1;
        return nSum;
    }
    return 0;
}

//响应添加模型
int CDataManager::notifyAddModel(std::string strId, bool bSucess, bool bNeedAdd)
{
    std::unique_lock lock(m_mutexTree2);
    if (m_pTreeItem)
    {
        return m_pTreeItem->notifyAddModel(strId, bSucess, bNeedAdd);
    }
    return -1;
}

/*******************************************************************************
* 函数名称：
* 功能描述：响应删除模型
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

int CDataManager::notifyDelModel(std::string strId)
{
    std::unique_lock lock(m_mutexTree2);
    if (m_pTreeItem)
    {
        return m_pTreeItem->notifyDelModel(strId);
    }
    return -1;
}

int CDataManager::notifyDelModel(std::vector<std::string> strIds)
{
    std::unique_lock lock(m_mutexTree2);
    if (m_pTreeItem)
    {
        for (size_t i = 0; i < strIds.size(); i++)
        {
            m_pTreeItem->notifyDelModel(strIds[i]);
        }
        return 0;
    }
    return -1;
}

int CDataManager::notifyUpdateModel(std::vector<std::string> strIds)
{
    std::unique_lock lock(m_mutexTree2);
    if (m_pTreeItem)
    {
        for (size_t i = 0; i < strIds.size(); i++)
        {
            m_pTreeItem->notifyUpdateMode(strIds[i]); //update
        }
        return 0;
    }
    return -1;
}

int CDataManager::setMainViewPointByInterface(std::string& strRespone)
{
    int nRet = 0;

    SetMainViewPoint();
    strRespone = "{}";

    return nRet;
}

//保存当前rte场景资源
int CDataManager::getCurResourceFormatBuild(std::string& rSid, std::string& strOut, std::string& strMessage)
{
    //组装大json数据上传到pms
    if (!m_pmsData) {
        strMessage = "pms data is null";
        return -1;
    }

    cJSON* pSendJson = cJSON_CreateObject();

    cJSON* pData = cJSON_GetObjectItem(m_pmsData, "data");
    if (!pData)
        return -1;

    std::string sid;
    cJSON_LoadFromObject(pData, "sid", sid);
    rSid = sid;

    //首先从3d中同步第一人称视角数据
    saveFPM_Data();
    //cJSON_AddStringToObject(pSendJson, "sid", sid.c_str());
    cJSON* pFirstContent = GetGlobalSettingInstance().toJson();

    //fist content 填充second content
    cJSON* pSecondContent = cJSON_CreateObject();
    cJSON_AddItemToObject(pSecondContent, "sceneList", m_pScenceList->toJson());
    {
        std::unique_lock lock(m_mutexTree2);
        cJSON_AddItemToObject(pSecondContent, "treeItem", m_pTreeItem->toJson());
    }

    cJSON_AddItemToObject(pFirstContent, "content", pSecondContent);
    cJSON_AddItemToObject(pSendJson, "content", pFirstContent);

    char* pOut = cJSON_Print(pSendJson);
    strOut.append(pOut, strlen(pOut));
    free(pOut);

    cJSON_Delete(pSendJson);
    return 0;
}

void CDataManager::hideFirstPersonManipultor()
{
    std::string jsonData = R"(
        {
	        "event": "FirstPersonManipultor",
	        "method": "control",
	        "operate": {
	        	"_displayLabel": false
	        }
        }
    )";

    std::string strResult;
    int nRet = Interface::eventHandle(jsonData, strResult);
    if (nRet)
    {
        Log(Tool::Error, "sceneAmibentWidget config error %d\n %s", nRet, strResult.c_str());
    }
    Log(Tool::Info,"sceneAmibentWidget config %s", jsonData.c_str());
}

void CDataManager::saveFPM_Data()
{
    std::string strData;
    Interface::saveFPM_Data(strData);
    if (!strData.empty()) {
        std::vector<std::pair<std::string, std::string>> vpList;
        Interface::getFPM_ViewPointList(vpList);
        GetGlobalSettingInstance().resetFirstPersonManipultor(strData);
        //Poco::Logger::get("RteLogger").debug("reset FPM data sucess %s", strData);
    }
}

void CDataManager::LoadFPM_Data()
{
    //加载第一人称视点数据
    std::string strFirstmanViewData = GetGlobalSettingInstance().getFirstPersonManipultor();

    //加载第一人称视角漫游器
    Interface::loadFPM_Data(strFirstmanViewData);

    //隐藏第一人称漫游器图标
    hideFirstPersonManipultor();

    //设置主视点
    SetMainViewPoint();
}

//工程配置参数设置
void CDataManager::ProjectConfig()
{
    cJSON* pJson = cJSON_CreateObject();
    cJSON_AddStringToObject(pJson, "event", "ProjectConfig");
    cJSON_AddStringToObject(pJson, "method", "control");

    cJSON* pJsonOperate = cJSON_CreateObject();

    std::string strPmsIp;
    unsigned int nPort;
    GetConfigSettingInstance().GetResouceInfo(strPmsIp, nPort);
    char buf[512] = { 0 };
    sprintf(buf, "http://%s:%d", strPmsIp.c_str(), nPort);
    cJSON_AddStringToObject(pJsonOperate, "serverIPPort", buf);
    cJSON_AddItemToObject(pJson, "operate", pJsonOperate);

    std::string request;
    std::string strResult;
    if (pJson)
    {
        char* pOut = cJSON_Print(pJson);
        if (pOut)
        {
            request = pOut;
            free(pOut);
        }
        cJSON_free(pJson);
    }
    int nRet = Interface::eventHandle(request, strResult);
    if (nRet)
    {
        Log(Tool::Debug, "ProjectConfig error:%d %s", nRet, strResult.c_str());
    }
}

//更新特效模板,//build sdk 设置effect模板
void CDataManager::updateEffectTemplate()
{
    std::unordered_set<std::string> ids = GetGlobalSettingInstance().getResourceIDList(SDT_SceneEffects);
    for (auto itor = ids.begin(); itor != ids.end(); itor++)
    {
        cJSON* pTemplate = GetGlobalSettingInstance().getByResourceID(SDT_SceneEffects, itor->c_str());

        if (pTemplate)
        {
            cJSON* pElementStyle = cJSON_DetachItemFromObject(pTemplate, "elementStyle");

            if (pElementStyle) {
                cJSON* p3dRequst = cJSON_CreateObject();
                cJSON_AddStringToObject(p3dRequst, "id", itor->c_str());
                cJSON_AddStringToObject(p3dRequst, "element", "Effect");
                cJSON_AddStringToObject(p3dRequst, "method", "update");

                cJSON_AddItemToObject(p3dRequst, "data", pElementStyle);

                char* pOut = cJSON_Print(p3dRequst);
                if (pOut) {
                    std::string out;
                    Interface::elementHandle(pOut, out);

                    free(pOut);
                }
            }

            cJSON_Delete(pTemplate);
        }
    }
}

//加载模型,初始化一些配置,需要3d场景初始化接口调用完成后调用。
int CDataManager::loadModelsTo3DEngine()
{
    if (!m_bLoadComplite) 
    {
        ProjectConfig();
        updateEffectTemplate();

        //设置成多线程渲染模式
        if (Interface::getMainViewer())
        {
            osgViewer::ViewerBase* viewbase = static_cast<osgViewer::ViewerBase*> (Interface::getMainViewer());
            if (viewbase)
                viewbase->setThreadingModel(osgViewer::ViewerBase::ThreadingModel::CullDrawThreadPerContext);
        }

        std::thread t1([&]() {
            //std::lock_guard<std::mutex> lock(m_mutexTree); //这里不上锁是由于初始加载时不会修改删除添加tree操作，所以是安全的，同时避免notify-addmode等待此锁，造成卡顿。
            if (m_pTreeItem) {
                //设置主视点
                SetMainViewPoint();

                //设置实时光影开关
                bool realTimeLighting = GetGlobalSettingInstance().getRealTimeLighting();
                std::string strRespone;
                switchRealTimeLighting(realTimeLighting, strRespone);

                //设置物体全局可视距离
                int nRange = GetGlobalSettingInstance().getGlobalLoadScale();
                SetGlobalLoadScale(nRange);

                int nRet = m_pTreeItem->loadElementDataTo3D();
                if (nRet == 1)
                {
                    //空场景处理逻辑
                    LoadFPM_Data();
                    LoadGraphicsConfig();
                    GetGlobalSettingInstance().setGlobalSetting();//设置POI聚合,必须在加载完元素之后设置有效。
                    m_bLoadComplite.store(true);
                    if (m_callback)
                        m_callback("LOADPROJECT_COMPLITE", "true", m_pUser);
                    Log(Tool::Debug, "loadElementDataTo3D null");
                }

                AddProiMageMap();
            }
            });
        t1.detach();
        return 0;
    }
    else
        return -1;
}

void CDataManager::LoadGraphicsConfig()
{
    //获取当前使用的环境光配置
    std::string strGraphicsConfigData;

    //GetGlobalSettingInstance().getVerseGraphicsConfig("isCurrent", strGraphicsConfigData);
    GetGlobalSettingInstance().getCurrentVerse(strGraphicsConfigData);

    cJSON* pJson = cJSON_CreateObject();
    cJSON_AddStringToObject(pJson, "event", "GraphicsConfig");
    cJSON_AddStringToObject(pJson, "method", "add");

    cJSON* pJsonData = cJSON_Parse(strGraphicsConfigData.c_str());
    cJSON_AddItemToObject(pJson, "data", pJsonData);

    std::string request;
    std::string strResult;

    if (pJson)
    {
        char* pOut = cJSON_Print(pJson);
        if (pOut)
        {
            request = pOut;
            free(pOut);
        }
        cJSON_free(pJson);
    }
    int nRet = Interface::eventHandle(request, strResult);
    if (nRet)
    {
        Log(Tool::Error, "LoadGraphicsConfig error %d\n %s", nRet, strResult.c_str());
    }
    Log(Tool::Info, "LoadGraphicsConfig info %s", request.c_str());
}

int CDataManager::LoadGraphicsConfig(std::string config, std::string& strRespone)
{
    //获取图形配置参数数据
    std::string strGraphicsConfigData = config;

    cJSON* pJson = cJSON_CreateObject();
    cJSON_AddStringToObject(pJson, "event", "GraphicsConfig");
    cJSON_AddStringToObject(pJson, "method", "add");

    cJSON* pJsonData = cJSON_Parse(strGraphicsConfigData.c_str());
    cJSON_AddItemToObject(pJson, "data", pJsonData);

    std::string request;
    std::string strResult;

    if (pJson)
    {
        char* pOut = cJSON_Print(pJson);
        if (pOut)
        {
            request = pOut;
            free(pOut);
        }
        cJSON_free(pJson);
    }
    int nRet = Interface::eventHandle(request, strResult);
    if (nRet)
    {
        Log(Tool::Error, "LoadGraphicsConfig error %d\n %s", nRet, strResult.c_str());
    }
    Log(Tool::Info, "LoadGraphicsConfig info %s", request.c_str());

    strRespone = strResult;
    return nRet;
}


int CDataManager::switchRealTimeLighting(bool bSet, std::string& strRespone)
{
    //获取图形配置参数数据
    //std::string strGraphicsConfigData = config;

    cJSON* pJson = cJSON_CreateObject();
    cJSON_AddStringToObject(pJson, "event", "GraphicsConfig");
    cJSON_AddStringToObject(pJson, "method", "update");

    cJSON* pData = cJSON_CreateObject();
    cJSON_AddBoolToObject(pData, "realTimeLighting", bSet);

    cJSON_AddItemToObject(pJson, "data", pData);

    std::string request;
    std::string strResult;

    if (pJson)
    {
        char* pOut = cJSON_Print(pJson);
        if (pOut)
        {
            request = pOut;
            free(pOut);
        }
        cJSON_free(pJson);
    }
    int nRet = Interface::eventHandle(request, strResult);
    if (nRet)
    {
        Log(Tool::Debug, "realTimeLighting config error %d\n %s", nRet, strResult.c_str());
    }
    strRespone = strResult;
    return nRet;
}

/*
* {
    "method": "control",
    "operate": {
        "GlobalLodScale": 25.0
    }
}

POI和text不支持
*/
int CDataManager::SetGlobalLoadScale(int nValue)
{
    cJSON* pRoot = cJSON_CreateObject();
    cJSON_AddStringToObject(pRoot, "method", "control");

    cJSON* pItem = cJSON_CreateObject();
    cJSON_AddNumberToObject(pItem, "GlobalLodScale", nValue);

    cJSON_AddItemToObject(pRoot, "operate", pItem);

    char* pOut = cJSON_Print(pRoot);

    std::string strResult;
    std::string str3dRequest;
    if (pOut)
    {
        str3dRequest = pOut;
        free(pOut);
    }

    int nRet = Interface::elementHandle(str3dRequest, strResult);
    cJSON_free(pRoot);

    return nRet;
}

int CDataManager::AddProiMageMap()
{
    //step 1 获取默认地图数据
    std::string strRequese = R"(
        {
	        "event": "Linkage2D3D",
            "method":"get",
	        "builtinLayer": {}
        }
    )";

    std::string strRespone;
    int nRet = Interface::eventHandle(strRequese, strRespone);
    if (nRet)
    {
        Log(Tool::Error,"get image map error");
        return -1;
    }

    cJSON* pRet = cJSON_Parse(strRespone.c_str());
    if (pRet) {
        cJSON* pData = cJSON_GetObjectItem(pRet, "data");

        if (pData)
        {
            //step 2 添加默认地图
            cJSON* pRoot = cJSON_CreateObject();

            cJSON_AddStringToObject(pRoot, "element", "Layer");
            cJSON_AddStringToObject(pRoot, "method", "add");
            cJSON_AddItemToObject(pRoot, "data", cJSON_Duplicate(pData, true));

            char* pOut = cJSON_Print(pRoot);
            if (pOut) {
                std::string strRet;
                int code = Interface::elementHandle(pOut, strRet);

                free(pOut);
                cJSON_Delete(pRoot);
            }

            cJSON_Delete(pRet);
            return 0;
        }
    }
    else
        return -1;

    return 0;
}

int CDataManager::switchCallback(std::string request, std::string& strRespone)
{
    cJSON* pRoot = cJSON_Parse(request.c_str());
    if (pRoot) {
        for (auto itor = pRoot->child; itor != NULL; )
        {
            if (itor->string) {
                auto itorFind = m_mapCallbackSubscribe.find(itor->string);
                if (itorFind != m_mapCallbackSubscribe.end())
                    itorFind->second = itor->type == cJSON_True ? true : false;
            }
            itor = itor->next;
        }
        strRespone = R"(
        {
         })";
        return 0;
    }
    return -1;
}

void CDataManager::open2DWnd(std::string& strRespone)
{
    /*if (!m_pViewerSlave) {
        m_pViewerSlave = dynamic_cast<osgViewer::Viewer*> (Interface::createViewerRTE2(m_hWnd, 0, 0, m_nWndWidth / 2, m_nWndHeight / 2, 16, false).get());
        m_pViewerSlave->setName("slaveView");

        Interface::initSyzEarth2D(m_pViewerSlave, true);
    }
    else {
        cJSON* pRoot = cJSON_CreateObject();
        cJSON_AddStringToObject(pRoot, "event", "Linkage2D3D");
        cJSON_AddStringToObject(pRoot, "method", "control");

        cJSON* p2DView = cJSON_CreateObject();
        cJSON_AddNumberToObject(p2DView, "x", 0);
        cJSON_AddNumberToObject(p2DView, "y", 0);
        cJSON_AddNumberToObject(p2DView, "width", m_nWndWidth / 2);
        cJSON_AddNumberToObject(p2DView, "height", m_nWndHeight / 2);

        cJSON* pOperate = cJSON_CreateObject();
        cJSON_AddItemToObject(pOperate, "2DView", p2DView);
        cJSON_AddItemToObject(pRoot, "operate", pOperate);

        std::string request;
        char* pOut = cJSON_Print(pRoot);
        if (pOut) {
            request = pOut;
            free(pOut);
        }
        int nRet = Interface::eventHandle(request, strRespone);
        cJSON_Delete(pRoot);
    }

    strRespone = R"(
                        {
		                    "message":"成功"
	                    }
                    )";*/
}

void CDataManager::close2DWnd(std::string& strRespone)
{
    /*if (m_pViewerSlave)
    {
        std::string request;
        cJSON* pRoot = cJSON_CreateObject();
        cJSON_AddStringToObject(pRoot, "event", "Linkage2D3D");
        cJSON_AddStringToObject(pRoot, "method", "control");

        cJSON* p2DView = cJSON_CreateObject();
        cJSON_AddNumberToObject(p2DView, "x", 0);
        cJSON_AddNumberToObject(p2DView, "y", 0);
        cJSON_AddNumberToObject(p2DView, "width", 1);
        cJSON_AddNumberToObject(p2DView, "height", 1);

        cJSON* pOperate = cJSON_CreateObject();
        cJSON_AddItemToObject(pOperate, "2DView", p2DView);
        cJSON_AddItemToObject(pRoot, "operate", pOperate);

        char* pOut = cJSON_Print(pRoot);
        if (pOut) {
            request = pOut;
            free(pOut);
        }
        int nRet = Interface::eventHandle(request, strRespone);
        cJSON_Delete(pRoot);
    }
    else {
        strRespone = R"(
                        {
		                "message":"窗口未打开，操作失败"
	                    }
                    )";
    }*/
}