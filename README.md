
# BoAT-X Framework
[![Issue](https://img.shields.io/github/issues/aitos-io/BoAT-X-Framework)](https://github.com/aitos-io/BoAT-X-Framework/issues)![Forks](https://img.shields.io/github/forks/aitos-io/BoAT-X-Framework)![Stars](https://img.shields.io/github/stars/aitos-io/BoAT-X-Framework)[![GitHub Release](https://img.shields.io/github/license/aitos-io/BoAT-X-Framework)](https://github.com/aitos-io/BoAT-X-Framework/blob/master/LICENSE)[![Join the chat at https://gitter.im/BoAT-X/community](https://badges.gitter.im/BoAT-X/community.svg)](https://gitter.im/BoAT-X/community?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

![BoAT logo](https://aitos-io.github.io/BoAT-X-Framework/logo/BoAT_RGB_Horizontal_100.png)


## Introduction
Welcome to the official implementation and documents of BoAT-X Framework!([中文版](./README_cn.md)) 

**BoAT-X** is a blockchain application framework for IoT, a big portion dedicated to a family of BoAT components where **BoAT** stands for *Blockchain of AI Things* and **X** is a placeholder for a set of middleware components, services and tools that enable IoT devices to access blockchain services. BoAT-X depicts the vision of a boat transferring trustworthy information from the IoT data source to a data lake.

- **BoAT-Engine** is an embedded blockchain client or an IoT device wallet. It's an IoT-device-oriented lightweight blockchain client SDK written in C language. BoAT-Engine may work alone or together with the *BoAT-Anchor*.

- **BoAT-Anchor** (in development) is an edge service or a blockchain gateway that works with other BoAT components to relays transactions between an IoT device and a blockchain network.

- **BoAT-Mast** (in development) is a key management system or a blockchain DID service for the IoT. BoAT-Mast provides tools for managing the public keys of the devices. BoAT-Mast also verifies if the on-chain data originates from a registered IoT device.


### Why BoAT-X Contributes to a Trustworthy Foundation for IoT Data

IoT or the Internet of Things has now been transformed into infrastructure for typical everyday life. Numerous IoT devices capture data via sensors, sending data to the servers and executing commands, such as a smart lock on a shared bike. The worldwide industries have matured in realization that data surely is valuable, provided that the data is truethful.

To be precise, it is the information extracted from the data that is valuable. If we review a typical use case of a commercial data producer (the one who collects the data yet isn’t solely responsible for analyzing the data and extracting the information), it becomes crystal clear that when data is shared with the consumer, a question gets raised about how that data producer can prove the data was captured from actual IoT devices, rather than something randomly generated in a computer. Hence, the main purpose of BoAT-X is to allow IoT devices to access the blockchain services and thus extend the IoT data lifecycle with a blockchain-based credible proof at the very edge closest to where the data is being captured.

Blockchain is a decentralized immutable ledger, which is conducive for multiple entities who don't naturally trust each other, to share data in a trusted way. Moreover, once the data is saved on the blockchain, that data becomes tamper-resistant thoughout its lifecycle. However, there is still a missing gap between how data saved on a blockchain can ensure conformity to the data captured by the IoT device.

IoT devices capture data and transport it to a database within a cloud IoT platform. Therefore, there are three possible anchor (attestation) points to generate a blockchain-based proof for the data: the cloud, the edge or the device.

If the cloud IoT platform generates the data credibility proof on the blockchain, the trusted anchor point is at the IoT platform. The IoT platform itself must be trusted (authority, reputation, etc.).

If the anchor point is the edge converging data from part of the IoT devices and relaying it to the cloud while generating the proof, the edge must be trusted.

If the IoT device generates the proof, the device is the trust anchor point.

It's much more difficult and expensive to tamper with each in-field device than infringing or tampering with the data in the cloud database. Thus, the closer the anchor point is to the data source, the higher trustworthiness the proof will become.

The BoAT-X family offers the essential components to establish a trust anchor point at the device side, closest to the data source. That's why BoAT-X’s productiveness with a blockchain contributes to the trustworthy foundation of IoT data.


### What makes BoAT-X Different

Most blockchain or BaaS (Blockchain as a Service) variants deliver a node and/or client/wallet software. However, this software was designed for personal computers, cloud servers or smartphones and is usually written in high-level languages like Go, Java, JavaScript, Python, etc. Some require a cumbersome virtual machine or interpreter to execute, and some even have to download code dynamically at runtime. Meanwhile, IoT devices are not as powerful and typically run on an RTOS or lightweight Linux. Due to constrained resources, most IoT devices could only support native C language applications and thus can hardly access blockchain services.

*BoAT-X* Family implements the *BoAT-Engine*, a lightweight C-language multi-chain client supporting a number of wireless modules and chipsets, as well as the *BoAT-Anchor* for gateway relay and the *BoAT-Mast* for key management. It extends the blockchain capability from computers and cellphones onto IoT devices.



### How BoAT-X Works

BoAT-X has two main operating methods.

#### The Direct Approach

![Direct Approach](https://aitos-io.github.io/BoAT-X-Framework/en-us/images/BoAT_README_Direct_Approach.png)

The direct approach is suitable for IoT devices capable of direct access to a blockchain node. The IoT device sends data to the IoT platform, and by calling *BoAT-Engine* API, sends the hash of the data to the blockchain. The data consumer later compares the on-chain hash to the hash of the data normally stored on the IoT platform to determine its data credibility. The consumer also requests *BoAT-Mast* to check if the data comes from a registered device.

#### The Indirect Approach

![Indirect Approach](https://aitos-io.github.io/BoAT-X-Framework/en-us/images/BoAT_README_Indirect_Approach.png)

The indirect approach accomodates IoT devices that can not otherwise directly access the blockchain node, due to various possible reasons such as an IP whitelist restriction and unmatched cryptographic algorithm capability.

By calling *BoAT-Engine* APIs, the IoT device signs the data with the device's cryptographic key. It then send the signature to an intermediate edge gateway. The gateway which runs the *BoAT-Anchor* relays the signature (i.e. digital fingerprint) to the blockchain. It also requests *BoAT-Mast* to check if the data come from a registered device.

The data consumer could later verify the data stored on the IoT platform against the on-chain signature.




### Code Directories

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
Coming soon.

#### BoAT-Mast
Coming soon.


## Release and Status

### Releases
For a complete list of new features, please read [Release Notes](https://github.com/aitos-io/BoAT-X-Framework/releases).

### Project Status Reports
For project status updates, please visit [BoAT Project Status Update Reports](https://github.com/aitos-io/project-status-update).

### Supported Blockchains and IoT Modules
See [Supported List](./SUPPORTED_LIST.md) for supported blockchains and IoT modules.


## Documentation

### Blockchain IoT Module Whitepaper
See [BoAT Blockchain IoT Module Product White Paper](https://aitos-io.github.io/BoAT-X-Framework/en-us/BoAT_Blockchain_IoT_Module_Product_White_Paper_en.pdf).

### Blockchain IoT Module Technology and Application
See [BoAT Blockchain IoT Module Technology and Application](https://aitos-io.github.io/BoAT-X-Framework/en-us/BoAT_Blockchain_IoT_Module_Technology_and_Application_en.pdf).

### Full Documents
For full documents, please visit [BoAT documentation](https://aitos-io.github.io/BoAT-X-Framework)


### FAQ
Coming soon.


## Community
The BoAT-X Framework community can be found at:
+ Contact Mail: info@aitos.io
+ Report Bugs: [BoAT-X Issues](https://github.com/aitos-io/BoAT-X-Framework/issues)
+ Medium: https://aitos-io.medium.com/
+ LinkedIn: https://www.linkedin.com/company/aitos-io


## Contribution
We are glad to have contributors out of the core team; contributions, including (but not limited to) style/bug fixes,
implementation of features, proposals of schemes/algorithms, and thorough documentation, are welcomed. Please refer to our [Contribution Guideline](./CONTRIBUTING.md) for more information.

Find development documentation at [BoAT documentation](https://aitos-io.github.io/BoAT-X-Framework).

Submit Pull Requests at [Pull Requests](https://github.com/aitos-io/BoAT-X-Framework/pulls)


## License

Apache License 2.0, see [LICENSE](./LICENSE).


## Join us as a BoAT-Mariner and build a pioneering spirit on your IoT+Blockchain path

**Open source by [aitos.io](http://www.aitos.io/)**

[![aitos.io](https://aitos-io.github.io/BoAT-X-Framework/logo/aitos_logo_100.png)](http://www.aitos.io/)
