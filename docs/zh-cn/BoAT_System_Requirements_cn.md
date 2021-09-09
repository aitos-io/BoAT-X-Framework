# BoAT系统需求

## 引言

### 概述
本文描述BoAT Framework SDK区块链客户端（C语言版本）对蜂窝模组的系统需求。BoAT是一个运行在模组的应用处理器上的SDK。对于OpenCPU蜂窝模组，BoAT作为库被应用程序链接和调用。对于非OpenCPU的蜂窝模组，BoAT的API需要扩展为AT命令，供上位机上的应用调用。

### 缩写术语
|术语  |解释                                                    |
|:---- |:----------------------------------------------------- |
|BoAT  |Blockchain of AI Things                                |
|SDK   |Software Development Kit                               |
|API   |Application Programming Interface                      |
|MCU   |Microcontroller Unit                                   |
|RTOS  |Real Time Operating System                             |
|TRNG  |True Random Number Generator                           |
|CSPRNG|Cryptographically Secure Pseudo-Random Number Generator|
|RTC   |Real Time Clock                                        |
|NTP   |Network Time Protocol                                  |
|HTTP  |Hyper Text Transfer Protocol                           |
|HTTPs |Hyper Text Transfer Protocol Secure                    |
|CoAP  |Constrained Application Protocol                       |
|MQTT  |Message Queuing Telemetry Transport                    |
|TCP   |Transmission Control Protocol                          |
|TEE   |Trusted Execution Environment                          |
|TA    |Trusted Application                                    |
|ECDSA |Elliptic Curve Digital Signature Algorithm             |
|SHA2  |Secure Hash Algorithm 2                                |


## 存储需求

仅支持以太坊/PlatONE/FISCO BCOS时，BoAT Framework SDK（C语言版本）自身对存储的需求约为：
- Flash（代码和只读数据）：约210kB
- Flash（持久性读写数据）：数百Bytes
- RAM（全局变量、堆、栈）：约10kB

仅支持HyperLedger Fabric时，BoAT Framework SDK（C语言版本）自身对存储的需求约为：
- Flash（代码和只读数据）：约520kB
- Flash（持久性读写数据）：数千Bytes
- RAM（全局变量、堆、栈）：约30kB

以上不含BoAT Framework SDK（C语言版本）所依赖的系统库，具体数值随不同区块链协议而有所不同。

## 处理能力需求

支持以太坊时，BoAT Framework SDK（C语言版本）在100MHz左右频率的ARM Cortex M4上，完成一次区块链交易或智能合约调用相关的密码学运算，约需1秒（不含网络通信时间）。具体时间随不同区块链协议有所不同。
确切的处理能力需求，取决于调用BoAT Framework SDK的应用，对功耗和时延的要求。BoAT自身没有特别要求。

## 操作系统和通信需求

BoAT Framework SDK（C语言版本）对操作系统没有特别要求，Linux和各种RTOS均可移植，但至少应支持以下能力：
1. 支持动态内存分配/释放。
2. 支持互斥量保护机制。
3. 支持按指定时长挂起线程（可选）。若不支持，则BoAT不支持超时或轮询功能，其他功能不受影响。
4. 按优先级从高到低，至少支持如下随机数生成器之一：<br>
   (1)	TRNG，真随机数发生器（需硬件支持）<br>
   (2)	CSPRNG，密码学安全伪随机数发生器。对于Linux，可由OpenSSL库提供该能力 <br>
   (3)	PRNG，（非密码学安全的）伪随机数发生器
5. 按优先级从高到低，至少支持获得如下时间之一（用于伪随机数种子以及数据和日志中的时刻）：<br>
   (1)	RTC时刻，且可通过NTP等协议与真实时间保持一致 <br>
   (2)	RTC时刻，需人工设置时间 <br>
   (3)	自上电以来的tick

6. 按优先级从高到低，至少支持如下通信协议之一：<br>
   (1)	HTTP/HTTPS（对于Linux，可由curl库提供该能力）<br>
   (2)	CoAP <br>
   (3)	MQTT <br>
   (4)	TCP <br>
若受运营商限制，设备只能连接特定运营商、服务商的IoT平台（例如OneNET、OceanConnect），则通信协议遵循运营商要求。
7. 支持memcpy()、strcpy()、snprintf()等常用标准C库函数（printf等涉及I/O的可选）。
8. 对于采用Linux操作系统的蜂窝模组，在调试期间应支持adb或类似的登录机制，且具有root权限。

## TEE和远程认证（可选）

### TEE

若蜂窝模组的应用处理器支持TEE（Trusted Execution Environment），则可以利用TEE来保护密钥、签名、加密等敏感数据和过程。TEE至少应支持以下能力：
1. 开放客户二次开发TA（Trusted Application）的能力（如开放必要的密钥）
2. 配套支持Secure Boot、fuse等
3. 支持安全存储

### 远程认证

远程认证（Remote Attestation）是利用芯片中预埋的Root of Trust，为设备的数据提供签名服务，并可能采集设备运行环境特征信息的机制。远程认证可以帮助服务商远程认证设备的真伪。若模组的芯片支持远程认证，至少应支持以下能力：
1. 支持对给定的数据进行认证签名，且可在远端服务器上验证签名
2. 若支持TEE，应支持在TEE内对数据进行认证签名（可选）


## 密码学运算硬件加速（可选）

若硬件支持密码学运算加速，可以提高密码运算性能。

支持以太坊/PlatONE/FISCO BCOS时，BoAT至少需要如下密码学运算：
1. 椭圆曲线签名ECDSA（secp256k1曲线）
2. Keccak-256（Keccak-256为SHA3-256的变种）

支持HyperLedger Fabric时，BoAT至少需要如下密码学运算：
1. 椭圆曲线签名ECDSA（secp256r1曲线）
2. SHA2-256


