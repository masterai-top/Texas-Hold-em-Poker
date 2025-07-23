#include <sstream>
#include "GameHttpImp.h"
#include "servant/Application.h"
#include "OuterFactoryImp.h"
#include "XGameHttp.pb.h"
#include "AsyncLoginCallback.h"

using namespace std;
using namespace XGame;
using namespace XGameHttp;

//需要鉴权的接口
extern vector<string> gvecAuthCGI;
extern map<string, string> gmapAuthCGI;
extern string gsLoginObjName;

//////////////////////////////////////////////////////
void GameHttpImp::initialize()
{
    //initialize servant here:
    //...

    LOG_DEBUG << "GameHttpImp Init ..." << endl;
}

//////////////////////////////////////////////////////
void GameHttpImp::destroy()
{
    //destroy servant here:
    //...
}

int GameHttpImp::doRequest(tars::TarsCurrentPtr current, vector<char> &response)
{
    current->setResponse(false);

    auto req = new HttpRequest();
    auto rsp = new HttpResponse();

    __TRY__

    auto v = current->getRequestBuffer();
    req->decode(&v[0], v.size());

    string sUrlKey = req->getRequestUrl();
    LOG_DEBUG << "requestUrl : " << sUrlKey<<", "<< TC_Common::lower(sUrlKey) << endl;

    bool bFindFlag = false;
    if (TC_Common::lower(sUrlKey) == "/version")
    {
        doVersion(current, req, rsp);
        bFindFlag = true;
    }
    else if(TC_Common::lower(sUrlKey) == "/reserve")
    {
        doReserve(current, req, rsp);
        bFindFlag = true;
    }
    else if (TC_Common::lower(sUrlKey) == "/hello")
    {
        doHello(current, req, rsp);
        bFindFlag = true;
    }
    else
    {
        string sUrl = "/";
        auto urlList = TC_Common::sepstr<string>(sUrlKey, "/");
        if(!urlList.empty())
        {
            sUrl += urlList[0];
        }

        auto it = g_objOuterPrx->_mTransMitSvrIdObjMap.find(sUrl);
        if (it != g_objOuterPrx->_mTransMitSvrIdObjMap.end())
        {
            doHttpRequest(current, sUrl, req, rsp);
            bFindFlag = true;
        }
        else
        {
            bFindFlag = false;
        }
    }

    if (!bFindFlag)
    {
        g_app.sendErrorPage(current, *req, 404);
    }

    return 0;
    __CATCH__
    g_app.sendErrorPage(current, *req, 500);
    return 0;
}

int GameHttpImp::doHello(TarsCurrentPtr current, HttpRequestPtr req, HttpResponsePtr rsp)
{
    static int iCount = 0;
    iCount++;

    stringstream s;
    s << "Remote IP: " << req->getHeader("X-Forwarded-For-Pound") << "\r" << endl;
    s << "Remote IP: " << current->getIp() << "\r" << endl;
    s << "Url: " << req->getURL().getURL() << "\r" << endl;
    s << "Count: " << iCount << "\r" << endl;
    s << "\r" << endl;
    rsp->setHeader("Access-Control-Allow-Origin", "*");
    rsp->setResponse(s.str().c_str(), s.str().size());
    g_app.sendHttpResponse(current, *req, *rsp);
    return 0;
}

int GameHttpImp::doVersion(TarsCurrentPtr current, HttpRequestPtr req, HttpResponsePtr rsp)
{
    std::string str = g_objOuterPrx->getJsonContent();
    rsp->setResponse(str.c_str(), str.length());
    g_app.sendHttpResponse(current, *req, *rsp);
    return 0;
}

int GameHttpImp::doReserve(TarsCurrentPtr current, HttpRequestPtr req, HttpResponsePtr rsp)
{
    std::string str = g_objOuterPrx->getReserveJsonContent();
    rsp->setResponse(str.c_str(), str.length());
    g_app.sendHttpResponse(current, *req, *rsp);
    return 0;
}

int GameHttpImp::doMonitor(TarsCurrentPtr current, HttpRequestPtr req, HttpResponsePtr rsp)
{
    string str = "Hello GameHttpServer.";
    rsp->setResponse(200, "OK", str);
    g_app.sendHttpResponse(current, *req, *rsp);
    return 0;
}

