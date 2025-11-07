#include "stdafx.h"
#include "CallbackManager.h"
#include "SceneDataType.h"
#include "SceneCore/VideoManager.h"
#include "cJSON.h"
#include "config.h"

using namespace RTE;
#define Log LogN(1102)

#define CALLBACK_AND_LOG(event_name) \
    if (pOut) { \
        if (m_callback) \
        { \
            m_callback(event_name, pOut, m_pUser); \
        } \
        Log(Tool::Debug, "%s", pOut); \
        free(pOut); \
    }

CallbackManager::CallbackManager(CDataManager& resource) : m_resource(resource), m_nLoadMode(0)
{
}

CallbackManager::~CallbackManager()
{
}

int CallbackManager::Init()
{
    Interface::registerCallBack_3D(CBT_ADDMODEL, std::bind(&CallbackManager::CallBack_AddModeSuccess, this, std::placeholders::_1));
    Interface::registerCallBack_3D(CBT_ADDMODEL_ERROR, std::bind(&CallbackManager::CallBack_AddError, this, std::placeholders::_1));
    Interface::registerCallBack_3D(CBT_ADDNEWLAYER_ERROR, std::bind(&CallbackManager::CallBack_AddLayerError, this, std::placeholders::_1));
    Interface::registerCallBack_3D(CBT_PICKPOINT, std::bind(&CallbackManager::CallBack_3DPoint, this, std::placeholders::_1));
    Interface::registerCallBack_3D(CBT_PIXELCOORDS, std::bind(&CallbackManager::CallBack_PixelCoords, this, std::placeholders::_1)); //CBT_PIXELCOORDS
    //Interface::registerCallBack_3D(CBT_PIXELCOORDSEX, std::bind(&CallbackManager::CallBack_PixelCoordsEx, this, std::placeholders::_1)); //CBT_PIXELCOORDSEX 播放视频时返回像素宽高，
    Interface::registerCallBack_3D(CBT_PICKMODEL, std::bind(&CallbackManager::CallBack_PickModel, this, std::placeholders::_1));

    Interface::registerCallBack_3D(CBT_EYETOSF, std::bind(&CallbackManager::CallBack_EyeToSpaceFence, this, std::placeholders::_1));
    Interface::registerCallBack_3D(CBT_MODELTOSF, std::bind(&CallbackManager::CallBack_ModelToSpaceFence, this, std::placeholders::_1));

    Interface::registerCallBack_3D(CBT_SCENESTATE, std::bind(&CallbackManager::CallBack_SceneState, this, std::placeholders::_1));

    Interface::registerCallBack_3D(CBT_MOUSEDATAPOS, std::bind(&CallbackManager::CallBack_MouseDataPos, this, std::placeholders::_1));

    Interface::registerCallBack_3D(CBT_DELMODEL, std::bind(&CallbackManager::CallBack_DelModels, this, std::placeholders::_1));

    //设置视频相关回调
    Interface::registerCallBack_3D(CBT_VIDEO, std::bind(&CallbackManager::openVideoCallBack, this, std::placeholders::_1));
    VideoManager::setVideoCallback(Interface::dirtyImageData);

    //元素位置订阅
    Interface::registerCallBack_3D(CBT_PositionSubscribe, std::bind(&CallbackManager::CallBack_ElementPosition, this, std::placeholders::_1));

    Interface::registerCallBack_3D(CBT_ClientToScreen, std::bind(&CallbackManager::CallBack_ClientToScreen, this, std::placeholders::_1));

    // 虚拟相机进入离开监测回调
    Interface::registerCallBack_3D(CBT_ProjectorAreaDetectionModel, std::bind(&CallbackManager::CallBack_EnterOrLeaveMonitor, this, std::placeholders::_1));

    // 模型更新回调，主要处理数据驱动更新
    Interface::registerCallBack_3D(CBT_UPDATEMODEL, std::bind(&CallbackManager::CallBack_UpdateMode, this, std::placeholders::_1));

#ifdef WIN32
    //视频时间戳回调
    Interface::registerCallBack_3D(CBT_VideoTimeStamp, std::bind(&CallbackManager::Callback_VideoTimestamp, this, std::placeholders::_1));
#endif

    Interface::registerCallBack_3D(CBT_DataPannelClick, std::bind(&CallbackManager::Callback_DataPannelClickBackData, this, std::placeholders::_1));

    //球机追视附近相机回调
    Interface::registerCallBack_3D(CBT_OptimalCamera, std::bind(&CallbackManager::Callback_OptimalCameraBackData, this, std::placeholders::_1));

    //POI鼠标悬停回调
    Interface::registerCallBack_3D(CBT_PoiMouseHover, std::bind(&CallbackManager::Callback_PoiMouseHoverData, this, std::placeholders::_1));

    //第一人称漫游器录像结束回调
    Interface::registerCallBack_3D(CBT_FPM_RecordTrackEnd, std::bind(&CallbackManager::Callback_FPM_RecordTrackEndData, this, std::placeholders::_1));

    //第一人称进入退出回调
    Interface::registerCallBack_3D(CBT_FPM_InOut, std::bind(&CallbackManager::Callback_FPM_InOutData, this, std::placeholders::_1));

    //第一人称录像播放回调进度回调
    Interface::registerCallBack_3D(CBT_FPM_ReplayTrack, std::bind(&CallbackManager::Callback_FPM_ReplayTrackData, this, std::placeholders::_1));

    //interface::close
    Interface::registerCallBack_3D(CBT_CloseProject, std::bind(&CallbackManager::Callback_Close3D, this));

    //碰撞体触发回调：CBT_InsectNode
    Interface::registerCallBack_3D(CBT_InsectNode, std::bind(&CallbackManager::Callback_CBT_InsectNode, this, std::placeholders::_1));

    //视锥触发回调：CBT_CYNAMIC_VIDEO_TRIGGER_TriggerAction
    Interface::registerCallBack_3D(CBT_CYNAMIC_VIDEO_TRIGGER_TriggerAction, std::bind(&CallbackManager::Callback_CBT_CYNAMIC_VIDEO_TRIGGER_TriggerAction, this, std::placeholders::_1));

    //初始化订阅map
    InitSubscriMap();

	return 0; 
}

