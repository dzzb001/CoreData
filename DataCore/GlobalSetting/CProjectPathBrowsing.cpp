#include "CProjectPathBrowsing.h"

//#include <Poco/Net/HTTPClientSession.h>
//#include <Poco/Net/HTTPRequest.h>
//#include <Poco/Net/HTTPResponse.h>
//#include <Poco/StreamCopier.h>

#include <sstream>

//#include "../Config.h"

CProjectPathBrowsing::CProjectPathBrowsing() : IProjectTemplate() {
    return;
}

CProjectPathBrowsing::~CProjectPathBrowsing() {
    return;
}

void CProjectPathBrowsing::loadFromJson(cJSON */*pJson*/) {
    m_mapPathBrowsing.clear();

    cJSON *pJsonStyle = cJSON_Parse(this->getFromPMS().c_str());
    if (pJsonStyle != nullptr) {
        if (cJSON_IsObject(pJsonStyle)) {
            cJSON *pJsonRecords = cJSON_GetObjectItem(pJsonStyle, "records");
            if (pJsonRecords != nullptr) {
                if (cJSON_IsArray(pJsonRecords)) {
                    for (int i = 0; i < cJSON_GetArraySize(pJsonRecords); ++i) {
                        cJSON *pJsonItem = cJSON_GetArrayItem(pJsonRecords, i);
                        if (pJsonItem != nullptr) {
                            StyleModel styleModel;
                            styleModel.loadFromJson(pJsonItem);

                            m_mapPathBrowsing[styleModel.strResourceID] = styleModel;
                        }
                    }
                }
            }
        }
    }

    return;
}

cJSON *CProjectPathBrowsing::toJson() {
    cJSON *pJson = cJSON_CreateObject();
    if (pJson != nullptr) {
        cJSON *pJsonRecords = cJSON_CreateArray();
        if (pJsonRecords != nullptr) {
            for (auto [strKey, value] : m_mapPathBrowsing) {
                cJSON *pJsonItem = value.toJson();
                if (pJsonItem != nullptr) {
                    cJSON_AddItemToArray(pJsonRecords, pJsonItem);
                }
            }

            cJSON_AddItemToObject(pJson, "records", pJsonRecords);
        }
    }

    return pJson;
}

void CProjectPathBrowsing::addFromJson(cJSON *pJson) {
    return;
}

void CProjectPathBrowsing::updateFromJson(cJSON *pJson) {
    return;
}

void CProjectPathBrowsing::deleteByResourceID(std::string strResourceID) {
    return;
}

std::unordered_set<std::string> CProjectPathBrowsing::getResourceIDList() {
    std::unordered_set<std::string> setResourceID;

    for (auto [strKey, value] : m_mapPathBrowsing) {
        setResourceID.insert(strKey);
    }

    return setResourceID;
}

cJSON *CProjectPathBrowsing::getByResourceID(std::string strResourceID) {
    cJSON *pJson = nullptr;

    auto iter = m_mapPathBrowsing.find(strResourceID);
    if (iter != m_mapPathBrowsing.end()) {
        pJson = iter->second.toJson();
    }

    return pJson;
}

cJSON *CProjectPathBrowsing::getByName(std::string strResourceID, std::string strName) {
    return cJSON_CreateObject();
}

std::string CProjectPathBrowsing::getFromPMS() {
#if 0
    std::string strPMSIp = Config::CConfig::GetInstance().GetPMSIp();
    unsigned short nPort = Config::CConfig::GetInstance().GetPMSPort();
    std::string strURL = "/v2/api/pms/resources?pageNo=1&pageSize=99999&resourceType=image&style=image_lujingguihua";

    Poco::Net::HTTPClientSession session(strPMSIp.c_str(), nPort);
    session.setKeepAlive(true);

    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, strURL);

    request.setContentLength(0);
    request.setContentType("application/json");

    request.setKeepAlive(true);
    session.sendRequest(request);

    Poco::Net::HTTPResponse response;
    std::istream &responseStream = session.receiveResponse(response);

    std::ostringstream out_string_stream;
    Poco::StreamCopier::copyStream(responseStream, out_string_stream);
    std::string strBody = out_string_stream.str();

    return strBody;
#else
    return std::string();
#endif
}