int GameHttpImp::doReport(TarsCurrentPtr current, HttpRequestPtr req, HttpResponsePtr rsp)
{
#if 0
    __TRY__

    if(!req->isPOST())
    {
        LOG_ERROR << req->getRequestUrl() << " should be POST req" << endl;
        g_app.sendErrorPage(current, *req, 500);
        return 0;
    }

    string sPostData = req->getContent();
    LOG_DEBUG << " Content-Length: " << req->getContentLength() << " Real Data Length: " << sPostData.size() << endl;

    if (sPostData.size() <= 0)
    {
        LOG_ERROR << "Content length too short|" << sPostData.size() << endl;
        g_app.sendErrorPage(current, *req, 500);
        return 0;
    }

    TarsInputStream<> jis;
    jis.setBuffer(sPostData.data(), sPostData.size());
    proto4mqqgameclient::MqqGameClientActiveInfo pkgReq;
    pkgReq.readFrom(jis);

    //g_app.sendErrorPage(current, *req, 500);
    rsp->setResponse(200, "OK", "success");
    g_app.sendHttpResponse(current, *req, *rsp);

    tars::TC_Http::http_header_type rspHeaders = rsp->getHeaders();
    for (auto it = rspHeaders.begin(); it != rspHeaders.end(); ++it)
    {
        LOG_DEBUG << it->first << ": " << it->second << endl;
    }


    FDLOG("report") << pkgReq.mac << "|" << pkgReq.gameid << "|" << pkgReq.uin << "|" << pkgReq.os << "|" << pkgReq.osver << endl;
    return 0;

    __CATCH__

    g_app.sendErrorPage(current, *req, 500);
#endif
    return 0;
}


int GameHttpImp::doHttpRequest(TarsCurrentPtr current, string &sUrlKey, HttpRequestPtr req, HttpResponsePtr rsp)
{
    __TRY__

    // 处理跨域请求
    // if (req->isOPTIONS())
    // {
    //     g_app.allowCrow(current, *req);
    //     return 0;
    // }

    if (!req->isPOST())
    {
        LOG_ERROR << req->getRequestUrl() << " should be POST req" << endl;
        g_app.sendErrorPage(current, *req, 500);
        return 0;
    }

    string sPostData = req->getContent();
    if (sPostData.empty())
    {
        LOG_ERROR << "Content length too short, sz=" << sPostData.size() << endl;
        g_app.sendErrorPage(current, *req, 500);
        return 0;
    }

    LOG_DEBUG << "CGI-SubPath:" << req->getRequestUrl() << " Content-Length: " << req->getContentLength() << ", Real Data Length: " << sPostData.size()<< ", sUrlKey:"<< sUrlKey << endl;

    map<string, string> extInfo;
    extInfo["RemoteIp"] = g_app.getClientIP(current, *req);
    extInfo["VisitUrl"] = req->getURL().getURL();

    vector<char> reqData;
    reqData.assign(sPostData.c_str(), sPostData.c_str() + sPostData.size());
    auto it = gmapAuthCGI.find(req->getRequestUrl());
    if (it != gmapAuthCGI.end())
    {
        AuthRequest(current, req, reqData, extInfo, sUrlKey);
        return 0;
    }

    g_objOuterPrx->asyncRequestGameHttp(current, *req, reqData, extInfo, sUrlKey, true);
    return 0;
    __CATCH__
    g_app.sendErrorPage(current, *req, 500);
    return 0;
}

/*
* 需要鉴权的请求
*/
int GameHttpImp::AuthRequest(TarsCurrentPtr current, HttpRequestPtr Req, vector<tars::Char> &vecPostBody, map<string, string> &mapExtInfo, string &sUrlkey)
{
    __TRY__

    XGameHttp::THttpPackage tHttpPkt;
    if (!vecPostBody.empty())
    {
        pbToObj(vecPostBody, tHttpPkt);
    }

    LOG_DEBUG << tHttpPkt.stuid().stoken() << "|Uin=" << tHttpPkt.stuid().luid() << endl;

    login::CheckLoginTokenReq reqData;
    reqData.lUid = tHttpPkt.stuid().luid();
    reqData.sToken = tHttpPkt.stuid().stoken();
    auto cb = new AsyncLoginCallback(current, Req, vecPostBody, mapExtInfo, sUrlkey, reqData);
    auto pProxy = Application::getCommunicator()->stringToProxy<LoginServantPrx>(gsLoginObjName);
    if (pProxy)
    {
        pProxy->tars_hash(tHttpPkt.stuid().luid())->async_checkLoginToken(cb, reqData);
    }
    else
    {
        LOG_ERROR << " pProxy is null!" << endl;
    }

    LOG_DEBUG << "auth request has sent." << endl;
    __CATCH__
    return 0;
}



