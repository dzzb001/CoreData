#include "CTreeItem.h"

#include <SceneCore/interface/Interface.h>
#include <osgViewer/Viewer>
#include "SceneDataType.h"
#include "CGlobalSetting.h"
#include "SceneDataType.h"
#include "config.h"

#include "Tool/TLog.h"

using namespace RTE;

#define Log LogN(1103)

CTreeItem::CTreeItem(cJSON* p) : CJson_base(p),m_pElementNode(nullptr)
, m_nModeCount(0)
, m_nLayerCount(0)
{
    m_pElementNode = parseJsonToTree(m_pJson, nullptr);
}

CTreeItem::CTreeItem(cJSON* p, std::string strSenceId) : CJson_base(p), m_pElementNode(nullptr),m_strCurSenceId(strSenceId)
, m_nModeCount(0)
, m_nLayerCount(0)
{
    m_pElementNode = parseJsonToTree(m_pJson, nullptr);
    ////Poco::Logger::get("RteLogger").debug("node parse complite");
}


CTreeItem::~CTreeItem()
{
}

//json数据转换成tree
std::shared_ptr<Node> CTreeItem::parseJsonToTree(cJSON* jsonData, std::shared_ptr<Node> parent) {
    if (!jsonData)
        return nullptr;
        
    std::shared_ptr<Node> node = std::make_shared<Node>();
    node->parent = parent;

    cJSON* guid = cJSON_GetObjectItem(jsonData, "guid");
    if (!guid || cJSON_IsNull(guid)) {
        node->guid = nullptr;
    }
    else {
        node->guid = std::shared_ptr<char>(strdup(guid->valuestring), free);
        {
            node->pJson = jsonData;
            //std::lock_guard<std::mutex> lock(m_mutexNode);
            m_mapNode[std::string(node->guid.get())] = node;
        }
    }

    cJSON* resourceId = cJSON_GetObjectItem(jsonData, "resourceId");
    if (!resourceId || cJSON_IsNull(resourceId)) {
        node->resourceId = nullptr;
    }
    else {
        node->resourceId = std::shared_ptr<char>(strdup(resourceId->valuestring), free);
    }

    cJSON* isLockArray = cJSON_GetObjectItem(jsonData, "isLockArray");
    if (!isLockArray || cJSON_IsNull(isLockArray)) {
        node->isLockArray = nullptr;
    }
    else {
        //将isLockArray 拷贝到树中，这样也不损坏原始json结构
        node->isLockArray = std::shared_ptr<cJSON>(cJSON_Duplicate(isLockArray,1), cJSON_Delete);
    }

    cJSON* data = cJSON_GetObjectItem(jsonData, "data");
    if (!data || cJSON_IsNull(data)) {
        node->data = nullptr;
    }
    else {
        node->data = std::shared_ptr<cJSON>(cJSON_Duplicate(data, 1), cJSON_Delete);
    }

    cJSON* elementData = cJSON_GetObjectItem(jsonData, "elementData");
    if (!elementData || cJSON_IsNull(elementData)) {
        node->elementData = nullptr;
    }
    else {
        node->elementData = std::shared_ptr<cJSON>(cJSON_Duplicate(elementData, 1), cJSON_Delete);
        bool isLayer = false;
        isLayer = cJSON_HasObjectItem(elementData, "_layerType");
        if (isLayer)
        {
            node->order2 = m_order++;
        }
    }

    cJSON* strType = cJSON_GetObjectItem(jsonData, "strType");
    if (!strType || cJSON_IsNull(strType)) {
        node->strType = nullptr;
    }
    else {
        char* pStr = strdup(strType->valuestring);
        if(pStr)
            node->strType = std::shared_ptr<char>(pStr, free);
    }

    cJSON* icon = cJSON_GetObjectItem(jsonData, "icon");
    if (!icon || cJSON_IsNull(icon)) {
        node->icon = nullptr;
    }
    else {
        char* pStr = strdup(icon->valuestring);
        if(pStr)
            node->icon = std::shared_ptr<char>(pStr, free);
    }

    cJSON* opacityStyleColor = cJSON_GetObjectItem(jsonData, "opacityStyleColor");
    if (!opacityStyleColor || cJSON_IsNull(opacityStyleColor)) {
        node->opacityStyleColor = nullptr;
    }
    else {
        node->opacityStyleColor = std::shared_ptr<char>(strdup(opacityStyleColor->valuestring), free);
    }

    cJSON* buildID = cJSON_GetObjectItem(jsonData, "buildID");
    if (!buildID || cJSON_IsNull(buildID)) {
        node->buildID = nullptr;
    }
    else {
        node->buildID = std::shared_ptr<char>(strdup(buildID->valuestring), free);
        node->index["buildID"] = node->buildID;
    }

    cJSON* type = cJSON_GetObjectItem(jsonData, "type");
    if (!type || cJSON_IsNull(type)) {
        node->type = nullptr;
    }
    else {
        node->type = std::make_unique<int>(type->valueint);
    }

    cJSON* secondType = cJSON_GetObjectItem(jsonData, "secondType");
    if (!secondType || cJSON_IsNull(secondType)) {
        node->secondType = nullptr;
    }
    else {
        node->secondType = std::make_unique<int>(secondType->valueint);
    }

    cJSON* opacityStyleShowBuildOpacity = cJSON_GetObjectItem(jsonData, "opacityStyleShowBuildOpacity");
    if (!opacityStyleShowBuildOpacity || cJSON_IsNull(opacityStyleShowBuildOpacity)) {
        node->opacityStyleShowBuildOpacity = nullptr;
    }
    else {
        node->opacityStyleShowBuildOpacity = std::make_unique<bool>(opacityStyleShowBuildOpacity->type == cJSON_True ? true : false);
    }

    cJSON* sceneId = cJSON_GetObjectItem(jsonData, "sceneId");
    if (!sceneId || cJSON_IsNull(sceneId)) {
        node->sceneId = nullptr;
    }
    else {
        node->sceneId = std::shared_ptr<char>(strdup(sceneId->valuestring), free);
    }

    cJSON* isDrag = cJSON_GetObjectItem(jsonData, "isDrag");
    if (!isDrag || cJSON_IsNull(isDrag)) {
        node->isDrag = nullptr;
    }
    else {
        node->isDrag = std::make_unique<bool>(isDrag->type == cJSON_True ? true : false);
    }

    cJSON* bindGuid = cJSON_GetObjectItem(jsonData, "bindGuid");
    if (!bindGuid || cJSON_IsNull(bindGuid)) {
        node->bindGuid = nullptr;
    }
    else {
        node->bindGuid = std::shared_ptr<char>(strdup(bindGuid->valuestring), free);
    }

    cJSON* checkedArray = cJSON_GetObjectItem(jsonData, "checkedArray");
    if (!checkedArray || cJSON_IsNull(checkedArray)) {
        node->checkedArray = nullptr;
    }
    else {
        node->checkedArray = std::shared_ptr<cJSON>(cJSON_Duplicate(checkedArray, 1), cJSON_Delete);
    }

    cJSON* order = cJSON_GetObjectItem(jsonData, "order");
    if (!order || cJSON_IsNull(order)) {
        node->order = nullptr;
    }
    else {
        node->order = std::make_unique<int>(order->valueint);
    }

    cJSON* opacityStyleOpen = cJSON_GetObjectItem(jsonData, "opacityStyleOpen");
    if (!opacityStyleOpen || cJSON_IsNull(opacityStyleOpen)) {
        node->opacityStyleOpen = nullptr;
    }
    else {
        node->opacityStyleOpen = std::make_unique<bool>(opacityStyleOpen->type == cJSON_True ? true : false);
    }

    cJSON* version = cJSON_GetObjectItem(jsonData, "version");
    if (!version || cJSON_IsNull(version)) {
        node->version = nullptr;
    }
    else {
        node->version = std::make_unique<double>(type->valuedouble);
    }

    cJSON* url = cJSON_GetObjectItem(jsonData, "url");
    if (!url || cJSON_IsNull(url)) {
        node->url = nullptr;
    }
    else {
        node->url = std::shared_ptr<char>(strdup(url->valuestring), free);
    }

    cJSON* tagList = cJSON_GetObjectItem(jsonData, "tagList");
    if (!tagList || cJSON_IsNull(tagList)) {
        node->tagList = nullptr;
    }
    else {
        node->tagList = std::shared_ptr<cJSON>(cJSON_Duplicate(tagList, 1), cJSON_Delete);

        int nSize = cJSON_GetArraySize(node->tagList.get());

        if (nSize>0)
        {
            //tagmannager 添加tag
            for (size_t i = 0; i < nSize; i++)
            {
                cJSON* pItem = cJSON_GetArrayItem(node->tagList.get(), i);
                if (pItem->child && pItem->child->string)
                {
                    m_tagManager.addTag(pItem->child->string, node->guid.get(), node->type.get()==nullptr ? -1 : *(node->type));
                }
            }
        }
        else {
            m_tagManager.addTag("[]", node->guid.get(), node->type.get() == nullptr ? -1 : *(node->type)); //如果array 的size为0 标记为特殊字符 "[]"
        }
    }

    cJSON* isExpand = cJSON_GetObjectItem(jsonData, "isExpand");
    if (!isExpand || cJSON_IsNull(isExpand)) {
        node->isExpand = nullptr;
    }
    else {
        node->isExpand = std::make_unique<bool>(isExpand->type == cJSON_True ? true : false);
    }

    cJSON* visibleArray = cJSON_GetObjectItem(jsonData, "visibleArray");
    if (!visibleArray || cJSON_IsNull(visibleArray)) {
        node->visibleArray = nullptr;
    }
    else {
        node->visibleArray = std::shared_ptr<cJSON>(cJSON_Duplicate(visibleArray, 1), cJSON_Delete);
    }

    cJSON* showSetBuild = cJSON_GetObjectItem(jsonData, "showSetBuild");
    if (!showSetBuild || cJSON_IsNull(showSetBuild)) {
        node->showSetBuild = nullptr;
    }
    else {
        node->showSetBuild = std::make_unique<bool>(showSetBuild->type == cJSON_True ? true : false);
    }

    cJSON* name = cJSON_GetObjectItem(jsonData, "name");
    if (!name || cJSON_IsNull(name)) {
        node->name = nullptr;
    }
    else {
        node->name = std::shared_ptr<char>(strdup(name->valuestring), free);
        node->index["name"] = node->name;
    }

    cJSON* opacityStyleVisible = cJSON_GetObjectItem(jsonData, "opacityStyleVisible");
    if (!opacityStyleVisible || cJSON_IsNull(opacityStyleVisible)) {
        node->opacityStyleVisible = nullptr;
    }
    else {
        node->opacityStyleVisible = std::make_unique<bool>(opacityStyleVisible->type == cJSON_True ? true : false);
    }

    cJSON* foreignKey = cJSON_GetObjectItem(jsonData, "foreignKey");
    if (!foreignKey || cJSON_IsNull(foreignKey)) {// 真没有这个字段不维护
        node->foreignKey = nullptr;
    }
    else {
        node->foreignKey = std::shared_ptr<cJSON>(cJSON_Duplicate(foreignKey, 1), cJSON_Delete); 
        if (cJSON_IsArray(node->foreignKey.get())) {
            int nSize = cJSON_GetArraySize(node->foreignKey.get());
            if (nSize > 0)
            {
                cJSON* pItem = cJSON_GetArrayItem(node->foreignKey.get(), 0);
                if (pItem && pItem->valuestring)
                {
                    m_mapForeignKeyNode[pItem->valuestring] = node; // 创建外键表，外键唯一，只取第一个
                    node->strForeignKey = pItem->valuestring;
                }
            }
            else {
                m_mapForeignKeyNullNode[node->guid.get()] = node;
            }
        }
    }

    //业务属性
    cJSON* attributeList = cJSON_GetObjectItem(jsonData, "attributeList");
    if (!attributeList || cJSON_IsNull(attributeList)) {
        node->attributeList = nullptr;
    }
    else {
        Attributes attr;
        node->attributeList = std::shared_ptr<cJSON>(cJSON_Duplicate(attributeList, 1), cJSON_Delete);

        if (cJSON_IsArray(node->attributeList.get()))
        {
            int nSize = cJSON_GetArraySize(node->attributeList.get());
            if (nSize > 0) {
                for (size_t i = 0; i < nSize; i++)
                {
                    cJSON* pItem = cJSON_GetArrayItem(node->attributeList.get(), i);
                    cJSON* pAttri = pItem->child;
                    if (pAttri != nullptr)
                    {
                        switch (pAttri->type)
                        {
                        case cJSON_False:
                            attr.setBoolValue(false);
                            break;
                        case cJSON_True:
                            attr.setBoolValue(true);
                            break;
                        case cJSON_Number:
                            attr.setDoubleValue(pAttri->valuedouble);
                            break;
                        case cJSON_String:
                            attr.setStringValue(pAttri->valuestring);
                            break;
                        default:
                            break;
                        }
                        if (pAttri->string)
                            m_AttriManager.addBusAttr(pAttri->string, attr, node->guid.get());
                    }

                }
            }
            else {
                attr.setStringValue("[]");
                m_AttriManager.addBusAttr("[]", attr, node->guid.get());
            }
        }

#if 0
        cJSON* pAttri = node->attributeList->child;
        for (; pAttri!=nullptr; )
        {
            switch (pAttri->type)
            {
            case cJSON_False:
                attr.setBoolValue(false);
                break;
            case cJSON_True:
                attr.setBoolValue(true);
                break;
            case cJSON_Number:
                attr.setDoubleValue(pAttri->valuedouble);
                break;
            case cJSON_String:
                attr.setStringValue(pAttri->valuestring);
                break;
            default:
                break;
            }
            if(pAttri->string)
                m_AttriManager.addBusAttr(pAttri->string, attr, node->guid.get());
            pAttri = pAttri->next;
        }
#endif
    }

    cJSON* viewPointList = cJSON_GetObjectItem(jsonData, "viewPointList");
    if (!viewPointList || cJSON_IsNull(viewPointList)) {
        node->viewPointList = nullptr;
    }
    else {
        node->viewPointList = std::shared_ptr<cJSON>(cJSON_Duplicate(viewPointList, 1), cJSON_Delete);
    }

    cJSON* projectId = cJSON_GetObjectItem(jsonData, "projectId");
    if (!projectId || cJSON_IsNull(projectId)) {
        node->projectId = nullptr;
    }
    else {
        node->projectId = std::shared_ptr<char>(strdup(projectId->valuestring), free);
    }

    cJSON* floorIndex = cJSON_GetObjectItem(jsonData, "floorIndex");
    if (!floorIndex || cJSON_IsNull(floorIndex)) {
        node->floorIndex = nullptr;
    }
    else {
        node->floorIndex = std::make_unique<int>(floorIndex->valueint);
    }

    cJSON* opacityStyleOpacity = cJSON_GetObjectItem(jsonData, "opacityStyleOpacity");
    if (!opacityStyleOpacity || cJSON_IsNull(opacityStyleOpacity)) {
        node->opacityStyleOpacity = nullptr;
    }
    else {
        node->opacityStyleOpacity = std::make_unique<int>(opacityStyleOpacity->valueint);
    }

    //递归查找子节点
    cJSON* childrenData = cJSON_GetObjectItem(jsonData, "childArray");
    int numChildren = cJSON_GetArraySize(childrenData);
    if (numChildren > 0) {
        for (int i = 0; i < numChildren; i++) {
            cJSON* childData = cJSON_GetArrayItem(childrenData, i);
            std::shared_ptr<Node> childNode = parseJsonToTree(childData, node);
            if(childNode)
                node->childArray.push_back(childNode);
        }
    }
    
    if (node->type  && *node->type.get() == 14 && node->guid)
    {
        m_strGroupId = node->guid.get();
    }

    //在此处统计模型和图层数量更准确
    if (node->checkedArray && isCheck(node->checkedArray.get(), m_strCurSenceId))
    {
        if (node->elementData)
        {
            if (node->elementData.get()) {
                cJSON* pElementDataJson = node->elementData.get();
                bool isLayer = false;
                isLayer = cJSON_HasObjectItem(pElementDataJson, "_layerType");

                if (isLayer)
                {
                    m_nLayerCount++;
                }
                else {
                    bool isModel = false;
                    isModel = cJSON_HasObjectItem(pElementDataJson, "_modelType");
                    if(isModel)
                        m_nModeCount++;
                    else {
                        node->elementData = nullptr;
                    }
                }
            }
        }
        else {
            ;// //Poco::Logger::get("RteLogger").debug("%s  elementData is null", std::string(node->guid.get()));
        }
    }

    return node;
}

//将tree转换成json
/*
* recurse         是否递归返回子节点数据
* bShowData       是否返回ui和3d数据
* bCheckShow true 将未勾选的数据返回，false 将未勾选的数据过滤
*/
cJSON* CTreeItem::parseTreeToJson(std::shared_ptr<Node> node, bool recurse, bool bShowData, bool bUnCheckShow) 
{
    bool bIsCheck = true;
    if (!bUnCheckShow) { //判断是否勾选,默认按都勾选处理
        bIsCheck = isCheck(node->checkedArray.get(), m_strCurSenceId);
    }

    if (!bIsCheck) {
        /*char buf[64] = { 0 };
        sprintf(buf, "%s is not checkarray\n", node->guid.get());
        OutputDebugStringA(buf);*/
        return nullptr;
    }

    cJSON* jsonData = cJSON_CreateObject();

    if (node->resourceId == nullptr) {
        ;// cJSON_AddNullToObject(jsonData, "resourceId");
    }
    else {
        cJSON_AddStringToObject(jsonData, "resourceId", node->resourceId.get());
    }

    if (node->isLockArray == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "isLockArray");
    }
    else {
        //复制json, 1为递归复制
        cJSON_AddItemToObject(jsonData, "isLockArray", cJSON_Duplicate(node->isLockArray.get(),1));
        //char *pOut = cJSON_Print(jsonData);
        ////Poco::Logger::get("RteLogger").information("%s", std::string(pOut));
    }

    if (node->data == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "data");
    }
    else {
        if(bShowData)
            cJSON_AddItemToObject(jsonData, "data", cJSON_Duplicate(node->data.get(), 1));
    }

    if (node->elementData == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "elementData");
    }
    else {
        cJSON_AddItemToObject(jsonData, "elementData", cJSON_Duplicate(node->elementData.get(), 1));
    }

    if (node->strType == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "strType");
    }
    else {
        cJSON_AddStringToObject(jsonData, "strType", node->strType.get());
    }

    if (node->icon == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "icon");
    }
    else {
        cJSON_AddStringToObject(jsonData, "icon", node->icon.get());
    }

    if (node->opacityStyleColor == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "opacityStyleColor");
    }
    else {
        cJSON_AddStringToObject(jsonData, "opacityStyleColor", node->opacityStyleColor.get());
    }

    if (node->buildID == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "buildID");
    }
    else {
        cJSON_AddStringToObject(jsonData, "buildID", node->buildID.get());
    }

    if (node->type == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "type");
    }
    else {
        cJSON_AddNumberToObject(jsonData, "type", *(node->type));
    }

    if (node->secondType == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "secondType");
    }
    else {
        ;//cJSON_AddNumberToObject(jsonData, "secondType", *(node->secondType));
    }

    if (node->opacityStyleShowBuildOpacity == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "opacityStyleShowBuildOpacity");
    }
    else {
        *(node->opacityStyleShowBuildOpacity) ? cJSON_AddTrueToObject(jsonData, "opacityStyleShowBuildOpacity") : cJSON_AddFalseToObject(jsonData, "opacityStyleShowBuildOpacity");
    }

    if (node->sceneId == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "sceneId");
    }
    else {
        cJSON_AddStringToObject(jsonData, "sceneId", node->sceneId.get());
    }

    if (node->isDrag == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "isDrag");
    }
    else {
        *(node->isDrag) ? cJSON_AddTrueToObject(jsonData, "isDrag") : cJSON_AddFalseToObject(jsonData, "isDrag");
    }

    if (node->bindGuid == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "bindGuid");
    }
    else {
        cJSON_AddStringToObject(jsonData, "bindGuid", node->bindGuid.get());
    }

    if (node->checkedArray == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "checkedArray");
    }
    else {
        cJSON_AddItemToObject(jsonData, "checkedArray", cJSON_Duplicate(node->checkedArray.get(), 1));
    }

    if (node->order == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "order");
    }
    else {
        cJSON_AddNumberToObject(jsonData, "order", *(node->order));
    }

    if (node->opacityStyleOpen == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "opacityStyleOpen");
    }
    else {
        *(node->opacityStyleOpen) ? cJSON_AddTrueToObject(jsonData, "opacityStyleOpen") : cJSON_AddFalseToObject(jsonData, "opacityStyleOpen");
    }

    if (node->version == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "version");
    }
    else {
        cJSON_AddNumberToObject(jsonData, "version", *(node->version));
    }

    if (node->url == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "url");
    }
    else {
        cJSON_AddStringToObject(jsonData, "url", node->url.get());
    }

    if (node->tagList == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "tagList");
    }
    else {
        cJSON_AddItemToObject(jsonData, "tagList", cJSON_Duplicate(node->tagList.get(), 1));

    }

    if (node->isExpand == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "isExpand");
    }
    else {
        *(node->isExpand) ? cJSON_AddTrueToObject(jsonData, "isExpand") : cJSON_AddFalseToObject(jsonData, "isExpand");
    }

    if (node->visibleArray == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "visibleArray");
    }
    else {
        cJSON_AddItemToObject(jsonData, "visibleArray", cJSON_Duplicate(node->visibleArray.get(), 1));
    }

    if (node->showSetBuild == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "showSetBuild");
    }
    else {
        *(node->showSetBuild) ? cJSON_AddTrueToObject(jsonData, "showSetBuild") : cJSON_AddFalseToObject(jsonData, "showSetBuild");
    }

    if (node->name == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "name");
    }
    else {
        cJSON_AddStringToObject(jsonData, "name", node->name.get());
    }

    if (node->guid == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "guid");
    }
    else {
        cJSON_AddStringToObject(jsonData, "guid", node->guid.get());
    }

    if (node->opacityStyleVisible == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "opacityStyleVisible");
    }
    else {
        *(node->opacityStyleVisible) ? cJSON_AddTrueToObject(jsonData, "opacityStyleVisible") : cJSON_AddFalseToObject(jsonData, "opacityStyleVisible");
    }

    if (node->foreignKey == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "foreignKey");
    }
    else {
        cJSON_AddItemToObject(jsonData, "foreignKey", cJSON_Duplicate(node->foreignKey.get(), 1));
    }

    if (node->attributeList == nullptr) {
        ;//do noting
    }
    else {
        cJSON_AddItemToObject(jsonData, "attributeList", cJSON_Duplicate(node->attributeList.get(), 1));
    }

    if (node->viewPointList != nullptr) {
        cJSON_AddItemToObject(jsonData, "viewPointList", cJSON_Duplicate(node->viewPointList.get(), 1));
    }

    if (node->projectId == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "projectId");
    }
    else {
        cJSON_AddStringToObject(jsonData, "projectId", node->projectId.get());
    }

    if (node->floorIndex == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "floorIndex");
    }
    else {
        cJSON_AddNumberToObject(jsonData, "floorIndex", *(node->floorIndex));
    }

    if (node->opacityStyleOpacity == nullptr) {
        ;//cJSON_AddNullToObject(jsonData, "opacityStyleOpacity");
    }
    else {
        cJSON_AddNumberToObject(jsonData, "opacityStyleOpacity", *(node->opacityStyleOpacity));
    }

    if (recurse) {
        //递归将node子节点转换成json
        if (!node->childArray.empty()) {
            cJSON* childrenArray = cJSON_CreateArray();
            for (const auto& childNode : node->childArray) {
                cJSON* childJsonData = parseTreeToJson(childNode, recurse, bShowData, bUnCheckShow);
                cJSON_AddItemToArray(childrenArray, childJsonData);
            }
            cJSON_AddItemToObject(jsonData, "childArray", childrenArray);
        }
    }

    return jsonData;
}