void CallbackManager::InitSubscriMap()
{
    m_mapSubscribType[3] = "model";
    m_mapSubscribType[24] = "model";
    m_mapSubscribType[4] = "video";
    m_mapSubscribType[5] = "data";
    m_mapSubscribType[7] = "orop";
    m_mapSubscribType[25] = "effect";
    m_mapSubscribType[26] = "poi";
    m_mapSubscribType[47] = "simplePoi";
    m_mapSubscribType[28] = "path";
    m_mapSubscribType[29] = "area";
    m_mapSubscribType[30] = "contour";
    m_mapSubscribType[31] = "fence";
    m_mapSubscribType[44] = "heatmap";
    m_mapSubscribType[45] = "vegetation";
    m_mapSubscribType[46] = "river";
    m_mapSubscribType[50] = "model";//"vehicle";
    m_mapSubscribType[49] = "model";//"virtualCamera";
}

int CallbackManager::NotifyProgramCompleted()
{
    std::string strRequese = R"(
        {
	        "method": "control",
	        "operate": {
		        "OpenProgramCompleted": {}
	        }
        }
    )";

    std::string strRespone;
    int nRet = Interface::elementHandle(strRequese, strRespone);
    if (nRet)
    {
        Log(Tool::Debug, "NotifyProgramCompleted error %d\n %s", nRet, strRespone.c_str());
    }
    return nRet;
}

void CallbackManager::openVideoCallBack(CallBackMsg msg)
{
    VideoCallBackData* data = msg->_data->getData<VideoCallBackData>();
    if (data) VideoManager::openVideo(*data);
}

void CallbackManager::CallBack_AddModeSuccess(CallBackMsg msg)
{
    AddModelCallBackData* pData = msg->_data ? msg->_data->getData<AddModelCallBackData>() : nullptr;
    if (pData && pData->_modelIDs.size()>0) {
        int nSum = m_resource.GetModelCount();
        if (m_resource.m_nLoadMode >= nSum) { //操作添加处理逻辑

            for (size_t i = 0; i < pData->_modelIDs.size(); i++)
            {
                int nRet = m_resource.notifyAddModel(pData->_modelIDs[i].c_str(), true, true);

                cJSON* pRoot = cJSON_CreateObject();
                cJSON_AddNumberToObject(pRoot, "code", 0);
                cJSON_AddStringToObject(pRoot, "id", pData->_modelIDs[i].c_str());
                cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
                cJSON_AddStringToObject(pRoot, "projectId", m_resource.m_strProjectId.c_str());
                cJSON_AddStringToObject(pRoot, "sceneId", m_resource.m_strSenceId.c_str());
                char* pOut = cJSON_Print(pRoot);

                CALLBACK_AND_LOG("ADD_MODEL");
                cJSON_Delete(pRoot);
            }
        }
        else { //启动加载时处理逻辑//启动时单独加载所以只取_modelIDs第一个即可

            int nRet = m_resource.notifyAddModel(pData->_modelIDs[0].c_str(), true, false); //因为要还原pms中的json，所以在加载时不需要等待回调已经添加到mapnode中，
                                                                                            //因此这里不需要在添加到map，最后一个参数为false。
            m_resource.m_nLoadMode++;
            double f = (double)m_resource.m_nLoadMode / (double)nSum;
            int nProcess = f * 100;

            cJSON* pRoot = cJSON_CreateObject();
            cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
            cJSON_AddStringToObject(pRoot, "projectId", m_resource.m_strProjectId.c_str());
            cJSON_AddStringToObject(pRoot, "sceneId", m_resource.m_strSenceId.c_str());
            cJSON_AddNumberToObject(pRoot, "process", nProcess);

            char* pOut = cJSON_Print(pRoot);
            CALLBACK_AND_LOG("LOADPROJECT_PROCESS");
            cJSON_Delete(pRoot);

            bool bComplite = f == 1.0 ? true : false;

            cJSON_AddBoolToObject(pRoot, "result", true);

            if (bComplite || m_resource.m_nLoadMode>= nSum) {
                m_resource.LoadFPM_Data();
                m_resource.LoadGraphicsConfig();
                m_resource.m_bLoadComplite.store(true);
                m_resource.SetMainViewPoint();

                Log(Tool::Debug, "Load 3d models Complite");
                NotifyProgramCompleted();
                if (m_callback)
                    m_callback("LOADPROJECT_COMPLITE", "true", m_pUser);
            }
        }   
    }
}

