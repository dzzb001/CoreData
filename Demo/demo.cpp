#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>

#include <SceneCore/interface/Interface.h>
#include <osgViewer/Viewer>

#include "DataCoreInterface.h"
#include "cJSON.h"

void _data_callback(const char* channel, const char* data, void* pUser);

std::thread g_threadRender;
osg::ref_ptr<osgViewer::View>		g_pViewer = nullptr;
osg::ref_ptr<osgViewer::View>		g_pViewerSlave = nullptr;
bool                                g_bRunning = false;
bool                                g_bStart3D = true;

int ReadPmsJson(const char *path,std::string &content)
{
    std::ifstream inFile(path);
    if (!inFile.is_open()) {
        return -1;
    }

    char buf[4096] = { 0 };
    do
    {
        memset(buf, 0, 4096);
        inFile.read(buf, 4096);
        content.append(buf, inFile.gcount());
        if (inFile.gcount() < 4096)
            break;
    } while (inFile.gcount() > 0);

    inFile.close();

    return 0;
}

void test_getElement()
{
      std::string strQuery = R"(
           {
              "@Column" : [{ "guid": "guid"}]
          }
      )";

    std::string strRespone;
    DataCore::getElement(strQuery, strRespone);
    std::cout << "result:  " << strRespone << std::endl;
}

void test_getElement1(std::string strQuery)
{
    std::string strRespone;
    DataCore::getElement(strQuery, strRespone);
    std::cout << "result:  " << strRespone << std::endl;
}

void switchScene()
{
    std::string strQuery = R"(
           {
              "projectId": "93be9f13-da47-4212-b796-9528c0ca1cce",
              "sceneId" : "277fa1d4-25c8-4d0f-a5d4-4c332e08fa59" 
           }
      )";

    std::string strRespone;
    if (DataCore::canSwitchScence(strQuery, strRespone))
    {
        std::string strContent;
        ReadPmsJson("pmsdata-1.bin", strContent);
        if (!DataCore::switchScence(strContent, strQuery, strRespone))
        {
            //重新加载模型
            DataCore::loadModelsTo3DEngine();
        }
    }
}


void switchScene1()
{
    std::string strQuery = R"(
           {
              "projectId": "8ffecec5-66c9-460e-beb3-f13b865112bd",
              "sceneId" : "57004315-7b0e-4c05-b162-643206e050a8" 
           }
      )";

    std::string strRespone;
    if (DataCore::canSwitchScence(strQuery, strRespone))
    {
        std::string strContent;
        ReadPmsJson("pmsdata.bin", strContent);

        if (!DataCore::switchScence(strContent, strQuery, strRespone))
        {
            //重新加载模型
            DataCore::loadModelsTo3DEngine();
        }
    }
}

#define INVOKE_DATA_FUNCTION(func_type, strQuery) \
    do { \
        std::string strRespone; \
        DataCore::##func_type(strQuery, strRespone); \
        std::cout << "result:  " << strRespone << std::endl; \
    } while(0)


#define INVOKE_DATA_FUNCTION2(func_type, strQuery,strQuery1) \
    do { \
        std::string strRespone; \
        DataCore::##func_type(strQuery, strQuery1,strRespone); \
        std::cout << "result:  " << strQuery << std::endl; \
        std::cout << "result:  " << strQuery1 << std::endl; \
        std::cout << "result:  " << strRespone << std::endl; \
    } while(0)

// INVOKE_DATA_FUNCTION(getTagList, "query_content");

void Init()
{
    std::string strContent;
    ReadPmsJson("pmsdata.bin", strContent);
    DataCore::Init("93sdfds2_C1", "0bd7bb22-1de3-4d09-afe6-2f6cd2c03514", "4376d77f-1bca-4959-b3cf-667e392cb675", "192.168.1.179", 9869, strContent.c_str());
    DataCore::RegisterCallback(_data_callback, nullptr);
}

/*
* app:          app id 93fd_C1
* projectId:    项目id
* sid：         场景id
* resuoceIp：   资源服务器ip
* resoucePort： 资源服务器端口
* json_bin_path：pms json数据
*/
void InitBtParams(const char* app, const char* projectId, const char *sid, const char* resuoceIp, int resoucePort, const char* json_bin_path)
{
    std::string strContent;
    ReadPmsJson(json_bin_path, strContent);
    DataCore::Init(app, projectId, sid, resuoceIp, resoucePort, strContent.c_str());
    DataCore::RegisterCallback(_data_callback, nullptr);
}

void Render_thread()
{
    g_pViewer = dynamic_cast<osgViewer::View*> (Interface::createViewerRTE2(nullptr, 0, 0, 1280, 720, 16, false, false).get());
    std::string ip;
    int nPort = 0;
    bool bEnableVerse = true;
    Interface::initSyzEarth3D(g_pViewer, ip.c_str(), nPort, "./3d_resources", false, bEnableVerse);
    Interface::setSceneMode(ESceneRoam);
    DataCore::loadModelsTo3DEngine();
    while (1) {
        Interface::singleFrame();
        if (!g_bRunning) {
            Interface::close();
            break;
        }
    }
}

void StartRender()
{
    g_bRunning = true;
    g_threadRender = std::thread(Render_thread);
}

void StopRender()
{
    g_bRunning = false;
    g_threadRender.join();
}

void _data_callback(const char* channel, const char* data, void* pUser)
{
    OutputDebugStringA(channel);
    OutputDebugStringA("\n");
    OutputDebugStringA(data);
    OutputDebugStringA("\n");
}

#define BUF_SIZE 1024