std::shared_ptr<Node> CTreeItem::addChildNode(std::shared_ptr<Node> parentNode) {
    std::shared_ptr<Node> newNode = std::make_shared<Node>();
    newNode->parent = parentNode;
    parentNode->childArray.push_back(newNode);
    return newNode;
}

bool CTreeItem::deleteNode(std::shared_ptr<Node> node) {
    if (node == nullptr) {
        return false;
    }
    if (node->parent != nullptr) {
        auto& children = node->parent->childArray;
        children.erase(std::remove(children.begin(), children.end(), node), children.end());
    }
    node->resourceId = nullptr;
    node->isLockArray = nullptr;
    node->data = nullptr;
    node->strType = nullptr;
    node->icon = nullptr;
    node->opacityStyleColor = nullptr;
    node->buildID = nullptr;
    node->type = nullptr;
    node->secondType = nullptr;
    node->opacityStyleShowBuildOpacity = nullptr;
    node->sceneId = nullptr;
    node->isDrag = nullptr;
    node->bindGuid = nullptr;
    node->checkedArray = nullptr;
    node->order = nullptr;
    node->opacityStyleOpen = nullptr;
    node->version = nullptr;
    node->url = nullptr;
    node->tagList = nullptr;
    node->isExpand = nullptr;
    node->visibleArray = nullptr;
    node->showSetBuild = nullptr;
    node->name = nullptr;
    node->guid = nullptr;
    node->opacityStyleVisible = nullptr;
    node->foreignKey = nullptr;
    node->projectId = nullptr;
    node->floorIndex = nullptr;
    node->opacityStyleOpacity = nullptr;
    node->attributeList = nullptr;
    node->viewPointList = nullptr;

    node->childArray.clear();


    return true;
}

bool CTreeItem::modifyNode(std::shared_ptr<Node> node, const std::string& newName, int newAge) {
    return false;
}

