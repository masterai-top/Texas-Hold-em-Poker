[简体中文](README.md) | [繁體中文](README.zh-TW.md) | [English](README.en.md)

# 德州扑克多人实时游戏平台源码

这是一套面向**多人实时德州扑克平台**的客户端、牌局协议与 C++ 服务端工程参考。与同账号中聚焦“俱乐部运营”或“赛事平台”的项目不同，本仓库重点展示从移动端牌桌到房间状态、下注消息、用户状态和跨模块协议的工程链路。

## 产品是什么

玩家可以进入大厅，创建或加入俱乐部、联盟、好友局和私人房，并在实时牌桌中完成买入、下注、弃牌、结算和离桌。产品截图还展示 MTT 赛事报名/牌桌、个人中心、俱乐部币、联盟和好友局等界面。玩法与交付范围应以实际分支、配置和验收清单为准。

## 主要功能与玩法

- **多人实时牌桌**：房间加入/离开、玩家在线状态、牌局开始与结束、下注与结算流程。
- **好友局与私人房**：创建私人牌桌，邀请熟人或俱乐部成员参与。
- **俱乐部与联盟**：创建俱乐部、申请加入、联盟入口及俱乐部币等产品模块。
- **赛事入口**：截图与 `match*.proto.bytes`、赛事页面展示 MTT、SNG 等赛事相关流程；完整赛事规则需结合服务端配置验收。
- **移动客户端**：仓库包含 Unity/客户端脚本、资源与 Android/iOS 配置相关文件。
- **运营模块**：个人中心、排行、签到、商城、订单、推送和后台接口等协议或页面。

## 真实产品截图

| 实时牌桌 | 好友私人局 |
| --- | --- |
| ![德州扑克多人实时牌桌](docs/assets/screenshots/poker-table-01.jpg) | ![德州扑克好友私人局](docs/assets/screenshots/private-table-04.jpg) |
| MTT 赛事 | 创建俱乐部 |
| ![德州扑克 MTT 赛事界面](docs/assets/screenshots/mtt-tournament-03.jpg) | ![德州扑克创建俱乐部](docs/assets/screenshots/create-club-02.jpg) |
| 个人中心 | 俱乐部币 |
| ![德州扑克个人中心](docs/assets/screenshots/player-profile-05.jpg) | ![德州扑克俱乐部币](docs/assets/screenshots/club-coins-07.jpg) |

[打开图文产品页](https://masterai-top.github.io/Texas-Hold-em-Poker/zh-cn/)

## 可从目录核实的技术结构

| 层级 | 文件/目录 | 说明 |
| --- | --- | --- |
| 牌局逻辑 | `allin.cpp`、`autobet.cpp`、`autofold.cpp`、`gamebanker.h`、`gameend.h` | 下注、自动操作、庄家/结束等牌局逻辑文件 |
| 房间生命周期 | `userlefttable.*`、`useroffline.*`、`userinfo.*`、`userinfomapprivate.*` | 离桌、掉线、玩家资料和私桌映射 |
| 协议层 | `*.proto.bytes`、`GameTcp.tars`、`Push.tars`、`OrderServant.tars` | 大厅、房间、聊天、好友、赛事、订单和推送协议 |
| 服务端 | `RouterServer.*`、`OrderServer.*`、`Processor.*`、`OuterFactoryImp.*` | 路由、订单、消息处理和外部服务适配 |
| 客户端 | `GameApp.ts`、`GameLoading.ts`、`MsgHandlerModel.ts`、`Login/`、`Assets/` | 客户端启动、加载、消息处理、登录和资源 |
| 数据与运营 | `RankBoard.proto.bytes`、`mall.proto.bytes`、`SignIn.proto.bytes`、`Task.proto.bytes` | 排行、商城、签到、任务等接口定义 |

## 与同账号项目的定位边界

- 本仓库主关键词：**multiplayer poker source code、real-time poker game、poker protocol、C++ poker server、Unity poker client**。
- 俱乐部运营细节应优先链接 Club-Source 项目；赛事平台专题应优先链接 Tournament-Event 项目；完整商业方案不在本页重复承诺。
- 不使用 hhpoker、wpk 等第三方品牌做比较性标题，避免品牌争议和关键词内耗。

## 开发与部署提醒

公开目录包含工程代码、协议文件、部分客户端资源和文档，但上线前仍需核实依赖库、数据库结构、配置、证书、平台 SDK、监控、压测和地区合规。请勿把截图等同于全部可交付功能。

## 联系方式

- Telegram：[@xuzongbin001](https://t.me/xuzongbin001)
- Email：[masterai918@gmail.com](mailto:masterai918@gmail.com)

请遵守所在地法律、平台规则、隐私与未成年人保护要求，不得用于非法赌博。