void CallbackManager::CallBack_AddError(CallBackMsg msg)
{
    AddModelErrorCallBackData* pData = msg->_data ? msg->_data->getData<AddModelErrorCallBackData>() : nullptr;
    if (pData && pData->_modelIDs.size() > 0) {
        int nSum = m_resource.GetModelCount();
        if (m_resource.m_nLoadMode >= nSum) { //新添加的
            for (size_t i = 0; i < pData->_modelIDs.size(); i++)
            {
                int nRet = m_resource.notifyAddModel(pData->_modelIDs[i].c_str(), false, true);
                cJSON* pRoot = cJSON_CreateObject();
                cJSON_AddNumberToObject(pRoot, "code", 1);
                cJSON_AddStringToObject(pRoot, "id", pData->_modelIDs[i].c_str());
                cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
                cJSON_AddStringToObject(pRoot, "projectId", m_resource.m_strProjectId.c_str());
                cJSON_AddStringToObject(pRoot, "sceneId", m_resource.m_strSenceId.c_str());
                char* pOut = cJSON_Print(pRoot);
                CALLBACK_AND_LOG("ADD_MODEL");
                cJSON_Delete(pRoot);
            }
        }
        else { //初始化加载失败
            m_resource.m_nLoadMode++;
            m_resource.notifyAddModel(pData->_modelIDs[0].c_str(), false, false);
            int nSum = m_resource.GetModelCount();
            double f = (double)m_resource.m_nLoadMode / (double)nSum;

            int nProcess = f * 100;
            cJSON* pRoot = cJSON_CreateObject();
            cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
            cJSON_AddStringToObject(pRoot, "projectId", m_resource.m_strProjectId.c_str());
            cJSON_AddStringToObject(pRoot, "sceneId", m_resource.m_strSenceId.c_str());
            cJSON_AddNumberToObject(pRoot, "process", nProcess);
            bool bComplite = f == 1.0 ? true : false;
            cJSON_AddBoolToObject(pRoot, "result", false);

            char* pOut = cJSON_Print(pRoot);
            CALLBACK_AND_LOG("LOADPROJECT_PROCESS");
            cJSON_Delete(pRoot);

            if (bComplite || m_resource.m_nLoadMode >= nSum) {
                m_resource.LoadFPM_Data();
                m_resource.LoadGraphicsConfig();
                m_resource.m_bLoadComplite.store(true);
                m_resource.SetMainViewPoint();
                NotifyProgramCompleted();
                if (m_callback)
                    m_callback("LOADPROJECT_COMPLITE", "true", m_pUser);
            }
        }
    }
}

void CallbackManager::CallBack_DelModels(CallBackMsg msg)
{
    DelModelCallBackData* pData = msg->_data ? msg->_data->getData<DelModelCallBackData>() : nullptr;
    if (pData && pData->_modelIDs.size()>0)
    {
        m_resource.notifyDelModel(pData->_modelIDs);
    }
}

void CallbackManager::CallBack_3DPoint(CallBackMsg msg)
{
    //获取订阅信息
    auto bState = m_resource.GetCallbackState("coordinate");

     if (bState) {
        PickPointCallBackData* pData = msg->_data ? msg->_data->getData<PickPointCallBackData>() : nullptr;
        if (pData) {
            cJSON* pRoot = cJSON_CreateObject();
            cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
            cJSON_AddNumberToObject(pRoot, "longitude", pData->_mapPoint.x);
            cJSON_AddNumberToObject(pRoot, "latitude", pData->_mapPoint.y);
            cJSON_AddNumberToObject(pRoot, "height", pData->_mapPoint.z);

            std::string strClick = pData->_doubleClick ? "double" : "single";
            cJSON_AddStringToObject(pRoot, "click", strClick.c_str());
            char* pOut = cJSON_Print(pRoot);

            CALLBACK_AND_LOG("PICKPOINT");
            cJSON_Delete(pRoot);
        }
    }
}

