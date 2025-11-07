#include "stdafx.h"
#include "DataCoreInterface.h"
#include "DataManager.h"
#include "CallbackManager.h"
#include <thread>
#include "config.h"

namespace DataCore {
    static CDataManager g_DataManager;
    std::shared_ptr<CallbackManager> g_CallbackManager = nullptr;
    static bool         g_Init = false;

    void  InitLog();

    int Init(const char* appId, const char* projectId, const char* scenseId, const char* ResourceIp, unsigned int ResourcePort, const char* dataJson)
    {
        if (!g_Init)
        {
            //日志初始化
            InitLog();
            GetConfigSettingInstance().SetResouceInfo(ResourceIp, ResourcePort);
            GetConfigSettingInstance().SetAppId(appId);

            //初始化json
            if (g_DataManager.ParsePmsJson(dataJson, projectId, scenseId))
                return -1;

            g_DataManager.InitMapCallback();

            g_CallbackManager = std::make_shared< CallbackManager>(g_DataManager);
            g_CallbackManager->Init();

            g_Init = true;
            return 0;
        }

        return -1;
    }

    int DeInit()
    {
        return 0;
    }

    int loadModelsTo3DEngine()
    {
        if (!g_Init)
            return -1;

        return g_DataManager.loadModelsTo3DEngine();
    }

    void InitLog()
    {
        int nKeepDays = 10;
        int nLevel = Tool::Info;
        Tool::TLog::Init(("log"), ("datacore"), nKeepDays);
        Tool::TLog::SetWriteFile(true);
        Tool::TLog::SetPrint(true);
        Tool::TLog::SetLogLevel(nLevel);
    }

    void getTagList(std::string request, std::string& strRespone)
    {
        if (!g_Init)
            return;

        g_DataManager.getTagList(request, strRespone);
    }
    void getTag(std::string request, std::string& strRespone)
    {
        if (!g_Init)
            return;
        g_DataManager.getTag(request, strRespone);
    }
    int updateTag(std::string request, std::string& strRespone)
    {
        if (!g_Init)
            return -1;
        return g_DataManager.updateTag(request, strRespone);
    }

    //元素相关接口
    void getElement(std::string request, std::string& strRespone)
    {
        if (!g_Init)
            return;
        g_DataManager.getElement(request, strRespone);
    }

    int addElement(std::string request, std::string& strRespone)
    {
        if (!g_Init)
            return -1;
        return g_DataManager.addElement(request, strRespone);
    }
    int updateElement(std::string request, std::string& strRespone)
    {
        if (!g_Init)
            return -1;
        return g_DataManager.updateElement(request, strRespone);
    }
    int deleteElement(std::string request, std::string& strRespone)
    {
        if (!g_Init)
            return -1;
        return g_DataManager.deleteElement(request, strRespone);
    }
    int controlElement(std::string request, std::string& strRespone)
    {
        if (!g_Init)
            return -1;
        return g_DataManager.controlElement(request, strRespone);
    }

    //透传接口
    int GeneralInterface(std::string request, std::string& strRespone)
    {
        if (!g_Init)
            return -1;
        return g_DataManager.GeneralInterface(request, strRespone);
    }

    //event透传接口
    int EventGeneralInterface(std::string request, std::string& strRespone)
    {
        if (!g_Init)
            return -1;
        return g_DataManager.EventGeneralInterface(request, strRespone);
    }

    //获取elment 树型结构
    void getElementTree(std::string request, std::string& strRespone)
    {
        if (!g_Init)
            return;
        return g_DataManager.getElementTree(request, strRespone);
    }

    //element一些特殊接口
    void getElementGeneralInterface(std::string request, std::string& strRespone)
    {
        if (!g_Init)
            return;
        return g_DataManager.getElementGeneralInterface(request, strRespone);
    }

    //独立与元素的控制请求
    int controlVideoTrackPara(std::string request, std::string& strRespone)
    {
        if (!g_Init)
            return -1;
        return g_DataManager.controlVideoTrackPara(request, strRespone);
    }

    void RegisterCallback(DataCallback callback,void *pUser)
    {
        if (!g_Init)
            return;
        g_DataManager.RegisterCallback(callback, pUser);
        g_CallbackManager->RegisterCallback(callback, pUser);
    }

    bool GetCallbackState(std::string strEvent)
    {
        if (!g_Init)
            return false;
        return g_DataManager.GetCallbackState(strEvent);
    }

    int setMainViewPointByInterface(std::string& strRespone)
    {
        if (!g_Init)
            return -1;
        return g_DataManager.setMainViewPointByInterface(strRespone);
    }

    // 获取主视点
    void getSceneViewPoint(std::string strRequest, std::string& strRespone)
    {
        if (!g_Init)
            return ;
        g_DataManager.getSceneViewPoint(strRequest, strRespone);
    }

    // 更新主视点
    int updateSceneViewPoint(std::string strRequest, std::string& strRespone)
    {
        if (!g_Init)
            return -1;
        return g_DataManager.updateSceneViewPoint(strRequest, strRespone);
    }

    //获取当前rte场景资源的json
    int getCurResourceFormatBuild(std::string& rSid, std::string& strOut, std::string& strMessage)
    {
        if (!g_Init)
            return -1;
        return g_DataManager.getCurResourceFormatBuild(rSid, strOut, strMessage);
    }

    // 模板
    void getProjectTemplate(std::string strRequest, std::string& strRespone)
    {
        if (!g_Init)
            return;
        g_DataManager.getProjectTemplate(strRequest, strRespone);
    }
    int addProjectTemplate(std::string strRequest, std::string& strRespone)
    {
        if (!g_Init)
            return -1;
        return g_DataManager.addProjectTemplate(strRequest, strRespone);
    }

    int updateProjectTemplate(std::string strRequest, std::string& strRespone)
    {
        if (!g_Init)
            return -1;
        return g_DataManager.updateProjectTemplate(strRequest, strRespone);
    }

    int deleteProjectTemplate(std::string strRequest, std::string& strRespone)
    {
        if (!g_Init)
            return -1;
        return g_DataManager.deleteProjectTemplate(strRequest, strRespone);
    }

    //模板通用接口
    void ProjectTemplateGeneralInterface(std::string strRequest, std::string& strRespone)
    {
        if (!g_Init)
            return ;
        return g_DataManager.ProjectTemplateGeneralInterface(strRequest, strRespone);
    }

    bool canSwitchScence(std::string strRequest, std::string& strRespone)
    {
        if (!g_Init)
            return false;
        return g_DataManager.canSwitchScence(strRequest, strRespone);
    }

    //切换场景（支持不同项目场景切换，也支持同一项目不同场景切换）
    int switchScence(std::string strTreeJson, std::string strRequest, std::string& strRespone)
    {
        if (!g_Init)
            return -1;
        return g_DataManager.switchScence(strTreeJson, strRequest, strRespone);
    }

    int switchCallback(std::string request, std::string& strRespone)
    {
        if (!g_Init)
            return -1;
        return g_DataManager.switchCallback(request, strRespone);
    }

    void getSceneList(std::string strRequest, std::string& strRespone)
    {
        if (!g_Init)
            return;
        g_DataManager.getSceneList(strRequest, strRespone);
    }
};