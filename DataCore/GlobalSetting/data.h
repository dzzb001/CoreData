#pragma once

#include "common.h"

typedef struct Viewpoint {
	double fHeading; // 相机姿态（航向角）
	std::string strVPImageURL; // 图片（视点漫游的图片）
	double fLatitude; // 纬度
	std::string strSceneContentID;
	int nRelPosZ; // 自定义视点到模型位置的相对坐标（z）
	int nIndex; // 索引号
	double fRange; // 相机到位置（lon, lat, z）的直线距离
	int nRelPosY; // 自定义视点到模型位置的相对坐标（y）
	int nRelPosX; // 自定义视点到模型位置的相对坐标（x）
	double fThroughTime; // 经过时间（视点漫游）
	double fVersion;
	bool bHome; // 是否为主视点
	std::string strName; // 视点名称
	std::string strSceneID;
	double fStayTime; // 停留时间（视点漫游）
	bool bVaild;
	std::string strID; // 唯一id号
	double fPitch; // 相机姿态（倾斜角）
	std::string strProjectID;
	double fHeight; // 高度值
	double fLongitude; // 经度

	Viewpoint()
		: fHeading(0), strVPImageURL(""), fLatitude(0),
		strSceneContentID(""), nRelPosZ(0), nIndex(0),
		fRange(0), nRelPosY(0), nRelPosX(0), fThroughTime(0),
		fVersion(0), bHome(false), strName(""), strSceneID(""),
		fStayTime(0), bVaild(false), strID(""), fPitch(0),
		strProjectID(""), fHeight(0), fLongitude(0) {}

	void loadFromJson(cJSON *pJson) {
		if (pJson != nullptr) {
			if (cJSON_IsObject(pJson)) {
				cJSON_LoadFromObject(pJson, "heading", fHeading);
				cJSON_LoadFromObject(pJson, "vpImageURl", strVPImageURL);
				cJSON_LoadFromObject(pJson, "latitude", fLatitude);
				cJSON_LoadFromObject(pJson, "sceneContentId", strSceneContentID);
				cJSON_LoadFromObject(pJson, "relPosZ", nRelPosZ);
				cJSON_LoadFromObject(pJson, "index", nIndex);
				cJSON_LoadFromObject(pJson, "range", fRange);
				cJSON_LoadFromObject(pJson, "relPosY", nRelPosY);
				cJSON_LoadFromObject(pJson, "relPosX", nRelPosX);
				cJSON_LoadFromObject(pJson, "throughtime", fThroughTime);
				cJSON_LoadFromObject(pJson, "version", fVersion);
				cJSON_LoadFromObject(pJson, "home", bHome);
				cJSON_LoadFromObject(pJson, "name", strName);
				cJSON_LoadFromObject(pJson, "sceneId", strSceneID);
				cJSON_LoadFromObject(pJson, "staytime", fStayTime);
				cJSON_LoadFromObject(pJson, "vaild", bVaild);
				cJSON_LoadFromObject(pJson, "id", strID);
				cJSON_LoadFromObject(pJson, "pitch", fPitch);
				cJSON_LoadFromObject(pJson, "projectId", strProjectID);
				cJSON_LoadFromObject(pJson, "height", fHeight);
				cJSON_LoadFromObject(pJson, "longitude", fLongitude);
			}
		}

		return;
	}

	cJSON *toJson() {
		cJSON *pJson = cJSON_CreateObject();
		if (pJson != nullptr) {
			cJSON_AddToObject(pJson, "heading", fHeading);
			cJSON_AddToObject(pJson, "vpImageURl", strVPImageURL);
			cJSON_AddToObject(pJson, "latitude", fLatitude);
			cJSON_AddToObject(pJson, "sceneContentId", strSceneContentID);
			cJSON_AddToObject(pJson, "relPosZ", nRelPosZ);
			cJSON_AddToObject(pJson, "index", nIndex);
			cJSON_AddToObject(pJson, "range", fRange);
			cJSON_AddToObject(pJson, "relPosY", nRelPosY);
			cJSON_AddToObject(pJson, "relPosX", nRelPosX);
			cJSON_AddToObject(pJson, "throughtime", fThroughTime);
			cJSON_AddToObject(pJson, "version", fVersion);
			cJSON_AddToObject(pJson, "home", bHome);
			cJSON_AddToObject(pJson, "name", strName);
			cJSON_AddToObject(pJson, "sceneId", strSceneID);
			cJSON_AddToObject(pJson, "staytime", fStayTime);
			cJSON_AddToObject(pJson, "vaild", bVaild);
			cJSON_AddToObject(pJson, "id", strID);
			cJSON_AddToObject(pJson, "pitch", fPitch);
			cJSON_AddToObject(pJson, "projectId", strProjectID);
			cJSON_AddToObject(pJson, "height", fHeight);
			cJSON_AddToObject(pJson, "longitude", fLongitude);
		}

		return pJson;
	}

	void loadFrom3DJson(cJSON *pJson) {
		if (pJson != nullptr) {
			if (cJSON_IsObject(pJson)) {
				cJSON *pJsonCenter = cJSON_GetObjectItem(pJson, "_center");
				if (pJsonCenter != nullptr) {
					if (cJSON_IsObject(pJsonCenter)) {
						cJSON_LoadFromObject(pJsonCenter, "x", fLongitude);
						cJSON_LoadFromObject(pJsonCenter, "y", fLatitude);
						cJSON_LoadFromObject(pJsonCenter, "z", fHeight);
					}
				}

				cJSON_LoadFromObject(pJson, "_duration_s", fThroughTime);
				cJSON_LoadFromObject(pJson, "_heading", fHeading);
				cJSON_LoadFromObject(pJson, "_pitch", fPitch);
				cJSON_LoadFromObject(pJson, "_range", fRange);
				cJSON_LoadFromObject(pJson, "home", bHome);
			}
		}

		return ;
	}

	cJSON *to3DJson() {
		cJSON *pJson = cJSON_CreateObject();
		if (pJson != nullptr) {
			cJSON *pJsonCenter = cJSON_CreateObject();
			if (pJsonCenter != nullptr) {
				cJSON_AddToObject(pJsonCenter, "x", fLongitude);
				cJSON_AddToObject(pJsonCenter, "y", fLatitude);
				cJSON_AddToObject(pJsonCenter, "z", fHeight);
				cJSON_AddToObject(pJsonCenter, "w", (double)1.0);

				cJSON_AddItemToObject(pJson, "_center", pJsonCenter);
			}

			cJSON_AddToObject(pJson, "_duration_s", fThroughTime);
			cJSON_AddToObject(pJson, "_heading", fHeading);
			cJSON_AddToObject(pJson, "_pitch", fPitch);
			cJSON_AddToObject(pJson, "_range", fRange);
			cJSON_AddToObject(pJson, "home", bHome);
			cJSON_AddToObject(pJson, "fly", true);
		}

		return pJson;
	}

} Viewpoint;