void CallbackManager::CallBack_AddLayerError(CallBackMsg msg)
{
    AddNewLayerErrorCallBackData* pData = msg->_data ? msg->_data->getData<AddNewLayerErrorCallBackData>() : nullptr;
    if (pData) {
        Log(Tool::Debug, "%s", "CallBack_AddLayerError");
        m_resource.m_nLoadMode++;
    }
}


/*******************************************************************************
* 函数名称：
* 功能描述：模型点击回调 主题： "PICKELEM_POI"; 
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/
void CallbackManager::CallBack_PickModel(CallBackMsg msg)
{
    PickModelCallBackData* pData = msg->_data ? msg->_data->getData<PickModelCallBackData>() : nullptr;
    if (pData) {

        //这里操作node节点，所以需要加锁
        std::shared_lock lock(m_resource.m_mutexTree2);
        for (auto itor : pData->_modelIDs)
        {
            cJSON* pSend = cJSON_CreateObject();
            auto itorNode = m_resource.m_pTreeItem->getElementNode(itor);
            if (!itorNode) {
                cJSON_Delete(pSend);
                continue;
            }

            auto itor2 = m_mapSubscribType.find(*itorNode->type);
            if (itor2 == m_mapSubscribType.end()) {
                cJSON_Delete(pSend);
                continue;
            }
            std::string strKey = itor2->second;

            //获取订阅信息
            bool bState = m_resource.GetCallbackState(strKey);
            if (!bState) {
                cJSON_Delete(pSend);
                continue;
            }

            std::string strType = RTE::getModelStrTypeByType(*itorNode->type);
            cJSON_AddStringToObject(pSend, "taskId", GetConfigSettingInstance().GetAppId().c_str());
            cJSON_AddStringToObject(pSend, "id", itorNode->guid.get());
            cJSON_AddStringToObject(pSend, "type", strType.c_str());
            cJSON_AddNumberToObject(pSend, "proportionX", pData->_radiox);
            cJSON_AddNumberToObject(pSend, "proportionY", pData->_radioy);
            cJSON_AddStringToObject(pSend, "click", pData->_doubleClick ? "double" : "single");

            bool bFindBuild = false; //是否找到所属建筑
            int nFloor = 0;
            std::string strBuildingId;
            std::string strFloorId;
            for ( auto parent = itorNode->parent; parent!=nullptr && parent->guid != nullptr && strcmp(parent->guid.get(),"root"); parent = parent->parent)
            {
                if (*parent->type == SDT_Build)
                {
                    strBuildingId = parent->guid.get();
                    break;
                }

                if (*parent->type == SDT_Floor) {
                    //在这里根据再父节点 数组中的位置确定楼层
                    strFloorId = parent->guid.get();
                    /*auto tempNode = parent->parent;
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
                    }*/// 不需要写入楼层
                }
            }

            cJSON_AddNumberToObject(pSend, "floor", -1); //默认-1
            cJSON_AddStringToObject(pSend, "floorId", strFloorId.c_str());
            cJSON_AddStringToObject(pSend, "parentBuildingID", strBuildingId.c_str());
            char* pOut = cJSON_Print(pSend);
            
            CALLBACK_AND_LOG("PICKPOINT");
            cJSON_Delete(pSend);
        }
    }
}

/*******************************************************************************
* 函数名称：
* 功能描述：view 空间触发器回调
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/
void CallbackManager::CallBack_EyeToSpaceFence(CallBackMsg msg)
{
    //获取订阅信息
    bool bState = m_resource.GetCallbackState("fence");
    if (!bState) {
        return;
    }
    EyeToSFCallBackData* data = msg->_data ? msg->_data->getData<EyeToSFCallBackData>() : nullptr;
    if (data)
    {
        cJSON* pRoot = cJSON_CreateObject();
        cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
        cJSON_AddStringToObject(pRoot, "spaceFenceId", data->_sfID.c_str());
        cJSON_AddStringToObject(pRoot, "trigger", "view");
        cJSON_AddStringToObject(pRoot, "modelId", "");
        cJSON_AddNumberToObject(pRoot, "triggerType", data->_toSFState);

        char* pOut = cJSON_Print(pRoot);
        CALLBACK_AND_LOG("TRIGGER_SPACEFENCE");
        cJSON_Delete(pRoot);
    }
}

/*******************************************************************************
* 函数名称：
* 功能描述：model 空间触发器回调
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
* 修改日期		修改人			修改内容
* ------------------------------------------------------------------------------
* @@Today		张斌	      	创建
*******************************************************************************/
void CallbackManager::CallBack_ModelToSpaceFence(CallBackMsg msg)
{
    //获取订阅信息
    bool bState = m_resource.GetCallbackState("fence");
    if (!bState) {
        return;
    }
    ModelToSFCallBackData* data = msg->_data ? msg->_data->getData<ModelToSFCallBackData>() : nullptr;
    if (data)
    {
        cJSON* pRoot = cJSON_CreateObject();
        cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
        cJSON_AddStringToObject(pRoot, "spaceFenceId", data->_sfID.c_str());
        cJSON_AddStringToObject(pRoot, "trigger", "model");
        cJSON_AddStringToObject(pRoot, "modelId", data->_modelID.c_str());
        cJSON_AddNumberToObject(pRoot, "triggerType", data->_toSFState);

        char* pOut = cJSON_Print(pRoot);

        CALLBACK_AND_LOG("TRIGGER_SPACEFENCE");
        cJSON_Delete(pRoot);
    }
}

