#pragma once
#include "CJsonBase.h"
#include <iostream>
#include <memory>
#include <cstring>
#include <vector>
#include <map>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <atomic>

#include "CTagsManager.h"
#include <unordered_map>
#include "CBusinessAttributesManager.h"
#include "CMyCond.h"

/*
* 将arry类型的保存成cjson
    array
    isLockArray
    data
    elementData
    checkedArray
    tagList
    visibleArray
    foreignKey
*/

struct Node {
    std::shared_ptr<char>                       resourceId;                 //资源ID
    //std::shared_ptr<char>                   isLockArray;                  //字符串不能完成还原所以保存成cjson
    std::shared_ptr<cJSON>                      isLockArray;
    std::shared_ptr<cJSON>                      data;                         //ui数据+三维数据
    std::shared_ptr<cJSON>                      elementData;                  //三维数据
    std::shared_ptr<char>                       strType;
    std::shared_ptr<char>                       icon;
    std::shared_ptr<char>                       opacityStyleColor;
    std::shared_ptr<char>                       buildID;
    std::unique_ptr<int>                        type;
    std::unique_ptr<int>                        secondType;
    std::unique_ptr<bool>                       opacityStyleShowBuildOpacity;
    std::shared_ptr<char>                       sceneId;
    std::unique_ptr<bool>                       isDrag;
    std::shared_ptr<char>                       bindGuid;
    std::shared_ptr<cJSON>                      checkedArray;
    std::unique_ptr<int>                        order;
    std::unique_ptr<bool>                       opacityStyleOpen;
    std::shared_ptr<double>                     version;
    std::shared_ptr<char>                       url;
    std::shared_ptr<cJSON>                      tagList;
    std::unique_ptr<bool>                       isExpand;
    std::shared_ptr<cJSON>                      visibleArray;
    std::unique_ptr<bool>                       showSetBuild;
    std::shared_ptr<char>                       name;
    std::shared_ptr<char>                       guid;
    std::unique_ptr<bool>                       opacityStyleVisible;
    std::shared_ptr<cJSON>                      foreignKey;
    std::shared_ptr<cJSON>                      attributeList;
    std::string                                 strForeignKey;
    std::shared_ptr<char>                       projectId;
    std::unique_ptr<int>                        floorIndex;
    std::unique_ptr<int>                        opacityStyleOpacity;
    std::shared_ptr<Node>                       parent;
    std::shared_ptr<cJSON>                      viewPointList;
    std::vector<std::shared_ptr<Node>>          childArray;                   //子节点
    cJSON*                                      pJson;                        //这里的pJosn 保存的是大json中对应的节点,所以不需释放，有大json维护。     
    std::unordered_map<std::string, std::shared_ptr<char>> index;             //方便过滤输出
    int                                         order2 = -1;
};

//输出过滤类
class ColumnElement
{
public:
    ColumnElement() : bSecondLevel(false){};
    ~ColumnElement() {};

public:
    std::string strKey;     //属性字段名称
    bool        bSecondLevel;//是否包含第二层数据过滤
    std::vector<std::string> vecSecondLevelKey; //如果制定第二层要显示的字段把字段名称放到数组中
};

/*
*  CTreeItem 的所有操作不是线程安全的，如果多线程处理，外部需要加锁处理
*/
class CTreeItem : public CJson_base
{
public:
    CTreeItem(cJSON* p);
    CTreeItem(cJSON* p, std::string strSenceId);
    ~CTreeItem();

    //json数据转换成tree
    std::shared_ptr<Node> parseJsonToTree(cJSON* jsonData, std::shared_ptr<Node> parent);


    //将tree转换成json
    cJSON* parseTreeToJson(std::shared_ptr<Node> node, bool recurse, bool bShowData=true, bool bCheckShow=true);

    //添加节点
    std::shared_ptr<Node> addChildNode(std::shared_ptr<Node> parentNode);

    //删除节点
    bool deleteNode(std::shared_ptr<Node> node);

    //修改节点
    bool modifyNode(std::shared_ptr<Node> node, const std::string& newName, int newAge); //for test

    //将所有原始数据添加到3维场景
    int loadElementDataTo3D();

    int getModeCount() { return m_nModeCount; }
    int getLayConut() { return m_nLayerCount; }

    cJSON* getElementJson(std::string id);

    //update 制定元素tags
    int updateTagsByElementId(std::string strId, cJSON* pTags, std::string updateType,std::string& strRespones);
    int updateTagsByElementId_Replace(std::string strId, cJSON* pTags, std::string& strRespones);
    int updateTagsByNode_Replace(std::shared_ptr<Node> pNode, cJSON* pTags, std::string& strRespones);
    int updateTagsByElementId_Add(std::string strId, cJSON* pTags, std::string& strRespones);
    int updateTagsByElementId_Delete(std::string strId, cJSON* pTags, std::string& strRespones);

    //条件解析器
    bool conditionPaser(std::string request, std::string strMethod,
        std::unordered_map<std::string, std::pair<bool, std::vector<std::string>>>& FCondition,
        std::unordered_map<std::string, std::pair<int, std::vector<std::string>>>& Condition,
        std::vector<ColumnElement>& column);

    //前置搜索需要的接口
    // 遍历guid 
    void ergodicGuid(bool nState, bool bAll, std::vector<std::string> vecCondition, std::unordered_map<std::string,std::shared_ptr<Node>> &retMap);

    //查询此ID下的childid
    void ergodicParentId(bool nState, bool bAll, std::vector<std::string> vecCondition, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap);

