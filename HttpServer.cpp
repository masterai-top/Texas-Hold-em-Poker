#include "HttpServer.h"
#include "GameHttpImp.h"
#include "LogComm.h"
#include "OuterFactoryImp.h"
#include "util/tc_network_buffer.h"

//
using namespace std;

//
HttpServer g_app;

//需要鉴权的接口
vector<string> gvecAuthCGI;
map<string, string>gmapAuthCGI;

//登录鉴权对象名
string gsLoginObjName;

/////////////////////////////////////////////////////////////////
struct HttpProtocol
{
    /**
    * 解析Http请求
    * @param in
    * @param out
    *
    * @return int
    */
    static int parseHttp(string &in, string &out)
    {
        try
        {
            if (TC_HttpRequest::checkRequest(in.c_str(), in.length()))
            {
                out = in;
                in  = "";
                return TC_NetWorkBuffer::PACKET_TYPE::PACKET_FULL;
            }

            return TC_NetWorkBuffer::PACKET_TYPE::PACKET_LESS;
        }
        catch (exception &ex)
        {
            LOG_DEBUG << "parse http error: " << ex.what() << endl;
            return TC_NetWorkBuffer::PACKET_TYPE::PACKET_ERR;
        }
    }

    static TC_NetWorkBuffer::PACKET_TYPE parseHttp2(TC_NetWorkBuffer &in, vector<char> &out)
    {
        try
        {
            if (TC_HttpRequest::checkRequest(in.getBuffersString().c_str(), in.getBufferLength()))
            {
                out = in.getBuffers();
                in.clearBuffers();
                return TC_NetWorkBuffer::PACKET_TYPE::PACKET_FULL;
            }

            return TC_NetWorkBuffer::PACKET_TYPE::PACKET_LESS;
        }
        catch (exception &ex)
        {
            LOG_DEBUG << "parse http error: " << ex.what() << endl;
            return TC_NetWorkBuffer::PACKET_TYPE::PACKET_ERR;
        }
    }
};


/////////////////////////////////////////////////////////////////
void HttpServer::initialize()
{
    //initialize application here:
    //...

    addServant<GameHttpImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".GameHttpObj");
    addServantProtocol(ServerConfig::Application + "." + ServerConfig::ServerName + ".GameHttpObj", &HttpProtocol::parseHttp2);

    //
    m_bKeepAlive = true;
    m_bAccessLog = true;

    LOG_INFO << "Begin To Call loadConfig" << endl;

    //加载服务配置
    loadConfig();

    //
    g_objOuterPrx = new OuterFactoryImp;

    //
    LOG_DEBUG << "Begin To Call reload Svr Config" << endl;

    // 注册动态加载命令
    TARS_ADD_ADMIN_CMD_NORMAL("reload", HttpServer::reloadSvrConfig);
}

/////////////////////////////////////////////////////////////////
void
HttpServer::destroyApp()
{
    //destroy application here:
    //...
}

/*
* 加载服务配置
*/
void HttpServer::loadConfig()
{
    __TRY__

    //拉取远程配置
    addConfig(ServerConfig::ServerName + ".conf");
    //游戏版本配置
    addConfig("App.json");
    //游戏版本配置
    addConfig("ResApp.json");

    //本地配置文件
    TC_Config conf;

    conf.parseFile(ServerConfig::BasePath + ServerConfig::ServerName + ".conf");

    m_bKeepAlive = S2B(conf.get("/Main/App<KeepAlive>", "Y"));
    m_bAccessLog = S2B(conf.get("/Main/App<AccessLog>", "Y"));

    FDLOG_CONFIG_INFO << "m_bKeepAlive: " << m_bKeepAlive << ", m_bAccessLog: " << m_bAccessLog << endl;

    //登录鉴权对象名
    gsLoginObjName = conf.get("/Main/<LoginObj>", "");
    if (gsLoginObjName.empty())
    {
        LOG_DEBUG << "LoginObjName Is Empty" << endl;
        exit(0);
    }

    FDLOG_CONFIG_INFO << "gsLoginObjName: " << gsLoginObjName << endl;

    //需要鉴权的接口
    gvecAuthCGI = conf.getDomainKey("/Main/AuthCGI/");
    if (gvecAuthCGI.size() == 0)
    {
        return;
    }

    gmapAuthCGI.clear();
    for (auto iter = gvecAuthCGI.begin(); iter != gvecAuthCGI.end(); iter++)
    {
        gmapAuthCGI[*iter] = *iter;
        FDLOG_CONFIG_INFO << "AuthCGI:" << *iter << endl;
    }

    __CATCH__
}

