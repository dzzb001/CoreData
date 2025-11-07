#pragma once
#include "cJSON.h"

class CJson_base
{
public:
	CJson_base(cJSON* p) : m_pJson(p) {}
	virtual ~CJson_base() {
		if (m_pJson)
			cJSON_Delete(m_pJson);
	};

protected:
	cJSON* m_pJson;
};