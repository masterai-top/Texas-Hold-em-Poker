#include "AsyncLoginCallback.h"
#include "globe.h"
#include "LogComm.h"

using namespace XGame;

AsyncLoginCallback::AsyncLoginCallback(tars::TarsCurrentPtr current, HttpRequestPtr httpReq, vector<char> &vecPostBody, map<string, string> &mapExtInfo, string &sUrlkey, const CheckLoginTokenReq &req)
    : current(current), request(httpReq), vecPostBody(vecPostBody), mapExtInfo(mapExtInfo), sUrlkey(sUrlkey), reqData(req)
{

}

AsyncLoginCallback::~AsyncLoginCallback()
{

}

void AsyncLoginCallback::callback_checkLoginToken(tars::Int32 ret)
{
    LOG_DEBUG << "check login token callback, ret : " << ret << endl;

    FDLOG_LOGIN_INFO << "|" << reqData.lUid << "|" << reqData.sToken << "|" << ret << "|" << endl;

    if (ret == 0)
        g_objOuterPrx->asyncRequestGameHttp(current, *request, vecPostBody, mapExtInfo, sUrlkey, true);
    else
        g_app.sendErrorPage(current, *request, 500);
}

void AsyncLoginCallback::callback_checkLoginToken_exception(tars::Int32 ret)
{
    LOG_DEBUG << "check login token callback exception, ret : " << ret << endl;
    FDLOG_LOGIN_EXCEPTION_INFO << "|" << reqData.lUid << "|" << reqData.sToken << "|" << ret << "|" << endl;
    g_app.sendErrorPage(current, *request, 500);
}

