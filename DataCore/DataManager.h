#pragma once
#include <string>
#include <map>
#include <atomic>
#include <shared_mutex>

#include "GlobalResource.h"
#include "CSceneList.h"
#include "CTreeItem.h"
#include "CGlobalSetting.h"
#include "CMyCond.h"

typedef void (*DataCallback)(const char* channel, const char* data, void* pUser);
class CDataManager
{
public:
	CDataManager();
	~CDataManager();

	void InitMapCallback();
public:
	//解析大json，然后结构化数据
	int ParsePmsJson(const char* pSrcData, const char* ProjectId, const char* SenceId);

	//tag
	void getTagList(std::string request, std::string& strRespone);
	void getTag(std::string request, std::string& strRespone);
	int updateTag(std::string request, std::string& strRespone);

	//元素相关接口
	void getElement(std::string request, std::string& strRespone);
	int addElement(std::string request, std::string& strRespone);
	int updateElement(std::string request, std::string& strRespone);
	int deleteElement(std::string request, std::string& strRespone);
	int controlElement(std::string request, std::string& strRespone);

	//透传接口
	int GeneralInterface(std::string request, std::string& strRespone);

	//event透传接口
	int EventGeneralInterface(std::string request, std::string& strRespone);

	//获取elment 树型结构
	void getElementTree(std::string request, std::string& strRespone);

	//element一些特殊接口
	void getElementGeneralInterface(std::string request, std::string& strRespone);

	//独立与元素的控制请求
	int controlVideoTrackPara(std::string request, std::string& strRespone);

	//获取当前rte场景资源的json
	/*
	* rSid :当前场景id
	* strOut：当前场景的json
	* strMessage：空正常，否则错误信息。
	*/
	int getCurResourceFormatBuild(std::string& rSid, std::string& strOut, std::string& strMessage);

	//更新上传json数据到PMS
	//int updataPmsFormatBuild(std::string request, std::string& strRespone);

	// 模板
	void getProjectTemplate(std::string strRequest, std::string& strRespone);
	int addProjectTemplate(std::string strRequest, std::string& strRespone);
	int updateProjectTemplate(std::string strRequest, std::string& strRespone);
	int deleteProjectTemplate(std::string strRequest, std::string& strRespone);

	//模板通用接口
	void ProjectTemplateGeneralInterface(std::string strRequest, std::string& strRespone);

	//场景列表
	void getSceneList(std::string strRequest, std::string& strRespone);

	//场景设置
	//拿到大json是一个比较耗时的请求，先判断下是否可以切换。
	bool canSwitchScence(std::string strRequest, std::string& strRespone);

	//切换场景（支持不同项目场景切换，也支持同一项目不同场景切换）
	int switchScence(std::string strTreeJson, std::string strRequest, std::string& strRespone);

	void SetMainViewPoint();

	bool GetCallbackState(std::string strEvent);

	int GetModelCount();

	//响应添加模型
	int notifyAddModel(std::string strId, bool bSucess, bool bNeedAdd);
	//响应删除模型
	int notifyDelModel(std::string strId);
	int notifyDelModel(std::vector<std::string> strIds);

	//响应模型更新回调
	int notifyUpdateModel(std::vector<std::string> strIds);

	void NotifyCloseCallback() {
		m_condCloseCallback.notify("123");
	}

	//跳到主视点
	int setMainViewPointByInterface(std::string& strRespone);

	// 获取主视点
	void getSceneViewPoint(std::string strRequest, std::string& strRespone);
	// 更新主视点
	int updateSceneViewPoint(std::string strRequest, std::string& strRespone);


	//工程配置参数设置
	void ProjectConfig();
	//更新特效模板,//build sdk 设置effect模板
	void updateEffectTemplate();
	
	//加载模型,初始化一些配置,需要3d场景初始化接口调用完成后调用。
	int loadModelsTo3DEngine();

	void saveFPM_Data();

	//加载第一人称视角
	void LoadFPM_Data();

	//隐藏第一漫游器
	void hideFirstPersonManipultor();

	//加载图形配置参数
	void LoadGraphicsConfig();

	int LoadGraphicsConfig(std::string config, std::string& strRespone);

	//控制实时光影开关
	int switchRealTimeLighting(bool bSet, std::string& strRespone);

	//设置元素默认可视距参数
	int SetGlobalLoadScale(int nValue);

	//添加预制地图
	int AddProiMageMap();

	void RegisterCallback(DataCallback callback, void* pUser) { m_callback = callback; m_pUser = pUser; }
	
	int switchCallback(std::string request, std::string& strRespone);

	void open2DWnd(std::string& strRespone);
	void close2DWnd(std::string& strRespone);
public:
	std::string						m_strProjectId;			//项目id
	std::string						m_strSenceId;			//sceneContentId, 新场景，新场景1
	int								m_nLoadMode;			//当前已经加载的模型数量，包含失败的
	std::shared_mutex				m_mutexTree2;

	//保存树形结构,及所有元素管理(内存管理)
	std::shared_ptr<CTreeItem>		m_pTreeItem;
	std::atomic<bool>				m_bLoadComplite;		//场景加载完成标志
private:
	std::map<std::string, bool>		m_mapCallbackSubscribe;//前端订阅事件回调表

	//全局信息，包括模板、特效、等信息
	std::shared_ptr<CGlobalContent> m_pGlobal;

	//当前项目的场景列表
	std::shared_ptr<CSceneList>		m_pScenceList;

	cJSON*							m_pmsData; //接收到的pms原始数据。
	//是否已经加载
	bool							m_bLoad = false;
	CMyCond                         m_condCloseCallback;

	DataCallback					m_callback = nullptr;
	void*							m_pUser = nullptr;
};