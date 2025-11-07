#pragma once

#include <condition_variable>
class CMyCond
{
public:
	CMyCond();
	~CMyCond();

	int wait(std::string id);
	void notify(std::string id);

	void reset() { m_strWaitId = ""; }

	int wait_for(std::string id,int nSeconds);
public:
	std::string m_strWaitId;

	std::mutex	m_mutex;
	std::condition_variable m_cv;
private:
	bool bState = false;
};