/*******************************************************************************
* 函数名称：
* 功能描述：
* 输入参数：
* 输出参数：
* 返 回 值：0 有元素加载， 1 没有任何元素加载
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

int CTreeItem::loadElementDataTo3D()
{
    std::map<int, std::shared_ptr<Node>> mapDataLay;
    {
        std::unordered_map<std::string, std::shared_ptr<Node>>::iterator itor = m_mapNode.begin();
        for (; itor != m_mapNode.end(); itor++)
        {
            //首先判断当前模型是否需要加载，只有build框选的模型加载
            if (itor->second->checkedArray && isCheck(itor->second->checkedArray.get(), m_strCurSenceId))
            {
                if (itor->second->elementData)
                {
                    if (itor->second->elementData.get()) {
                        cJSON* pElementDataJson = itor->second->elementData.get();
                        bool isLayer = false;
                        isLayer = cJSON_HasObjectItem(pElementDataJson, "_layerType");

                        if (isLayer)
                        {
                            mapDataLay[itor->second->order2] = itor->second;
                        }
                        else {
                            //获取模板id, 设置模板引用计数//////////////////////
                            cJSON* _effectTemplateID = cJSON_GetObjectItem(itor->second->elementData.get(), "_effectTemplateID");
                            std::string strid;
                            if (_effectTemplateID && _effectTemplateID->valuestring)
                                strid = _effectTemplateID->valuestring;
                            GetGlobalSettingInstance().addRef((RTE::SceneDataType) * (itor->second->type), strid);
                            ///////////////////////////////////////////////////////////////////////////////////////

                            std::string strType;
                            strType = RTE::getModelStrTypeByType((RTE::SceneDataType) * (itor->second->type));
                            if (*(itor->second->type) == SDT_DataPannel) //
                            {
                                cJSON* _modelType = cJSON_GetObjectItem(itor->second->elementData.get(), "_modelType");
                                if (_modelType)
                                {
                                    if (_modelType->valueint == MODELTYPE_Blend2DData) //新版数据面片枚举值
                                    {
                                        strType = "Blend2DData";
                                    }
                                }
                            }
                           
                            cJSON* p3dRequest = cJSON_CreateObject();
                            std::string str3dRequest;
                            cJSON_AddStringToObject(p3dRequest, "id", "");
                            cJSON_AddStringToObject(p3dRequest, "element", strType.c_str());
                            cJSON_AddStringToObject(p3dRequest, "method", "add");

                            cJSON_AddItemToObject(p3dRequest, "data", cJSON_Duplicate(pElementDataJson, true));
                            char* pOut = cJSON_Print(p3dRequest);

                            if (pOut)
                            {
                                str3dRequest = pOut;
                                free(pOut);
                            }
                            std::string strRet;
                            int nRet = Interface::elementHandle(str3dRequest, strRet);
                            if (!nRet)
                            {
                                m_cond.wait_for(itor->second->guid.get(), 1);
                            }
                            else {
                                m_nModeCount--;
                            }
                            cJSON_Delete(p3dRequest);
                        }
                    }
                }
            }
        }

        //按顺序加载图层,因为图层有先后顺序，否则会显示不全。
        //std::vector<LayerData*> sceneDataLay;
        for (auto itor : mapDataLay)
        {
            std::string strType = "Layer";
            cJSON* p3dRequest = cJSON_CreateObject();
            std::string str3dRequest;
            cJSON_AddStringToObject(p3dRequest, "element", strType.c_str());
            cJSON_AddStringToObject(p3dRequest, "method", "add");

            cJSON* pElementDataJson = itor.second->elementData.get();
            cJSON_AddItemToObject(p3dRequest, "data", cJSON_Duplicate(pElementDataJson, true));
            char* pOut = cJSON_Print(p3dRequest);
            if (pOut)
            {
                str3dRequest = pOut;
                free(pOut);
            }
            
            std::string strRet;
            int nRet = Interface::elementHandle(str3dRequest, strRet);
            if (nRet)
            {
                m_nLayerCount--;
            }

            cJSON_Delete(p3dRequest);
        }

        if (m_nModeCount + m_nLayerCount == 0)
        {
            return 1;
        }
    }

    return 0;
}

//判断当前场景下元素或者图层是否勾选,或者隐藏
bool CTreeItem::isCheck(cJSON* item, std::string strSenceId)
{
    if (item) {
        int nSize = cJSON_GetArraySize(item);
        for (size_t i = 0; i < nSize; i++)
        {
            cJSON* pItem = cJSON_GetArrayItem(item, i);
            if (pItem->child->string) {
                if (!strcmp(pItem->child->string, strSenceId.c_str()))
                {
                    if (pItem->child->valueint != 0) {
                        return true;
                    }
                    break;
                }
            }
        }
    }
    return false;
}


/*******************************************************************************
* 函数名称：
* 功能描述：根据元素id获取json
* 输入参数：
* 输出参数：
* 返 回 值：json 结构体，外部维护释放
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

cJSON* CTreeItem::getElementJson(std::string id)
{
    //std::lock_guard<std::mutex> lock(m_mutexNode);
    std::unordered_map<std::string, std::shared_ptr<Node>>::iterator itorFind = m_mapNode.find(id);
    if (itorFind == m_mapNode.end())
        return nullptr;
    else
        return parseTreeToJson(itorFind->second,false);
}

/*******************************************************************************
* 函数名称：
* 功能描述：更新制定元素id的tags列表, 这里是否需要及时同步到数据库
* 输入参数：strId  元素id
*			pTags  tags列表，它为cjson指针外部负责维护内存，因此不能直接复制指针
*			strRespones 如果报错，返回给调用方错误信息
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

int CTreeItem::updateTagsByElementId(std::string strId, cJSON* pTags, std::string updateType, std::string& strRespones)
{
    //step 0 检测请求数据格式是否正确
    if (!strId.compare("") || !pTags)
    {
        //Poco::Logger::get("RteLogger").warning("strId or Tags is null");
        strRespones = "strId or Tags is null";
        return -1;
    }

    if (!cJSON_IsArray(pTags))
    {
        //Poco::Logger::get("RteLogger").warning("Tags is not array");
        strRespones = "Tags is not array";
        return -1;
    }

    if (updateType.compare(""))
    {
        return updateTagsByElementId_Replace(strId, pTags, strRespones);
    }
    else if (updateType.compare("+")) {
        return updateTagsByElementId_Add(strId, pTags, strRespones);
    }
    else if (updateType.compare("-")) {
        return updateTagsByElementId_Delete(strId, pTags, strRespones);
    }
    return 0;
}

/*******************************************************************************
* 函数名称：
* 功能描述：根据id替换 tags
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

int CTreeItem::updateTagsByElementId_Replace(std::string strId, cJSON* pTags, std::string& strRespones)
{
    //node 节点操作需要加锁
    //std::lock_guard<std::mutex> lock(m_mutexNode);
    //step 1 根据id找到对应的node
    std::unordered_map<std::string, std::shared_ptr<Node>>::iterator itorFind = m_mapNode.find(strId);
    if (itorFind == m_mapNode.end()) {
        strRespones = "strId not found";
        return -1;
    }


    std::shared_ptr<Node>& pNode = itorFind->second;
    //step 2 将之前的tag 卸载下来，并同步卸载tags模块的内存管理
    cJSON* pTagsOld = pNode->tagList.get();
    if (pTagsOld)
    {
        //同步卸载tags模块
        if (cJSON_IsArray(pTagsOld)) {
            int nCount = cJSON_GetArraySize(pTagsOld);
            if (nCount > 0) {
                for (size_t i = 0; i < nCount; i++)
                {
                    cJSON* pItem = cJSON_GetArrayItem(pTagsOld, i);
                    if (pItem && pItem->string)
                    {
                        m_tagManager.deleteTag(pItem->string, strId);
                    }
                }
            }
            else {
                m_tagManager.deleteTag("[]", strId);
            }
        }
        //cJSON_Delete(pTagsOld);
    }
    pNode->tagList.reset(cJSON_Duplicate(pTags, 1), cJSON_Delete);

    //step 3 将新的tags更新到node节点，然后安装新的tags到tags模块
    int nCount2 = cJSON_GetArraySize(pTags);
    if (nCount2 > 0) {
        for (size_t i = 0; i < nCount2; i++)
        {
            cJSON* pItem = cJSON_GetArrayItem(pTags, i);
            if (pItem && pItem->string)
            {
                m_tagManager.addTag(pItem->string, strId, pNode->type.get() == nullptr ? -1 : *(pNode->type));
            }
        }
    }else {
        m_tagManager.addTag("[]", strId, pNode->type.get() == nullptr ? -1 : *(pNode->type)); //如果array 的size为0 标记为特殊字符 "[]"
    }
    return 0;
}


int CTreeItem::updateTagsByNode_Replace(std::shared_ptr<Node> pNode, cJSON* pTags, std::string& strRespones)
{
    //step 1 根据id找到对应的node
    //step 2 将之前的tag 卸载下来，并同步卸载tags模块的内存管理
    if (!pNode)
        return -1;

    cJSON* pTagsOld = pNode->tagList.get();
    if (pTagsOld)
    {
        //同步卸载tags模块
        if (cJSON_IsArray(pTagsOld)) {
            int nCount = cJSON_GetArraySize(pTagsOld);

            if (nCount > 0) {
                for (size_t i = 0; i < nCount; i++)
                {
                    cJSON* pItem = cJSON_GetArrayItem(pTagsOld, i);
                    if (pItem && pItem->child && pItem->child->string)
                    {
                        m_tagManager.deleteTag(pItem->child->string, pNode->guid.get());
                    }
                }
            }
            else {
                m_tagManager.deleteTag("[]", pNode->guid.get());
            }
        }
    }
    pNode->tagList.reset(cJSON_Duplicate(pTags, 1), cJSON_Delete);

    //step 3 将新的tags更新到node节点，然后安装新的tags到tags模块
    int nCount2 = cJSON_GetArraySize(pTags);
    if (nCount2 > 0) {
        for (size_t i = 0; i < nCount2; i++)
        {
            cJSON* pItem = cJSON_GetArrayItem(pTags, i);
            if (pItem && pItem->child && pItem->child->string)
            {
                m_tagManager.addTag(pItem->child->string, pNode->guid.get(), pNode->type.get() == nullptr ? -1 : *(pNode->type));
            }
        }
    }
    else {
        m_tagManager.addTag("[]", pNode->guid.get(), pNode->type.get() == nullptr ? -1 : *(pNode->type)); //如果array 的size为0 标记为特殊字符 "[]"
    }
    return 0;
}

/*******************************************************************************
* 函数名称：
* 功能描述：根据id添加 指定tags
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

int CTreeItem::updateTagsByElementId_Add(std::string strId, cJSON* pTags, std::string& strRespones)
{
    //node 节点操作需要加锁
    //std::lock_guard<std::mutex> lock(m_mutexNode);
    //step 1 根据id找到对应的node
    std::unordered_map<std::string, std::shared_ptr<Node>>::iterator itorFind = m_mapNode.find(strId);
    if (itorFind == m_mapNode.end()) {
        strRespones = "strId not found";
        return -1;
    }

    std::shared_ptr<Node>& pNode = itorFind->second;
    //step 2 将之前的tag 卸载下来，并同步卸载tags模块的内存管理
    cJSON* pTagsOld = pNode->tagList.get();
    //pNode->tagList.reset(cJSON_Duplicate(pTags, 1), cJSON_Delete);
    if (pTagsOld)
    {
        //同步卸载tags模块
        if (cJSON_IsArray(pTagsOld)) {
            int nCount = cJSON_GetArraySize(pTagsOld);

            if (nCount > 0) {
                for (size_t i = 0; i < nCount; i++)
                {
                    cJSON* pItem = cJSON_GetArrayItem(pTagsOld, i);
                    if (pItem && pItem->string)
                    {
                        m_tagManager.deleteTag(pItem->string, strId);
                    }
                }
            }
            else {
                m_tagManager.deleteTag("[]", strId);
            }
        }
        //cJSON_Delete(pTagsOld);
    }
    pNode->tagList.reset(cJSON_Duplicate(pTags, 1), cJSON_Delete);

    //step 3 将新的tags更新到node节点，然后安装新的tags到tags模块
    int nCount2 = cJSON_GetArraySize(pTags);
    if (nCount2 > 0)
    {
        for (size_t i = 0; i < nCount2; i++)
        {
            cJSON* pItem = cJSON_GetArrayItem(pTags, i);
            if (pItem && pItem->string)
            {
                m_tagManager.addTag(pItem->string, strId, pNode->type.get() == nullptr ? -1 : *(pNode->type));
            }
        }
    }else {
        m_tagManager.addTag("[]", pNode->guid.get(), pNode->type.get() == nullptr ? -1 : *(pNode->type)); //如果array 的size为0 标记为特殊字符 "[]"
    }
    return 0;
}

/*******************************************************************************
* 函数名称：
* 功能描述：根据id 删除指定的 tags
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

int CTreeItem::updateTagsByElementId_Delete(std::string strId, cJSON* pTags, std::string& strRespones)
{
    return 0;
}

//条件解析器  
/*
 {
"view”: true,
“type”: [26,27] //这里可以是字符串
“外部ID”: [""]
"id":[]
"外部属性": [] //模糊搜索
“tags”:[ “111”,”2222”], //模糊搜索
"area":{}
"@FCondition": ["view",”!area”,”tag”,”type”,"外部ID--foreignKey","Id"]   // 如果外部id唯一建立索引
"@Condition": ["name"]
"@Column": [{ID},{iver:{x,y,z}}]
}

{
    "method": "get",
    "element": {
        "view": true,
        "type": [26,27],
        "guid": [],
        "foreignKey":[],
        "tags":["1111","222"],
        "area":{"x":2, "y":3, "z":4 },
        "name":"poi",
        "@FCondition": ["view","!area","tag", "type","foreignKey","guid"],
        "@Condition": ["name{}"],
        "@Column": [{ "id": "id"},{"elementData":["_bbCenterWorldPos","_pulseColor"]}]
    }
}
*/
bool CTreeItem::conditionPaser(std::string request, std::string strMethod, 
    std::unordered_map<std::string, std::pair<bool,std::vector<std::string>>>& FCondition,
    std::unordered_map<std::string, std::pair<int,std::vector<std::string>>>& Condition,
    std::vector<ColumnElement>& column)
{
    cJSON* pRoot = cJSON_Parse(request.c_str());
    if (pRoot)
    {
        //首先查找前置条件
        if (cJSON_HasObjectItem(pRoot, "@FCondition"))
        {
            cJSON* pFcondition = cJSON_GetObjectItem(pRoot, "@FCondition");
            if (pFcondition)
            {
                //Poco::Stopwatch watch;
                //watch.start();
                //遍历前置条件数组
                int nSize = cJSON_GetArraySize(pFcondition);
                for (size_t i = 0; i < nSize; i++)
                {
                    //首先判断是否为非
                    cJSON* pItem = cJSON_GetArrayItem(pFcondition, i);
                    if (pItem && pItem->valuestring) {
                        std::string strItem = pItem->valuestring;
                        bool ret = strstr(strItem.c_str(),"!") ? true : false;
                        strItem = ret ? strItem.substr(1) : strItem;

                        ret = !ret;//带! 为false 
                        //从root找其具体参数  //strItem 为前置筛选条件
                        if (cJSON_HasObjectItem(pRoot, strItem.c_str())) 
                        {
                            cJSON* pTemp = cJSON_GetObjectItem(pRoot, strItem.c_str());
                            if (pTemp && cJSON_IsArray(pTemp))
                            {
                                int nSize2 = cJSON_GetArraySize(pTemp);
                                std::vector<std::string> values;
                                for (size_t i = 0; i < nSize2; i++)
                                {
                                    cJSON* pItem2 = cJSON_GetArrayItem(pTemp, i);
                                    if (cJSON_IsObject(pItem2)) {
                                        char* pOut = cJSON_Print(pTemp);
                                        if (pOut) {
                                            values.push_back(pOut);
                                            free(pOut);
                                        }
                                        break;
                                    }
                                    else {
                                        std::string strVaule;
                                        if (pItem2->valuestring) // 字符串
                                        {
                                            strVaule = pItem2->valuestring;
                                        }
                                        else {                   //int bool，转字符串
                                            strVaule = std::to_string(pItem2->valueint);
                                        }

                                        //if (!strVaule.empty()) // "" 字符串也算查询条件
                                        {
                                            values.push_back(strVaule);
                                        }
                                    }
                                }
                                FCondition[strItem] = std::pair<bool, std::vector<std::string>>(ret, values);
                            }
                            else {
                                //这里不是数组的前置条件
                                if (pTemp && pTemp->string)
                                {
                                    std::string strType = pTemp->string;
                                    if (!strType.compare("view"))
                                    {
                                        std::vector<std::string> values;
                                        if (pTemp->type == cJSON_True)
                                            values.push_back("true");
                                        else
                                            values.push_back("false");

                                        FCondition[strItem] = std::pair<bool, std::vector<std::string>>(ret,values);
                                    }
                                    else if (!strType.compare("area"))
                                    {
                                        char* pOut = cJSON_Print(pTemp);
                                        if (pOut) {
                                            std::vector<std::string> values;
                                            values.push_back(pOut);
                                            FCondition[strItem] = std::pair<bool, std::vector<std::string>>(ret, values);
                                            free(pOut);
                                        }
                                    }
                                    else if (!strType.compare("attributeList")) 
                                    {
                                        char* pOut = cJSON_Print(pTemp);
                                        if (pOut) {
                                            std::vector<std::string> values;
                                            values.push_back(pOut);
                                            FCondition[strItem] = std::pair<bool, std::vector<std::string>>(ret, values);
                                            free(pOut);
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            //Poco::Logger::get("RteLogger").debug("condition paser not found %s value", strItem);
                        }
                    }
                }
                //Poco::Logger::get("RteLogger").debug("遍历前置条件数组use time %d value", (int)watch.elapsed());
            }
        }
        //step2 遍历后置条件
        if (cJSON_HasObjectItem(pRoot, "@Condition"))
        {
            if (cJSON_HasObjectItem(pRoot, "@Condition"))
            {
                cJSON* pCondition = cJSON_GetObjectItem(pRoot, "@Condition");
                if (pCondition)
                {
                    //遍历后置条件数组
                    //std::map<std::string, std::vector<std::pair<int, std::string>>>& Condition,
                    int nSize = cJSON_GetArraySize(pCondition);
                    for (int i = 0; i < nSize; i++)
                    {
                        cJSON* pItem = cJSON_GetArrayItem(pCondition, i);
                        if (pItem && pItem->valuestring)
                        {
                            //首先判断其请求类型 如：==, != , {}, }{
                            std::string strParam = pItem->valuestring;
                            int nType = -1;
                            if (strstr(strParam.c_str(), "==")) {
                                nType = 0;
                                strParam = strParam.substr(0, strParam.find("=="));
                            }
                            else if (strstr(strParam.c_str(), "!=")) {
                                nType = 1;
                                strParam = strParam.substr(0, strParam.find("!="));
                            }
                            else if (strstr(strParam.c_str(), "{}")) {
                                nType = 2;
                                strParam = strParam.substr(0, strParam.find("{}"));
                            }
                            else if (strstr(strParam.c_str(), "}{")) {
                                nType = 3;
                                strParam = strParam.substr(0, strParam.find("}{"));
                            }
                            else
                                continue;

                            std::vector<std::string> vecValues;
                            //从root中找item对应的参数
                            if (cJSON_HasObjectItem(pRoot, strParam.c_str()))
                            {
                                cJSON* pJsonValues = cJSON_GetObjectItem(pRoot, strParam.c_str());
                                int nSize2 = cJSON_GetArraySize(pJsonValues);
                                for (int i = 0; i < nSize2; i++)
                                {
                                    cJSON* pItem2 = cJSON_GetArrayItem(pJsonValues, i);
                                    if (pItem2 && pItem2->valuestring) //不是字符串形式的是否判断，后期根据需要在添加
                                    {
                                        vecValues.push_back(pItem2->valuestring);
                                    }
                                }
                            }

                            if(vecValues.size()>0)
                                Condition[strParam] = std::pair<int, std::vector<std::string>>(nType, vecValues);
                        }
                    }
                }
            }
        }

        //step3 解析输出限制， [{ "id": "id"},{"elementData":["_bbCenterWorldPos","_pulseColor"]}] 
        if (cJSON_HasObjectItem(pRoot, "@Column"))
        {
            cJSON* pColumnJson = cJSON_GetObjectItem(pRoot, "@Column");
            if (pColumnJson && cJSON_IsArray(pColumnJson))
            {
                int nSize3 = cJSON_GetArraySize(pColumnJson);
                for (int i = 0; i < nSize3; i++)
                {
                    cJSON* pItem3 = cJSON_GetArrayItem(pColumnJson, i);

                    if (!pItem3)
                        continue;

                    if (pItem3->child && pItem3->child->string) {
                        ColumnElement temp;
                        temp.strKey = pItem3->child->string;
                        if (cJSON_IsArray(pItem3->child)) {
                            temp.bSecondLevel = true;
                            int nSize4 = cJSON_GetArraySize(pItem3->child);
                            for (size_t k = 0; k < nSize4; k++)
                            {
                                cJSON* pItem4 = cJSON_GetArrayItem(pItem3->child, k);
                                if (pItem4 && pItem4->valuestring)
                                {
                                    temp.vecSecondLevelKey.push_back(pItem4->valuestring);
                                }
                            }
                        }
                        column.push_back(temp);
                    }
                }
            }
        }

        return true;
    }
    return false;
}

/*******************************************************************************
* 函数名称：
* 功能描述：遍历guid
* 输入参数：nState: true ==, false !=  
            vecCondition:查询条件数组间为||关系
            bAll: 是否在整个容器中搜索
* 输出参数：retVec 匹配的node数组,传入传出参数
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/
void CTreeItem::ergodicGuid(bool nState, bool bAll, std::vector<std::string> vecCondition, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap)
{
    if (bAll)
    {
        std::map<std::string, std::shared_ptr<Node>> tmpMap;
        for (auto itor : vecCondition)
        {
            std::unordered_map<std::string, std::shared_ptr<Node>>::iterator itorFind = m_mapNode.find(itor);
            if (itorFind != m_mapNode.end())
                tmpMap[itorFind->second->guid.get()] = itorFind->second;
        }

        if (nState) {
            retMap.insert(tmpMap.begin(), tmpMap.end());
        }
        else { //取反将数据放到retMap
            std::map<std::string, std::shared_ptr<Node>> tmpMap2;
            tmpMap2.insert(m_mapNode.begin(), m_mapNode.end()); //备份一份查找源数据

            //过滤掉tmp中的数据
            for ( auto itor : tmpMap)
            {
                auto itorFind = tmpMap2.find(itor.first);
                if (itorFind != tmpMap2.end())
                    tmpMap2.erase(itorFind);
            }

            retMap.insert(tmpMap2.begin(), tmpMap2.end());
        }
    }
    else {
        if (retMap.size() < 0)
            return;

        std::map<std::string, std::shared_ptr<Node>> tmpMap;
        //在retmap 中查找制定元素
        for (auto itor : vecCondition)
        {
            auto itorFind = retMap.find(itor);
            if (itorFind != retMap.end()) {
                tmpMap[itorFind->second->guid.get()] = itorFind->second;
            }
        }

        if (nState) {
            retMap.clear();
            retMap.insert(tmpMap.begin(), tmpMap.end());
        }
        else {
            //过滤掉tmp中的数据
            for (auto itor : tmpMap)
            {
                auto itorFind = retMap.find(itor.first);
                if (itorFind != retMap.end())
                    retMap.erase(itorFind);
            }
        }
    }
}


//查询此ID下的childid
void CTreeItem::ergodicParentId(bool nState, bool bAll, std::vector<std::string> vecCondition, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap)
{
    if (bAll)
    {
        std::map<std::string, std::shared_ptr<Node>> tmpMap;
        std::map<std::string, std::shared_ptr<Node>> tmpMapParentId;
        for (auto itor : vecCondition)
        {
            std::unordered_map<std::string, std::shared_ptr<Node>>::iterator itorFind = m_mapNode.find(itor);
            if (itorFind != m_mapNode.end())
                tmpMapParentId[itorFind->second->guid.get()] = itorFind->second;
        }

        //parentid 查询条件没有并集交集说法，直接取parent 下的子id
        for (auto itor : tmpMapParentId)
        {
            for (size_t i = 0; i < itor.second->childArray.size(); i++)
            {
                tmpMap[itor.second->childArray[i]->guid.get()] = itor.second->childArray[i];
            }
        }
        retMap.insert(tmpMap.begin(), tmpMap.end());
    }
    else {
        if (retMap.size() < 0)
            return;

        std::map<std::string, std::shared_ptr<Node>> tmpMap;
        std::map<std::string, std::shared_ptr<Node>> tmpMap1;
        std::map<std::string, std::shared_ptr<Node>> tmpMapParentId;
        for (auto itor : vecCondition)
        {
            std::unordered_map<std::string, std::shared_ptr<Node>>::iterator itorFind = m_mapNode.find(itor);
            if (itorFind != m_mapNode.end())
                tmpMapParentId[itorFind->second->guid.get()] = itorFind->second;
        }

        //parentid 查询条件没有并集交集说法，直接取parent 下的子id
        for (auto itor : tmpMapParentId)
        {
            for (size_t i = 0; i < itor.second->childArray.size(); i++)
            {
                tmpMap[itor.second->childArray[i]->guid.get()] = itor.second->childArray[i];
            }
        }

        for (auto itor : tmpMapParentId)
        {
            auto itorFind = retMap.find(itor.first);
            if (itorFind != retMap.end()) {
                tmpMap1[itorFind->second->guid.get()] = itorFind->second;
            }
        }

        if (nState) {
            retMap.clear();
            retMap.insert(tmpMap1.begin(), tmpMap1.end());
        }
        else {
            //过滤掉tmp中的数据
            for (auto itor : tmpMap1)
            {
                auto itorFind = retMap.find(itor.first);
                if (itorFind != retMap.end())
                    retMap.erase(itorFind);
            }
        }
    }
}

/*******************************************************************************
* 函数名称：
* 功能描述：遍历外键
* 输入参数：nState: true ==, false != 
            vecCondition:查询条件数组间为||关系
            bAll: 是否在整个容器中搜索
* 
* 输出参数：retVec 匹配的node数组,传入传出参数
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/
void CTreeItem::ergodicForeignKey(bool nState, bool bAll, std::vector<std::string> vecCondition, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap)
{
    //外键索引中查找
    std::map<std::string, std::shared_ptr<Node>> tmpMap;

    if (vecCondition.size() > 0)
    {
        for (auto itor : vecCondition)
        {
            std::map<std::string, std::shared_ptr<Node>>::iterator itorFind = m_mapForeignKeyNode.find(itor); //外键索引表中查询
            if (itorFind != m_mapForeignKeyNode.end())
                tmpMap[itorFind->second->guid.get()] = itorFind->second;
        }
    }
    else {
        tmpMap.insert(m_mapForeignKeyNullNode.begin(), m_mapForeignKeyNullNode.end());
    }

    //查看是否在结果中能找到。
    if (bAll) {
        if (nState) {
            retMap.insert(tmpMap.begin(), tmpMap.end());
        }
        else {
            //这里需要注意外键表不一定保存所有元素，外键为空的未放到外键索引里，所以需要在元素索引中查找
            std::vector<std::string> vecDeleteKy; //要删除的元素
            bool bFind = false;
            for (auto itor : m_mapNode)
            {
                for (auto itor2 : vecCondition) {
                    if (itor.first == itor2)
                    {
                        bFind = true;
                        break;
                    }
                }
                if (bFind)
                {
                    vecDeleteKy.push_back(itor.first);
                    bFind = false;
                }
            }

            std::map<std::string, std::shared_ptr<Node>> tmpMap;
            tmpMap.insert(m_mapNode.begin(), m_mapNode.end()); //备份一份查找源数据
            for (auto itor : vecDeleteKy)
            {
                tmpMap.erase(itor);
            }
            //将数据拷贝到retMap
            retMap.insert(tmpMap.begin(), tmpMap.end());
        }
    }
    else {
        std::map<std::string, std::shared_ptr<Node>> tmpMap2;
        auto itor = tmpMap.begin();
        std::vector<std::string> vecDelete;
        for (; itor!=tmpMap.end(); itor++) //取交集
        {
            auto itor2 = retMap.find(itor->first);
            if (itor2 != retMap.end())
            {
                tmpMap2[itor2->first] = itor2->second;
                vecDelete.push_back(itor2->first);
            }
        }

        if (nState) {
            retMap.clear();
            retMap.insert(tmpMap2.begin(), tmpMap2.end());
        }
        else {
            for (auto itor : vecDelete)
                retMap.erase(itor);
        }
    }
}

/*******************************************************************************
* 函数名称：
* 功能描述：遍历tags
* 输入参数：nState: true ==, false !=  
            vecCondition:查询条件数组间为||关系
            bAll: 是否在整个容器中搜索
* 输出参数：retVec 匹配的node数组,传入传出参数
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/
void CTreeItem::ergodicTags(bool nState, bool bAll, std::vector<std::string> vecCondition, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap)
{
    std::map<std::string, std::vector<std::pair<std::string, int>>> tagsMap = m_tagManager.getTagMap();
    if (bAll) {
        //根据tag 寻找guid
        std::vector<std::string> vecFind;
        for (auto itor : vecCondition)
        {
            auto itor2 = tagsMap.find(itor);
            if (itor2 != tagsMap.end())
            {
                for (auto itor3 : itor2->second)
                {
                    vecFind.push_back(itor3.first);
                }
            }
        }

        //查找完毕后，根据nstate判断是否取反
        if (nState) {
            //在元素表中，查找这些id
            for (auto itor : vecFind) {
                auto itor2 = m_mapNode.find(itor);
                if (itor2 != m_mapNode.end()) {
                    retMap.insert(*itor2);
                }
            }
        }
        else {
            //拷贝元素表
            std::map<std::string, std::shared_ptr<Node>> tmpMap;
            tmpMap.insert(m_mapNode.begin(), m_mapNode.end());
            //删除查找到的数据

            for (auto itor : vecFind) {
                tmpMap.erase(itor);
            }

            //将过滤数据拷贝到返回map
            retMap.insert(tmpMap.begin(), tmpMap.end());
        }
    }
    else {
        //根据tag 寻找guid
        std::vector<std::string> vecFind;
        for (auto itor : vecCondition)
        {
            auto itor2 = tagsMap.find(itor);
            if (itor2 != tagsMap.end())
            {
                for (auto itor3 : itor2->second)
                {
                    vecFind.push_back(itor3.first);
                }
            }
        }
        std::map<std::string, std::shared_ptr<Node>> tmpNodeMap;
        //查找完毕后，根据nstate判断是否取反
        if (nState) {
            //在retmap表中，查找这些id
            for (auto itor : vecFind) {
                auto itor2 = retMap.find(itor);
                if (itor2 != retMap.end()) {
                    tmpNodeMap.insert(*itor2);
                }
            }
            retMap.clear();
            retMap.insert(tmpNodeMap.begin(), tmpNodeMap.end());
        }
        else {
            //删除查找到的数据
            for (auto itor : vecFind) {
                retMap.erase(itor);
            }
        }
    }
}

/*******************************************************************************
* 函数名称：
* 功能描述：遍历type
* 输入参数：nState: true ==, false !=
            vecCondition:查询条件数组间为||关系
            bAll: 是否在整个容器中搜索
* 输出参数：retVec 匹配的node数组,传入传出参数
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/
void CTreeItem::ergodicType(bool nState, bool bAll, std::vector<std::string> vecCondition, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap)
{
    if (bAll) {
        std::map<std::string, std::shared_ptr<Node>> tmpNodeMap;
        for (auto itor : m_mapNode)
        {
            for (auto itor2 : vecCondition) {
                if (!itor.second->type)
                    continue;
                if (*itor.second->type == atoi(itor2.c_str())) {
                    tmpNodeMap.insert(itor);
                    break;
                }
            }
        }

        if (nState) {
            retMap.insert(tmpNodeMap.begin(), tmpNodeMap.end());
        }
        else {
            std::map<std::string, std::shared_ptr<Node>> tmpNodeMap2;
            tmpNodeMap2.insert(m_mapNode.begin(), m_mapNode.end());

            for (auto itor : tmpNodeMap) {
                tmpNodeMap2.erase(itor.first);
            }
        }
    }
    else {
        std::map<std::string, std::shared_ptr<Node>> tmpNodeMap;
        for (auto itor : retMap) {
            for (auto itor2 : vecCondition) {
                if (!itor.second->type)
                    continue;
                if (*itor.second->type == atoi(itor2.c_str())) {
                    tmpNodeMap.insert(itor);
                    break;
                }
            }
        }
        if (nState) {
            retMap.clear();
            retMap.insert(tmpNodeMap.begin(), tmpNodeMap.end());
        }
        else {
            for (auto itor : tmpNodeMap) {
                retMap.erase(itor.first);
            }
        }
    }
}


/*******************************************************************************
* 函数名称：
* 功能描述：遍历业务属性，查找符合条件的元素
* 输入参数：
*      nState： 多个条件的情况下，true 为取并集，false 为取交集。
*           
       vecCondition 数组size 必为1，数据格式 {"indoor":true,"CHid":"11100001"}
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

void CTreeItem::ergodicAttributeList(bool nState, bool bAll, std::vector<std::string> vecCondition, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap)
{
    if (vecCondition.size() > 0) 
    {
        std::map<std::string, std::string> resultMap;
        cJSON* pJson = cJSON_Parse(vecCondition[0].c_str());
        if (pJson && cJSON_IsArray(pJson))
        {
            Attributes attr;
            std::vector<std::pair<std::string, Attributes>> vecAttr;

            int nSize = cJSON_GetArraySize(pJson);
            for (size_t i = 0; i < nSize; i++)
            {
                cJSON* pItem = cJSON_GetArrayItem(pJson, i);

                cJSON* pChild = pItem->child;
                if (pChild) {
                    switch (pChild->type)
                    {
                    case cJSON_False:
                        attr.setBoolValue(false);
                        break;
                    case cJSON_True:
                        attr.setBoolValue(true);
                        break;
                    case cJSON_Number:
                        attr.setDoubleValue(pChild->valuedouble);
                        break;
                    case cJSON_String:
                        attr.setStringValue(pChild->valuestring);
                        break;
                    default:
                        break;
                    }

                    if(nState)
                        m_AttriManager.findIDsByAttri(pChild->string, attr, resultMap, !nState);
                    else
                        vecAttr.push_back(std::make_pair(pChild->string, attr));
                }
            }

            if (!nState){//取交集
                m_AttriManager.findIDsByAttri(vecAttr, resultMap);
            }
        }

        if (bAll) {
            //直接赋值
            for (const auto &itor : resultMap)
            {
                auto itorFind = m_mapNode.find(itor.first);
                if(itorFind!=m_mapNode.end())
                    retMap.insert(*itorFind);
            }
        }
        else {
            //取交集
            for (auto itor = retMap.begin(); itor != retMap.end(); )
            {
                if (resultMap.find(itor->first) == resultMap.end()) {
                    itor = retMap.erase(itor);
                }
                else
                    itor++;
            }
        }
    }
}

/*******************************************************************************
* 函数名称：
* 功能描述：根据查询条件从3d中获取元素id
* 输入参数：
* {
	"element": "HighlightArea", //不传即为所有元素
	"method": "get",
	"view": true,
	"area": {
		"point": {
			"w": 1.0,
			"x": 116.38863941993556,
			"y": 39.90365656287492,
			"z": 0.03636673651635647
		},
		"radius": 100.0
	},
	"id": []  // 不传即为所有
}
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/
void CTreeItem::ergodicViewBy3D(bool nState, bool bAll, std::vector<std::string> vecCondition, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap)
{
    if (vecCondition.size() <= 0) {
        return;
    }
    
    std::string condition = vecCondition[0];
    bool bView = !condition.compare("true") ? true : false;
    bView = nState ? bView : !bView;

    cJSON* pInput = cJSON_CreateObject();
    cJSON_AddStringToObject(pInput, "method", "get");
    cJSON_AddBoolToObject(pInput, "view", bView);
    
    //如果 bALL 为true, retMap肯定为空
    std::string strInput;
    if (bAll)
    {
        char* pOut = cJSON_Print(pInput);
        strInput = pOut;
        if (pOut)
            free(pOut);
    }
    else {
        if (retMap.size() > 0) {
            cJSON* pArray = cJSON_CreateArray();
            for (auto itor : retMap) {
                cJSON_AddItemToArray(pArray, cJSON_CreateString(itor.first.c_str()));
            }
            cJSON_AddItemToObject(pInput, "id", pArray);

            char* pOut = cJSON_Print(pInput);
            if (pOut) {
                strInput = pOut;
                free(pOut);
            }
        } 
    }
    cJSON_Delete(pInput);

    //调用3d接口返回 id数组
    std::string strResult;
    if (!Interface::elementHandle(strInput, strResult)) //成功才去过滤，失败不处理
    {
        cJSON* pResultJson = cJSON_Parse(strResult.c_str());
        do
        {
            if (pResultJson) {
                cJSON* code = cJSON_GetObjectItem(pResultJson, "code");
                if (code && code->valueint != 0)
                    break;

                cJSON* pData = cJSON_GetObjectItem(pResultJson, "data");

                if (pData && cJSON_IsArray(pData)) {
                    retMap.clear();
                    int nCount = cJSON_GetArraySize(pData);
                    for (size_t i = 0; i < nCount; i++)
                    {
                        cJSON* pItem = cJSON_GetArrayItem(pData, i);
                        if (pItem && pItem->valuestring)
                        {
                            auto itor = m_mapNode.find(pItem->valuestring);
                            if(itor!=m_mapNode.end())
                                retMap.insert(*itor);
                        }
                    }
                }
            }
        } while (0);

        if (pResultJson)
            cJSON_Delete(pResultJson);
    } 
}

/*******************************************************************************
* 函数名称：
* 功能描述：区域内 根据查询条件从3d中获取元素id
* 输入参数：retMap 之前搜索到的结果
*           retVector 输出有顺序的结果
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/
void CTreeItem::ergodicAreaBy3D(bool nState, bool bAll, std::vector<std::string> vecCondition, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap, std::vector<std::shared_ptr<Node>>& retVector)
{
    if (vecCondition.size() <= 0)
        return;

    cJSON* pConditionJson = cJSON_Parse(vecCondition[0].c_str());
    if (!pConditionJson) {
        //Poco::Logger::get("RteLogger").warning("ergodicAreaBy3D error %s", vecCondition[0]);
    }

    cJSON* pInput = cJSON_CreateObject();
    std::string strInput;

    cJSON_AddStringToObject(pInput, "method", "get");
    cJSON_AddItemToObject(pInput, "area", pConditionJson);
    if (bAll)
    {
        char* pOut = cJSON_Print(pInput);
        if (pOut) {
            strInput = pOut;
            free(pOut);
        }
    }
    else {
        if (retMap.size() > 0) {
            cJSON* pArray = cJSON_CreateArray();
            for (auto itor : retMap) {
                cJSON_AddItemToArray(pArray, cJSON_CreateString(itor.first.c_str()));
            }
            cJSON_AddItemToObject(pInput, "id", pArray);

            char* pOut = cJSON_Print(pInput);
            if (pOut) {
                strInput = pOut;
                free(pOut);
            }
        }
    }

    if (pInput)
        cJSON_Delete(pInput);

    //调用3d接口返回 id数组
    //int elementHandle(const std::string & strJson, std::string & resultJson);

    std::string strResult;
    std::vector<std::string> vecId; //vecId 是按着远近顺序返回的
    if (!Interface::elementHandle(strInput, strResult))
    {
        cJSON* pResultJson = cJSON_Parse(strResult.c_str());
        do
        {
            if (pResultJson) {
                cJSON* code = cJSON_GetObjectItem(pResultJson, "code");
                if (code && code->valueint != 0)
                    break;

                cJSON* pData = cJSON_GetObjectItem(pResultJson, "data");

                if (pData && cJSON_IsArray(pData)) {
                    int nCount = cJSON_GetArraySize(pData);
                    for (size_t i = 0; i < nCount; i++)
                    {
                        cJSON* pItem = cJSON_GetArrayItem(pData, i);
                        if (pItem && pItem->valuestring)
                        {
                            vecId.push_back(pItem->valuestring);
                        }
                    }
                }
            }
        } while (0);

        if (pResultJson)
            cJSON_Delete(pResultJson);
    }

    if (bAll) {
        if (nState) {
            for (auto itor : vecId)
            {
                auto itorFind = m_mapNode.find(itor);
                if (itorFind != m_mapNode.end()) {
                    retVector.push_back(itorFind->second);
                    //retMap.insert(*itorFind);
                }
            }
        }
        else {
            //拷贝元素map，剔除vecID
            retMap.insert(m_mapNode.begin(), m_mapNode.end());
            for (auto itor : vecId)
            {
              //如果取反的话就没有顺序了,所以修改retMap即可。
                retMap.erase(itor);
            }
        }
    }
    else {
        if (nState) {
            retMap.clear();
            for (auto itor : vecId)
            {
                auto itorFind = m_mapNode.find(itor);
                if(itorFind!=m_mapNode.end())
                    retVector.push_back(itorFind->second);
                    //retMap.insert(*itorFind);
            }
        }
        else {
            for (auto itor : vecId)
            {
                retMap.erase(itor);
            }
        }
    }
}

/*******************************************************************************
* 函数名称：
* 功能描述：遍历后置条件的某一项
* 输入参数：
*           strColumnName 过滤字段名称
*           std::pair<int, std::vector<std::string>> mapCondition  first 过滤字段类型，second 具体值
*           retMap                                                                     需要过滤的map
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

void CTreeItem::ergodicConditionColumn(std::string strColumnName, std::pair<int, std::vector<std::string>> Conditions, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap)
{
    std::unordered_map<std::string, std::shared_ptr<Node>> tmpMap;
    for (auto itorNode : retMap) {
        auto itorVal = itorNode.second->index.find(strColumnName);
        if (itorVal == itorNode.second->index.end()) {
            //Poco::Logger::get("RteLogger").warning("condition not support %s", strColumnName);
            continue;
        }
        //遍历条件
        switch (Conditions.first)
        {
        case 0: // == 
        {
            for (auto itorValue : Conditions.second) {
                if (itorVal->second.get() && !itorValue.compare(itorVal->second.get())) { //只要有一个满足条件即可
                    tmpMap.insert(itorNode);
                    break;
                }
            }
        }
        break;
        case 1: // !=
        {
            bool bOk = true;
            for (auto itorValue : Conditions.second) {
                if (itorVal->second.get() && !itorValue.compare(itorVal->second.get())) { //只要有一个满足条件即可
                    bOk = false;
                }
            }
            if (bOk) {
                tmpMap.insert(itorNode);
            }
        }
        break;
        case 2: // {}
        {
            for (auto itorValue : Conditions.second) {
                if (itorVal->second.get() && strstr(itorVal->second.get(),itorValue.c_str())) { //只要有一个满足条件即可
                    tmpMap.insert(itorNode);
                    break;
                }
            }
        }
        break;
        case 3: // }{
        {
            bool bOk = true;
            for (auto itorValue : Conditions.second) {
                if (itorVal->second.get() && strstr(itorVal->second.get(), itorValue.c_str())) { //只要有一个满足条件即可
                    bOk = false;
                }
            }
            if (bOk) {
                tmpMap.insert(itorNode);
            }
        }
        break;
        default:
            break;
        }
    }
    retMap.clear();
    retMap.insert(tmpMap.begin(), tmpMap.end());
}

void CTreeItem::ergodicCondition(bool bAll, std::unordered_map<std::string, std::pair<int, std::vector<std::string>>> mapCondition, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap)
{
    //后置条件无索引,遍历整个nodemap
    if (bAll) {
        std::unordered_map<std::string, std::shared_ptr<Node>> tmpMap;
        tmpMap.insert(m_mapNode.begin(), m_mapNode.end());
        for ( auto itor : mapCondition)
        {
            if (tmpMap.size() > 0)
                ergodicConditionColumn(itor.first, itor.second, tmpMap);
            else
                break;
        }
        retMap.insert(tmpMap.begin(), tmpMap.end());
    }
    else {
        for (auto itor : mapCondition)
        {
            if (retMap.size() > 0)
                ergodicConditionColumn(itor.first, itor.second, retMap);
            else
                break;
        }
    }
}

/*******************************************************************************
* 函数名称：
* 功能描述：将搜索结果，按column规则将数据写入strResone
* 输入参数：bAll 是否显示所有元素
*           vecColumn 输出字段
*           retMap    根据条件查询出来的nodes
*           strResone 返回数据
* 输出参数：
* 返 回 值：
* 
* {
    "code": 200,
    "data": [
        {
        "GeneralModelTemplate":[
            {
                "name": "temp.ive",
                "resourceId": "5990852a-172e-4262-9daf-c0fbd64b484f",
                "type": "自定义模型"
            },
            {
                "name": "消防员.ive",
                "resourceId": "ae117764-f1c6-4f34-af38-7e268eb118e3",
                "type": "数据驱动类"
            }
          ]
       }
    ],
    "message": " success"
}
* 
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/
void CTreeItem::ColumnPrint(std::vector<ColumnElement> vecColumn, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap, std::string& strRespone)
{
    std::map<int, std::vector<std::shared_ptr<Node>>> mapResult; //将结果按类型分类
   
    //遍历元素map
    for (auto itor = retMap.begin();itor!= retMap.end(); itor++)
    {
        if (!itor->first.compare(""))
            continue;

        //从结果中过滤未勾选的元素
        if (itor->second->checkedArray && isCheck(itor->second->checkedArray.get(), m_strCurSenceId))
        {
            if (itor->second->type) {
                auto itor2 = mapResult.find(*itor->second->type);
                if (itor2 != mapResult.end())
                {
                    itor2->second.push_back(itor->second);
                }
                else {
                    std::vector<std::shared_ptr<Node>> vecNode;
                    vecNode.push_back(itor->second);
                    mapResult[*itor->second->type] = vecNode;
                }
            }
        }
    }

    std::string strDescrib = "success";

    cJSON* pData = cJSON_CreateArray();

    if (mapResult.size() > 0)
    {
        auto itorMap = mapResult.begin();
        for (; itorMap != mapResult.end(); itorMap++) //按类型分组
        {
            std::string strTypeName = RTE::getModelStrTypeByType(itorMap->first);

            cJSON* pTypeJson = cJSON_CreateObject();
            cJSON_AddItemToArray(pData, pTypeJson);

            cJSON* pArray = cJSON_AddArrayToObject(pTypeJson, strTypeName.c_str());
            //添加item
            if (vecColumn.size() > 0) //过滤显示
            {
                auto itorVec = itorMap->second.begin();
                for (; itorVec != itorMap->second.end(); itorVec++)
                {
                    //step1 将node转成json, true为递归转换
                    //step2 新建一个json
                    //step3 将需要显示的字段拷贝到新json上
                    //step4 将新建json添加到pAarry中，然后删除node json
                    //将node转成json, true为递归复制
                    cJSON* nodeJson = parseTreeToJson(*itorVec, false,false);
                    cJSON* pNewJson = NULL;
                    if (nodeJson) {
                        pNewJson = cJSON_CreateObject();
                        auto itor = vecColumn.begin();
                        for (; itor!=vecColumn.end(); itor++)
                        {
                            if (!itor->bSecondLevel) {
                                cJSON* pItem = cJSON_GetObjectItem(nodeJson, itor->strKey.c_str());
                                if (pItem)
                                    cJSON_AddItemToObject(pNewJson, itor->strKey.c_str(), cJSON_Duplicate(pItem, 1));
                            }
                            else { //有二级过滤
                                //cJSON_AddItemToObject(pNewJson, itor->strKey.c_str(), cJSON_Duplicate(pItem, 1));
                                cJSON* pItem = cJSON_GetObjectItem(nodeJson, itor->strKey.c_str());
                                if (pItem && pItem->type!= cJSON_NULL)
                                {
                                    cJSON* pItem2 = NULL;
                                    if (itor->vecSecondLevelKey.size() > 0) {
                                        pItem2 = cJSON_CreateObject();
                                    }

                                    auto secNameItor = itor->vecSecondLevelKey.begin();                                    
                                    for (; secNameItor!=itor->vecSecondLevelKey.end(); secNameItor++)
                                    {
                                        cJSON* pSencondNode = cJSON_GetObjectItem(pItem, secNameItor->c_str());
                                        if (pSencondNode) {
                                            cJSON_AddItemToObject(pItem2, secNameItor->c_str(), cJSON_Duplicate(pSencondNode, 1));
                                        }
                                    }

                                    if(pItem2)
                                        cJSON_AddItemToObject(pNewJson, itor->strKey.c_str(), pItem2);
                                }
                            }
                        }
                    }
                    if (nodeJson && pNewJson) {
                        cJSON_AddItemToArray(pArray, pNewJson);
                        cJSON_Delete(nodeJson);
                    }
                }
                
            }
            else { //显示全部
                auto itorVec = itorMap->second.begin();
                for (; itorVec != itorMap->second.end(); itorVec++)
                {
                    //将node转成json, true为递归转换
                    cJSON* nodeJson = parseTreeToJson(*itorVec, false,false);
                    if(nodeJson)
                        cJSON_AddItemToArray(pArray, nodeJson);
                }
            }
        }
    }
    //cJSON_AddStringToObject(pRetJosn, "message", strDescrib.c_str());
    //char *pOut = cJSON_Print(pRetJosn);
    char* pOut = cJSON_Print(pData);
    strRespone = pOut;
#if 0
    //Poco::Logger::get("RteLogger").information("ColumnPrint+++");
    //Poco::Logger::get("RteLogger").information("%s", strRespone);
    //Poco::Logger::get("RteLogger").information("ColumnPrint+++");
#endif
    if (pOut)
    {
        free(pOut);
    }
    //cJSON_Delete(pRetJosn);
    cJSON_Delete(pData);
}

void CTreeItem::ColumnPrintVecotr(std::vector<ColumnElement> vecColumn, std::vector<std::shared_ptr<Node>>& retVector, std::string& strResone)
{
    std::map<int, std::vector<std::shared_ptr<Node>>> mapResult; //将结果按类型分类

    //遍历元素map
    //for (auto itor = retVector.begin(); itor != retVector.end(); itor++)
    for (int i = 0; i < retVector.size(); i++)
    {
        //从结果中过滤未勾选的元素
        if (retVector[i]->checkedArray && isCheck(retVector[i]->checkedArray.get(), m_strCurSenceId))
        {
            if (retVector[i]->type) {
                auto itor2 = mapResult.find(*retVector[i]->type);
                if (itor2 != mapResult.end())
                {
                    itor2->second.push_back(retVector[i]);
                }
                else {
                    std::vector<std::shared_ptr<Node>> vecNode;
                    vecNode.push_back(retVector[i]);
                    mapResult[*retVector[i]->type] = vecNode;
                }
            }
        }
    }

    std::string strDescrib = "success";

    cJSON* pData = cJSON_CreateArray();

    if (mapResult.size() > 0)
    {
        auto itorMap = mapResult.begin();
        for (; itorMap != mapResult.end(); itorMap++) //按类型分组
        {
            std::string strTypeName = RTE::getModelStrTypeByType(itorMap->first);

            cJSON* pTypeJson = cJSON_CreateObject();
            cJSON_AddItemToArray(pData, pTypeJson);

            cJSON* pArray = cJSON_AddArrayToObject(pTypeJson, strTypeName.c_str());
            //添加item
            if (vecColumn.size() > 0) //过滤显示
            {
                auto itorVec = itorMap->second.begin();
                for (; itorVec != itorMap->second.end(); itorVec++)
                {
                    //step1 将node转成json, true为递归转换
                    //step2 新建一个json
                    //step3 将需要显示的字段拷贝到新json上
                    //step4 将新建json添加到pAarry中，然后删除node json
                    //将node转成json, true为递归复制
                    cJSON* nodeJson = parseTreeToJson(*itorVec, false, false);
                    cJSON* pNewJson = NULL;
                    if (nodeJson) {
                        pNewJson = cJSON_CreateObject();
                        auto itor = vecColumn.begin();
                        for (; itor != vecColumn.end(); itor++)
                        {
                            if (!itor->bSecondLevel) {
                                cJSON* pItem = cJSON_GetObjectItem(nodeJson, itor->strKey.c_str());
                                if (pItem)
                                    cJSON_AddItemToObject(pNewJson, itor->strKey.c_str(), cJSON_Duplicate(pItem, 1));
                            }
                            else { //有二级过滤
                                //cJSON_AddItemToObject(pNewJson, itor->strKey.c_str(), cJSON_Duplicate(pItem, 1));
                                cJSON* pItem = cJSON_GetObjectItem(nodeJson, itor->strKey.c_str());
                                if (pItem && pItem->type != cJSON_NULL)
                                {
                                    cJSON* pItem2 = NULL;
                                    if (itor->vecSecondLevelKey.size() > 0) {
                                        pItem2 = cJSON_CreateObject();
                                    }

                                    auto secNameItor = itor->vecSecondLevelKey.begin();
                                    for (; secNameItor != itor->vecSecondLevelKey.end(); secNameItor++)
                                    {
                                        cJSON* pSencondNode = cJSON_GetObjectItem(pItem, secNameItor->c_str());
                                        if (pSencondNode) {
                                            cJSON_AddItemToObject(pItem2, secNameItor->c_str(), cJSON_Duplicate(pSencondNode, 1));
                                        }
                                    }

                                    if (pItem2)
                                        cJSON_AddItemToObject(pNewJson, itor->strKey.c_str(), pItem2);
                                }
                            }
                        }
                    }
                    if (nodeJson && pNewJson) {
                        cJSON_AddItemToArray(pArray, pNewJson);
                        cJSON_Delete(nodeJson);
                    }
                }

            }
            else { //显示全部
                auto itorVec = itorMap->second.begin();
                for (; itorVec != itorMap->second.end(); itorVec++)
                {
                    //将node转成json, true为递归转换
                    cJSON* nodeJson = parseTreeToJson(*itorVec, false, false);
                    if (nodeJson)
                        cJSON_AddItemToArray(pArray, nodeJson);
                }
            }
        }
    }
    //cJSON_AddStringToObject(pRetJosn, "message", strDescrib.c_str());
    //char *pOut = cJSON_Print(pRetJosn);
    char* pOut = cJSON_Print(pData);
    strResone = pOut;
    if (pOut)
    {
        free(pOut);
    }
    //cJSON_Delete(pRetJosn);
    cJSON_Delete(pData);
}


/*******************************************************************************
* 函数名称：
* 功能描述：供外部调用，根据请求json执行get操作
* 输入参数：request 请求json字符串，
*           strRespone 正常返回json字符串，失败返回错误描述
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

void CTreeItem::getElement(std::string request, std::string& strRespone)
{
    //Poco::Logger::get("RteLogger").debug("getElement requset body %s", request);
     //step1 解析过滤条件
    std::unordered_map<std::string, std::pair<bool, std::vector<std::string>>> mapFCondition;
    std::unordered_map<std::string, std::pair<int, std::vector<std::string>>> mapCondition;
    std::vector<ColumnElement> vecColumn;

    //Poco::Stopwatch watch;
    //watch.start();
    bool isAll = true;  //是否搜索全部
    std::unordered_map<std::string, std::shared_ptr<Node>> retMap; //查询返回数据
    std::vector<std::shared_ptr<Node>> retVector; //有些结果数据是按顺序排序的

    do
    {
        if (conditionPaser(request, "get", mapFCondition, mapCondition, vecColumn))
        {
            //step1 首先遍历后置条件
            if (mapCondition.size() > 0) {
                //遍历后置条件
                ergodicCondition(isAll, mapCondition, retMap);
                isAll = false;
            }
            //std::cout << "Condition time " << watch.elapsed() << std::endl;

            //step2 然后遍历前置条件，因为前置条件中的 area 是有顺序的,所有最后处理有顺序的查询条件
            if (mapFCondition.size() > 0) {
                //step2 前置条件在前一个条件过滤后循环遍历，遍历顺序guid、foreignKey、tags、type、view、!area

                //遍历guid,过滤
                auto itor = mapFCondition.find("guid");
                if(itor != mapFCondition.end())
                    if (itor->second.second.size() > 0){
                        ergodicGuid(itor->second.first, isAll, itor->second.second, retMap);
                        isAll = false;
                        if (retMap.size() == 0)break;
                    }

                //遍历parentId
                itor = mapFCondition.find("parentID");
                if (itor != mapFCondition.end())
                    if (itor->second.second.size() > 0) {
                        ergodicParentId(itor->second.first, isAll, itor->second.second, retMap);
                        isAll = false;
                        if (retMap.size() == 0)break;
                    }

                //std::cout << "guid time " << watch.elapsed() << std::endl;
                //遍历foreignkey,过滤
                itor = mapFCondition.find("foreignKey");

                if (itor != mapFCondition.end())
                    //if (itor->second.second.size() > 0)
                    {
                        ergodicForeignKey(itor->second.first, isAll, itor->second.second, retMap);
                        isAll = false;
                        if (retMap.size() == 0)break;
                    }

                //std::cout << "foreignKey time " << watch.elapsed() << std::endl;
                
                //遍历tags,过滤
                itor = mapFCondition.find("tagList");

                if (itor != mapFCondition.end())
                    if (itor->second.second.size() > 0) {
                        ergodicTags(itor->second.first, isAll, itor->second.second, retMap);
                        isAll = false;
                        if (retMap.size() == 0)break;
                    }
                    else {
                        std::vector<std::string> vecFind;
                        vecFind.push_back("[]");
                        ergodicTags(itor->second.first, isAll, vecFind, retMap);
                        isAll = false;
                        if (retMap.size() == 0)break;
                    }

                //std::cout << "tagList time " << watch.elapsed() << std::endl;
                //遍历type,过滤
                itor = mapFCondition.find("type");

                if (itor != mapFCondition.end())
                    if (itor->second.second.size() > 0) {
                        ergodicType(itor->second.first, isAll, itor->second.second, retMap);
                        isAll = false;
                        if (retMap.size() == 0)break;
                    }

                //std::cout << "type time " << watch.elapsed() << std::endl;

                //遍历业务属性
                itor = mapFCondition.find("attributeList");
                if (itor != mapFCondition.end())
                    if (itor->second.second.size() > 0) {
                        ergodicAttributeList(itor->second.first, isAll, itor->second.second, retMap);
                        isAll = false;
                        if (retMap.size() == 0)break;
                    }

                //std::cout << "attributeList time " << watch.elapsed() << std::endl;

                //遍历view  是否按顺序？
                itor = mapFCondition.find("view");

                if (itor != mapFCondition.end())
                    if (itor->second.second.size() > 0) {
                        ergodicViewBy3D(itor->second.first, isAll, itor->second.second, retMap);
                        isAll = false;
                        if (retMap.size() == 0)break;
                    }

                //遍历area, area 是有顺序的
                itor = mapFCondition.find("area");
                if (itor != mapFCondition.end())
                    if (itor->second.second.size() > 0) {
                        ergodicAreaBy3D(itor->second.first, isAll, itor->second.second, retMap, retVector);
                        isAll = false;
                        if (retMap.size() == 0)break;
                    }
            }
        }
        else {
            std::string strDescrib = "invalid data";
            cJSON* pRetJosn = cJSON_CreateObject();
            //cJSON_AddNumberToObject(pRetJosn, "code", 500);
            //cJSON_AddStringToObject(pRetJosn, "message", strDescrib.c_str());
            strRespone = cJSON_Print(pRetJosn);
            cJSON_Delete(pRetJosn);
            return;
        }
    } while (0);

    //输出限制
    if (isAll) {
        ColumnPrint(vecColumn, m_mapNode, strRespone); //从所有元素map中输出
    }
    else {
        if(retVector.size()>0)
            ColumnPrintVecotr(vecColumn, retVector, strRespone);//从过滤条件中输出
        else {
            ColumnPrint(vecColumn, retMap, strRespone);//从过滤条件中输出
        }
    }

    //std::cout << "ColumnPrint time " << watch.elapsed() << std::endl;

    return;
}


/*******************************************************************************
* 函数名称：
* 功能描述：根据json请求添加元素
*           
* 输入参数：request 请求json
*           {
	"method": "add",
	"Element": {
		"type": 29,
		"data": [{
			"id": "",//可以为空，为空rte内部创建， 添加不支持批量添加
			"parentID": "",//可以为空，为空放到root group下
			"resourceId": "15c5ca81-5dc5-42b9-a5b1-681e49a0b524",//必填
			"elementData": { //对应3d 中的data字段
				"_worldMapPos":	{ //对应之前的position
			        "w":	1,
			        "x":	116.38941706515035,
			        "y":	39.9062634690774,
			        "z":	0
		        }
			},
			"tags": [
				"api添加poi"
			],
            "name": "新增POI",
			"foreignKey": []
		}]
	}
}
            strRespone： 返回json数据
* 输出参数：
* 返 回 值：成功返回 0，失败返回非 0
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/
int CTreeItem::addElement(std::string request, std::string& strRespone)
{
    //Poco::Logger::get("RteLogger").debug("addElement requset body %s", request);
    cJSON* pRequest = cJSON_Parse(request.c_str());
    strRespone = "Invalid parameter";
    if (!pRequest) {
        return -1;
    }

    //判断前置条件是否存在
    //step1 
    cJSON* type = cJSON_GetObjectItem(pRequest, "type");
    cJSON* data = cJSON_GetObjectItem(pRequest, "data");
    
    if (!type || !data)
    {
        //Poco::Logger::get("RteLogger").error("type or data is null");
        cJSON_Delete(pRequest);
        return -1;
    }

    std::string strId, strParentId;
    int nType = -1;
    nType = type->valueint;

    int nRet = 0;
    if (!cJSON_IsArray(data)) {
        return -1;
    }

    int nDataSize = cJSON_GetArraySize(data);
    std::string strRet;

    bool bError = false;
    bool bOk = false;
    cJSON* retJson = cJSON_CreateArray();
    for (size_t i = 0; i < nDataSize; i++)
    {
        //这里需要将添加数据缓存到map中，等待3d响应后在将数据存储到node map中
        cJSON* pItem = cJSON_GetArrayItem(data, i);
        std::string strId;
        nRet = addElementImpl(nType,pItem,strRet,strId);
        //m_mapAddElement.emplace(strId, std::pair<int, cJSON*>(nType, cJSON_Duplicate(pItem, true)));
        if (nRet)
            bError = true;
        if (!nRet)
            bOk = true;
        cJSON* pItemRet = cJSON_CreateObject();
        cJSON_AddNumberToObject(pItemRet, "code", nRet);
        cJSON_AddStringToObject(pItemRet, "id", strId.c_str());
        cJSON_AddStringToObject(pItemRet, "message", strRet.c_str());

        cJSON_AddItemToArray(retJson, pItemRet);
    }
    char* pOutRet = cJSON_Print(retJson);
    strRespone = pOutRet;
    free(pOutRet);
    cJSON_Delete(retJson);
    cJSON_Delete(pRequest);

    if (bError && !bOk)
        return 1;
    else if (!bError && bOk)
        return 0;
    else
        return 2;
}

/*******************************************************************************
* 函数名称：
* 功能描述：生成通用模型json
* 输入参数：json指针，调用者管理，函数内部不可释放。
* 输出参数：
* 返 回 值：json 指针数据 调用者释放
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

cJSON* CTreeItem::genModelJson(cJSON* pElement, cJSON * pTemplate)
{
    if (!pElement)
        return nullptr;

    cJSON* pItem = cJSON_Duplicate(pElement, true);
    if (pItem) {
        char tempBuf[256] = { 0 };

        std::string strResourceIp;
        unsigned int nPort;
        GetConfigSettingInstance().GetResouceInfo(strResourceIp, nPort);

        sprintf(tempBuf, "http://%s:%d", strResourceIp.c_str(), nPort);
        if (pTemplate) {
            cJSON* ivePath = cJSON_GetObjectItem(pTemplate, "ivePath");
            std::string strIve = tempBuf;
            if (ivePath && ivePath->valuestring)
                strIve += ivePath->valuestring;
            cJSON_AddStringToObject(pItem, "_modelUrl", strIve.c_str());
        }
    }
    return pItem;
}


cJSON* CTreeItem::genEffectJson(cJSON* pElement, cJSON* pTemplate, std::string resouceId, int elementType)
{
    if (!pElement)
        return nullptr;

    cJSON* pItem = cJSON_Duplicate(pElement, true);
    if (pItem) {
        std::string strResourceIp;
        unsigned int nPort = 0;
        GetConfigSettingInstance().GetResouceInfo(strResourceIp, nPort);
        char tempBuf[256] = { 0 };
        sprintf(tempBuf, "http://%s:%d", strResourceIp.c_str(), nPort);
        if (pTemplate) {
            cJSON* ivePath = cJSON_GetObjectItem(pTemplate, "iconPath");
            if(ivePath==nullptr)
                ivePath = cJSON_GetObjectItem(pTemplate, "ivePath");

            std::string strIve = tempBuf;
            if (ivePath && ivePath->valuestring)
                strIve += ivePath->valuestring;
            cJSON_AddStringToObject(pItem, "_modelUrl", strIve.c_str());
            if(elementType == SDT_SceneEffects)
                cJSON_AddStringToObject(pItem, "_effectTemplateID", resouceId.c_str());
        } 
    }
    return pItem;
}

cJSON* CTreeItem::genBlend2DDataJson(cJSON* pElement, cJSON* pTemplate, std::string resouceId)
{
    if (!pElement)
        return nullptr;

    cJSON* pItem = cJSON_Duplicate(pElement, true);

    if (pItem) {
        if (pTemplate) {
            cJSON_AddStringToObject(pItem, "_effectTemplateID", resouceId.c_str());

            cJSON* scaleFactor = cJSON_GetObjectItem(pTemplate, "scaleFactor");
            if (scaleFactor)
                cJSON_AddNumberToObject(pItem, "scaleFactor", scaleFactor->valueint);

            cJSON* viewRangeMax = cJSON_GetObjectItem(pTemplate, "viewRangeMax");
            if(viewRangeMax)
                cJSON_AddNumberToObject(pItem, "viewRangeMax", viewRangeMax->valueint);

            cJSON* viewRangeMin = cJSON_GetObjectItem(pTemplate, "viewRangeMin");
            if (viewRangeMin)
                cJSON_AddNumberToObject(pItem, "viewRangeMin", viewRangeMin->valueint);

            cJSON* path = cJSON_GetObjectItem(pTemplate, "path");
            if (path && path->valuestring)
                cJSON_AddStringToObject(pItem, "path", path->valuestring);

            cJSON* width = cJSON_GetObjectItem(pTemplate, "width");
            if (width)
                cJSON_AddNumberToObject(pItem, "width", width->valueint);

            cJSON* height = cJSON_GetObjectItem(pTemplate, "height");
            if (height)
                cJSON_AddNumberToObject(pItem, "height", height->valueint);

            cJSON* opacity = cJSON_GetObjectItem(pTemplate, "opacity");
            if (opacity)
                cJSON_AddNumberToObject(pItem, "opacity", opacity->valueint);

            cJSON* children = cJSON_GetObjectItem(pTemplate, "children");
            if (children)
                cJSON_AddItemToObject(pItem, "children", cJSON_Duplicate(children, true));
        }
    }
    return pItem;
}
/*
* 
pElement
    _poiLightOffStyle
    _poiLightOnStyle
*/
cJSON* CTreeItem::genPoiJson(cJSON* pElement, cJSON* pTemplate)
{
    cJSON* pItem = cJSON_Duplicate(pElement, true);
    if (pItem) {
        if (pTemplate) {
            //遍历模板字段拷贝到item中
            for (cJSON* pItem2 = pTemplate->child; pItem2 != nullptr;)
            {
                //如果elementdata存在不需替换
                if (pItem2->string) {
                    cJSON* pDetect = cJSON_GetObjectItem(pItem,pItem2->string);
                    if(pDetect == nullptr)
                        cJSON_AddItemToObject(pItem, pItem2->string, cJSON_Duplicate(pItem2, true));

                    //代码逻辑，添加的elementdata中有_poiLightOffStyle， _poiLightOnStyle，这两个字段
                    //和模板中的_poiLightOffStyle_t _poiLightOnStyle_t,取并集，已有的不需更换
                    if (!strcmp(pItem2->string, "_poiLightOffStyle_t") || !strcmp(pItem2->string, "_poiLightOnStyle_t"))
                    {
                        //cJSON_Duplicate
                        std::string strKey = pItem2->string;
                        strKey = strKey.substr(0, strKey.size() - 2);
                        cJSON* p1 = cJSON_GetObjectItem(pItem,strKey.c_str());
                        if (p1 == nullptr) {
                            cJSON_AddItemToObject(pItem, strKey.c_str(), cJSON_Duplicate(pItem2, true));
                        }
                        else {
                            for (cJSON* pKey = pItem2->child; pKey != nullptr;) // 遍历模板
                            {
                                cJSON* p2 = cJSON_GetObjectItem(p1, pKey->string);
                                if (p2 == nullptr) {
                                    cJSON_AddItemToObject(p1, pKey->string, cJSON_Duplicate(pKey, true));
                                }

                                pKey = pKey->next;
                            }
                        }
                    }
                }

                pItem2 = pItem2->next;
            }
        }
    }
    return pItem;
}


cJSON* CTreeItem::genPathBrowsing(cJSON* pElement, cJSON* pTemplate)
{
    if (!pElement)
        return nullptr;

    cJSON* pItem = cJSON_Duplicate(pElement, true);
    if (pItem) {
        std::string strResourceIp;
        unsigned int nPort;
        GetConfigSettingInstance().GetResouceInfo(strResourceIp, nPort);
        char tempBuf[256] = { 0 };
        sprintf(tempBuf, "http://%s:%d", strResourceIp.c_str(), nPort);
        if (pTemplate) {
            //cJSON* ivePath = cJSON_GetObjectItem(pTemplate, "ivePath");
            std::string path;
            cJSON_LoadFromObject(pTemplate, "path", path);
            std::string _textureImageUrl = tempBuf;
            _textureImageUrl += path;
            cJSON_AddStringToObject(pItem, "_textureImageUrl", _textureImageUrl.c_str());
        }
    }
    return pItem;
}

/*
* 
{
	"method": "add",
	"Element": {
		"type": 29,
		"data": [{
			"id": "",//可以为空，为空rte内部创建， 添加不支持批量添加
			"parentID": "",//可以为空，为空放到root group下
			"resourceId": "15c5ca81-5dc5-42b9-a5b1-681e49a0b524",//必填
			"elementData": { //对应3d 中的data字段, 自定义，闪烁，与不闪烁字段
				"_polygonColor": {
					"w": 0.5,
					"x": 0.0,
					"y": 1.0,
					"z": 0.0
				}
			},
			"position": {
				"height": 15.80159896,
				"latitude": 39.90968387,
				"longitude": 116.39709311
			},
			"tags": [
				"api添加poi"
			],
            "name": "新增POI",
			"foreignKey": []
		}]
	}
}
*/
int CTreeItem::addElementImpl(int nType, cJSON* pElement, std::string& strRespone, std::string& strId)
{
    //pElement 外部传入不需删除
    if (!pElement) {
        strRespone = "element data is null";
        return 1;
    }

    std::string strType = RTE::getModelStrTypeByType(nType);

    if (!strType.compare("")) {
        strRespone = "type is invalude";
        return -1;
    }

    cJSON* id;          //
    cJSON* parentID;    //
    cJSON* resourceId;  //必填, 对应的时模板中的 guid 或者元素data中的 poiStyleID
    cJSON* elementData; // elementData中的_worldMapPos 必填,对应老接口的position

    cJSON* name;

    cJSON* foreignKey = nullptr;
    foreignKey = cJSON_GetObjectItem(pElement, "foreignKey");
    if (foreignKey && foreignKey->child && foreignKey->child->valuestring)
    {
        if (strcmp(foreignKey->child->valuestring, "")) {
            std::map<std::string, std::shared_ptr<Node>>::iterator itorFind = m_mapForeignKeyNode.find(foreignKey->child->valuestring);
            if (itorFind != m_mapForeignKeyNode.end()) {
                strRespone = "foreignKey already exists";
                return 1;
            }
        }
    }

    // 根据elementData添加自定义选项.
    elementData = cJSON_GetObjectItem(pElement, "elementData");

    if(!elementData) {
        //strRespone = "type is invalude"; 有些元素可以为空
        //return -1;
    }

    resourceId = cJSON_GetObjectItem(pElement, "resourceId");

    name = cJSON_GetObjectItem(pElement, "name");

    std::string strParentId, strResourceId;

    id = cJSON_GetObjectItem(pElement, "id"); //适配id和guid
    if(!id)
        id = cJSON_GetObjectItem(pElement, "guid");

    if (id && id->valuestring)
        strId = id->valuestring;

    if (!strId.compare(""))
    {
        //创建id
        strId = GUIDStr();
    }

    parentID = cJSON_GetObjectItem(pElement, "parentID");
    if (parentID && parentID->valuestring) {
        strParentId = parentID->valuestring;

        if (!strParentId.compare("")) {
            if (m_pElementNode->childArray.size() > 0) {
                strParentId = m_strGroupId;// m_pElementNode->childArray[0]->guid.get();//root node id
            }
        }
        cJSON_SetValuestring(parentID, strParentId.c_str());
    }
    else {
        if (m_pElementNode->childArray.size() > 0) {
            strParentId = m_strGroupId;// m_pElementNode->childArray[0]->guid.get();//root node id
            cJSON_AddStringToObject(pElement, "parentID", strParentId.c_str());
        }
    }

    if (resourceId && resourceId->valuestring)
        strResourceId = resourceId->valuestring;

    //根据resourceid 获取模型模板
    cJSON* pTemplate = NULL;
    switch (nType)
    {
    case SDT_VirtualCamera: //虚拟相机和特效用的是一个模板
    case SDT_SceneEffects:
        pTemplate = GetGlobalSettingInstance().getByResourceID(SDT_SceneEffects,strResourceId);
        break;
    case SDT_ModelBase:
        pTemplate = GetGlobalSettingInstance().getByResourceID(SDT_ModelBase, strResourceId);
        break;
    case SDT_Model:
        pTemplate = GetGlobalSettingInstance().getByResourceID(SDT_Model,strResourceId);
        break;
    case SDT_AreaContour:
        pTemplate = GetGlobalSettingInstance().getByResourceID(SDT_AreaContour, strResourceId);
        break;
    case SDT_Radar:
        break;
    case SDT_VehicleSimulation: //车辆仿真
        pTemplate = GetGlobalSettingInstance().getByResourceID(SDT_VehicleSimulation,strResourceId);
        break;
    case SDT_POI: {
        pTemplate = GetGlobalSettingInstance().getByResourceID(SDT_POI, strResourceId);
        cJSON* poiStyleId = GetGlobalSettingInstance().getByName(SDT_POI, strResourceId, "poiStyleId");
        if (pTemplate && poiStyleId)
        {
            cJSON_AddItemToObject(pTemplate, "_poiStyleID", poiStyleId);
        }
        break; 
    }
    case SDT_PathBrowsing:
        pTemplate = GetGlobalSettingInstance().getByResourceID(SDT_PathBrowsing, strResourceId);
        break;
    case SDT_DataPannel: //数据面片，新版本支持模板
        pTemplate = GetGlobalSettingInstance().getByResourceID(SDT_DataPannel, strResourceId);
        break;
    default:
        break;
    }

    //组装3d json
    bool bBlend2DData = false; //释放是模板数据面片
    if (nType == 5) {
        if (pTemplate)
        {
            bBlend2DData = true;
            strType = "Blend2DData";
        }
    }
    cJSON* p3dRequest = cJSON_CreateObject();
    std::string str3dRequest;
    cJSON_AddStringToObject(p3dRequest, "id", "");
    cJSON_AddStringToObject(p3dRequest, "element", strType.c_str());
    cJSON_AddStringToObject(p3dRequest, "method", "add");

    switch (nType)
    {
    case SDT_DataPannel: 
    case SDT_SceneEffects: 
    {
        if (nType == SDT_SceneEffects || nType == SDT_DataPannel)
        {
            cJSON* pAttr = GetGlobalSettingInstance().getByName((SceneDataType)nType, strResourceId, "attributeList");
            if (pAttr)
            {
                cJSON_AddItemToObject(pElement, "attributeList_templte", pAttr); //添加特殊字段，如果attributeList为空，先添加模板
            }
        }

        if (bBlend2DData)
        {
            cJSON* pItem = genBlend2DDataJson(elementData, pTemplate, strResourceId);
            if (!pItem)
                pItem = cJSON_CreateObject();

            cJSON_AddStringToObject(pItem, "_id", strId.c_str()); //将id 传入元素data参数，外部参数无效
            cJSON_AddItemToObject(p3dRequest, "data", pItem);
            char* pOut = cJSON_Print(p3dRequest);
            str3dRequest = pOut;
            free(pOut);
        }
        else {
            cJSON* pItem = genEffectJson(elementData, pTemplate, strResourceId, nType);
            if (!pItem)
                pItem = cJSON_CreateObject();
            cJSON_AddStringToObject(pItem, "_id", strId.c_str()); //将id 传入元素data参数，外部参数无效
            cJSON_AddItemToObject(p3dRequest, "data", pItem);
            char* pOut = cJSON_Print(p3dRequest);
            str3dRequest = pOut;
            free(pOut);
        }
    }
        break;
    case SDT_HighlightArea:
    case SDT_HeatMap:
    case SDT_RiverSystem:
    case SDT_VehicleSimulation:
    case SDT_VirtualCamera:
    case SDT_ViewPath:
    case SDT_ModelBase:
    case SDT_SceneRoame:
    case SDT_Text:
    case SDT_Model: {
        cJSON* pItem = genModelJson(elementData, pTemplate);
        if (!pItem)
            pItem = cJSON_CreateObject();
        cJSON_AddStringToObject(pItem, "_id", strId.c_str()); //将id 传入元素data参数，外部参数无效
        cJSON_AddItemToObject(p3dRequest, "data", pItem);
        char* pOut = cJSON_Print(p3dRequest);
        str3dRequest = pOut;
        free(pOut);
    }
        break;
    case SDT_POI: {
        cJSON* pAttr = GetGlobalSettingInstance().getByName(SDT_POI, strResourceId, "attributeList");
        if (pAttr)
        {
            cJSON_AddItemToObject(pElement, "attributeList_templte", pAttr); //添加特殊字段，如果attributeList为空，先添加模板
        }

        cJSON* pItem = genPoiJson(elementData, pTemplate);
        cJSON_AddStringToObject(pItem, "_id", strId.c_str()); //将id 传入元素data参数，外部参数无效

        cJSON_AddItemToObject(p3dRequest, "data", pItem);
        char* pOut = cJSON_Print(p3dRequest);
        if (pOut) {
            str3dRequest = pOut;
            free(pOut);
        }
    }
        break;
    case SDT_AreaContour:  //SDT_AreaContour，SDT_PathBrowsing都是根据resourceid 获取_textureImageUrl
    case SDT_PathBrowsing: {
        cJSON* pItem = genPathBrowsing(elementData, pTemplate);
        if (!pItem)
            pItem = cJSON_CreateObject();
        cJSON_AddStringToObject(pItem, "_id", strId.c_str()); //将id 传入元素data参数，外部参数无效
        cJSON_AddItemToObject(p3dRequest, "data", pItem);
        char* pOut = cJSON_Print(p3dRequest);
        str3dRequest = pOut;
        free(pOut);
    }
        break;
    case SDT_Radar:
    default: {
        cJSON * pItem = cJSON_Duplicate(elementData, true);/*cJSON_CreateObject();*/
        cJSON_AddStringToObject(pItem, "_id", strId.c_str()); //将id 传入元素data参数，外部参数无效
        cJSON_AddItemToObject(p3dRequest, "data", pItem);
        char* pOut = cJSON_Print(p3dRequest);
        str3dRequest = pOut;
        free(pOut);
    }
        break;
    }

    std::string strRet;
    ////Poco::Logger::get("RteLogger").debug("request add ret begin");
    int nRet = Interface::elementHandle(str3dRequest, strRet);
    ////Poco::Logger::get("RteLogger").debug("request add ret %d", nRet);
    m_mapAddElement.emplace(strId, std::pair<int, cJSON*>(nType, cJSON_Duplicate(pElement, true)));
    ////Poco::Logger::get("RteLogger").debug("request %s", str3dRequest);
    
    if (!nRet)
        strRespone = "create success!";
    else
        strRespone = "create failed!";

    if(pTemplate)
        cJSON_Delete(pTemplate);

    if(p3dRequest)
        cJSON_Delete(p3dRequest);
    return nRet;
}

/*
* 
{
    "id": ["e0498830-0870-41bf-abbe-6ca7a52b1051", "e0498830-0870-41bf-abbe-6ca7a52b1050"],
    "type": 29,
    "elementData": {
        "_polygonColor": {
            "w": 0.5,
            "x": 0.0,
            "y": 1.0,
            "z": 0.0
        }
    },
    "foreignKey": []
}
id、type 前置条件必填，其他为修改参数
*/
/*******************************************************************************
* 函数名称：
* 功能描述：根据类型批量更新元素数据
* 输入参数：
*           strMessage 传入传出错误消息
* 输出参数：
* 返 回 值：strRespone 成功返回json， 失败返回错误信息（错误信息非json）
*           //int：0 - (全部)成功；1 - (全部)失败；2 - (部分)成功，
            可在resultJson具体查看；3 - jsValue缺少必要值；4 - 该元素不存在；5 - 操作对象不存在；
            -1 - 请求数据格式错误
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/
int CTreeItem::updateElement(std::string request, std::string& strRespone, /*int& nCode,*/std::string &strMessage)
{
    //Poco::Logger::get("RteLogger").debug("updateElement requset body %s", request);
    //Poco::Stopwatch watch;
    //watch.start();

    cJSON* pRequest = cJSON_Parse(request.c_str());
    strRespone = "Invalid parameter";
    if (!pRequest) {
        return -1;
    }

    //step1 
    cJSON* pTypeJson = cJSON_GetObjectItem(pRequest, "type");
    cJSON* pIds = cJSON_GetObjectItem(pRequest, "id");
    cJSON* pElementData = cJSON_GetObjectItem(pRequest, "elementData");

    std::vector<cJSON*> updateParam; //需要更新的主节点中的字段。
    if (!pTypeJson || !pIds /*|| !pElementData*/) //pElementData 可以为空
    {
        //Poco::Logger::get("RteLogger").error("id || type || elementData is null");
        cJSON_Delete(pRequest);
        return -1;
    }

    std::vector<std::string> vecLockIds;

    cJSON* foreignKey = nullptr;
    foreignKey = cJSON_GetObjectItem(pRequest, "foreignKey");
    bool bUpdateForeignKey = true;
    if (foreignKey && foreignKey->child && foreignKey->child->valuestring)
    {
        if (strcmp(foreignKey->child->valuestring, "")) {
            std::map<std::string, std::shared_ptr<Node>>::iterator itorFind = m_mapForeignKeyNode.find(foreignKey->child->valuestring);
            if (itorFind != m_mapForeignKeyNode.end()) {
                if (cJSON_IsArray(pIds))
                {
                    if (strcmp(pIds->child->valuestring, itorFind->second->guid.get()))
                    {
                        strRespone = "foreignKey already exists";
                        return 1;
                    }
                    else {
                        bUpdateForeignKey = false;
                    }
                }
                else {
                    std::string strId;
                    cJSON_LoadFromObject(pRequest, "id", strId);
                    if (strcmp(strId.c_str(), itorFind->second->guid.get()))
                    {
                        strRespone = "foreignKey already exists";
                        return 1;
                    }
                    else {
                        bUpdateForeignKey = false;
                    }
                }
            }
        }
    }

#if 0  //目前版本不处理锁定状态，有3d引擎去处理。
    //判断是否为锁定状态，锁定状态不处理。
    if (cJSON_IsArray(pIds))
    {
        //判断id是否lock ,过滤锁定的id
        int nSize = cJSON_GetArraySize(pIds);
        for (size_t i = 0; i < nSize; i++)
        {
            cJSON* pItem = cJSON_GetArrayItem(pIds, i);
            if (pItem && pItem->valuestring) {
                std::string guid = pItem->valuestring;
                auto itorNode = m_mapNode.find(guid);
                if (itorNode != m_mapNode.end()) {
                    if (isCheck(itorNode->second->isLockArray.get(), m_strCurSenceId)) //锁定的不能修改,
                    {
                        vecLockIds.push_back(guid);
                        cJSON_DeleteItemFromArray(pIds, i);
                        nSize--;
                        i--;
                    }
                }
            }
        }
    }
    else {
        std::string strId;
        cJSON_LoadFromObject(pRequest, "id", strId);

        //锁定的不能update
        auto itorNode = m_mapNode.find(strId);
        if (itorNode != m_mapNode.end()) {
            if (isCheck(itorNode->second->isLockArray.get(), m_strCurSenceId))
            {
                strRespone = "id islocked";
                cJSON_Delete(pRequest);
                return -1;
            }
        }
    }
#endif

    for (cJSON *pItem = pRequest->child; pItem != NULL;)
    {
        if (!strcmp(pItem->string, "type") || !strcmp(pItem->string, "id") || !strcmp(pItem->string, "elementData")) { //elementData 使用3d修改成功后，查询出来的elementdata，然后更新到node
            pItem = pItem->next;
            continue;
        }
        if (!strcmp(pItem->string, "foreignKey")) //外键过滤未更改的，外键修改相对复杂点。
        {
            if(bUpdateForeignKey)
                updateParam.push_back(pItem);
        }
        else {
            updateParam.push_back(pItem);
        }
        pItem = pItem->next;
    }

    int nType = -1;
    std::string strType;
    nType = pTypeJson->valueint;
    strType = RTE::getModelStrTypeByType(nType);

    //修改模板数据面片的elementData里内容，elementData中必须传入_modelType: 50
    if (nType == 5 && pElementData) {
        cJSON* _modelType = cJSON_GetObjectItem(pElementData, "_modelType");
        if (_modelType)
        {
            if (_modelType->valueint == MODELTYPE_Blend2DData) //新版数据面片枚举值
            {
                strType = "Blend2DData";
            }
        }
    }

    //std::cout << "update 3d use time " << watch.elapsed() << std::endl;

    if (!strType.compare("")) {
        cJSON_Delete(pRequest);
        return -1;
    }
    int nRet = 0;
    //std::vector<std::string> vecUpdate; //只更新3d成功的
    if (pElementData) {
        //创建3d请求json
        cJSON* p3DJson = cJSON_CreateObject();
        cJSON_AddItemToObject(p3DJson, "id", cJSON_Duplicate(pIds, true));
        cJSON_AddStringToObject(p3DJson, "element", strType.c_str());
        cJSON_AddStringToObject(p3DJson, "method", "update");

        cJSON_AddItemToObject(p3DJson, "data", cJSON_Duplicate(pElementData, true));

        std::string strResult;
        std::string request3d;
        char* pOut = cJSON_Print(p3DJson);
        if (!pOut) {
            cJSON_Delete(pRequest);
            cJSON_Delete(p3DJson);
            return -1;
        }
        request3d = pOut;
        free(pOut);

        //3d 返回 int：0 - (全部)成功；1 - (全部)失败；2 - (部分)成功，可在resultJson具体查看；3 - jsValue缺少必要值；4 - 该元素不存在；5 - 操作对象不存在； 6 - 元素被锁定
        //node 返回    10 操作对象不存在
        nRet = Interface::elementHandle(request3d, strResult);
        //Poco::Logger::get("RteLogger").debug("update 3d use time %d, ret:%d", (int)watch.elapsed(),nRet);
        strRespone = strResult;
        cJSON* pResultJson = cJSON_Parse(strResult.c_str());
        cJSON* pRetJosn = nullptr;
        if (pResultJson) {
            if (nRet == 0 || nRet == 2) // 有成功的需要更新node
            {
                cJSON* pResultJsonSub = cJSON_GetObjectItem(pResultJson, "result");
                if (pResultJsonSub && cJSON_IsArray(pResultJsonSub))
                {
                    pRetJosn = cJSON_Duplicate(pResultJsonSub,true);
                    
                    int nCount = cJSON_GetArraySize(pResultJsonSub);
                    for (int i = 0; i < nCount; i++)
                    {
                        std::vector<cJSON*> updateParamTemp;  //返回的elementdata不一定都一样所以特殊处理
                        updateParamTemp.assign(updateParam.begin(), updateParam.end());

                        cJSON* pItem = cJSON_GetArrayItem(pResultJsonSub, i);
                        if (pItem) {
                            cJSON* pId = cJSON_GetObjectItem(pItem, "id");
                            cJSON* pCode = cJSON_GetObjectItem(pItem, "code");
                            if (pCode && pId && pId->valuestring) {
                                int nCode = pCode->valueint;
                                if (nCode == 0) {
                                    //更新node
                                    //获取更新后的element data
                                    cJSON* pUpdate = cJSON_CreateObject();
                                    cJSON* pElementChange = getElementDataFrom3D(pId->valuestring, strType);

                                    if (pElementChange) {
                                        cJSON_AddItemToObject(pUpdate, "elementData", pElementChange);
                                        if (pUpdate->child)
                                            updateParamTemp.push_back(pUpdate->child);

                                        int re = updateElementNode(updateParamTemp, pId->valuestring); //更新node数据
                                        if (re) {
                                            cJSON_SetIntValue(pCode, re);
                                        }
                                    }
                                    if (pUpdate)
                                        cJSON_Delete(pUpdate);
                                }
                            }
                        }
                    }
                }
                else { //目前的pac调用都是以数组方式调用，不会调用到else
                    cJSON* pId = cJSON_GetObjectItem(pResultJson, "id");
                    cJSON* pCode = cJSON_GetObjectItem(pResultJson, "code");
                    if (pCode && pId && pId->valuestring) {
                        int nCode = pCode->valueint;
                        if (nCode == 0) {
                            //更新node
                            //获取更新后的element data

                            cJSON* pUpdate = cJSON_CreateObject();
                            cJSON* pElementChange = getElementDataFrom3D(pId->valuestring, strType);

                            if (pElementChange) {
                                cJSON_AddItemToObject(pUpdate, "elementData", pElementChange);
                                if (pUpdate->child)
                                    updateParam.push_back(pUpdate->child);
                                int re = updateElementNode(updateParam, pId->valuestring); //更新node数据
                                if (re) {
                                    cJSON_SetIntValue(pCode, re);
                                }
                            }
                            if (pUpdate)
                                cJSON_Delete(pUpdate);
                        }
                    }
                }
            }
            cJSON_Delete(pResultJson);
        }


        if (!pRetJosn)
        {
            pRetJosn = cJSON_CreateArray();
        }

        if (pRetJosn && cJSON_IsArray(pRetJosn))
        {
            //如果检测到有锁定的id，将锁定数据返回给客户。
            if (vecLockIds.size() > 0)
            {
                for (size_t i = 0; i < vecLockIds.size(); i++)
                {
                    cJSON* lockItem = cJSON_CreateObject();
                    cJSON_AddNumberToObject(lockItem, "code", 6);
                    cJSON_AddStringToObject(lockItem, "id", vecLockIds[i].c_str());
                    cJSON_AddStringToObject(lockItem, "message", "element locked!");
                    cJSON_AddItemToArray(pRetJosn, lockItem);
                }
            }
            char* pOutResult = cJSON_Print(pRetJosn);
            if (pOutResult) {
                strRespone = pOutResult;
                free(pOutResult);
            }
        }
        cJSON_Delete(pRetJosn); 
        cJSON_Delete(pRequest);
        cJSON_Delete(p3DJson);
    }
    else {
        cJSON* pResult = cJSON_CreateArray();

        //先添加锁定的数据
        if (vecLockIds.size() > 0)
        {
            for (size_t i = 0; i < vecLockIds.size(); i++)
            {
                cJSON* lockItem = cJSON_CreateObject();
                cJSON_AddNumberToObject(lockItem, "code", 6);
                cJSON_AddStringToObject(lockItem, "id", vecLockIds[i].c_str());
                cJSON_AddStringToObject(lockItem, "message", "element locked!");
                cJSON_AddItemToArray(pResult, lockItem);
            }
        }

        int nCount = cJSON_GetArraySize(pIds);
        for (int i = 0; i < nCount; i++) {
            cJSON* pItem = cJSON_GetArrayItem(pIds, i);
            if (pItem && pItem->valuestring) {
                int re = updateElementNode(updateParam, pItem->valuestring); //更新node
                cJSON* pItemRet = cJSON_CreateObject();
                cJSON_AddStringToObject(pItemRet, "id", pItem->valuestring);
                cJSON_AddNumberToObject(pItemRet, "code", re);
                cJSON_AddStringToObject(pItemRet, "message", !re ? "update successed" : "update error");
                cJSON_AddItemToArray(pResult, pItemRet);

                if (re) {
                    nRet = 2;
                }
            }
        }

        char* pOut = cJSON_Print(pResult);
        if (pOut) {
            strRespone = pOut;
            free(pOut);
        }

        cJSON_Delete(pResult);
        cJSON_Delete(pRequest);
        //return nRet;
    }

    //std::cout << "updateElement use time " << watch.elapsed() << std::endl;
    //Poco::Logger::get("RteLogger").debug("updateElement use time %d", (int)watch.elapsed());
    return nRet;
}


/*******************************************************************************
* 函数名称：
* 功能描述： 修改指定id 元素数据，不包含elementdata
* 输入参数： vecChangeParam node需要修改的变量
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

int CTreeItem::updateElementNode(std::vector<cJSON*> vecChangeParam, std::string strId, bool bAdd)
{
    //step 1 ,查找node
    auto pNode = m_mapNode.find(strId);
    if (pNode != m_mapNode.end())
    {
        for (auto itor : vecChangeParam) {
            if (parseJsonModifyNode(itor, pNode->second,bAdd))
            {
                //Poco::Logger::get("RteLogger").warning("修改参数失败[%s]", std::string(itor->string));
            }
        }

        //如果到现在还没有checkedArray字段，代码自动添加
        if (!pNode->second->checkedArray.get()) {
            pNode->second->checkedArray.reset();

            cJSON* checkedArray = cJSON_CreateArray();

            for (size_t i = 0; i < m_vecSence.size(); i++)
            {
                cJSON* pItem = cJSON_CreateObject();
                //cJSON_AddNumberToObject(pItem, m_strCurSenceId.c_str(), 2); //0不勾选  1半选 2全选,  这里是不是该添加一个数组
                cJSON_AddNumberToObject(pItem, m_vecSence[i].c_str(), 2); //0不勾选  1半选 2全选
                cJSON_AddItemToArray(checkedArray, pItem);
            }

            pNode->second->checkedArray = std::shared_ptr<cJSON>(checkedArray, cJSON_Delete);
        }

#if 0
        //修改完成后，调用下显隐
        if (pNode->second->visibleArray.get())
        {
            if (isCheck(pNode->second->visibleArray.get(), m_strCurSenceId))
            { //显示
                cJSON* pElement = cJSON_CreateObject();

                cJSON_AddStringToObject(pElement, "id", strId.c_str());
                cJSON_AddStringToObject(pElement, "method", "control");
                cJSON* data = cJSON_CreateObject();
                cJSON_AddBoolToObject(data, "_displayModel", true);
                cJSON_AddItemToObject(pElement, "data", data);

                char* pOut = cJSON_Print(pElement);
                std::string strInput = pOut;
                free(pOut);
                std::string strResult;
                int nRet = Interface::elementHandle(strInput, strResult);
                if (nRet) {
                    //Poco::Logger::get("RteLogger").error(strResult);
                }
                cJSON_Delete(pElement);
            }
            else {
                //隐藏 
                cJSON* pElement = cJSON_CreateObject();
                cJSON_AddStringToObject(pElement, "id", strId.c_str());
                cJSON_AddStringToObject(pElement, "method", "control");
                cJSON* data = cJSON_CreateObject();
                cJSON_AddBoolToObject(data, "_displayModel", false);
                cJSON_AddItemToObject(pElement, "data", data);

                char* pOut = cJSON_Print(pElement);
                std::string strInput = pOut;
                free(pOut);
                std::string strResult;
                int nRet = Interface::elementHandle(strInput, strResult);
                if (nRet) {
                    //Poco::Logger::get("RteLogger").error(strResult);
            }
                cJSON_Delete(pElement);
        }

    }
#endif
        return 0;
    }
    return -1;
}

/*******************************************************************************
* 函数名称：
* 功能描述：更新node节点中element data数据
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

int CTreeItem::updateNodeElementData(cJSON* pElementData, std::shared_ptr<Node> pNode)
{
    if (pNode.get()) {
        if (!pNode->elementData) {
            pNode->elementData = std::shared_ptr<cJSON>(cJSON_CreateObject(), cJSON_Delete);
        }

        for (cJSON* pItem = pElementData->child; pItem!=NULL; pItem = pItem->next)
        {
            if (cJSON_HasObjectItem(pNode->elementData.get(), pItem->string)) {
                //卸载参数
                cJSON* pFree = cJSON_DetachItemFromObject(pNode->elementData.get(), pItem->string);
                if (pFree)
                    cJSON_Delete(pFree);

                //安装新的参数
                cJSON_AddItemToObject(pNode->elementData.get(), pItem->string, cJSON_Duplicate(pItem, true));
            }
            else {
                //安装新的参数
                cJSON_AddItemToObject(pNode->elementData.get(), pItem->string, cJSON_Duplicate(pItem, true));
            }
        }
    }
    return -1;
}
/*******************************************************************************
* 函数名称：
* 功能描述：根据请求的json数据修改node
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

int CTreeItem::parseJsonModifyNode(cJSON* ChangeParam, std::shared_ptr<Node> pNode, bool bAdd)
{
    if (!ChangeParam)
        return -1;

    std::string strName = ChangeParam->string;

    if (!strName.compare("resourceId") && ChangeParam->valuestring)
    {
        pNode->resourceId.reset();
        pNode->resourceId = std::shared_ptr<char>(strdup(ChangeParam->valuestring), free);
    }
    else if (!strName.compare("isLockArray"))
    {
        pNode->isLockArray.reset();
        pNode->isLockArray = std::shared_ptr<cJSON>(cJSON_Duplicate(ChangeParam, 1), cJSON_Delete);
    }
    else if (!strName.compare("data"))
    {
        pNode->data.reset();
        pNode->data = std::shared_ptr<cJSON>(cJSON_Duplicate(ChangeParam, 1), cJSON_Delete);
    }
    else if (!strName.compare("elementData"))
    {
        //pNode->elementData.reset();
        //pNode->elementData = std::shared_ptr<cJSON>(cJSON_Duplicate(ChangeParam, 1), cJSON_Delete);
        updateNodeElementData(ChangeParam, pNode);
    }
    else if (!strName.compare("strType") && ChangeParam->valuestring)
    {
        pNode->strType.reset();
        pNode->strType = std::shared_ptr<char>(strdup(ChangeParam->valuestring), free);
    }
    else if (!strName.compare("icon") && ChangeParam->valuestring)
    {
        pNode->icon.reset();
        pNode->icon = std::shared_ptr<char>(strdup(ChangeParam->valuestring), free);
    }
    else if (!strName.compare("opacityStyleColor") && ChangeParam->valuestring)
    {
        pNode->opacityStyleColor.reset();
        pNode->opacityStyleColor = std::shared_ptr<char>(strdup(ChangeParam->valuestring), free);
    }
    else if (!strName.compare("buildID") && ChangeParam->valuestring)
    {
        pNode->buildID.reset();
        pNode->buildID = std::shared_ptr<char>(strdup(ChangeParam->valuestring), free);
    }
    else if (!strName.compare("type"))
    {
        if(pNode->type.get())
            *(pNode->type.get()) = ChangeParam->valueint;
        else {
            pNode->type = std::make_unique<int>(ChangeParam->valueint);
        }
    }
    else if (!strName.compare("secondType"))
    {
        if (pNode->secondType.get())
            *(pNode->secondType.get()) = ChangeParam->valueint;
        else
            pNode->secondType = std::make_unique<int>(ChangeParam->valueint);
    }
    else if (!strName.compare("opacityStyleShowBuildOpacity"))
    {
        if (pNode->opacityStyleShowBuildOpacity.get())
            *(pNode->opacityStyleShowBuildOpacity.get()) = ChangeParam->type == cJSON_True ? true : false;
        else
            pNode->opacityStyleShowBuildOpacity = std::make_unique<bool>(ChangeParam->type == cJSON_True ? true : false);
    }
    else if (!strName.compare("sceneId") && ChangeParam->valuestring)
    {
        pNode->sceneId.reset();
        pNode->sceneId = std::shared_ptr<char>(strdup(ChangeParam->valuestring), free);
    }
    else if (!strName.compare("isDrag"))
    {
        if (pNode->isDrag.get())
            *(pNode->isDrag.get()) = ChangeParam->type == cJSON_True ? true : false;
        else
            pNode->isDrag = std::make_unique<bool>(ChangeParam->type == cJSON_True ? true : false);
    }
    else if (!strName.compare("bindGuid") && ChangeParam->valuestring)
    {
        pNode->bindGuid.reset();
        pNode->bindGuid = std::shared_ptr<char>(strdup(ChangeParam->valuestring), free);
    }
    else if (!strName.compare("checkedArray"))
    {
        pNode->checkedArray.reset();
        pNode->checkedArray = std::shared_ptr<cJSON>(cJSON_Duplicate(ChangeParam, 1), cJSON_Delete);
    }
    else if (!strName.compare("order"))
    {
        if (pNode->order.get())
            *(pNode->order.get()) = ChangeParam->valueint;
        else
            pNode->order = std::make_unique<int>(ChangeParam->valueint);
    }
    else if (!strName.compare("opacityStyleOpen"))
    {
        if (pNode->opacityStyleOpen.get())
            *(pNode->opacityStyleOpen.get()) = ChangeParam->type == cJSON_True ? true : false;
        else
            pNode->opacityStyleOpen = std::make_unique<bool>(ChangeParam->type == cJSON_True ? true : false);
    }
    else if (!strName.compare("version"))
    {
        if (pNode->version.get())
            *(pNode->version.get()) = ChangeParam->valuedouble;
        else
            pNode->version = std::make_unique<double>(ChangeParam->valuedouble);
    }
    else if (!strName.compare("url") && ChangeParam->valuestring)
    {
        pNode->url.reset();
        pNode->url = std::shared_ptr<char>(strdup(ChangeParam->valuestring), free);
    }
    else if (!strName.compare("tagList"))
    {
        std::string strRespones;
        updateTagsByNode_Replace(pNode, ChangeParam, strRespones);
    }
    else if (!strName.compare("viewPointList")) {
        pNode->viewPointList.reset();
        pNode->viewPointList = std::shared_ptr<cJSON>(cJSON_Duplicate(ChangeParam, 1), cJSON_Delete);
    }
    else if (!strName.compare("isExpand"))
    {
        if (pNode->isExpand.get())
            *(pNode->isExpand.get()) = ChangeParam->type == cJSON_True ? true : false;
        else
            pNode->isExpand = std::make_unique<bool>(ChangeParam->type == cJSON_True ? true : false);
    }
    else if (!strName.compare("visibleArray"))
    {
            pNode->visibleArray.reset();
            pNode->visibleArray = std::shared_ptr<cJSON>(cJSON_Duplicate(ChangeParam, 1), cJSON_Delete);
    }
    else if (!strName.compare("showSetBuild"))
    {
        if (pNode->showSetBuild.get())
            *(pNode->showSetBuild.get()) = ChangeParam->type == cJSON_True ? true : false;
        else
            pNode->showSetBuild = std::make_unique<bool>(ChangeParam->type == cJSON_True ? true : false);
    }
    else if (!strName.compare("name") && ChangeParam->valuestring)
    {
        pNode->name.reset();
        pNode->name = std::shared_ptr<char>(strdup(ChangeParam->valuestring), free);
        pNode->index["name"] = pNode->name;
    }
    else if (!strName.compare("opacityStyleVisible"))
    {
        if (pNode->opacityStyleVisible.get())
            *(pNode->opacityStyleVisible.get()) = ChangeParam->type == cJSON_True ? true : false;
        else
            pNode->opacityStyleVisible = std::make_unique<bool>(ChangeParam->type == cJSON_True ? true : false);
    }
    else if (!strName.compare("foreignKey"))
    {
        //step1 把之前索引中的数据删掉
        if(pNode->foreignKey.get())
            if (cJSON_IsArray(pNode->foreignKey.get())) {
                int nSize = cJSON_GetArraySize(pNode->foreignKey.get());
                if (nSize > 0)
                {
                    cJSON* pItem = cJSON_GetArrayItem(pNode->foreignKey.get(), 0);
                    if (pItem && pItem->valuestring)
                    {
                        //删除外键表中对应数据
                        std::map<std::string, std::shared_ptr<Node>>::iterator itorFind = m_mapForeignKeyNode.find(pItem->valuestring);
                        if (itorFind != m_mapForeignKeyNode.end())
                            m_mapForeignKeyNode.erase(itorFind);
                    }
                }
                else {
                    //删除 foreignKey 数组为空的map
                    m_mapForeignKeyNullNode.erase(pNode->guid.get());
                }
            }
        //step2 修改外键
        pNode->foreignKey.reset();
        pNode->foreignKey = std::shared_ptr<cJSON>(cJSON_Duplicate(ChangeParam, 1), cJSON_Delete);

        if (cJSON_IsArray(pNode->foreignKey.get())) {
            int nSize = cJSON_GetArraySize(pNode->foreignKey.get());
            if (nSize > 0)
            {
                cJSON* pItem = cJSON_GetArrayItem(pNode->foreignKey.get(), 0);
                if (pItem && pItem->valuestring)
                {
                    m_mapForeignKeyNode[pItem->valuestring] = pNode; // 创建外键表，外键唯一，只取第一个
                    pNode->strForeignKey = pItem->valuestring;
                }
            }
            else {
                m_mapForeignKeyNullNode[pNode->guid.get()] = pNode;
            }
        }
    }
    else if (!strName.compare("attributeList_templte")) //将模板中的业务属性添加到元素中
    {
        /*if (!pNode->attributeList.get())
        {
            pNode->attributeList = std::shared_ptr<cJSON>(cJSON_Duplicate(ChangeParam,true), cJSON_Delete);
        }*/

        //填充模板中的业务属性
        if (ChangeParam) {
            if (cJSON_IsArray(ChangeParam)) {
                int nSize = cJSON_GetArraySize(ChangeParam);
                if (nSize > 0) {
                    for (size_t i = 0; i < nSize; i++)
                    {
                        cJSON* pObject = cJSON_GetArrayItem(ChangeParam, i);
                        cJSON* pChild = pObject->child;
                        if (pChild && pChild->string && pChild->valuestring) {
                            //if (mapAttri.find(pChild->string) != mapAttri.end())
                            {
                                Attributes attr;
                                attr.setStringValue(pChild->valuestring);
                                m_AttriManager.addBusAttr(pChild->string, attr, pNode->guid.get());
                                if (cJSON_IsArray(pNode->attributeList.get()))
                                {
                                    //将object添加到业务属性数组
                                    cJSON_AddItemToArray(pNode->attributeList.get(), /*pObject*/cJSON_Duplicate(pObject, true));
                                }
                                else {
                                    //将object添加到业务属性数组
                                    pNode->attributeList = std::shared_ptr<cJSON>(cJSON_CreateArray(), cJSON_Delete);
                                    cJSON_AddItemToArray(pNode->attributeList.get(), /*pObject*/cJSON_Duplicate(pObject, true));
                                }
                            }
                        }
                    }
                }
                else {
                    Attributes attr;
                    attr.setStringValue("[]");
                    m_AttriManager.deleteAttri("[]", attr, pNode->guid.get());
                }
            }
        }
    }
    else if (!strName.compare("attributeList"))
    {
        /*"attributeList":	[{
            "chid":	"111"
            }, {
                "nvrid":	"111"
            }, {
                "test":	"test123"
            }] */

            //step0 此时，type肯定是一致的，然后根据type拿到不同的资源id。
            //step1 根据资源id获取poi、或者特效模板是否可以修改业务属性， 修改条件字段value为空可以修改，否则不能修改
            //特效和poi有模板，其他类型元素添加修改业务属性不受模板限制。
            //获取可以修改的业务属性 
        cJSON* pAttr = nullptr;
        std::string ResouceID;

        switch (*(pNode->type))
        {
        case SDT_SceneEffects:
        {
            //if (pNode->elementData) {
            //    cJSON_LoadFromObject(pNode->elementData.get(), "_effectTemplateID", ResouceID); //_effectTemplateID 字段3d 返回的为空
            //}
            if (pNode->resourceId.get())
                ResouceID = pNode->resourceId.get();
            pAttr = GetGlobalSettingInstance().getByName(SDT_SceneEffects, ResouceID, "attributeList");
        }
        break;
        case SDT_POI:
        {
            if (pNode->data) {
                cJSON_LoadFromObject(pNode->data.get(), "poiStyleID", ResouceID);
            }
            pAttr = GetGlobalSettingInstance().getByName(SDT_POI, ResouceID, "attributeList"); //获取模板业务属性 模板格式
        }
        break;
        default:
            break;
        }

        std::map<std::string, std::string> mapAttri; //可以修改的属性map
        if (pAttr && cJSON_IsArray(pAttr))
        {
            int nSize = cJSON_GetArraySize(pAttr);
            for (size_t i = 0; i < nSize; i++) {
                cJSON* pItem = cJSON_GetArrayItem(pAttr, i);
                cJSON* pAttri = pItem->child;
                if (pAttri->string && pAttri->valuestring) {
                    if (!strcmp(pAttri->valuestring, ""))
                    {
                        mapAttri[pAttri->string] = "";
                    }
                }
            }
        }

        if (!bAdd) { //如果是修改就把之前的删除，添加新的
            //step1 把之前map索引中的数据,以及可修改的数据删掉
            if (pNode->attributeList.get())
            {
                //for (auto itor : mapAttri)  //目前不需要根据模板判断是否需要修改，接口都能修改，包括模板中的数据。
                {
                    Attributes attr;
                    std::string strValue;
                    cJSON* pAttr = pNode->attributeList.get();
                    if (pAttr && cJSON_IsArray(pAttr))
                    {
                        int nSize = cJSON_GetArraySize(pAttr);

                        if (nSize > 0) {
                            for (size_t i = 0; i < nSize; /*i++*/)
                            {
                                strValue = "";
                                cJSON* pItem = cJSON_GetArrayItem(pAttr, i);
                                if (pItem && pItem->child) {
                                    //if (!strcmp(pItem->child->string, itor.first.c_str())) //删除数组的的这个objec
                                    {
                                        strValue = pItem->child->valuestring ? pItem->child->valuestring : "";

                                        attr.setStringValue(strValue.c_str());
                                        m_AttriManager.deleteAttri(pItem->child->string, attr, pNode->guid.get()); //业务属性中删除对应的id的业务属性。
                                        cJSON_DeleteItemFromArray(pAttr, i);
                                        nSize--;
                                    }
                                    //else
                                    //    i++;
                                }
                            }
                        }
                        else {
                            attr.setStringValue("[]");
                            m_AttriManager.deleteAttri("[]", attr, pNode->guid.get());
                        }
                    }
                }
            }
        }
        //填充修改后的业务属性
        if (ChangeParam) {
            if (cJSON_IsArray(ChangeParam)) {
                int nSize = cJSON_GetArraySize(ChangeParam);

                if (nSize > 0) {
                    for (size_t i = 0; i < nSize; i++)
                    {
                        cJSON* pObject = cJSON_GetArrayItem(ChangeParam, i);
                        cJSON* pChild = pObject->child;
                        if (pChild && pChild->string && pChild->valuestring) {
                            //if (mapAttri.find(pChild->string) != mapAttri.end())
                            {
                                Attributes attr;
                                attr.setStringValue(pChild->valuestring);
                                m_AttriManager.addBusAttr(pChild->string, attr, pNode->guid.get());
                                if (cJSON_IsArray(pNode->attributeList.get()))
                                {
                                    //将object添加到业务属性数组
                                    cJSON_AddItemToArray(pNode->attributeList.get(), /*pObject*/cJSON_Duplicate(pObject, true));
                                }
                                else {
                                    //将object添加到业务属性数组
                                    pNode->attributeList = std::shared_ptr<cJSON>(cJSON_CreateArray(), cJSON_Delete);
                                    cJSON_AddItemToArray(pNode->attributeList.get(), /*pObject*/cJSON_Duplicate(pObject, true));
                                }
                            }
                        }
                    }
                }
                else {
                    Attributes attr;
                    attr.setStringValue("[]");
                    m_AttriManager.deleteAttri("[]", attr, pNode->guid.get());
                }
            }
        }
    }
    else if (!strName.compare("projectId") && ChangeParam->valuestring)
    {
        pNode->projectId.reset();
        pNode->projectId = std::shared_ptr<char>(strdup(ChangeParam->valuestring), free);
    }
    else if (!strName.compare("floorIndex"))
    {
        if (pNode->floorIndex.get())
            *(pNode->floorIndex.get()) = ChangeParam->valueint;
        else
            pNode->floorIndex = std::make_unique<int>(ChangeParam->valueint);
    }
    else if (!strName.compare("opacityStyleOpacity"))
    {
        if (pNode->opacityStyleOpacity.get())
            *(pNode->opacityStyleOpacity.get()) = ChangeParam->valueint;
        else
            pNode->opacityStyleOpacity = std::make_unique<int>(ChangeParam->valueint);
    }
    return 0;
}

/*******************************************************************************
* 函数名称：
* 功能描述：处理删除操作，只删除3d能处理的数据，group不做处理。
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/
int CTreeItem::deleteElement(std::string request, std::string& strRespone)
{
    //Poco::Logger::get("RteLogger").debug("deleteElement requset body %s", request);
    cJSON* pRequest = cJSON_Parse(request.c_str());
    strRespone = "Invalid parameter";
    if (!pRequest) {
        return -1;
    }

    cJSON* pTypeJson = cJSON_GetObjectItem(pRequest, "type");
    cJSON* pIds = cJSON_GetObjectItem(pRequest, "id");

    if (!pTypeJson || !pIds)
    {
        //Poco::Logger::get("RteLogger").error("id || type || elementData is null");
        cJSON_Delete(pRequest);
        return -1;
    }


    int nType = -1;
    std::string strType;
    nType = pTypeJson->valueint;
    strType = RTE::getModelStrTypeByType(nType);

    cJSON* p3dJson = cJSON_CreateObject();
    cJSON_AddStringToObject(p3dJson, "element", strType.c_str());
    cJSON_AddStringToObject(p3dJson, "method", "delete");

    std::vector<std::string> vecLockIds;
    if (cJSON_IsArray(pIds)) {
        //判断id是否lock ,过滤锁定的id
        int nSize = cJSON_GetArraySize(pIds);
        for (size_t i = 0; i < nSize; i++)
        {
            cJSON* pItem = cJSON_GetArrayItem(pIds, i);
            if (pItem && pItem->valuestring) {
                std::string guid = pItem->valuestring;
                auto itorNode = m_mapNode.find(guid);
                if (itorNode != m_mapNode.end()) {
                    if (isCheck(itorNode->second->isLockArray.get(), m_strCurSenceId)) //锁定的不能删除
                    {
                        vecLockIds.push_back(guid);
                        cJSON_DeleteItemFromArray(pIds, i);
                        nSize--;
                        i--;
                    }
                }
            }
        }
    }
    else {
        std::string guid;
        if (pIds->valuestring)
            guid = pIds->valuestring;
        auto itorNode = m_mapNode.find(guid);
        if (itorNode != m_mapNode.end()) {
            if (isCheck(itorNode->second->isLockArray.get(), m_strCurSenceId)) //锁定的不能删除
            {
                strRespone = "id islocked";
                cJSON_Delete(pRequest);
                cJSON_Delete(p3dJson);
                return -1;
            }
        }
    }
    cJSON_AddItemToObject(p3dJson, "id", cJSON_Duplicate(pIds, true));

    char* pOut = cJSON_Print(p3dJson);
    std::string strInput;
    if (pOut) {
        strInput = pOut;
        free(pOut);
    }
    cJSON_Delete(p3dJson);

    //调用3d接口返回 id数组
    std::string strResult;
    int nRet = Interface::elementHandle(strInput, strResult);
    cJSON* pResultJson = cJSON_Parse(strResult.c_str());
    if (pResultJson) {
        if (nRet == 0 || nRet == 2) //
        {
            if (pResultJson && cJSON_IsArray(pResultJson))
            {
                int nCount = cJSON_GetArraySize(pResultJson);
                for (int i = 0; i < nCount; i++)
                {
                    cJSON* pItem = cJSON_GetArrayItem(pResultJson, i);
                    if (pItem) {
                        cJSON* pId = cJSON_GetObjectItem(pItem, "id");
                        cJSON* pCode = cJSON_GetObjectItem(pItem, "code");
                        if (pCode && pId && pId->valuestring) {
                            int nCode = pCode->valueint;
                            if (nCode == 0) {
                                deleteNode(pId->valuestring); //只删除3d删除成功的，这样过滤掉删除group等误操作。
                            }
                        }
                    }
                }
            }
        }
        //如果检测到有锁定的id，将锁定数据返回给客户。
        if (vecLockIds.size() > 0)
        {
            cJSON* lockIds = cJSON_CreateArray();
            for (size_t i = 0; i < vecLockIds.size(); i++)
            {
                cJSON_AddItemToArray(lockIds, cJSON_CreateString(vecLockIds[i].c_str()));
            }
            cJSON_AddItemToObject(pResultJson, "lockIds", lockIds);
        }

        char* pOutResult = cJSON_Print(pResultJson);
        if (pOutResult) {
            strRespone = /*pOutResult*/"{}";
            free(pOutResult);
        }
        cJSON_Delete(pResultJson);
    }

    cJSON_Delete(pRequest);
    return 0;
}