typedef struct Point {
	double fX;
	double fY;
	double fZ;
	double fW;
	std::string strVersion;

	Point()
		: fX(0.0),
		fY(0.0),
		fZ(0.0),
		fW(0.0),
		strVersion("") {}

	void loadFromJson(cJSON *pJson) {
		if (pJson != nullptr) {
			if (cJSON_IsObject(pJson)) {
				cJSON_LoadFromObject(pJson, "x", fX);
				cJSON_LoadFromObject(pJson, "y", fY);
				cJSON_LoadFromObject(pJson, "z", fZ);
				cJSON_LoadFromObject(pJson, "w", fW);
				cJSON_LoadFromObject(pJson, "version", strVersion);
			}
		}

		return ;
	}

	cJSON *toJson() {
		cJSON *pJson = cJSON_CreateObject();
		if (pJson != nullptr) {
			cJSON_AddToObject(pJson, "x", fX);
			cJSON_AddToObject(pJson, "y", fY);
			cJSON_AddToObject(pJson, "z", fZ);
			cJSON_AddToObject(pJson, "w", fW);
			cJSON_AddToObject(pJson, "version", strVersion);
		}

		return pJson;
	}

} Point;

typedef struct TemplateModel {
	std::string strIVEPath;
	std::string strResourceId;
	std::string strDataType;
	std::string strImgPath;
	std::string strName;
	std::string strSceneID;
	std::string strID;
	std::string strProjectID;
	double fVersion;
	bool bIsUsed;
	std::string strPOIStyleID;

	TemplateModel()
		: strIVEPath(""),
		strResourceId(""),
		strDataType(""),
		strImgPath(""),
		strName(""),
		strSceneID(""),
		strID(""),
		strProjectID(""),
		fVersion(0),
		bIsUsed(false),
		strPOIStyleID("") {}

	void loadFromJson(cJSON *pJson) {
		if (pJson != nullptr) {
			if (cJSON_IsObject(pJson)) {
				cJSON_LoadFromObject(pJson, "ivePath", strIVEPath);
				cJSON_LoadFromObject(pJson, "resourceId", strResourceId);
				cJSON_LoadFromObject(pJson, "dataType", strDataType);
				cJSON_LoadFromObject(pJson, "imgPath", strImgPath);
				cJSON_LoadFromObject(pJson, "name", strName);
				cJSON_LoadFromObject(pJson, "sceneId", strSceneID);
				cJSON_LoadFromObject(pJson, "id", strID);
				cJSON_LoadFromObject(pJson, "projectId", strProjectID);
				cJSON_LoadFromObject(pJson, "version", fVersion);
				cJSON_LoadFromObject(pJson, "isUsed", bIsUsed);
				cJSON_LoadFromObject(pJson, "poiStyleId", strPOIStyleID);
			}
		}

		return ;
	}

	cJSON *toJson() {
		cJSON *pJson = cJSON_CreateObject();
		if (pJson != nullptr) {
			cJSON_AddToObject(pJson, "ivePath", strIVEPath);
			cJSON_AddToObject(pJson, "resourceId", strResourceId);
			cJSON_AddToObject(pJson, "dataType", strDataType);
			cJSON_AddToObject(pJson, "imgPath", strImgPath);
			cJSON_AddToObject(pJson, "name", strName);
			cJSON_AddToObject(pJson, "sceneId", strSceneID);
			cJSON_AddToObject(pJson, "id", strID);
			cJSON_AddToObject(pJson, "projectId", strProjectID);
			cJSON_AddToObject(pJson, "version", fVersion);
			cJSON_AddToObject(pJson, "isUsed", bIsUsed);
			cJSON_AddToObject(pJson, "poiStyleId", strPOIStyleID);
		}

		return pJson;
	}

} TemplateModel;

