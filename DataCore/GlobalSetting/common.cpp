#include "common.h"

std::string cJSON_PrintFromJSON(cJSON *pJson) {
	std::string strJson = "";
	char *pCh = cJSON_Print(pJson);
	if (pCh != nullptr) {
		strJson = pCh;

		free(pCh);
		pCh = nullptr;
	}

	return strJson;
}