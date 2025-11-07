#include "CMyCond.h"

CMyCond::CMyCond()
{
}

CMyCond::~CMyCond()
{
	m_cv.notify_all();
}

int CMyCond::wait(std::string id)
{
	m_strWaitId = id;
	bState = false;
	if (m_strWaitId.compare(""))
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait(lock);
		return true;
	}
	return false;
}

int CMyCond::wait_for(std::string id,int nSeconds)
{
	printf("wait for id %s\n", id.c_str());
	m_strWaitId = id;
	bState = false;
	std::unique_lock<std::mutex> lock(m_mutex);
	return m_cv.wait_for(lock, std::chrono::seconds(nSeconds), [&] { return bState; });
}

void CMyCond::notify(std::string id)
{
	if (!m_strWaitId.compare(id)) {
		std::lock_guard<std::mutex> lock(m_mutex);
		bState = true;
		m_cv.notify_one();
	}
}