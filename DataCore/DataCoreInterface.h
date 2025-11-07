#pragma once
#include <string>

#ifdef _WIN32
    #define DATACORE_API __declspec(dllexport)
#else
    #define DATACORE_API __attribute__((visibility("default")))
#endif

namespace DataCore {
    //构建datacore结构数据
	/*
	* appId: 容器id
	* projectId：项目id
	* scenseId：当前项目的场景ID,如新场景的ID
	* ResourceIp：pms 资源服务器ip
	* ResourcePort：pms资源服务器的port
	* dataJson:pms 大json数据
	*/
    DATACORE_API int Init( const char* appId, const char* projectId, const char* scenseId, const char* ResourceIp, unsigned int ResourcePort, const char* dataJson);

    DATACORE_API int DeInit();

	//加载3d模型，配置build中设置的一些配置
	/*
	* 注意：此函数只是加载模型，和设置build中的一些设置。因此在调用此函数之前需要创建好Viewer，调用initSyzEarth3D等接口。
	*/
	DATACORE_API int loadModelsTo3DEngine();

	//外部调用操作元素相关函数 必须 等待加载完成模型操作才有效（init3DEngine()之后调用）。
    //元素相关设置
	DATACORE_API void getTagList(std::string request, std::string& strRespone);
	DATACORE_API void getTag(std::string request, std::string& strRespone);
	DATACORE_API int updateTag(std::string request, std::string& strRespone);

	//元素相关接口
	/*
	* {
		"view": true,
		"type": [26,27],
		"guid": [],             //空不做判断，必须是数组否则无效
		"foreignKey":[{"111"},{"222"}],        //空不做判断，必须是数组否则无效
		"tagList":["1111","222"],
        "attributeList":[{"indoor":true},{"CHid":"11100001"}],
		"area":{
		    "point": {
			    "w": 1.0,
			    "x": 116.38863941993556,
			    "y": 39.90365656287492,
			    "z": 0.03636673651635647
                }
		},
		"name":["poi","yunzhou"],
		"@FCondition": ["view","!area","tagList","attributeList", "type","foreignKey","guid"],
		"@Condition": ["name{}"], // 只支持name 
		"@Column": [{ "guid": "guid"},{"elementData":["_bbCenterWorldPos","_pulseColor"]}]
	}
	  rte2 接口请求json中的Elemet数据。
	*/
	DATACORE_API void getElement(std::string request, std::string& strRespone);
	DATACORE_API int addElement(std::string request, std::string& strRespone);
	DATACORE_API int updateElement(std::string request, std::string& strRespone);
	DATACORE_API int deleteElement(std::string request, std::string& strRespone);
	DATACORE_API int controlElement(std::string request, std::string& strRespone);

	//获取elment 树型结构
	DATACORE_API void getElementTree(std::string request, std::string& strRespone);

	//element一些特殊接口
	DATACORE_API void getElementGeneralInterface(std::string request, std::string& strRespone);

	//球机追视
	DATACORE_API int controlVideoTrackPara(std::string request, std::string& strRespone);

	//回调订阅开关
	DATACORE_API int switchCallback(std::string request, std::string& strRespone);

	// 获取主视点
	DATACORE_API void getSceneViewPoint(std::string strRequest, std::string& strRespone);

	// 更新主视点
	DATACORE_API int updateSceneViewPoint(std::string strRequest, std::string& strRespone);

	//透传接口，直接透传到3d
	DATACORE_API int GeneralInterface(std::string request, std::string& strRespone);

	//event透传接口
	DATACORE_API int EventGeneralInterface(std::string request, std::string& strRespone);


    //全局参数设置
	//获取指定回调订阅状态
	DATACORE_API bool GetCallbackState(std::string strEvent);

	//跳到主视点
	DATACORE_API int setMainViewPointByInterface(std::string& strRespone);

	//获取当前rte场景资源的大json
	/*
	* rSid :返回当前场景id
	* strOut：返回当前场景的json
	* strMessage：空正常，否则错误信息。
	*/
	DATACORE_API int getCurResourceFormatBuild(std::string& rSid, std::string& strOut, std::string& strMessage);

	// 模板
	DATACORE_API void getProjectTemplate(std::string strRequest, std::string& strRespone);
	DATACORE_API int addProjectTemplate(std::string strRequest, std::string& strRespone);
	DATACORE_API int updateProjectTemplate(std::string strRequest, std::string& strRespone);
	DATACORE_API int deleteProjectTemplate(std::string strRequest, std::string& strRespone);

	//模板通用接口
	DATACORE_API void ProjectTemplateGeneralInterface(std::string strRequest, std::string& strRespone);

	//场景列表
	DATACORE_API void getSceneList(std::string strRequest, std::string& strRespone);

	//切换场景相关接口
	//判断下是否可以切换。
	DATACORE_API bool canSwitchScence(std::string strRequest, std::string& strRespone);

	//切换场景（支持不同项目场景切换，也支持同一项目不同场景切换）
	DATACORE_API int switchScence(std::string strTreeJson, std::string strRequest, std::string& strRespone);

    //callback 回调相关接口
    //data ,rte2 redis publish的json数据
    typedef void (*DataCallback)(const char* channel,const char* data, void* pUser);

    //注册回调 
    DATACORE_API void RegisterCallback(DataCallback callback, void* pUser);
};