    // 遍历外键
    void ergodicForeignKey(bool nState, bool bAll, std::vector<std::string> vecCondition, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap);
    //遍历 tags
    void ergodicTags(bool nState, bool bAll, std::vector<std::string> vecCondition, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap);
    //遍历type
    void ergodicType(bool nState, bool bAll, std::vector<std::string> vecCondition, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap);

    //遍历业务属性
    void ergodicAttributeList(bool nState, bool bAll, std::vector<std::string> vecCondition, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap);

  
    // 视野内
    void ergodicViewBy3D(bool nState, bool bAll, std::vector<std::string> vecCondition, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap);
    
    // 区域内
    void ergodicAreaBy3D(bool nState, bool bAll, std::vector<std::string> vecCondition, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap, std::vector<std::shared_ptr<Node>> &retVector);

    //后置条件遍历搜索
    void ergodicCondition(bool bAll, std::unordered_map<std::string, std::pair<int, std::vector<std::string>>> mapCondition, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap);
    void ergodicConditionColumn(std::string strColumnName, std::pair<int, std::vector<std::string>> mapCondition, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap);

    //输出显示控制
    void ColumnPrint(std::vector<ColumnElement> vecColumn, std::unordered_map<std::string, std::shared_ptr<Node>>& retMap, std::string& strResone);

    void ColumnPrintVecotr(std::vector<ColumnElement> vecColumn, std::vector<std::shared_ptr<Node>>& retVector, std::string& strResone);
   
    //元素相关接口
    int addElement(std::string request, std::string& strRespone);
    int addElementImpl(int nType,cJSON* pElement, std::string& strRespone,std::string &strId);

    //元素添加支持批量
    int addElementArrary(std::string request, std::string& strRespone);
    cJSON* addElementImpl2(int nType, cJSON* pElement, std::string strId); //返回真实添加json array，批量通过sdk添加

    int notifyAddModel(std::string id, bool bSucess, bool bNeedAdd);
    int notifyDelModel(std::string id);
    int notifyUpdateMode(std::string id);

    cJSON* genModelJson(cJSON* pElement, cJSON* pTemplate);
    cJSON* genEffectJson(cJSON* pElement, cJSON* pTemplate,std::string resouceId,int elementType);
    cJSON* genPoiJson(cJSON* pElement, cJSON* pTemplate);

    cJSON* genPathBrowsing(cJSON* pElement, cJSON* pTemplate);

    cJSON* genBlend2DDataJson(cJSON* pElement, cJSON* pTemplate, std::string resouceId);

    int deleteElement(std::string request, std::string& strRespone);
    int updateElement(std::string request, std::string& strRespone,/* int &nCode,*/std::string &strMessage);
    void getElement(std::string request, std::string& strRespone);
    int controlElement(std::string request, std::string& strRespone);

    //elementhandle 通用接口
    int GeneralInterface(std::string request, std::string& strRespone);

    //eventHandle 事件通用接口
    int EventGeneralInterface(std::string request, std::string& strRespone);

    void getElementTree(std::string request, std::string& strRespone);

    void getElementTree2(std::string request, std::string& strRespone);

    void getElementBuildID(std::string request, std::string& strRespone);

    int deleteNode(std::string strId);
    int updateElementNode(std::vector<cJSON*> vecChangeParam, std::string strId, bool bAdd = false);

    int parseJsonModifyNode(cJSON* vecChangeParam, std::shared_ptr<Node> pNode,bool bAdd = false);

    //更新node elementdata
    int updateNodeElementData(cJSON* pElementData, std::shared_ptr<Node> pNode);

    std::shared_ptr<Node> getElementNode(std::string id);

    cJSON* toJson() {
        return parseTreeToJson(m_pElementNode, true);
    };

    void setCurSenceId(std::string strId) { m_strCurSenceId = strId; }
    void setSenceList(std::vector<std::string> vecSence) { m_vecSence.assign(vecSence.begin(), vecSence.end()); };

    //判断当前场景下元素或者图层是否勾选
    bool isCheck(cJSON* item, std::string strSenceId);

    //从3d中获取elementdata json数据
    cJSON* getElementDataFrom3D(std::string id, std::string strType);
public:
    CTagsManager                                    m_tagManager;   //tag管理

    CBusinessAttributesManager                      m_AttriManager; //业务属性管理
    CMyCond							                m_cond;			//用于同步图层加载，
private:
    std::shared_ptr<Node>                           m_pElementNode; //根节点

    //存储元素节点,元素guid为key，node节点为value
    std::unordered_map<std::string, std::shared_ptr<Node>>    m_mapNode;
    //std::mutex				                    m_mutexNode; //如果多线程操作 所有节点操作，以及引用智能指针都需要加锁，不仅限于map，避免访问地址不存在

    //存储外键索引
    std::map<std::string, std::shared_ptr<Node>>    m_mapForeignKeyNode;  //外键id
    
    //存储外键为"[]"索引
    std::map<std::string, std::shared_ptr<Node>>    m_mapForeignKeyNullNode;  //node guid,Node

    std::atomic<int>                                m_nModeCount;
    std::atomic<int>                                m_nLayerCount;

    std::string                                     m_strGroupId;        //默认组id

    std::map<std::string, std::pair<int,cJSON *>>   m_mapAddElement;//等待添加到node中的节点,在3d回调中处理
    std::string                                     m_strCurSenceId;

    int                                             m_order = 0;    //所有带有elementData  _layerType，的加载顺序
    std::vector<std::string>                        m_vecSence;
};

