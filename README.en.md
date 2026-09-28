[简体中文](README.md) | [繁體中文](README.zh-TW.md) | [English](README.en.md)

# Multiplayer Texas Hold'em Poker Source Code

An engineering reference for a **multiplayer real-time poker platform**, covering client assets, table protocols and C++ server components. Unlike the same account's club-operations or tournament-focused repositories, this project emphasizes the chain from a mobile poker table to room state, action messages, player state and cross-module protocols.

## What the product is

Players can enter a lobby, create or join clubs, alliances, friends rooms and private tables, then complete buy-in, betting, folding, settlement and exit flows in a real-time table. The product screens show MTT registration/table views, profile, club currency, alliance and friends-room flows. Exact delivery scope must be verified against the selected branch, configuration and acceptance checklist.

## Product features and gameplay

- **Real-time multiplayer tables**: room join/leave, online state, game start/end, betting and settlement flows.
- **Friends rooms and private tables**: create a private table and invite friends or club members.
- **Clubs and alliances**: club creation, join application, alliance entry and club-currency modules.
- **Tournament entry**: screenshots plus `match*.proto.bytes` and match pages show MTT/SNG-related flows; complete tournament rules require server configuration review.
- **Mobile client**: Unity/client scripts, assets and Android/iOS configuration files are included in the source tree.
- **Operations modules**: profile, rankings, sign-in, mall, order, push and management interfaces or protocols.

## Real product screens

| Live table | Friends private room |
| --- | --- |
| ![Multiplayer Texas Holdem poker table](docs/assets/screenshots/poker-table-01.jpg) | ![Texas Holdem friends private room](docs/assets/screenshots/private-table-04.jpg) |
| MTT tournament | Create club |
| ![Texas Holdem MTT tournament screen](docs/assets/screenshots/mtt-tournament-03.jpg) | ![Create a Texas Holdem poker club](docs/assets/screenshots/create-club-02.jpg) |
| Player profile | Club currency |
| ![Texas Holdem player profile](docs/assets/screenshots/player-profile-05.jpg) | ![Texas Holdem club currency](docs/assets/screenshots/club-coins-07.jpg) |

[Open the illustrated product page](https://masterai-top.github.io/Texas-Hold-em-Poker/en/)

## Technical structure verifiable in the tree

| Layer | Files/directories | Scope |
| --- | --- | --- |
| Table logic | `allin.cpp`, `autobet.cpp`, `autofold.cpp`, `gamebanker.h`, `gameend.h` | Betting, automated actions, banker/end-game logic |
| Room lifecycle | `userlefttable.*`, `useroffline.*`, `userinfo.*`, `userinfomapprivate.*` | Exit, offline state, player data and private-table mapping |
| Protocols | `*.proto.bytes`, `GameTcp.tars`, `Push.tars`, `OrderServant.tars` | Hall, room, chat, friends, match, order and push protocols |
| Services | `RouterServer.*`, `OrderServer.*`, `Processor.*`, `OuterFactoryImp.*` | Routing, orders, message handling and external-service adapters |
| Client | `GameApp.ts`, `GameLoading.ts`, `MsgHandlerModel.ts`, `Login/`, `Assets/` | Startup, loading, message handling, login and assets |
| Data and operations | `RankBoard.proto.bytes`, `mall.proto.bytes`, `SignIn.proto.bytes`, `Task.proto.bytes` | Ranking, mall, sign-in and task definitions |

## Positioning against sibling repositories

- Primary terms for this repository: **multiplayer poker source code, real-time poker game, poker protocol, C++ poker server, Unity poker client**.
- Link club-operations searches to the Club-Source repository and tournament-specific searches to the Tournament-Event repository. Do not duplicate a generic “complete solution” claim here.
- Avoid competitor brand names in titles and meta descriptions; this reduces ambiguity and unnecessary keyword conflict.

## Contact

- Telegram: [@xuzongbin001](https://t.me/xuzongbin001)
- Email: [masterai918@gmail.com](mailto:masterai918@gmail.com)

Follow applicable laws, platform policies, privacy and minor-protection requirements. Do not use this project for illegal gambling.