void readCmd()
{
    char buf[BUF_SIZE] = { 0 };
    int nCount = 0;
    std::string strBuf; 
    std::string strFuc;
    while (std::cin.getline(buf, BUF_SIZE))
    {
        strBuf.clear();
        strBuf.append(buf, BUF_SIZE);

        std::string::size_type pos = strBuf.find(" ");
        if (std::string::npos == pos) {
            strFuc.append(buf);
            std::cout << "result:  " << strFuc << std::endl;
        }
        else {
            strFuc = strBuf.substr(0, pos);
        }

        if (strFuc.compare("getElement") == 0)
        {
            INVOKE_DATA_FUNCTION(getElement, strBuf.substr(strlen("getElement")));
        }
        else if (strFuc.compare("getTagList") == 0)
        {
            INVOKE_DATA_FUNCTION(getTagList, strBuf.substr(strlen("getTagList")));
        }
        else if (strFuc.compare("getTag") == 0)
        {
            INVOKE_DATA_FUNCTION(getTag, strBuf.substr(strlen("getTag")));
        }
        else if (strFuc.compare("updateTag") == 0)
        {
            INVOKE_DATA_FUNCTION(updateTag, strBuf.substr(strlen("updateTag")));
        }
        else if (strFuc.compare("updateElement") == 0)
        {
            INVOKE_DATA_FUNCTION(updateElement, strBuf.substr(strlen("updateElement")));
        }
        else if (strFuc.compare("deleteElement") == 0)
        {
            INVOKE_DATA_FUNCTION(deleteElement, strBuf.substr(strlen("deleteElement")));
        }
        else if (strFuc.compare("addElement") == 0)
        {
            INVOKE_DATA_FUNCTION(addElement, strBuf.substr(strlen("addElement")));
        }
        else if (strFuc.compare("controlElement") == 0)
        {
            INVOKE_DATA_FUNCTION(controlElement, strBuf.substr(strlen("controlElement")));
        }
        else if (strFuc.compare("getElementTree") == 0)
        {
            INVOKE_DATA_FUNCTION(getElementTree, strBuf.substr(strlen("getElementTree")));
        }
        else if (strFuc.compare("getElementGeneralInterface") == 0)
        {
            INVOKE_DATA_FUNCTION(getElementGeneralInterface, strBuf.substr(strlen("getElementGeneralInterface")));
        }
        else if (strFuc.compare("controlVideoTrackPara") == 0)
        {
            INVOKE_DATA_FUNCTION(controlVideoTrackPara, strBuf.substr(strlen("controlVideoTrackPara")));
        }
        else if (strFuc.compare("switchCallback") == 0)
        {
            INVOKE_DATA_FUNCTION(switchCallback, strBuf.substr(strlen("switchCallback")));
        }
        else if (strFuc.compare("getSceneViewPoint") == 0)
        {
            INVOKE_DATA_FUNCTION(getSceneViewPoint, strBuf.substr(strlen("getSceneViewPoint")));
        }
        else if (strFuc.compare("updateSceneViewPoint") == 0)
        {
            INVOKE_DATA_FUNCTION(updateSceneViewPoint, strBuf.substr(strlen("updateSceneViewPoint")));
        }
        else if (strFuc.compare("getProjectTemplate") == 0)
        {
            INVOKE_DATA_FUNCTION(getProjectTemplate, strBuf.substr(strlen("getProjectTemplate")));
        }
        else if (strFuc.compare("addProjectTemplate") == 0)
        {
            INVOKE_DATA_FUNCTION(addProjectTemplate, strBuf.substr(strlen("addProjectTemplate")));
        }
        else if (strFuc.compare("updateProjectTemplate") == 0)
        {
            INVOKE_DATA_FUNCTION(updateProjectTemplate, strBuf.substr(strlen("updateProjectTemplate")));
        }
        else if (strFuc.compare("deleteProjectTemplate") == 0)
        {
            INVOKE_DATA_FUNCTION(deleteProjectTemplate, strBuf.substr(strlen("deleteProjectTemplate")));
        }
        else if (strFuc.compare("ProjectTemplateGeneralInterface") == 0)
        {
            INVOKE_DATA_FUNCTION(ProjectTemplateGeneralInterface, strBuf.substr(strlen("ProjectTemplateGeneralInterface")));
        }
        else if (strFuc.compare("getSceneList") == 0)
        {
            INVOKE_DATA_FUNCTION(getSceneList, strBuf.substr(strlen("getSceneList")));
        }
        else if (strFuc.compare("GeneralInterface") == 0)
        {
            INVOKE_DATA_FUNCTION(GeneralInterface, strBuf.substr(strlen("GeneralInterface")));
        }
        else if (strFuc.compare("EventGeneralInterface") == 0)
        {
            INVOKE_DATA_FUNCTION(EventGeneralInterface, strBuf.substr(strlen("EventGeneralInterface")));
        }
        else if (strFuc.compare("getCurResourceFormatBuild") == 0)
        {
            std::string str1, str2;
            INVOKE_DATA_FUNCTION2(getCurResourceFormatBuild, str1,str2);
        }
        else if (0 == strcmp("switchScene", buf))
        {
            if (nCount % 2 == 0)
                switchScene();
            else
                switchScene1();
            nCount++;
        }
        else if (0 == strcmp("exit", buf))
        {
            break;
        }
        memset(buf, 0, BUF_SIZE);
        Sleep(10);
    }
}

int main(int argc, char* argv[])
{
    // 启用内存泄漏检测
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    if (argc == 6)
    {
        InitBtParams(argv[0], argv[1], argv[2], argv[3], atoi(argv[4]), argv[5]);
    }else
        Init();

    StartRender();
    readCmd();
    StopRender();
    return 0;
}