int CTreeItem::deleteNode(std::string strId)
{
    //step1 删除元素索引
    auto itorNode = m_mapNode.find(strId);
    if (itorNode != m_mapNode.end()) {

        //step2 删除外键索引
        if (itorNode->second->foreignKey && cJSON_IsArray(itorNode->second->foreignKey.get())) {
            int nSize = cJSON_GetArraySize(itorNode->second->foreignKey.get());
            if (nSize > 0)
            {
                cJSON* pItem = cJSON_GetArrayItem(itorNode->second->foreignKey.get(), 0);
                if (pItem && pItem->valuestring)
                {
                    m_mapForeignKeyNode.erase(pItem->valuestring);
                }
            }
        }

        //step 3 删除tags 索引
        //std::map<std::string, std::vector<std::pair<std::string, int>>> tagsMap = m_tagManager.getTagMap();
        if (itorNode->second->tagList && cJSON_IsArray(itorNode->second->tagList.get())) {
            int nSize = cJSON_GetArraySize(itorNode->second->tagList.get());
            if (nSize > 0) {
                for (size_t i = 0; i < nSize; i++)
                {
                    cJSON* pItem = cJSON_GetArrayItem(itorNode->second->tagList.get(), i);
                    if (pItem->child && pItem->child->string)
                    {
                        m_tagManager.deleteTag(pItem->child->string, strId);
                    }
                }
            }
            else {
                m_tagManager.deleteTag("[]", strId);
            }
        }

        //step 4 删除业务属性
        if (itorNode->second->attributeList) {
            if (cJSON_IsArray(itorNode->second->attributeList.get()))
            {
                int nSize = cJSON_GetArraySize(itorNode->second->attributeList.get());

                if (nSize > 0) {
                    for (size_t i = 0; i < nSize; i++)
                    {
                        cJSON* pItem = cJSON_GetArrayItem(itorNode->second->attributeList.get(), i);
                        cJSON* pChild = pItem->child;
                        if (pChild->string && pChild->valuestring) {
                            Attributes attr;
                            attr.setStringValue(pChild->valuestring);
                            m_AttriManager.deleteAttri(pChild->string, attr, itorNode->second->guid.get());
                        }
                    }
                }
                else {
                    Attributes attr;
                    attr.setStringValue("[]");
                    m_AttriManager.deleteAttri("[]", attr, itorNode->second->guid.get());
                }
            }
            else {
                //Poco::Logger::get("RteLogger").error("delete node attributeList is not array %s", std::string(itorNode->second->guid.get()));
            }
        }

        std::shared_ptr<Node> pNode = itorNode->second;
        m_mapNode.erase(itorNode);
        deleteNode(pNode);
        return 0;
    }
    else
        return -1;   
}

