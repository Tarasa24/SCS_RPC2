<center>
<img align="left" src="https://cdn.discordapp.com/app-assets/529016610137309184/529052463643230211.png">
<h1>SCS Discord Rich Presence 2</h1>
Discord Rich Presence plugin for ETS2 and ATS
</center>
</br>

---

## Table of contents

- [General info](#general-info)
- [Technologies](#technologies)
- [Setup](#setup)
- [Execution](#execution)
- [Contact](#contact)
- [Acknowledgement](#acknowledgement)

## General info

Second verions of [SCS_RPC](https://github.com/Tarasa24/SCS_RPC), this time written in C++ using offical [Telemetry SDK](https://modding.scssoft.com/wiki/Documentation/Engine/SDK/Telemetry) for interacting with game data, as well as latest [Discord Game SDK](https://discord.com/developers/docs/game-sdk/sdk-starter-guide) for connecting to RPC pipe. Compiled dlls for both 64 and 32 bit version can be found in the [Releases page](https://github.com/Tarasa24/SCS_RPC2/releases).

##### What's new?

- Thanks to direct integration with Telemetry SDK and some code optimizations, users will be able to enjoy better performance and frame-by-frame data updates.
- That also means SCS_RPC is now standalone plugin and **won't require manually starting external program anymore**. Just copy the files and play!

![SCS_RPC2 ATS_1](https://i.imgur.com/lBUEDxF.png)
![SCS_RPC2 ATS_2](https://i.imgur.com/Z7jGm1G.png)
![SCS_RPC2 ETS2_1](https://i.imgur.com/MMPDg0o.png)
![SCS_RPC2 ETS2_2](https://i.imgur.com/B0LC7SM.png)

## Technologies

- <a href="https://modding.scssoft.com/wiki/Documentation/Engine/SDK/Telemetry" target="_blank">Telemetry SDK</a>
- <a href="https://discord.com/developers/docs/game-sdk/sdk-starter-guide" target="_blank">Discord Game SDK</a>
- <a href="https://rosettacode.org/wiki/K-d_tree#C.2B.2B" target="_blank">K-d tree code snippet</a>

## Setup

Download [latest release](https://github.com/Tarasa24/SCS_RPC2/releases), copy the files over to `bin/win_x64` or `bin/win_x86` in respective game directories. Note that _discord_game_sdk.dll_ has to be in the same directory as the game executable, whereas _SCS_RPC2.dll_ belongs to the plugins folder.

## Execution

Plugin is automatically run upon the game start.

## Contact

In case of any question hit me up on Discord **Tarasa24**#1761

---

###### Acknowledgement

I am not related to the company **SCS software s.r.o.** in any way. This project is non-profit and open source. This project represents fanbase both of SCS software and Discord. All assets were used in good faith.
Any problems relating the use of the company's name and/or the logo, will be resolved asap. I don't want deal with **any** legal disputes regarding this project. Thank you for understanding.
