#ifndef _OUTER_FACTORY_IMP_H_
#define _OUTER_FACTORY_IMP_H_

#include <string>
#include <map>

#include "servant/Application.h"
#include "globe.h"
#include "OuterFactory.h"

//wbl
#include <wbl/regex_util.h>

//配置服务
#include "ConfigServant.h"
#include "HallServant.h"
#include "GlobalServant.h"
#include "PushServant.h"

//
using namespace config;
using namespace mail;
using namespace global;
using namespace hall;
using namespace GoodsManager;

//时区
#define ONE_DAY_TIME (24*60*60)
#define ZONE_TIME_OFFSET (8*60*60)

//
class OuterFactoryImp;
typedef TC_AutoPtr<OuterFactoryImp> OuterFactoryImpPtr;

/**
 * 外部工具接口对象工厂
 */
class OuterFactoryImp : public OuterFactory
{
private:
    /**
     *
    */
    OuterFactoryImp();

    /**
     *
    */
    ~OuterFactoryImp();

    //
    friend class GMServantImp;
    //
    friend class GMServer;

public:
    //框架中用到的outer接口(不能修改):
    const OuterProxyFactoryPtr &getProxyFactory() const
    {
        return _pProxyFactory;
    }

    tars::TC_Config &getConfig() const
    {
        return *_pFileConf;
    }

public:
    //加载配置
    void load();
    //代理配置
    void readPrxConfig();
    //
    void printPrxConfig();

private:
    //
    void createAllObject();
    //
    void deleteAllObject();

public:
    // 获取代理
    const config::ConfigServantPrx getConfigServantPrx();
    //
    const hall::HallServantPrx getHallServantPrx(const long uid);
        //
    const hall::HallServantPrx getHallServantPrx(const string key);
    //
    const global::GlobalServantPrx getGlobalServantPrx(const long uid);
    //PushServer代理
    const push::PushServantPrx getPushServerPrx(const long uid);

public:
    //格式化时间
    string GetTLogTimeFormat();

private:
    //拆分字符串成整形
    int splitInt(string szSrc, vector<int> &vecInt);

private:
    //域名解析
    void getIp(char *domain, char *ip);
    //域名解析
    string getIp(const string &domain);

private:
    wbl::ReadWriteLocker m_rwlock;

private:
    //框架用到的共享对象(不能修改):
    tars::TC_Config *_pFileConf;

    OuterProxyFactoryPtr _pProxyFactory;

private:
    //业务自有的共享对象:
    //...
    // 用户信息
    std::string _HallServantObj;
    HallServantPrx _HallServerPrx;

    std::string _GlobalServantObj;
    global::GlobalServantPrx _GlobalServantPrx;

    std::string _ConfigServantObj;
    ConfigServantPrx _ConfigServantPrx;

    //推送服务
    string _sPushServantObj;
    push::PushServantPrx _pushServerPrx;
};

////////////////////////////////////////////////////////////////////////////////
#endif


