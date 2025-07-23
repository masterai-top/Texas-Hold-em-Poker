#ifndef _GameHttpImp_H_
#define _GameHttpImp_H_

//
#include "servant/Application.h"
#include "HttpServer.h"
#include "GameHttp.h"

/**
 * http请求处理类
 *
 */
class GameHttpImp : public tars::Servant
{
public:
    /**
    *
    */
    GameHttpImp() {}

    /**
     *
     */
    virtual ~GameHttpImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    /**
     *
     */
    virtual int doRequest(tars::TarsCurrentPtr current, vector<char> &response);

    /**
     *
     */
    virtual int doHello(tars::TarsCurrentPtr current, HttpRequestPtr req, HttpResponsePtr rsp);

    /**
    *
    */
    virtual int doVersion(tars::TarsCurrentPtr current, HttpRequestPtr req, HttpResponsePtr rsp);

    /**
    *
    */
    virtual int doReserve(tars::TarsCurrentPtr current, HttpRequestPtr req, HttpResponsePtr rsp);
    
    /**
     *
     */
    virtual int doMonitor(tars::TarsCurrentPtr current, HttpRequestPtr req, HttpResponsePtr rsp);

    /**
     *
     */
    virtual int doReport(tars::TarsCurrentPtr current, HttpRequestPtr req, HttpResponsePtr rsp);

    /**
     *
     */
    virtual int doHttpRequest(tars::TarsCurrentPtr current, string &sUrlKey, HttpRequestPtr req, HttpResponsePtr rsp);

private:
    /*
    * 需要鉴权的请求
    */
    int AuthRequest(TarsCurrentPtr current, HttpRequestPtr Req, vector<tars::Char> &vecPostBody, map<string, string> &mapExtInfo, string &sUrlkey);
};

/////////////////////////////////////////////////////
#endif