void CallbackManager::CallBack_PixelCoords(CallBackMsg msg)
{
    //获取订阅信息,默认true
    auto bState = m_resource.GetCallbackState("pixelPoint");

    if (bState) {
        PixelCallBackData *pData = msg->_data->getData<PixelCallBackData>();
        if (pData) {
            cJSON* pRoot = cJSON_CreateObject();
            cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
            //cJSON_AddStringToObject(pRoot, "videoId", pData->);
            cJSON_AddNumberToObject(pRoot, "x", pData->_pixelCoords.x);//宽百分比
            cJSON_AddNumberToObject(pRoot, "y", pData->_pixelCoords.y);//高百分比
            cJSON_AddNumberToObject(pRoot, "z", pData->_pixelCoords.z);//宽
            cJSON_AddNumberToObject(pRoot, "w", pData->_pixelCoords.z);//高

            std::string strClick = pData->_doubleClick ? "double" : "single";
            cJSON_AddStringToObject(pRoot, "click", strClick.c_str());

            char* pOut = cJSON_Print(pRoot);
            CALLBACK_AND_LOG("VIDEO_PIXEL");
            cJSON_Delete(pRoot);
        }
    }
}

void CallbackManager::CallBack_PixelCoordsEx(CallBackMsg msg)
{
    //获取订阅信息,默认true
    //auto bState = m_resource.GetCallbackState("coordinate");
    auto bState = m_resource.GetCallbackState("pixelPoint");

    if (bState) {
        //PickPointCallBackData* pData = msg->_data->getData<PickPointCallBackData>();
        PixelCallBackDateEx* pData = msg->_data->getData<PixelCallBackDateEx>();
        if (pData) {
            cJSON* pRoot = cJSON_CreateObject();
            cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
            //cJSON_AddStringToObject(pRoot, "videoId", pData->);
            cJSON_AddNumberToObject(pRoot, "x", pData->_pixelCoords.x);
            cJSON_AddNumberToObject(pRoot, "y", pData->_pixelCoords.y);
            cJSON_AddNumberToObject(pRoot, "z", pData->_pixelCoords.z);

            //std::string strClick = pData->_doubleClick ? "double" : "single";
            //cJSON_AddStringToObject(pRoot, "click", strClick.c_str());
            char* pOut = cJSON_Print(pRoot);
            CALLBACK_AND_LOG("VIDEO_PIXEL");
            cJSON_Delete(pRoot);
        }
    }
}

void CallbackManager::CallBack_SceneState(CallBackMsg msg)
{
    SceneStateCallBackData* data = msg->_data ? msg->_data->getData<SceneStateCallBackData>() : nullptr;
    bool bVideoState = false;
    if (data)
    {
        bool bCallback = false;
        int nType = (int)data->_stateType;
        switch (nType)
        {
        case modelPathFlyEnd:
        case modelPathFlyBegin:
        case modelPathFlyInProcess:
            bCallback = m_resource.GetCallbackState("modelPathFly");
            break;
        case cameraPathFlyEnd:
        case cameraPathFlyBegin:
        case cameraPathFlyInProcess:
            bCallback = m_resource.GetCallbackState("cameraPathFly");
            break;
        case cameraVPFlyEnd:
        case cameraVPFlyBegin:
        case cameraVPFlyInProcess:
            bCallback = m_resource.GetCallbackState("cameraVPFly");
            break;
        case videoStart:
        case VideoDecoderError:
        case videoStop:
        case videoPause:
        case videoResume:
        {
            bCallback = m_resource.GetCallbackState("video");
            bVideoState = true;
            break;
        }
        case pathFlyBegin:
        case pathFlyPause:
        case pathFlyEnd:
            bCallback = m_resource.GetCallbackState("pathFly");
            break;
        case inVirtualCamera:
        case outVirtualCamera:
            bCallback = m_resource.GetCallbackState("virtualCamera");
            break;
        default:
            break;
        }

        if (bCallback)
        {
            cJSON* pRoot = cJSON_CreateObject();
            cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
            cJSON_AddNumberToObject(pRoot, "sceneState", data->_stateType);
            if (bVideoState)
            {
                std::string id;
                if (data->_fusionID.compare(""))
                {
                    id = data->_fusionID;
                }
                else {
                    id = data->_modelID;
                }
                cJSON_AddStringToObject(pRoot, "id", id.c_str());
                cJSON_AddNumberToObject(pRoot, "index", data->_index);
            }
            else {
                cJSON_AddStringToObject(pRoot, "id", data->_modelID.c_str());
            }
            if (!data->_pathID.empty())
            {
                cJSON_AddStringToObject(pRoot, "targetId", data->_pathID.c_str());
            }

            if (data->_cPointIndex != -1)
            {
                cJSON_AddNumberToObject(pRoot, "pointIndex", data->_cPointIndex);
            }

            char* pOut = cJSON_Print(pRoot);
            CALLBACK_AND_LOG("SCENE_STATE");
            cJSON_Delete(pRoot);
        }
    }
}