/*******************************************************************************
* 函数名称：
* 功能描述：元素控制接口
* 输入参数：request 控制请求
*           
            {
		        "id": "e0498830-0870-41bf-abbe-6ca7a52b1051",
		        "type": 52, //雷达
		        "operate": {
			        "reset": "color"
		        }
	        }
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/
int CTreeItem::controlElement(std::string request, std::string& strRespone)
{
    //Poco::Logger::get("RteLogger").debug("controlElement requset body %s", request);
    cJSON* pRequest = cJSON_Parse(request.c_str());
    strRespone = "Invalid parameter";
    if (!pRequest) {
        return -1;
    }

    cJSON* pTypeJson = cJSON_GetObjectItem(pRequest, "type"); // 可以不填

    int nType = -1;
    std::string strType;

    if (pTypeJson) {
        nType = pTypeJson->valueint;
        if (nType == 11)
            strType = "Architecture";
        else
            strType = RTE::getModelStrTypeByType(nType);
    }

    cJSON* p3dJson = cJSON_Duplicate(pRequest, true);

    cJSON *pType = cJSON_DetachItemFromObject(p3dJson, "type");
    if (pType)
    {
        cJSON_Delete(pType);
    }

    cJSON_AddStringToObject(p3dJson, "method", "control");
    if (nType >= 0)
        cJSON_AddStringToObject(p3dJson, "element", strType.c_str());


    //查找有无"jump", 有则用之，无则用全局配置
    //首先读取全局配置
    //读取全局配置中的jump
    bool bHasJump = false;
    if (GetGlobalSettingInstance().m_pAdvancedSetting)
    {
        std::string strRet;
        GetGlobalSettingInstance().m_pAdvancedSetting->getSettingValue("自定义设置", "smoothPositioning", strRet);
        if (strRet.compare("0"))
            bHasJump = true;
        else
            bHasJump = false;
    }

    do
    {
        if (cJSON_HasObjectItem(p3dJson, "operate"))
        {
            cJSON* pOperate = cJSON_GetObjectItem(p3dJson, "operate");
            if (pOperate)
            {
                if (nType == SDT_ViewPath)
                {
                    if (!cJSON_HasObjectItem(pOperate, "jump"))
                    {
                        cJSON_AddBoolToObject(pOperate, "jump", bHasJump);
                    }
                    break;
                }

                if (cJSON_HasObjectItem(pOperate, "Location"))
                {
                    cJSON* pLocation = cJSON_GetObjectItem(pOperate, "Location");
                    if (!cJSON_HasObjectItem(pLocation, "jump"))
                    {
                        cJSON_AddBoolToObject(pLocation, "jump", bHasJump);
                    }
                    break;
                }

                if (cJSON_HasObjectItem(pOperate, "SceneViewPoint"))
                {
                    cJSON* pSceneViewPoint = cJSON_GetObjectItem(pOperate, "SceneViewPoint");
                    if (!cJSON_HasObjectItem(pSceneViewPoint, "jump"))
                    {
                        cJSON_AddBoolToObject(pSceneViewPoint, "jump", bHasJump);
                    }
                    break;
                }

                if (cJSON_HasObjectItem(pOperate, "ModelViewPoint"))
                {
                    cJSON* pModelViewPoint = cJSON_GetObjectItem(pOperate, "ModelViewPoint");
                    if (!cJSON_HasObjectItem(pModelViewPoint, "jump"))
                    {
                        cJSON_AddBoolToObject(pModelViewPoint, "jump", bHasJump);
                    }
                    break;
                }
            }
        }
    } while (0);    

    char* pOut = cJSON_Print(p3dJson);

    std::string str3dRequest;
    if (pOut) {
        str3dRequest = pOut;
        free(pOut);
    }
    cJSON_Delete(p3dJson);
    cJSON_Delete(pRequest);

    std::string strResult;
    int nRet = Interface::elementHandle(str3dRequest, strResult);
    ////Poco::Logger::get("RteLogger").debug(str3dRequest);
    strRespone = strResult;

    return nRet;
}


/*******************************************************************************
* 函数名称：
* 功能描述：直接将json数据透传给3d进行处理 elementHandle 通用接口
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

int CTreeItem::GeneralInterface(std::string request, std::string& strRespone)
{
    int nRet = -1;
    std::string strResult;
    nRet = Interface::elementHandle(request, strResult);
    strRespone = strResult;
    return nRet;
}

/*******************************************************************************
* 函数名称：
* 功能描述： build sdk 事件通用接口, 直接透传
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

int CTreeItem::EventGeneralInterface(std::string request, std::string& strRespone)
{
    int nRet = -1;
    std::string strResult;
    nRet = Interface::eventHandle(request, strResult);
    strRespone = strResult;

    //Poco::Logger::get("RteLogger").debug(strRespone);
    return nRet;
}

void CTreeItem::getElementTree(std::string request, std::string& strRespone)
{
    cJSON* pRoot = cJSON_Parse(request.c_str());
    if (pRoot) {
        std::string guid;
        cJSON_LoadFromObject(pRoot, "guid", guid);

        auto itor = m_mapNode.find(guid);
        if (itor != m_mapNode.end()) {
            cJSON *pJson = parseTreeToJson(itor->second, true, true,false);

            if (pJson) {
                char* pOut = cJSON_Print(pJson);
                strRespone = pOut;
                free(pOut);
                cJSON_Delete(pJson);
            }
        }
        cJSON_Delete(pRoot);
    }
}

/*******************************************************************************
* 函数名称：
* 功能描述：
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* {
        "event": "getElementTree",
        "data": {
            "guid": "36b6d532-a619-28f3-f89a-d5282a2b47d8"
        }
    }
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/

void CTreeItem::getElementTree2(std::string request, std::string& strRespone)
{
    cJSON* pRoot = cJSON_Parse(request.c_str());
    std::string guid = "";
    if (pRoot) {
        cJSON* data = cJSON_GetObjectItem(pRoot, "data");
        if (data) {
            cJSON_LoadFromObject(data, "guid", guid);
        }

        if (guid.compare("")) {
            auto itor = m_mapNode.find(guid);
            if (itor != m_mapNode.end()) {
                cJSON* pJson = parseTreeToJson(itor->second, true, true);

                if (pJson) {
                    char* pOut = cJSON_Print(pJson);
                    strRespone = pOut;
                    free(pOut);
                    cJSON_Delete(pJson);
                }
            }         
        }
        cJSON_Delete(pRoot);
    }
}
/*******************************************************************************
* 函数名称：
* 功能描述：
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* {
        "event": "getElementBuildID",
        "data": {
            "id": "1212121"
        }
  }
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/
void CTreeItem::getElementBuildID(std::string request, std::string& strRespone)
{
    cJSON* pRoot = cJSON_Parse(request.c_str());
    std::string id = "";
    if (pRoot) {
        cJSON* data = cJSON_GetObjectItem(pRoot, "data");
        if (data) {
            cJSON_LoadFromObject(data, "guid", id);
        }

        if (id.compare("")) {
            //查找此id的buildid
            do
            {
                cJSON* pSend = cJSON_CreateObject();
                auto itorNode = getElementNode(id);
                if (!itorNode) {
                    cJSON_Delete(pSend);
                    break;
                }

                std::string strBuildingId = "";

                int nFloor = 0;
                std::string strFloorId = "";
                for (auto parent = itorNode->parent; parent != nullptr && parent->guid!=nullptr && strcmp(parent->guid.get(), "root"); parent = parent->parent)
                {
                    if (*parent->type == SDT_Build)
                    {
                        strBuildingId = parent->guid.get();
                        break;
                    }

                    if (*parent->type == SDT_Floor) {
                        //在这里根据再父节点 数组中的位置确定楼层
                        strFloorId = parent->guid.get();
                       /* auto tempNode = parent->parent;
                        if (tempNode) {
                            int i = 0;
                            for (auto itor : tempNode->childArray)
                            {
                                if (!strcmp(parent->guid.get(), itor->guid.get())) {
                                    nFloor = tempNode->childArray.size() - i;
                                    break;
                                }
                                i++;
                            }
                        }*/ //不需要写入精确楼层。
                    }
                }

                cJSON_AddNumberToObject(pSend, "floor", /*nFloor*/-1);
                cJSON_AddStringToObject(pSend, "floorId", strFloorId.c_str());
                cJSON_AddStringToObject(pSend, "parentBuildingID", strBuildingId.c_str());
                char* pOut = cJSON_Print(pSend);
                strRespone = pOut;
                //Poco::Logger::get("RteLogger").debug(std::string(pOut));
                free(pOut);
                cJSON_Delete(pSend);
            } while (0);
        }
        cJSON_Delete(pRoot);
    }
}

