#pragma once
#include <string>
#include <map>
#include <functional>
#include <sstream>

#ifndef _WIN32
#include <uuid/uuid.h>
#endif // !_WIN32

namespace RTE {
	enum SceneDataType
	{
		SDT_Unvalid,                    // 0  无效的数据类型
		SDT_3DMap,                      // 1  3D 地图筛选组
		SDT_2DMap,                      // 2  2D 地图筛选组
		SDT_Model,                      // 3  建筑模型
		SDT_VideoPannel,                // 4  幕布
		SDT_DataPannel,                 // 5  数据面片, 新旧版本数据面片用一个
		SDT_Elevation,                  // 6  高程数据
		SDT_OROP,                       // 7  倾斜摄影
		SDT_3DSHP,                      // 8  3D 矢量
		SDT_2DSHP,                      // 9  2D 矢量
		SDT_Group,                      // 10 组数据
		SDT_Build,                      // 11 建筑
		SDT_Floor,                      // 12 楼层
		SDT_Room,                       // 13 房间
		SDT_DEFAULT_GROUP,              // 14 默认组
		SDT_ROOT_GROUP,					// 15
		SDT_3DGROUP,					// 16
		SDT_3DImageGroup,				// 17
		SDT_ElevationGroup,				// 18 
		SDT_OROPGroup,					// 19
		SDT_3DSHPGroup,					// 20
		SDT_2DGROUP,					// 21	
		SDT_2DMapGroup,					// 22
		SDT_2DSHPGroup,					// 23
		SDT_ModelBase,                  // 24 模型库
		SDT_SceneEffects,               // 25 场景特效
		SDT_POI,                        // 26 项目POI
		SDT_PLOTTING,                   // 27 标绘
		SDT_PathBrowsing,               // 28 路径浏览
		SDT_HighlightArea,              // 29 高亮区域
		SDT_AreaContour,                // 30 区域轮廓
		SDT_SpaceFence,                 // 31 空间围栏
		SDT_VisualArea,                 // 32 可视域
		SDT_ViewPath,                   // 33 视点浏览
		//SDT_Insect,					// 碰撞体 不显示在左侧树
		SDT_3DImage,                    // 34 3D 地图数据
		SDT_2DImage,                    // 35 2D 地图数据
		SDT_SceneRoame,                 // 36 路径漫游
		SDT_3DImageGeneralGroup,        // 37 卫星影像内的普通分组
		SDT_3DElevationGeneralGroup,    // 38 数据高程内的普通分组
		SDT_3DShpGeneralGroup,          // 39 三维矢量内的普通分组
		SDT_3DOropGeneralGroup,         // 40 倾斜摄影内的普通分组
		SDT_BuildItemModel,             // 41 建筑下子 model
		SDT_UAVVideo,                   // 42 无人机
		SDT_SceneLight,                 // 43 场景灯光  44
		SDT_HeatMap,                    // 44 热力图
		//SDT_MarkNode,					// 标绘
		SDT_Vegetation,                 // 45 植被
		SDT_RiverSystem,                // 46水系
		SDT_SimplePoi,					// 47 简易 poi
		SDT_SpaceSurvey,				// 48 空间测量
		SDT_VirtualCamera,				// 49 虚拟相机--这里不能放在前面，因为场景节点保存都是按照 int 类型来的，一旦放在前面，会打乱以前所有场景的节点类型！！！
		SDT_VehicleSimulation,          // 50 车辆仿真
		SDT_Unknow,                     // 51 未知类型
		SDT_Radar,						// 52 雷达
        SDT_Text,                       // 53 文字


        //以下是控制命令，并非真正的元素
        SDT_VideoTrack = 100,           //100 球机追视

        //以下是rte特有命令，以下是控制命令，并非真正的元素
        SDT_verseGraphicsConfig = 200
	};

