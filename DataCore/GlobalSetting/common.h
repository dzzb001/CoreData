#pragma once
#pragma execution_character_set("utf-8")

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <SceneCore/interface/Interface.h>

#include "cJSON.h"
#include "SceneDataType.h"

template<typename T>
inline void cJSON_LoadFromValue(cJSON *pJson, T &tValue) {
	if (pJson != nullptr) {
		if constexpr (std::is_same_v<T, int>) {
			if (cJSON_IsNumber(pJson)) {
				tValue = pJson->valueint;
			}
		} else if constexpr (std::is_same_v<T, double>) {
			if (cJSON_IsNumber(pJson)) {
				tValue = pJson->valuedouble;
			}
		} else if constexpr (std::is_same_v<T, bool>) {
			if (cJSON_IsBool(pJson)) {
				if (cJSON_IsTrue(pJson)) {
					tValue = true;
				} else {
					tValue = false;
				}
			}
		} else if constexpr (std::is_same_v<T, std::string>) {
			if (cJSON_IsString(pJson)) {
				tValue = pJson->valuestring;
			}
		} else {
			// pass
		}
	}

	return;
}

template<typename T>
inline void cJSON_LoadFromObject(cJSON *pJson, std::string strKey, T &tValue) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			cJSON *pJsonItem = cJSON_GetObjectItem(pJson, strKey.c_str());
			if (pJsonItem != nullptr) {
				cJSON_LoadFromValue(pJsonItem, tValue);
			}
		}
	}

	return;
}

template<typename T>
inline void cJSON_AddToObject(cJSON *pJson, std::string strKey, T tValue) {
	if (pJson != nullptr) {
		if (cJSON_IsObject(pJson)) {
			if constexpr (std::is_same_v<T, int>) {
				cJSON_AddNumberToObject(pJson, strKey.c_str(), (double)tValue);
			} else if constexpr (std::is_same_v<T, double>) {
				cJSON_AddNumberToObject(pJson, strKey.c_str(), tValue);
			} else if constexpr (std::is_same_v<T, bool>) {
				cJSON_AddBoolToObject(pJson, strKey.c_str(), (cJSON_bool)tValue);
			} else if constexpr (std::is_same_v<T, std::string>) {
				cJSON_AddStringToObject(pJson, strKey.c_str(), tValue.c_str());
			} else {
				// pass
			}
		}
	}

	return;
}

std::string cJSON_PrintFromJSON(cJSON *pJson);