std::shared_ptr<Node> CTreeItem::getElementNode(std::string id) {
    auto itor = m_mapNode.find(id);
    if (itor != m_mapNode.end())
        return itor->second;
    else
        return nullptr;
}

int CTreeItem::notifyAddModel(std::string id, bool bSucess, bool bNeedAdd)
{
    if (bNeedAdd) {
        cJSON* pRequstJson = nullptr; //请求添加的json
        int nType = -1;
        std::string strType;
        std::string strParentId;
        auto itor = m_mapAddElement.find(id);
        if (itor != m_mapAddElement.end()) {
            pRequstJson = itor->second.second;
            nType = itor->second.first;
            strType = RTE::getModelStrTypeByType(nType);
            if (nType == SDT_DataPannel)
            {
                std::string resourceId;
                cJSON_LoadFromObject(pRequstJson, "resourceId", resourceId);
                if (resourceId.compare(""))
                    strType = "Blend2DData";
            }
            m_mapAddElement.erase(itor);
        }

        //添加node
        //attributeList_templte
        std::vector<cJSON*> attributeList_templteVec;
        if (bSucess && pRequstJson) {
            std::vector<cJSON*> m_vecChangeParam;
            for (cJSON* pItem = pRequstJson->child; pItem != nullptr; pItem = pItem->next)
            {
                if (!strcmp(pItem->string, "id"))
                    continue;

                if (!strcmp(pItem->string, "parentID")) {
                    if (pItem->valuestring)
                        strParentId = pItem->valuestring;
                    continue;
                }

                if (!strcmp(pItem->string, "attributeList_templte")) {
                    attributeList_templteVec.push_back(pItem);
                    continue;
                }

                if (!strcmp(pItem->string, "elementData"))//用3d返回的
                    continue;
                m_vecChangeParam.push_back(pItem);
            }

            //从3d获取elementData
            if (!strParentId.empty()) {
                std::shared_ptr<Node> pParentNode = getElementNode(strParentId);
                if (pParentNode) {
                    std::shared_ptr<Node> pChildNode = addChildNode(pParentNode);

                    if (pChildNode) {
                        m_mapNode.emplace(id, pChildNode);   //添加到全局map

                        cJSON* pGetElementJson = cJSON_CreateObject();
                        cJSON_AddStringToObject(pGetElementJson, "element", strType.c_str());
                        cJSON_AddStringToObject(pGetElementJson, "method", "get");
                        cJSON_AddStringToObject(pGetElementJson, "id", id.c_str());

                        cJSON_AddStringToObject(pGetElementJson, "ObjectData", "");

                        std::string strInput, strOutput;
                        char* pOut = cJSON_Print(pGetElementJson);
                        if (pOut) {
                            strInput = pOut;
                            free(pOut);
                        }
                        cJSON_Delete(pGetElementJson);
                        std::string _effectTemplateID; //资源id
                        if (!Interface::elementHandle(strInput, strOutput)) {
                            cJSON* pElement = cJSON_Parse(strOutput.c_str());
                            if (pElement) {
                                cJSON* pData = cJSON_GetObjectItem(pElement, "data");
                                pChildNode->elementData.reset();
                                pChildNode->elementData = std::shared_ptr<cJSON>(cJSON_Duplicate(pData, true), cJSON_Delete);

                                cJSON_LoadFromObject(pChildNode->elementData.get(), "_effectTemplateID", _effectTemplateID);
                                cJSON_Delete(pElement);
                            }
                        }

                        pChildNode->guid.reset();
                        pChildNode->guid = std::shared_ptr<char>(strdup(id.c_str()), free);
                        pChildNode->type = std::make_unique<int>(nType);

                        updateElementNode(attributeList_templteVec, id,true);//先更新模板的业务属性
                        updateElementNode(m_vecChangeParam, id,true);

                        GetGlobalSettingInstance().addRef((RTE::SceneDataType)nType, _effectTemplateID);
                    }
                }
            }

            cJSON_Delete(pRequstJson);//这里可以释放了。
        }
    }
    else {
        m_cond.notify(id);
    }

    {
        auto itor = m_mapNode.find(id);
        if (itor != m_mapNode.end()) {

            //查看是否隐藏
            if (itor->second->visibleArray)
            {
                bool bDisPlay = true;
                if (!isCheck(itor->second->visibleArray.get(), m_strCurSenceId))
                {
                    bDisPlay = false;
                }

                //隐藏元素    //need 3d interface 
                cJSON* pElement = cJSON_CreateObject();

                cJSON_AddStringToObject(pElement, "id", itor->second->guid.get());
                cJSON_AddStringToObject(pElement, "method", "control");
                cJSON* data = cJSON_CreateObject();
                cJSON_AddBoolToObject(data, "_displayModel", bDisPlay);
                cJSON_AddItemToObject(pElement, "data", data);

                char* pOut = cJSON_Print(pElement);
                std::string strInput = pOut;
                free(pOut);
                std::string strResult;

                if (bSucess) {
                    int nRet = Interface::elementHandle(strInput, strResult);
                    if (nRet) {
                        //Poco::Logger::get("RteLogger").error(strResult);
                    }
                }

                cJSON_Delete(pElement);
            }
        }
    }

    return 0;
}