void CallbackManager::CallBack_MouseDataPos(CallBackMsg msg)
{
    //return; //暂时不用
    //获取订阅信息
    bool bState = m_resource.GetCallbackState("lonlatData");
    if (bState)
    {
        mouseDataInfoCallBackData* pData = msg->_data ? msg->_data->getData<mouseDataInfoCallBackData>() : nullptr;
        if (pData)
        {
            cJSON* pRoot = cJSON_CreateObject();
            cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
            //cJSON_AddStringToObject(pRoot, "videoId", pData->);
            cJSON_AddNumberToObject(pRoot, "lon", pData->_lon);
            cJSON_AddNumberToObject(pRoot, "lat", pData->_lat);
            cJSON_AddNumberToObject(pRoot, "height", pData->_height);
            cJSON_AddNumberToObject(pRoot, "heading", pData->_heading);
            cJSON_AddNumberToObject(pRoot, "pitch", pData->_pitch);
            cJSON_AddNumberToObject(pRoot, "range", pData->_roll);

            char* pOut = cJSON_Print(pRoot);
            CALLBACK_AND_LOG("LONLAT_DATA");
            cJSON_Delete(pRoot);
        }
    }
}

void CallbackManager::CallBack_ElementPosition(CallBackMsg msg)
{
    auto bState = m_resource.GetCallbackState("elementPosition");
    if (!bState)
        return;

    PositionSubscribeCallbackData* data = msg->_data ? msg->_data->getData<PositionSubscribeCallbackData>() : nullptr;
    if (data)
    {
        cJSON* pRoot = cJSON_CreateObject();
        cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
        
        cJSON* pPositions = cJSON_CreateArray();
        for (const auto& tuple : data->_changeList)
        {
            cJSON* pItem = cJSON_CreateObject();
            std::string strValue = std::get<0>(tuple);
            cJSON_AddStringToObject(pItem, "id", strValue.c_str());
            YZPoint2d point2dValue = std::get<1>(tuple);
            cJSON_AddItemToObject(pItem, "screenCoord", cJSON_Parse(point2dValue.toJson().c_str()));
            YZPoint pointValue = std::get<2>(tuple);
            cJSON_AddItemToObject(pItem, "worldCoord", cJSON_Parse(pointValue.toJson().c_str()));

            cJSON_AddItemToArray(pPositions, pItem);
        }
        cJSON_AddItemToObject(pRoot, "positions", pPositions);

        char* pOut = cJSON_Print(pRoot);
        CALLBACK_AND_LOG("ELEMENT_POSITION");
        cJSON_Delete(pRoot);
    }
}

void CallbackManager::CallBack_ClientToScreen(CallBackMsg msg)
{
#if 0
    //暂时没有需求
    ClientToScreenCallBackData* data = msg->_data ? msg->_data->getData<ClientToScreenCallBackData>() : nullptr;
    if (data)
    {
        data->_minScreenCoord.x = Config::CConfig::GetInstance().m_rect.left;
        data->_minScreenCoord.y = Config::CConfig::GetInstance().m_rect.top;

        data->_maxScreenCoord.x = Config::CConfig::GetInstance().m_rect.right;
        data->_maxScreenCoord.y = Config::CConfig::GetInstance().m_rect.bottom;
    }
#endif
}