    static std::string getModelStrTypeByType(int type)
    {
        switch (type) {
        case SDT_Unvalid:
            break;
        case SDT_3DMap:
            break;
        case SDT_2DMap:
            break;
        case SDT_Model:
            return "GeneralModel";
        case SDT_VideoPannel:
            return "VideoPannel";
        case SDT_DataPannel:
            return "DataPannel";
        case SDT_Elevation:
            break;
        case SDT_OROP:
            return "OROP";
            break;
        case SDT_3DSHP:
            break;
        case SDT_2DSHP:
            break;
        case SDT_Group:
            break;
        case SDT_Build:
            //return "Architerture";
            return "Architecture";
        case SDT_Floor:
            return "Floor";
        case SDT_Room:
            return "Room";
        case SDT_DEFAULT_GROUP:
            return "DefaultGroup";
            break;
        case SDT_ROOT_GROUP:
            return "RootGroup";
            break;
        case SDT_3DGROUP:
            return "3DGroup";
            break;
        case SDT_3DImageGroup:
            return "3DImageGroup";
            break;
        case SDT_ElevationGroup:
            return "ElevationGroup";
            break;
        case SDT_OROPGroup:
            return "OROPGroup";
            break;
        case SDT_3DSHPGroup:
            return "3DSHPGroup";
            break;
        case SDT_2DGROUP:
            return "2DGroup";
            break;
        case SDT_2DMapGroup:
            return "2DMapGroup";
            break;
        case SDT_2DSHPGroup:
            return "2DSHPGroup";
            break;
        case SDT_ModelBase:
            return "GeneralModel";
        case SDT_SceneEffects:
            return "Effect";
        case SDT_POI:
            return "Poi";
        case SDT_PLOTTING:
            return "Plotting";
            break;
        case SDT_PathBrowsing:
            return "PathBrowser";
        case SDT_HighlightArea:
            return "highlightArea";
        case SDT_AreaContour:
            return "Contour";
        case SDT_SpaceFence:
            return "fence";
        case SDT_VisualArea:
            return "VisualArea";
            break;
        case SDT_ViewPath:
            return "SceneViewPath";
        case SDT_3DImage:
            return "3DImage";
            break;
        case SDT_2DImage:
            return "2DImage";
            break;
        case SDT_SceneRoame:
            return "SceneRoame";
            break;
        case SDT_3DImageGeneralGroup:
            return "3DImageGeneralGroup";
            break;
        case SDT_3DElevationGeneralGroup:
            return "3DElevationGeneralGroup";
            break;
        case SDT_3DShpGeneralGroup:
            return "3DShpGeneralGroup";
            break;
        case SDT_3DOropGeneralGroup:
            return "3DOropGeneralGroup";
            break;
        case SDT_BuildItemModel:
            return "BuildItemModel";
            break;
        case SDT_UAVVideo:
            return "UAVVideo";
        case SDT_SceneLight:
            return "SceneLight";
        case SDT_HeatMap:
            return "HeatMap";
            //    case SDT_MarkNode:
            //        return "MarkNode";
        case SDT_Vegetation:
            return "vegetation";
        case SDT_RiverSystem:
            return "RiverSystem";
        case SDT_SimplePoi:
            return "SimplePoi";
        case SDT_VirtualCamera:
            return "VirtualCamera";
        case SDT_VehicleSimulation:
            return "VehicleSimulation";
        case SDT_SpaceSurvey:
            return "SpaceSurvey";
        case SDT_Unknow:
            break;
        case SDT_Radar:
            return "Radar";
        case SDT_Text:
            return "Text";
        default:
            break;
        }
        return "";
    }

    //生成一个GUID字符串
    static std::string GUIDStr()
    {
#ifdef _WIN32
        GUID guid;
        if (SUCCEEDED(::CoCreateGuid(&guid)))
        {
            std::ostringstream oss;
            oss << std::hex << guid.Data1 << "-" << std::hex << guid.Data2 << "-"
                << std::hex << guid.Data3 << "-" << std::hex << (short)guid.Data4[0]
                << std::hex << (short)guid.Data4[1] << std::hex << (short)guid.Data4[2]
                << std::hex << (short)guid.Data4[3] << std::hex << (short)guid.Data4[4]
                << std::hex << (short)guid.Data4[5] << std::hex << (short)guid.Data4[6]
                << std::hex << (short)guid.Data4[7];
            return oss.str();
        }
#else
        uuid_t uuid;
        char str[36];

        uuid_generate(uuid);
        uuid_unparse(uuid, str);

        std::string strUuid = str;
        return strUuid;
#endif
        return std::string();
    }
}