int CTreeItem::notifyDelModel(std::string id)
{
    //step1 删除模板引用
    auto itorNode = m_mapNode.find(id);
    if (itorNode != m_mapNode.end())
    {
        cJSON* elementData = itorNode->second->elementData.get();
        std::string _effectTemplateID;
        cJSON_LoadFromObject(elementData, "_effectTemplateID", _effectTemplateID);
        int type = *itorNode->second->type;

        GetGlobalSettingInstance().releaseRef((RTE::SceneDataType)type, _effectTemplateID);
    }

    //step2 元素map中删除此node
    deleteNode(id);
    return 0;
}

int CTreeItem::notifyUpdateMode(std::string id)
{
    auto itor = m_mapNode.find(id);
    if (itor != m_mapNode.end()) {
        std::string strType = RTE::getModelStrTypeByType((RTE::SceneDataType) * (itor->second->type));

        cJSON* pUpdate = cJSON_CreateObject();
        cJSON* pElementChange = getElementDataFrom3D(id, strType);

        if (pElementChange) {
            std::vector<cJSON*> updateParamTemp;
            cJSON_AddItemToObject(pUpdate, "elementData", pElementChange);
            if (pUpdate->child)
                updateParamTemp.push_back(pUpdate->child);

            int re = updateElementNode(updateParamTemp, id); //更新node数据
            if (re) {
                //Poco::Logger::get("RteLogger").error(std::string("update callback update elementData error"));
            }
        }
        else {
            //Poco::Logger::get("RteLogger").error(std::string("getElementDataFrom3D error"));
        }
        if (pUpdate)
            cJSON_Delete(pUpdate);
    }

    return 0;
}