//(鞍钢版本底层已经弃用，状态回调在 SceneState中触发)
void CallbackManager::CallBack_EnterOrLeaveMonitor(CallBackMsg msg)
{
    projectorAreaDetectionModelCallbackData* data = msg->_data ? msg->_data->getData<projectorAreaDetectionModelCallbackData>() : nullptr;
    if (data)
    {
        cJSON* pRoot = cJSON_CreateObject();
        cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
        cJSON_AddStringToObject(pRoot, "modelId", data->_modelID.c_str());
        cJSON_AddStringToObject(pRoot, "virtualCameraId", data->_projectorID.c_str());
        cJSON_AddBoolToObject(pRoot, "isInside", data->_inside);

        char* pOut = cJSON_Print(pRoot);
        CALLBACK_AND_LOG("ENTER_LEAVE_VIRTUALCAMERA");
        cJSON_Delete(pRoot);
    }
}


void CallbackManager::CallBack_UpdateMode(CallBackMsg msg)
{
    updateModelCallBackData* data = msg->_data ? msg->_data->getData<updateModelCallBackData>() : nullptr;
    if (data)
    {
        m_resource.notifyUpdateModel(data->_modelIDs);
    }
}

void CallbackManager::Callback_VideoTimestamp(CallBackMsg msg)
{
#ifdef WIN32
    bool bState = m_resource.GetCallbackState("videoTimestamp");
    if (bState) {
        videoTimeStampCallBackData* data = msg->_data ? msg->_data->getData<videoTimeStampCallBackData>() : nullptr;
        if (data)
        {
            cJSON* pRoot = cJSON_CreateObject();
            cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
            cJSON_AddStringToObject(pRoot, "modelId", data->_modelID.c_str());
            cJSON_AddNumberToObject(pRoot, "index", data->_index);
            cJSON_AddNumberToObject(pRoot, "timeStamp", data->_timeStamp);

            char* pOut = cJSON_Print(pRoot);
            CALLBACK_AND_LOG("VIDEO_TIMESTAMP");
            cJSON_Delete(pRoot);
        }
    }
#endif
}

void CallbackManager::Callback_DataPannelClickBackData(CallBackMsg msg)
{
    bool bState = true;//m_resource.GetCallbackState("videoTimestamp");
    if (bState) {
        DataPannelClickCallBackData* data = msg->_data ? msg->_data->getData<DataPannelClickCallBackData>() : nullptr;
        if (data)
        {
            cJSON* pRoot = cJSON_CreateObject();
            cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
            cJSON_AddStringToObject(pRoot, "modelId", data->_modelID.c_str());
            cJSON_AddStringToObject(pRoot, "childrenName", data->_childrenName.c_str());
            cJSON_AddStringToObject(pRoot, "childID", data->_childID.c_str());
            cJSON_AddNumberToObject(pRoot, "eventType", data->_eventType);

            char* pOut = cJSON_Print(pRoot);
            CALLBACK_AND_LOG("DATA_PANNEL_CLICK");
            cJSON_Delete(pRoot);

            if (!data->_eventType) // 0 隐藏数据面片
            {
                std::shared_ptr<cJSON>pElement = std::shared_ptr<cJSON>(cJSON_CreateObject(), cJSON_Delete);
                cJSON_AddStringToObject(pElement.get(), "id", data->_modelID.c_str());
                cJSON_AddNumberToObject(pElement.get(), "type", 5);

                cJSON* pelementData = cJSON_CreateObject();
                cJSON_AddNumberToObject(pelementData, "_modelType", 50);
                cJSON_AddBoolToObject(pelementData, "_displayModel", false);

                cJSON_AddItemToObject(pElement.get(), "elementData", pelementData);

                char* pdata = cJSON_Print(pElement.get());

                std::thread thrd1([&, pElement]() {
                    if (pElement) {
                            char* pdata = cJSON_Print(pElement.get());
                            if (pdata)
                            {
                                Log(Tool::Debug, "thread update _displaymodel");
                                std::string request, strRespone;
                                request = pdata;
                                m_resource.updateElement(request, strRespone);
                                free(pdata);
                                pdata = nullptr;
                            }
                        }
                    });
                thrd1.detach();
            }
        }
    }
}

void CallbackManager::Callback_OptimalCameraBackData(CallBackMsg msg)
{
    bool bState = true;//默认不需要订阅
    if (bState) {
        OptimalCameraCallBackData* data = msg->_data ? msg->_data->getData<OptimalCameraCallBackData>() : nullptr;
        if (data)
        {
            cJSON* pRoot = cJSON_CreateObject();
            cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
            //cJSON_AddStringToObject(pRoot, "modelId", data->_modelID.c_str());
            cJSON* pArrayIds = cJSON_CreateArray();
            
            for (auto itor : data->_cameraIDs) {
                cJSON_AddItemToArray(pArrayIds,  cJSON_CreateString(itor.c_str()));
            }
            cJSON_AddItemToObject(pRoot, "modelId", pArrayIds);

            char* pOut = cJSON_Print(pRoot);
            CALLBACK_AND_LOG("OPTIMAL_CAMERA");
            cJSON_Delete(pRoot);
        }
    }
}

