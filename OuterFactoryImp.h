#ifndef _OUTER_FACTORY_IMP_H_
#define _OUTER_FACTORY_IMP_H_

#include <string>
#include "servant/Application.h"
#include "OuterFactory.h"
#include "HttpServer.h"
#include "globe.h"
#include "GameHttp.h"

//
using namespace tars;
using namespace XGame;

/**
 * 外部工具接口对象工厂
 */
class OuterFactoryImp : public OuterFactory
{
public:
    /**
     *
    */
    OuterFactoryImp();

    /**
     *
    */
    virtual ~OuterFactoryImp();

public:
    //框架中用到的outer接口(不能修改):
    const OuterProxyFactoryPtr &getProxyFactory() const
    {
        return _pProxyFactory;
    }

    const tars::TC_Config &getConfig() const
    {
        return *_pFileConf;
    }

    /////////////////////////////////////////////////////////////////////////////////
    //业务自有的outer接口:
    /////////////////////////////////////////////////////////////////////////////////
    const XGame::GameHttpPrx getGameHttpPrx(string &sUrlKey)
    {
        auto iter = _mTransMitSvrIdObjMap.find(sUrlKey);
        if (iter == _mTransMitSvrIdObjMap.end())
            return NULL;

        return Application::getCommunicator()->stringToProxy<XGame::GameHttpPrx>(iter->second);
    }

    //目标处理对象
    std::map<string, string> _mTransMitSvrIdObjMap;

public:
    //异步请求
    void asyncRequestGameHttp(TarsCurrentPtr current, TC_HttpRequest &req, vector<char> &reqData,
                              map<std::string, std::string> &extInfo, string &urlKey, bool hasCallback = true);
    //加载配置
    int loadConfig();
    //加载JSON文件
    int loadJsonFile();
    //JSON文件内容
    std::string& getJsonContent();
    //加载备用JSON文件
    int loadReserveJsonFile();
    //备用JSON文件内容
    std::string& getReserveJsonContent();

private:
    //
    void createAllObject();
    //
    void deleteAllObject();
    //
    int isOpen();

private:
    //框架用到的共享对象(不能修改):
    tars::TC_Config *_pFileConf;
    //
    OuterProxyFactoryPtr _pProxyFactory;
    //
    int m_isOpen;
    //
    std::string jsonStr;
    //
    std::string reserveJsonStr;
};

typedef TC_AutoPtr<OuterFactoryImp> OuterFactoryImpPtr;
extern OuterFactoryImpPtr g_objOuterPrx;

//通用GameHttp消息回调
class RequestGameHttpCallback : public GameHttpPrxCallback
{
public:
    //
    RequestGameHttpCallback(TarsCurrentPtr current, TC_HttpRequest &req) : m_current(current), m_req(req)
    {

    }
    //
    virtual ~RequestGameHttpCallback()
    {

    }
    //
    virtual void callback_doRequest(tars::Int32 ret, const vector<tars::Char> &rspBuf);

    //
    virtual void callback_doRequest_exception(tars::Int32 ret);

private:
    //
    tars::TarsCurrentPtr m_current;
    //
    tars::TC_HttpRequest m_req;
    //
    tars::Int32 m_isOpen;
};

#endif




