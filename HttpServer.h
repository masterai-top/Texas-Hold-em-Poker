#ifndef _HttpServer_H_
#define _HttpServer_H_

#include <iostream>
#include "servant/Application.h"
#include "globe.h"

using namespace tars;

class HttpRequest : public TC_HttpRequest, public TC_HandleBase
{
public:
    HttpRequest()
    {

    }

    virtual ~HttpRequest()
    {

    }
};

/**
 *
*/
class HttpResponse : public TC_HttpResponse, public TC_HandleBase
{
public:
    HttpResponse()
    {

    }

    virtual ~HttpResponse()
    {

    }
};

//
typedef TC_AutoPtr<HttpRequest>  HttpRequestPtr;
typedef TC_AutoPtr<HttpResponse> HttpResponsePtr;

/**
 *
 **/
class HttpServer : public Application
{
public:
    /**
    *
    **/
    HttpServer()
    {

    }

    /**
    *
    **/
    virtual ~HttpServer()
    {

    }

    /**
    *
    **/
    virtual void initialize();

    /**
    *
    **/
    virtual void destroyApp();

public:
    /*
    * 加载服务配置
    */
    virtual void loadConfig();

    /*
    * 配置变更，重新加载配置
    */
    bool reloadSvrConfig(const string &command, const string &params, string &result);

public:
    /**
    * 请求ip
    */
    string getClientIP(tars::TarsCurrentPtr current, TC_HttpRequest &req);

    /**
     * 允许跨域请求
     */
    void allowCrow(TarsCurrentPtr current, TC_HttpRequest &req);

    /**
    * 404、500错误处理
    */
    void sendErrorPage(tars::TarsCurrentPtr current, TC_HttpRequest &req, int status);

    /**
    * 应答处理
    */
    void sendHttpResponse(tars::TarsCurrentPtr current, TC_HttpRequest &req, TC_HttpResponse &rsp);

private:
    //保持连接标志
    volatile bool m_bKeepAlive;
    //打印日志标记
    volatile bool m_bAccessLog;
};

////
extern HttpServer g_app;

////////////////////////////////////////////
#endif


