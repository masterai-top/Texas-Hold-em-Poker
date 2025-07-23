#ifndef _ASYNC_LOGIN_CB_H_
#define _ASYNC_LOGIN_CB_H_

//
#include "LoginServant.h"
#include "LoginProto.h"
#include "OuterFactoryImp.h"

//
using namespace XGame;
using namespace login;

/**
* Login异步回调类
*/
class AsyncLoginCallback : public LoginServantPrxCallback
{
public:
    /**
     *
    */
    AsyncLoginCallback(tars::TarsCurrentPtr current, HttpRequestPtr httpReq, vector<char> &vecPostBody, map<string, string> &mapExtInfo, string &sUrlkey, const CheckLoginTokenReq &req);

    /**
     *
    */
    ~AsyncLoginCallback();

public:
    //
    void callback_checkLoginToken(tars::Int32 ret);
    //
    void callback_checkLoginToken_exception(tars::Int32 ret);

private:
    //
    TarsCurrentPtr current;
    //
    HttpRequestPtr request;
    //
    vector<char> vecPostBody;
    //
    map<string, string> mapExtInfo;
    //
    string sUrlkey;
    //
    CheckLoginTokenReq reqData;
};

#endif


