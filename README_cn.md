
# BoAT-X Framework
[![Issue](https://img.shields.io/github/issues/aitos-io/BoAT-X-Framework)](https://github.com/aitos-io/BoAT-X-Framework/issues)![Forks](https://img.shields.io/github/forks/aitos-io/BoAT-X-Framework)![Stars](https://img.shields.io/github/stars/aitos-io/BoAT-X-Framework)[![GitHub Release](https://img.shields.io/github/license/aitos-io/BoAT-X-Framework)](https://github.com/aitos-io/BoAT-X-Framework/blob/master/LICENSE)[![Join the chat at https://gitter.im/BoAT-X/community](https://badges.gitter.im/BoAT-X/community.svg)](https://gitter.im/BoAT-X/community?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

![BoAT logo](https://aitos-io.github.io/BoAT-X-Framework/logo/BoAT_RGB_Horizontal_100.png)


## BoAT-X介绍
**BoAT-X** 是摩联科技推出的一套面向物联网的区块链应用框架，**它帮助物联网设备实现可信数据上链，赋能**数据从数据源头上传到云端以及后续物联网数据开放和交换下的全生命周期可信，是**BoAT家族各关键**组件的统称。**BoAT**全称为**B**lockchain **o**f **A**I **T**hing，**X泛指**本产品族涵盖的各相关组件，包括嵌入式区块链客户端，云端服务，和配套工具。物联网设备可以通过灵活调用该套组件访问各类区块链服务。BoAT-X所描绘的美好愿景是打造一艘船BoAT，从物联网设备这一数据的源头出发，承载着可信的物联网数据驶向千行百业的数据湖。

- **BoAT-Engine** 是嵌入式区块链客户端，也可以称之为物联网设备钱包。它是一个用C语言编写的面向物联网设备的轻量级区块链客户端SDK。**BoAT Engine**可以单独运行，也可以与**BoAT-Anchor**协同工作。

- **BoAT-Anchor** （开发中）是一种基于边缘设备的服务或区块链中继服务，它与其他BoAT组件一起协同工作，在物联网设备和区块链网络之间完成数据交易的中继。

- **BoAT-Mast** （开发中）是物联网设备的密钥管理服务，也可用于物联网设备的区块链DID服务。BoAT-Mast提供了完整的物联网设备公钥管理工具。同时BoAT-Mast还提供数据验证服务，验证链上的数据是否是来自于合法注册在网的真实物联网设备。


### 为什么BoAT-X赋能打造物联网可信数字底座

物联网（IoT 或Internet of Things）已经成为我们日常生活生产的数字基础设施。海量的物联网设备通过传感器采集各类数据，将线下采集的数据传输到云端服务器并执行相关指令，例如共享单车上的智能锁。目前全球范围内主要的行业都已经达成共识，唯有真实可信的数据才是有价值的数据。

准确地说，是从数据中提取的信息是有价值的。如果我们回顾一个数据生产者的典型商用案例（收集数据的人并不总是负责分析数据和提取信息），那么当数据被共享给使用方时，数据生产者必须证明数据是从实际的物联网设备捕获的，而不是在计算机中伪造生成的。 因此，BoAT-X的主要目的是使得物联网设备具备访问区块链服务的能力。BoAT Engine赋能物联网设备在尽可能靠近物联网数据源头的网络边缘实现数据上链，通过基于区块链的可信证明来延展数据价值的生命周期。

区块链是一个分布式的不可篡改的账本，有助于在彼此缺乏信任的各方间实现可信地数据共享。此外，一旦数据存储在区块链上，该数据在整个生命周期内都可以防篡改。然而目前仍存在一个障碍需要跨域，那就是如何确保区块链上存储的数据与物联网设备捕获的数据是一致的。

物联网设备捕获数据并将其传输到物联网云平台内的数据库，有三个可能的锚点来生成基于区块链的可信数据证明：云端、边缘侧或设备内部。

- 如果物联网云平台的数据通过区块链区块来生成**数据可信证明**，则可信锚点是物联网平台。物联网平台本身需要有权威性或者良好的声誉，以具备足够的可信程度。
- 如果是由物联网边缘侧汇聚物联网设备数据并生成**数据可信证明**，同时将其中继到云平台及区块链节点，则可信锚点相应延伸至该边缘侧设备或者网关。
- 如果是直接在物联网设备上生成**数据可信证明**，则该可信锚点将深入到物联网设备内部。

与攻击或篡改云端数据库相比，逐一篡改每一台现场物联网设备要困难得多，成本也高得多。因此，锚点离数据源头越近，证明的可信度就越高。

BoAT-X产品族提供了一整套基础组件，利用区块链技术改造物联网端侧设备，在物联网设备端最靠近数据源头的位置建立信任锚点。BoAT-X可以从数据采集源头保证数据可信，来提高物联网数据价值，打造物联网可信数字底座。


### BoAT-X的独特优势

大多数区块链或BaaS服务（区块链即服务）提供了不同形态的节点或客户端/钱包软件。然而，该节点或者软件是为个人电脑、云服务器或智能手机设计的，通常使用Go、Java、JavaScript、Python等高级语言编写。有些软件需要一个虚拟机或解释器来执行，有些甚至需要在运行时动态下载代码。与此同时，物联网设备由于使用场景差异及各种因素限制，不具备较大的硬件资源，通常运行在RTOS或轻量级Linux上，由于资源有限，大多数物联网设备只能支持本地C语言应用，因此难以访问区块链服务。

在BoAT-X产品族中，BoAT Engine组件是一个轻量级的C语言多链客户端，支持主流物联网无线模块和芯片，BoAT-Anchor 是用于网关中继， BoAT-Mast用于密钥管理及验证服务。BoAT-X将区块链的服务能力从仅面向计算机和手机扩展到可以面向碎片化的物联网设备。

### BoAT-X工作原理

BoAT-X有两种运行模式

#### 直接模式

![Direct Approach](https://aitos-io.github.io/BoAT-X-Framework/en-us/images/BoAT_README_Direct_Approach.png)

直接模式适用于能够直接访问区块链节点的物联网设备。物联网设备向物联网平台发送数据，并通过调用BoAT Engine API将数据哈希值和签名发送到区块链上。数据消费者随后将链上哈希值与存储在物联网平台上的数据产生的哈希值进行比对，以确定其数据可信度。消费者还可以通过BoAT Mast检查数据是否来自真实注册在网的物联网终端。

#### 间接模式

![Indirect Approach](https://aitos-io.github.io/BoAT-X-Framework/en-us/images/BoAT_README_Indirect_Approach.png)

间接模式适用于由某些因素（如：IP白名单限制，网络架构限制，硬件加密算法能力不足等）导致无法直接访问区块链节点的物联网设备的情况。

物联网设备通过调用BoAT Engine API，使用设备的密钥对数据进行签名，并将签名发送到边缘网关。运行**BoAT-Anchor**的将签名（即数据指纹）发送到区块链上。数据消费者随后将链上哈希值与存储在物联网平台上的数据产生的哈希值进行比对，以确定其数据可信度。消费者可以通过BoAT Mast检查数据是否来自真实注册在网的物联网终端。



### 代码目录

#### BoAT-Engine
```
<SDKRoot>
|
+---build           | Directory to store object and executable files
+---demo            | Demo application
+---docs            | API reference manual
+---vendor          | Special vendor dependency
|   \---common      |     Universal soft algorithms implementation
|   \---platform    |     Dependency of different platforms
+---include         | Header files for application to include
+---lib             | Lib files for application to link with
+---sdk             | SDK source
|   +---third-party |     Third party libraries
|   +---include     |     Header files for SDK internal use
|   +---protocol    |     Blockchain client protocol implementation
|   +---rlp         |     RLP encoder
|   +---utilities   |     Utility APIs
|   \---wallet      |     SDK entry API implementation
+---tests           | Test cases
\---tools           | Tools for generating C interface from contract ABI

NOTE: ./build and ./lib are created in building

```


#### BoAT-Anchor
敬请期待。

#### BoAT-Mast
敬请期待。


## 发布及状态

### 发布信息
新功能的完整列表，请访问 [发布信息](https://github.com/aitos-io/BoAT-X-Framework/releases)。

### 项目状态报告
项目状态更新，请访问[项目状态更新报告](https://github.com/aitos-io/project-status-update)。

### 已经支持BoAT的区块链物联网模组列表
已经支持BoAT的区块链物联网模块列表，请参阅[支持列表](./SUPPORTED_LIST.md)。


## 文档

### 区块链物联网模组白皮书
访问[BoAT区块链物联网模组产品白皮书](https://aitos-io.github.io/BoAT-X-Framework/en-us/BoAT_Blockchain_IoT_Module_Product_White_Paper_en.pdf)。

### 区块链物联网模组技术及应用
访问[BoAT区块链物联网模组技术及应用](https://aitos-io.github.io/BoAT-X-Framework/en-us/BoAT_Blockchain_IoT_Module_Technology_and_Application_en.pdf)。

### 文档总汇
文档总汇，请访问[BoAT文档](https://aitos-io.github.io/BoAT-X-Framework)。

### 常见问题
敬请期待。


## 社区
BoAT-X 社区相关信息：
+ 联系邮箱： info@aitos.io
+ 漏洞提交： [BoAT-X Issues](https://github.com/aitos-io/BoAT-X-Framework/issues)
+ 相关新闻： https://aitos-io.medium.com/
+ LinkedIn： https://www.linkedin.com/company/aitos-io


## 社区贡献
热烈欢迎社区核心团队中的贡献者，包括（但不限于）风格/缺陷修复、功能的实现、方案/算法的建议以及文档更新。

更多信息请参考 [Contribution Guideline](./CONTRIBUTING.md)。

开发文档请参考 [BoAT documentation](https://aitos-io.github.io/BoAT-X-Framework)。

提交Pull Requests请前往[Pull Requests](https://github.com/aitos-io/BoAT-X-Framework/pulls)。


## 许可

Apache License 2.0, 请访问[LICENSE](./LICENSE).


## 水手们，欢迎前来开启BoAT奇幻漂流之旅

**Open source by [aitos.io](http://www.aitos.io/)**

[![aitos.io](https://aitos-io.github.io/BoAT-X-Framework/logo/aitos_logo_100.png)](http://www.aitos.io/)
