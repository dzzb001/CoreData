#pragma once
#include <string>

#define GetConfigSettingInstance() CConfigSetting::getInstance()

// »´æ÷…Ë÷√
class CConfigSetting {
public:
	CConfigSetting();
	~CConfigSetting();

public:
	static CConfigSetting& getInstance(){ return s_ConfigSetting;}
	static CConfigSetting s_ConfigSetting;

	void GetResouceInfo(std::string& ip, unsigned int& port){ ip = m_strResouceIp; port = m_nResoucePort; }
	void SetResouceInfo(std::string ip, unsigned int port) { m_strResouceIp = ip; m_nResoucePort = port; }

	void SetAppId(std::string id) { m_strAppId = id; }
	std::string GetAppId() { return m_strAppId; }
private:
	std::string m_strAppId;
	std::string m_strResouceIp;
	unsigned int m_nResoucePort;
};