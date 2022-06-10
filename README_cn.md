
# BoAT-X Framework
[![Issue](https://img.shields.io/github/issues/aitos-io/BoAT-X-Framework)](https://github.com/aitos-io/BoAT-X-Framework/issues)![Forks](https://img.shields.io/github/forks/aitos-io/BoAT-X-Framework)![Stars](https://img.shields.io/github/stars/aitos-io/BoAT-X-Framework)[![GitHub Release](https://img.shields.io/github/license/aitos-io/BoAT-X-Framework)](https://github.com/aitos-io/BoAT-X-Framework/blob/master/LICENSE)[![Join the chat at https://gitter.im/BoAT-X/community](https://badges.gitter.im/BoAT-X/community.svg)](https://gitter.im/BoAT-X/community?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

![BoAT logo](https://aitos-io.github.io/BoAT-X-Framework/logo/BoAT_RGB_Horizontal_100.png)


## BoAT-X介绍
**BoAT-X** 是摩联科技推出的一套面向物联网的区块链应用框架，**它帮助物联网设备实现可信数据上链，赋能**数据从数据源头上传到云端以及后续物联网数据开放和交换下的全生命周期可信，是**BoAT家族各关键**组件的统称。**BoAT**全称为**B**lockchain **o**f **A**I **T**hing，**X泛指**本产品族涵盖的各相关组件，包括嵌入式区块链客户端，云端服务，和配套工具。物联网设备可以通过灵活调用该套组件访问各类区块链服务。BoAT-X所描绘的美好愿景是打造一艘船BoAT，从物联网设备这一数据的源头出发，承载着可信的物联网数据驶向千行百业的数据湖。

- **BoAT-Engine** 是嵌入式区块链客户端，也可以称之为物联网设备钱包。它是一个用C语言编写的面向物联网设备的轻量级区块链客户端SDK。**BoAT Engine**可以单独运行，也可以与**BoAT-Anchor**协同工作。

- **BoAT-Anchor** （开发中）是一种基于边缘设备的服务或区块链中继服务，它与其他BoAT组件一起协同工作，在物联网设备和区块链网络之间完成数据交易的中继。

- **BoAT-Mast** （开发中）是物联网设备的密钥管理服务，也可用于物联网设备的区块链DID服务。BoAT-Mast提供了完整的物联网设备公钥管理工具。同时BoAT-Mast还提供数据验证服务，验证链上的数据是否是来自于合法注册在网的真实物联网设备。