typedef struct StyleModel {
	int nID;
	std::string strResourceName;
	std::string strResourceID;
	int nPackageID;
	std::string strResourceType;
	std::string strStyle;
	std::string strStyleName;
	std::string strProjectIndex;
	double fCreateTime;
	double fUpdateTime;
	std::string strCreaterId;
	std::string strPath;
	std::string strPreviewPath;
	std::string strThumbnail;
	std::string strPreview;
	std::string strFileInfoList;
	std::string strTags;

	StyleModel()
		: nID(0), strResourceName(""), strResourceID(""),
		nPackageID(0), strResourceType(""), strStyle(""),
		strStyleName(""), strProjectIndex(""), fCreateTime(0),
		fUpdateTime(0), strCreaterId(""), strPath(""),
		strPreviewPath(""), strThumbnail(""), strPreview(""),
		strFileInfoList(""), strTags("") {}

	void loadFromJson(cJSON *pJson) {
		if (pJson != nullptr) {
			if (cJSON_IsObject(pJson)) {
				cJSON_LoadFromObject(pJson, "id", nID);
				cJSON_LoadFromObject(pJson, "resourceName", strResourceName);
				cJSON_LoadFromObject(pJson, "resourceId", strResourceID);
				cJSON_LoadFromObject(pJson, "packageId", nPackageID);
				cJSON_LoadFromObject(pJson, "resourceType", strResourceType);
				cJSON_LoadFromObject(pJson, "style", strStyle);
				cJSON_LoadFromObject(pJson, "styleName", strStyleName);
				cJSON_LoadFromObject(pJson, "projectIndex", strProjectIndex);
				cJSON_LoadFromObject(pJson, "createTime", fCreateTime);
				cJSON_LoadFromObject(pJson, "updateTime", fUpdateTime);
				cJSON_LoadFromObject(pJson, "createrId", strCreaterId);
				cJSON_LoadFromObject(pJson, "path", strPath);
				cJSON_LoadFromObject(pJson, "previewPath", strPreviewPath);
				cJSON_LoadFromObject(pJson, "thumbnail", strThumbnail);
				cJSON_LoadFromObject(pJson, "preview", strPreview);
				cJSON_LoadFromObject(pJson, "fileInfoList", strFileInfoList);
				strTags = "";
			}
		}

		return;
	}

	cJSON *toJson() {
		cJSON *pJson = cJSON_CreateObject();
		if (pJson != nullptr) {
			cJSON_AddToObject(pJson, "id", nID);
			cJSON_AddToObject(pJson, "resourceName", strResourceName);
			cJSON_AddToObject(pJson, "resourceId", strResourceID);
			cJSON_AddToObject(pJson, "packageId", nPackageID);
			cJSON_AddToObject(pJson, "resourceType", strResourceType);
			cJSON_AddToObject(pJson, "style", strStyle);
			cJSON_AddToObject(pJson, "styleName", strStyleName);
			cJSON_AddToObject(pJson, "projectIndex", strProjectIndex);
			cJSON_AddToObject(pJson, "createTime", fCreateTime);
			cJSON_AddToObject(pJson, "updateTime", fUpdateTime);
			cJSON_AddToObject(pJson, "createrId", strCreaterId);
			cJSON_AddToObject(pJson, "path", strPath);
			cJSON_AddToObject(pJson, "previewPath", strPreviewPath);
			cJSON_AddToObject(pJson, "thumbnail", strThumbnail);
			cJSON_AddToObject(pJson, "preview", strPreview);
			cJSON_AddToObject(pJson, "fileInfoList", strFileInfoList);

			cJSON *pJsonTags = cJSON_CreateArray();
			if (pJsonTags != nullptr) {
				cJSON_AddItemToObject(pJson, "tags", pJsonTags);
			}
		}

		return pJson;
	}

} StyleModel;