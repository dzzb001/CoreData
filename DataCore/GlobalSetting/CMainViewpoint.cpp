#include "CMainViewpoint.h"

// Ö÷ÊÓµã
CMainViewpoint::CMainViewpoint() : IGlobalSetting() {
	return;
}

CMainViewpoint::~CMainViewpoint() {
	return;
}

void CMainViewpoint::loadFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			m_viewpoint.loadFromJson(pJson);
		}
	}

	return;
}

cJSON *CMainViewpoint::toJson() {
	return m_viewpoint.toJson();
}

void CMainViewpoint::setMainViewpoint() {
	cJSON *pJson = cJSON_CreateObject();
	if (pJson != nullptr) {
		cJSON_AddToObject(pJson, "method", std::string("control"));

		cJSON *pJsonSceneViewPoint = m_viewpoint.to3DJson();
		if (pJsonSceneViewPoint != nullptr) {
			cJSON_AddItemToObject(pJson, "SceneViewPoint", pJsonSceneViewPoint);

			std::string strJson;
			std::string resultJson;

			strJson = cJSON_PrintFromJSON(pJson);
			cJSON_Delete(pJson);

			Interface::elementHandle(strJson, resultJson);
		}
	}

	return ;
}

cJSON *CMainViewpoint::mainViewpointToJson(cJSON *pJson) {
	cJSON *pJsonRespone = nullptr;

	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			bool bHome;
			cJSON_LoadFromObject(pJson, "home", bHome);

			cJSON *pJsonRequest = cJSON_CreateObject();
			if (pJsonRequest != nullptr) {
				cJSON_AddToObject(pJsonRequest, "method", std::string("get"));

				cJSON *pJsonSceneViewPoint = cJSON_CreateObject();
				if (pJsonSceneViewPoint != nullptr) {
					cJSON_AddToObject(pJsonSceneViewPoint, "home", bHome);

					cJSON_AddItemToObject(pJsonRequest, "SceneViewPoint", pJsonSceneViewPoint);

					std::string strJson;
					std::string resultJson;

					strJson = cJSON_PrintFromJSON(pJsonRequest);
					cJSON_Delete(pJsonRequest);

					Interface::elementHandle(strJson, resultJson);
					
					cJSON *pJsonResult = cJSON_Parse(resultJson.c_str());
					if (pJsonResult != nullptr) {
						if (cJSON_IsObject(pJsonResult)) {
							cJSON *pJsonData = cJSON_GetObjectItem(pJsonResult, "data");
							if (pJsonData != nullptr) {
								if (cJSON_IsObject(pJsonData)) {
									Viewpoint viewpoint;
									viewpoint = m_viewpoint;
									viewpoint.loadFrom3DJson(pJsonData);
									pJsonRespone = viewpoint.toJson();
								}
							}
						}

						cJSON_Delete(pJsonResult);
					}
				}
			}
		}
	}

	return pJsonRespone;
}

void CMainViewpoint::updateMainViewpointFromJson(cJSON *pJson) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			this->loadFromJson(pJson);
			this->setMainViewpoint();
		}
	}

	return ;
}