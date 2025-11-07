#pragma once
#include "DataManager.h"
#include <SceneCore/interface/Interface.h>
//#include "CRteRedis.h"

class CallbackManager
{
public:
	CallbackManager(CDataManager& resource);
	~CallbackManager();

	int Init();
	void InitSubscriMap();
	void RegisterCallback(DataCallback callback, void* pUser) { m_callback = callback; m_pUser = pUser; }
	int NotifyProgramCompleted();
private:
	void CallBack_AddModeSuccess(CallBackMsg msg);
	void CallBack_DelModels(CallBackMsg msg);
	void CallBack_3DPoint(CallBackMsg msg);
	void CallBack_AddError(CallBackMsg msg);
	void CallBack_AddLayerError(CallBackMsg msg);
	void CallBack_PickModel(CallBackMsg msg);

	void CallBack_PixelCoords(CallBackMsg msg);
	void CallBack_PixelCoordsEx(CallBackMsg msg);
	//"PICKELEM_POI"

	void CallBack_EyeToSpaceFence(CallBackMsg msg);
	void CallBack_ModelToSpaceFence(CallBackMsg msg);

	void CallBack_SceneState(CallBackMsg msg);
	void CallBack_MouseDataPos(CallBackMsg msg);

	void openVideoCallBack(CallBackMsg msg);

	void CallBack_ElementPosition(CallBackMsg msg);
	void CallBack_ClientToScreen(CallBackMsg msg);

	void CallBack_EnterOrLeaveMonitor(CallBackMsg msg);

	//void CallBack_MouseDataPos(CallBackMsg msg);
	//模型更新回调
	void CallBack_UpdateMode(CallBackMsg msg);

	//视频时间戳回调
	void Callback_VideoTimestamp(CallBackMsg msg);

	//数据面片点击回调
	void Callback_DataPannelClickBackData(CallBackMsg msg);

	//球机追视附近相机回调
	void Callback_OptimalCameraBackData(CallBackMsg msg);

	//POI鼠标悬停回调
	void Callback_PoiMouseHoverData(CallBackMsg msg);

	//第一人称漫游器录像结束回调
	void Callback_FPM_RecordTrackEndData(CallBackMsg msg);

	//第一人称进入退出回调
	void Callback_FPM_InOutData(CallBackMsg msg);

	//第一人称录像播放进度回调
	void Callback_FPM_ReplayTrackData(CallBackMsg msg);

	//场景关闭回调
	void Callback_Close3D();

	//碰撞体触发回调：CBT_InsectNode
	void Callback_CBT_InsectNode(CallBackMsg msg);

	//视锥触发回调：CBT_CYNAMIC_VIDEO_TRIGGER_TriggerAction
	void Callback_CBT_CYNAMIC_VIDEO_TRIGGER_TriggerAction(CallBackMsg msg);

private:
	//引用场景资源
	CDataManager&						m_resource;
	int									m_nLoadMode;// 不能自己统计，因为重新加载场景后，这里没有重置，通过m_resource重置。
	std::map<int, std::string>			m_mapSubscribType;  //订阅元素类型和 元素类型映射表
	DataCallback						m_callback = nullptr;
	void*								m_pUser = nullptr;
};