cJSON* CTreeItem::getElementDataFrom3D(std::string id,std::string strType)
{
    cJSON* pGetElementJson = cJSON_CreateObject();
    cJSON_AddStringToObject(pGetElementJson, "element", strType.c_str());
    cJSON_AddStringToObject(pGetElementJson, "method", "get");
    cJSON_AddStringToObject(pGetElementJson, "id", id.c_str());

    cJSON_AddStringToObject(pGetElementJson, "ObjectData", "");

    std::string strInput, strOutput;
    char* pOut = cJSON_Print(pGetElementJson);
    if (pOut) {
        strInput = pOut;
        free(pOut);
    }
    cJSON_Delete(pGetElementJson);
    std::string _effectTemplateID; //资源id
    if (!Interface::elementHandle(strInput, strOutput)) {
        cJSON* pElement = cJSON_Parse(strOutput.c_str());
        if (pElement) {
            cJSON* pData = cJSON_GetObjectItem(pElement, "data");
            if (pData) {
                cJSON* pDataRet = nullptr;
                pDataRet = cJSON_Duplicate(pData, true);
                cJSON_Delete(pElement);     //add at 20250527
                return pDataRet;
            }
            cJSON_Delete(pElement);
        }
    }
    return nullptr;
}

int CTreeItem::addElementArrary(std::string request, std::string& strRespone)
{
    //Poco::Logger::get("RteLogger").debug("addElement requset body %s", request);
    cJSON* pRequest = cJSON_Parse(request.c_str());
    strRespone = "Invalid parameter";
    if (!pRequest) {
        return -1;
    }

    //判断前置条件是否存在
    cJSON* type = cJSON_GetObjectItem(pRequest, "type");
    cJSON* data = cJSON_GetObjectItem(pRequest, "data");

    if (!type || !data)
    {
        //Poco::Logger::get("RteLogger").error("type or data is null");
        cJSON_Delete(pRequest);
        return -1;
    }

    std::string strId, strParentId;
    int nType = -1;
    nType = type->valueint;

    int nRet = 0;
    if (!cJSON_IsArray(data)) {
        return -1;
    }

    int nDataSize = cJSON_GetArraySize(data);
    cJSON* addJson = cJSON_CreateArray();
    for (size_t i = 0; i < nDataSize; i++)
    {
        //这里需要将添加数据缓存到map中，等待3d响应后在将数据存储到node map中
        std::string strId;
        cJSON* addItem = addElementImpl2(nType, cJSON_GetArrayItem(data, i), strId);
        cJSON_AddItemToArray(addJson, addItem);
    }

    if (addJson) {
        char* pOut = cJSON_Print(addJson);
        if (pOut) {
            nRet = Interface::elementHandle(pOut, strRespone);
            //Poco::Logger::get("RteLogger").debug("%s",std::string(pOut));
            free(pOut);
        }
        cJSON_Delete(addJson);
    }

    cJSON_Delete(pRequest);
    return nRet;
}

//返回真实添加json array，批量通过sdk添加
cJSON* CTreeItem::addElementImpl2(int nType, cJSON* pElement, std::string strId)
{
    //pElement 外部传入不需删除
    if (!pElement) {
        //Poco::Logger::get("RteLogger").error("addElementImpl2 pElement null");
        return nullptr;
    }

    std::string strType = RTE::getModelStrTypeByType(nType);

    if (!strType.compare("")) {
        //Poco::Logger::get("RteLogger").error("addElementImpl2 strType null");
        return nullptr;
    }

    cJSON* id;          //
    cJSON* parentID;    //
    cJSON* resourceId;  //必填, 对应的时模板中的 guid 或者元素data中的 poiStyleID
    cJSON* elementData; // elementData中的_worldMapPos 必填,对应老接口的position

    cJSON* name;

    cJSON* foreignKey = nullptr;
    foreignKey = cJSON_GetObjectItem(pElement, "foreignKey");
    if (foreignKey && foreignKey->child && foreignKey->child->valuestring)
    {
        if (strcmp(foreignKey->child->valuestring, "")) {
            std::map<std::string, std::shared_ptr<Node>>::iterator itorFind = m_mapForeignKeyNode.find(foreignKey->child->valuestring);
            if (itorFind != m_mapForeignKeyNode.end()) {
                //Poco::Logger::get("RteLogger").error("foreignKey already exists");
                return nullptr;
            }
        }
    }

    // 根据elementData添加自定义选项.
    elementData = cJSON_GetObjectItem(pElement, "elementData");

    if (!elementData) {
        //strRespone = "type is invalude"; 有些元素可以为空
        //return -1;
    }

    resourceId = cJSON_GetObjectItem(pElement, "resourceId");

    name = cJSON_GetObjectItem(pElement, "name");

    std::string strParentId, strResourceId;

    id = cJSON_GetObjectItem(pElement, "id"); //适配id和guid
    if (!id)
        id = cJSON_GetObjectItem(pElement, "guid");

    if (id && id->valuestring)
        strId = id->valuestring;

    if (!strId.compare(""))
    {
        //创建id
        strId = GUIDStr();
    }

    parentID = cJSON_GetObjectItem(pElement, "parentID");
    if (parentID && parentID->valuestring) {
        strParentId = parentID->valuestring;

        if (!strParentId.compare("")) {
            if (m_pElementNode->childArray.size() > 0) {
                strParentId = m_strGroupId;// m_pElementNode->childArray[0]->guid.get();//root node id
            }
        }
        cJSON_SetValuestring(parentID, strParentId.c_str());
    }
    else {
        if (m_pElementNode->childArray.size() > 0) {
            strParentId = m_strGroupId;// m_pElementNode->childArray[0]->guid.get();//root node id
            cJSON_AddStringToObject(pElement, "parentID", strParentId.c_str());
        }
    }

    if (resourceId && resourceId->valuestring)
        strResourceId = resourceId->valuestring;

    //根据resourceid 获取模型模板
    cJSON* pTemplate = NULL;
    switch (nType)
    {
    case SDT_VirtualCamera: //虚拟相机和特效用的是一个模板
    case SDT_SceneEffects:
        pTemplate = GetGlobalSettingInstance().getByResourceID(SDT_SceneEffects, strResourceId);
        break;
    case SDT_ModelBase:
        pTemplate = GetGlobalSettingInstance().getByResourceID(SDT_ModelBase, strResourceId);
        break;
    case SDT_Model:
        pTemplate = GetGlobalSettingInstance().getByResourceID(SDT_Model, strResourceId);
        break;
    case SDT_AreaContour:
        pTemplate = GetGlobalSettingInstance().getByResourceID(SDT_AreaContour, strResourceId);
        break;
    case SDT_Radar:
        break;
    case SDT_VehicleSimulation: //车辆仿真
        pTemplate = GetGlobalSettingInstance().getByResourceID(SDT_VehicleSimulation, strResourceId);
        break;
    case SDT_POI: {
        pTemplate = GetGlobalSettingInstance().getByResourceID(SDT_POI, strResourceId);
        cJSON* poiStyleId = GetGlobalSettingInstance().getByName(SDT_POI, strResourceId, "poiStyleId");
        if (pTemplate && poiStyleId)
        {
            cJSON_AddItemToObject(pTemplate, "_poiStyleID", poiStyleId);
        }
        break;
    }
    case SDT_PathBrowsing:
        pTemplate = GetGlobalSettingInstance().getByResourceID(SDT_PathBrowsing, strResourceId);
        break;
    case SDT_DataPannel: //数据面片，新版本支持模板
        pTemplate = GetGlobalSettingInstance().getByResourceID(SDT_DataPannel, strResourceId);
        break;
    default:
        break;
    }

    //组装3d json
    bool bBlend2DData = false; //释放是模板数据面片
    if (nType == 5) {
        if (pTemplate)
        {
            bBlend2DData = true;
            strType = "Blend2DData";
        }
    }

    cJSON* p3dRequest = cJSON_CreateObject();
    cJSON_AddStringToObject(p3dRequest, "id", "");
    cJSON_AddStringToObject(p3dRequest, "element", strType.c_str());
    cJSON_AddStringToObject(p3dRequest, "method", "add");

    switch (nType)
    {
    case SDT_DataPannel:
    case SDT_SceneEffects:
    {
        if (nType == SDT_SceneEffects || nType == SDT_DataPannel)
        {
            cJSON* pAttr = GetGlobalSettingInstance().getByName((SceneDataType)nType, strResourceId, "attributeList");
            if (pAttr)
            {
                cJSON_AddItemToObject(pElement, "attributeList_templte", pAttr); //添加特殊字段，如果attributeList为空，先添加模板
            }
        }

        if (bBlend2DData)
        {
            cJSON* pItem = genBlend2DDataJson(elementData, pTemplate, strResourceId);
            if (!pItem)
                pItem = cJSON_CreateObject();

            cJSON_AddStringToObject(pItem, "_id", strId.c_str()); //将id 传入元素data参数，外部参数无效
            cJSON_AddItemToObject(p3dRequest, "data", pItem);
        }
        else {
            cJSON* pItem = genEffectJson(elementData, pTemplate, strResourceId, nType);
            if (!pItem)
                pItem = cJSON_CreateObject();
            cJSON_AddStringToObject(pItem, "_id", strId.c_str()); //将id 传入元素data参数，外部参数无效
            cJSON_AddItemToObject(p3dRequest, "data", pItem);
        }
    }
    break;
    case SDT_HighlightArea:
    case SDT_HeatMap:
    case SDT_RiverSystem:
    case SDT_VehicleSimulation:
    case SDT_VirtualCamera:
    case SDT_ViewPath:
    case SDT_ModelBase:
    case SDT_SceneRoame:
    case SDT_Text:
    case SDT_Model: {
        cJSON* pItem = genModelJson(elementData, pTemplate);
        if (!pItem)
            pItem = cJSON_CreateObject();
        cJSON_AddStringToObject(pItem, "_id", strId.c_str()); //将id 传入元素data参数，外部参数无效
        cJSON_AddItemToObject(p3dRequest, "data", pItem);
    }
                  break;
    case SDT_POI: {
        cJSON* pAttr = GetGlobalSettingInstance().getByName(SDT_POI, strResourceId, "attributeList");
        if (pAttr)
        {
            cJSON_AddItemToObject(pElement, "attributeList_templte", pAttr); //添加特殊字段，如果attributeList为空，先添加模板
        }

        cJSON* pItem = genPoiJson(elementData, pTemplate);
        cJSON_AddStringToObject(pItem, "_id", strId.c_str()); //将id 传入元素data参数，外部参数无效

        cJSON_AddItemToObject(p3dRequest, "data", pItem);
    }
                break;
    case SDT_AreaContour:  //SDT_AreaContour，SDT_PathBrowsing都是根据resourceid 获取_textureImageUrl
    case SDT_PathBrowsing: {
        cJSON* pItem = genPathBrowsing(elementData, pTemplate);
        if (!pItem)
            pItem = cJSON_CreateObject();
        cJSON_AddStringToObject(pItem, "_id", strId.c_str()); //将id 传入元素data参数，外部参数无效
        cJSON_AddItemToObject(p3dRequest, "data", pItem);
    }
                break;
    case SDT_Radar:
    default: {
        cJSON* pItem = cJSON_Duplicate(elementData, true);/*cJSON_CreateObject();*/
        cJSON_AddStringToObject(pItem, "_id", strId.c_str()); //将id 传入元素data参数，外部参数无效
        cJSON_AddItemToObject(p3dRequest, "data", pItem);
        }
           break;
    }

    m_mapAddElement.emplace(strId, std::pair<int, cJSON*>(nType, cJSON_Duplicate(pElement, true)));

    if (pTemplate)
        cJSON_Delete(pTemplate);

    return p3dRequest;
}