/*
* 配置变更，重新加载配置
*/
bool HttpServer::reloadSvrConfig(const string &command, const string &params, string &result)
{
    try
    {
        //加载服务配置
        loadConfig();

        //加载配置
        g_objOuterPrx->loadConfig();
        result = "reload server config success";
        LOG_DEBUG << "reloadSvrConfig: " << result << endl;
        return true;
    }
    catch (TC_Exception const &e)
    {
        result = string("catch tc exception: ") + e.what();
    }
    catch (std::exception const &e)
    {
        result = string("catch std exception: ") + e.what();
    }
    catch (...)
    {
        result = "catch unknown exception";
    }

    result += "\n fail, please check it.";

    LOG_DEBUG << "reloadSvrConfig: " << result << endl;

    return true;
}

/**
* 请求ip
*/
string HttpServer::getClientIP(TarsCurrentPtr current, TC_HttpRequest &req)
{
    string ip = "";

    // ip = req.getHeader("X-Forwarded-For-Pound");
    ip = req.getHeader("X-Real-IP");
    if (ip.length() > 0)
    {
        return ip;
    }

    //
    ip = current->getIp();
    if (ip.length() > 0)
    {
        return ip;
    }

    return ip;
}

/**
 * [HttpServer::allowCrow description]
 * @param current [description]
 * @param req     [description]
 */
void HttpServer::allowCrow(TarsCurrentPtr current, TC_HttpRequest &req)
{
    TC_HttpResponse rsp;
    rsp.setStatus(204);
    rsp.setHeader("Access-Control-Allow-Origin", "*");
    rsp.setHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    rsp.setHeader("Access-Control-Allow-Headers", "DNT,X-Mx-ReqToken,Keep-Alive,User-Agent,X-Requested-With,If-Modified-Since,Cache-Control,Content-Type,Authorization");
    sendHttpResponse(current, req, rsp);
}

/**
* 404、500错误处理
*/
void HttpServer::sendErrorPage( TarsCurrentPtr current, TC_HttpRequest &req, int status)
{
    TC_HttpResponse rsp;
    rsp.setStatus(status);
    if (status == 404)
    {
        string str =
            "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">"
            "<html><head><title>404 Not Found</title></head><body><h1>Not Found</h1></body></html>";

        rsp.setResponse(status, "NOT FOUND", str);
    }
    else if (status == 500)
    {
        string str =
            "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">"
            "<html><head><title>500 Internal Server Error</title></head>"
            "<body><h1>Internal Server Error</h1><p>The server encountered an internal error or misconfiguration and was unable to complete your request.</p></body></html>";

        rsp.setResponse(status, "INTERNAL SERVER ERROR", str);
    }

    //应答
    sendHttpResponse(current, req, rsp);
}

/**
* 应答处理
*/
void HttpServer::sendHttpResponse(TarsCurrentPtr current, TC_HttpRequest &req, TC_HttpResponse &rsp)
{
    rsp.setServer("HttpServer");

    if (m_bKeepAlive && (req.getHeader("Connection") == "keep-alive"))
    {
        rsp.setConnection("keep-alive");
    }

    vector<char> response;
    rsp.encode(response);

    //应答
    current->sendResponse(&response[0], response.size());

    //判断是否Keep-Alive，如果是，则不关闭链接，否则关闭链接
    if (req.getHeader("Connection") != "keep-alive" || !m_bKeepAlive)
    {
        FDLOG("disconnect")
                << "|" << current->getIp()
                << "|" << g_app.getClientIP(current, req)
                << "|" << "-"
                << "|" << (req.isPOST() ? "POST" : "GET")
                << "|" << req.getRequestUrl()
                << "|" << req.getContentLength()
                << "|" << rsp.getStatus()
                << "|" << rsp.getContentLength()
                << "|" << req.getHeader("Connection")
                << "|" << (m_bKeepAlive ? "keep_alive" : "not_keep_alive" )
                << "|"
                << endl;

        current->close();
        current->sendResponse("", 0);
    }

    //访问日志
    if (m_bAccessLog)
    {
        FDLOG("access")
                << "|" << current->getIp()
                << "|" << g_app.getClientIP(current, req)
                << "|" << "-"
                << "|" << (req.isPOST() ? "POST" : "GET")
                << "|" << req.getRequestUrl()
                << "|" << req.getContentLength()
                << "|" << rsp.getStatus()
                << "|" << rsp.getContentLength()
                << "|"
                << endl;
    }
}


/////////////////////////////////////////////////////////////////
int
main(int argc, char *argv[])
{
    try
    {
        g_app.main(argc, argv);
        g_app.waitForShutdown();
    }
    catch (std::exception &e)
    {
        cerr << "std::exception:" << e.what() << std::endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << std::endl;
    }

    return -1;
}
/////////////////////////////////////////////////////////////////