void CallbackManager::Callback_PoiMouseHoverData(CallBackMsg msg)
{
    bool bState = true;//默认不需要订阅
    if (bState) {
        PoiMouseHoverCallbackData* data = msg->_data ? msg->_data->getData<PoiMouseHoverCallbackData>() : nullptr;
        if (data)
        {
            cJSON* pRoot = cJSON_CreateObject();
            cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
            cJSON_AddStringToObject(pRoot, "modelId", data->_poiID.c_str());
            cJSON_AddBoolToObject(pRoot, "flag", data->_flag);

            char* pOut = cJSON_Print(pRoot);
            CALLBACK_AND_LOG("POI_MOUSE_HOVER");
            cJSON_Delete(pRoot);
        }
    }
}

//第一人称漫游器录像结束回调
void CallbackManager::Callback_FPM_RecordTrackEndData(CallBackMsg msg)
{
    bool bState = true;//不需要订阅
    if (bState) {
        FPMRecordTrackEndCallBackData* data = msg->_data ? msg->_data->getData<FPMRecordTrackEndCallBackData>() : nullptr;
        if (data)
        {
            cJSON* pRoot = cJSON_CreateObject();
            cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
            cJSON_AddStringToObject(pRoot, "trackId", data->_trackID.c_str());

            char* pOut = cJSON_Print(pRoot);
            CALLBACK_AND_LOG("FPM_RECORD_TRACK_END");
            cJSON_Delete(pRoot);
        }
    }
}

//第一人称漫游器in out回调
void CallbackManager::Callback_FPM_InOutData(CallBackMsg msg)
{
    bool bState = true;//不需要订阅
    if (bState) {
        FPMInOutCallBackData* data = msg->_data ? msg->_data->getData<FPMInOutCallBackData>() : nullptr;
        if (data)
        {
            cJSON* pRoot = cJSON_CreateObject();
            cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
            cJSON_AddBoolToObject(pRoot, "bIn", data->_bIn);

            char* pOut = cJSON_Print(pRoot);
            CALLBACK_AND_LOG("FPM_RECORD_IN_OUT");
            cJSON_Delete(pRoot);
        }
    }
}

//第一人称录像播放进度回调
void CallbackManager::Callback_FPM_ReplayTrackData(CallBackMsg msg)
{
    bool bState = true;//不需要订阅
    if (bState) {
        FPMReplayTrackCallBackData* data = msg->_data ? msg->_data->getData<FPMReplayTrackCallBackData>() : nullptr;
        if (data)
        {
            cJSON* pRoot = cJSON_CreateObject();
            cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
            cJSON_AddNumberToObject(pRoot, "dProgress", data->_dProgress);

            char* pOut = cJSON_Print(pRoot);
            CALLBACK_AND_LOG("FPM_RECORD_REPLAY_TRACK");
            cJSON_Delete(pRoot);
        }
    }
}

//场景关闭回调
void CallbackManager::Callback_Close3D()
{
    m_resource.NotifyCloseCallback();
}

//碰撞体触发回调：CBT_InsectNode
void CallbackManager::Callback_CBT_InsectNode(CallBackMsg msg)
{
    bool bState = true;//不需要订阅
    if (bState) {
        InsectNodeCallbackData* data = msg->_data ? msg->_data->getData<InsectNodeCallbackData>() : nullptr;
        if (data)
        {
            cJSON* pRoot = cJSON_CreateObject();
            cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
            cJSON_AddStringToObject(pRoot, "modelId", data->_modelID.c_str());
            cJSON_AddBoolToObject(pRoot, "in", data->_in);

            char* pOut = cJSON_Print(pRoot);
            CALLBACK_AND_LOG("CBT_INSECTNODE");
            cJSON_Delete(pRoot);
        }
    }
}

//视锥触发回调：CBT_CYNAMIC_VIDEO_TRIGGER_TriggerAction
void CallbackManager::Callback_CBT_CYNAMIC_VIDEO_TRIGGER_TriggerAction(CallBackMsg msg)
{
    bool bState = true;//不需要订阅
    if (bState) {
        DynamicVideoTriggerMainCameraTriggerActionCallbackData* data = msg->_data ? msg->_data->getData<DynamicVideoTriggerMainCameraTriggerActionCallbackData>() : nullptr;
        if (data)
        {
            cJSON* pRoot = cJSON_CreateObject();
            cJSON_AddStringToObject(pRoot, "taskId", GetConfigSettingInstance().GetAppId().c_str());
            cJSON_AddStringToObject(pRoot, "modelId", data->_videoModelID.c_str());
            cJSON_AddBoolToObject(pRoot, "in", data->triggered);

            char* pOut = cJSON_Print(pRoot);
            CALLBACK_AND_LOG("CBT_CYNAMIC_VIDEO_TRIGGER_TRIGGERACTION");
            cJSON_Delete(pRoot);
        }
    }
}