# BoAT总体设计



## 引言

### 编写目的
本文对BoAT的系统总体设计进行描述，包括体系架构设计、子系统功能、内外部接口、关键流程及关键技术设计描述，以期指导各子系统的总体设计及测试策略/规范的编写。
本文的预期读者为：BoAT SDK详细设计人员。


### 缩写术语
|术语   |解释                         |
|:----- |:--------------------------- |
|ABI    |Application Binary Interface |
|BoAT   |Blockchain of AI Things      |
|RLP    |Recursive Length Prefix      |
|RNG    |Random Numeral Generator     |
|RPC    |Remote Procedure Call        |
|TEE    |Trusted Execution Environment|


## BoAT设计目标
作为物联网区块链的一个中间件，应以最小的改动代价，方便、快速的移植到各物联网模组中，BoAT的设计遵循以下原则:
+ 层次化设计  
+ 多个区块链协议支持  
+ 可扩展设计  
+ 密钥安全设计  
+ 针对不同区块链提供相应的C语言接口合约自动生成工具  


## BoAT SDK在整个区块链网络中的位置
BoAT SDK作为连接物联网设备和区块链的中间件，其在整个交互网络中的位置如图3-1所示。  
![BoAT position](./images/BoAT_Overall_Design_cn-F3-1-Boat_Position.png)  
图 3-1 BoAT在区块链交互网络中的位置  



## BoAT的实现框架
BoAT遵循层次化设计，由接口层，协议层，RPC层，供应商依赖层，通用工具和实用程序实现组成。各层具体功能如下：
+ 接口层： 提供物联网应用调用相应区块链的接口。
+ 协议层： 主要实现各区块链协议部分。
+ RPC层： 向协议层提供服务。
+ 供应商依赖层： 为接口层的钱包提供密码学算法、签名、存储等服务等。
+ 通用工具： 用于智能合约的C语言接口生成。
+ 实用程序： 向各层提供如数据格式转换和报文编解码等服务。  
  
BoAT的整体框架如图4-1所示。  
![BoAT整体框图](./images/BoAT_Overall_Design_cn-F4-1-BoAT_Overall_Framework.png)
图 4-1 BoAT整体框图  


### 接口层
#### 概述

接口层位于BoAT的最上层，向物联网应用提供访问各区块链的入口，接口层包含两部分接口：
+ 钱包接口
   * 钱包接口是BoAT SDK的入口，不同的区块链有着公共的钱包入口。
+ 交易接口
   * 每个区块链都提供一套交易接口，其功能类似，具体实现有所不同。

接口层的钱包接口由供应商依赖层提供支持，有关供应商依赖层的描述请参见[供应商依赖层](#供应商依赖层)。  
接口层的交易接口由协议层提供支持，有关协议层的描述请参见[协议层](#协议层)。  
#### 钱包接口
##### 钱包的数据结构和功能实现清单
钱包是一个容器，容器里除了存储着访问区块链所必须的信息，还包含了一个密钥管理系统，它们分别对应着钱包的数据结构和实现方法。
BoAT SDK运行在蜂窝模组的应用处理器上，其运行环境的资源是受限的，如有的蜂窝模组可能并不提供非易失性存储器的访问接口；另一方面，从用户的角度讲，可能有的用户仅仅希望创建一个临时的钱包用于测试，而不想长久的存储它。鉴于此，在BoAT SDK的设计里将钱包分为两类：持久化钱包和一次性钱包。持久化钱包存储在运行环境的非易失性存储器里，掉电不会丢失；一次性钱包存储在运行环境的RAM中，掉电即丢失。
在钱包的数据结构中，应至少包含以下元素：
  + 账户私钥
  + 区块链网络信息
  + RPC接口信息。

除必须包含的元素外，钱包的数据结构可选择包含：
  + 账户地址

钱包应当实现以下功能：  
  + SDK初始化
  + SDK反初始化
  + 钱包创建
  + 钱包删除
  + 钱包卸载
  + 根据索引查询钱包信息
  + 私钥设置
  + 私钥生成
  + 私钥校验

##### 钱包接口功能实现简述
###### SDK初始化/反初始化
+ SDK初始化：  
在使用BoAT SDK之前应做SDK初始化，该接口执行的内容包括：
   1. 钱包列表初始化  
钱包列表是一个包含了固定数量钱包和钱包相关使用信息的数据结构，钱包相关使用信息包括钱包使用标识、钱包名称、钱包所属区块链等。钱包列表是一个全局资源，钱包列表的初始化即将数据结构中的各成员执行一次赋初值，如将使用标识初始化为未使用等。  
   2. 其他全局资源初始化  
SDK使用的一些三方库如果在调用前需做一次初始化，则应放在此处执行初始化，如RPC层使用的三方库cURL。
+ SDK反初始化：  
在使用BoAT SDK结束后应做SDK反初始化释放资源，该接口执行的内容包括：  
   1. 执行钱包卸载  
   2. 其他全局资源反初始化  
SDK使用的一些三方库如果在调用前需做一次反初始化，则应放在此处执行反初始化，如RPC层使用的三方库cURL。

###### 钱包操作
+ 钱包创建：  
该接口用于实现一个新钱包的创建或一个已创建钱包的读取，该接口执行的内容包括：  
  1. 检查钱包列表是否已满  
  2. 根据具体入参决定是创建新钱包还是读取已创建的钱包  
  3. 根据具体入参决定是创建一次性钱包还是创建持久化钱包  
  4. 根据具体入参执行某一个区块链的钱包初始化，如执行Ethereum的钱包初始化，或者执行PlatONE的钱包初始化
+ 钱包删除：  
该接口从非易失性存储器中删除一个持久化钱包。注意：该接口并不从RAM中卸载钱包。持久钱包被删除后，它就变成了一次性钱包。该接口执行的内容包括：  
  1. 从非易失性存储器上删除指定的钱包文件
+ 钱包卸载：    
该接口用于从RAM中卸载一个钱包。注意：该接口仅仅从RAM中卸载钱包，并不从非易失性存储器中删除钱包。该接口执行的内容包括：  
  1. 根据具体入参执行某一个区块链的钱包反初始化，如执行Ethereum的钱包反初始化，或者执行PlatONE的钱包反初始化
+ 根据索引查询钱包信息：  
该接口用于查询钱包。该接口执行的内容包括：
  1. 根据具体入参(入参可以为存储序号索引值、钱包地址等)返回相应的钱包信息地址  

###### 私钥操作
+ 私钥设置：  
该接口用于设置钱包的私钥，私钥应注意小心保管。该接口执行的内容包括：  
  1. 执行私钥校验  
  2. 执行私钥赋值  
  3. 通过私钥生成公钥(可选)  
  4. 通过公钥生成地址(可选)  
+ 私钥生成：  
该接口用于生成一个有效的私钥。该接口执行的内容包括：  
  1. 通过产生随机数的方式产生私钥  
  2. 执行私钥校验  
  3. 如校验失败返回第1步重新产生私钥，校验成功返回产生的私钥  
+ 私钥校验：  
该接口用于检查给定的私钥是否有效。该接口执行的内容包括：  
  1. 检查私钥是否处于有效的私钥范围  

#### 交易接口
##### 交易的数据结构和功能实现清单
交易是经过签名后的一条消息，通过区块链的网络传播，被记录在区块链上。不同的区块链提供的交易接口的功能基本一致。  

对于Ethereum/PlatON/PlatONE/FISCO BCOS：

交易应至少包含如下元素：
+ 钱包的数据结构
+ 交易标识
+ 交易所需的报文  

交易应提供以下功能实现：  
+ 钱包初始化
+ 钱包反初始化
+ 账户余额查询
+ 交易初始化
+ 发送交易
+ 查询交易回执
+ 无状态消息调用  
  
 

##### Ethereum交易接口功能实现简述
+ 钱包初始化：  
  该接口执行的内容包括：
  1. 设置区块链合约地址
  2. 设置交易是否指定链ID
  3. 设置链ID
+ 钱包反初始化：  
  该接口执行的内容包括：
  1. 归零私钥信息
  2. 区块链网络信息资源释放
  3. 调用协议层提供的web3接口：web3接口反初始化
+ 账户余额查询：  
  该接口执行的内容包括：
  1. 准备账户余额查询所需的报文信息
  2. 调用协议层提供的web3接口：获取账户余额
+ 交易初始化：  
  该接口主要实现初始化交易结构中的字段。Ethereum的交易结构中除签名的v、r、s三个字段外，还有nonce、gasPrice、gasLimit、recipient、value、data等六个字段。请注意，交易的Nonce字段的设置并不在该接口中实现，而是放在交易发送时刻设置，因为用户可能同时创建多个交易报文，而这些报文被发送至区块链网络的顺序是不确定的。另外需要说明的是，Nonce和gasPrice字段宜通过调用协议层相应的web3接口从区块链获取，此种方式需要访问网络，会产生一定的网络流量。
  该接口执行的内容包括：
  1. 设置交易的GasPrice字段
  2. 设置交易的GasLimit字段
  3. 设置交易的Recipient字段
  4. 设置交易的Value字段
+ 发送交易  
  该接口执行的内容包括：  
  1. 判断发送同步/异步标识(如果有)
  2. 标识为同步发送：调用协议层提供的raw transaction接口：raw transaction同步发送
  3. 标识为异步发送：调用协议层提供的raw transaction接口：raw transaction异步发送
+ 查询交易回执：  
  该接口执行的内容包括：
  1. 准备查询交易回执所需的报文信息
  2. 每隔指定时间调用一次协议层提供的web3接口“获取交易回执”直至触发超时时间
  3. 返回查询结果
+ 发送Ether  
  该接口执行的内容包括：
  1. 设置交易的Nonce字段
  2. 设置交易的Value字段
  3. 设置交易的Data字段
  4. 执行发送交易
+ 无状态消息调用：  
  该接口执行的内容包括：  
  1. 准备无状态消息调用所需的报文信息  
  2. 调用协议层提供的web3接口“区块链无状态调用”  

##### PlatON交易接口功能实现简述
PlatON和Ethereum相比，差异主要集中在如下三点:  
  1. 双方地址不一样。  
    PlatON在以太坊的基础上，额外增加了一种类似Bitcoin的Bech32格式的地址。所以，在初始化交易时，除了以太坊需要的参数外，还需要额外设置地址的hrp(Human-readable parts)。钱包会在调用RPC时会自动使用指定的地址格式。  
  2. 通过RPC调用的函数名称不一样。  
    对BoAT SDK用户来说没有区别。  
  3. PlatON必须使用指定区块链ID方式。  
    EIP-155描述了两种可以用来交易的数据格式，但是PlatON只支持一种，具体参考[raw transaction接口简述](#raw-transaction接口简述)。   

在设计PlatON的数据结构及代码实现时，应考虑数据结构的继承及代码实现的复用，这样既减少代码量，也便于维护。尽管PlatON的交易结构和Ethereum一样，但是，在一些RPC命令中的调用时，需要用到Bech32格式的地址。因此，在数据结构的设计中，增加了两个存贮Bech32格式地址的字段，可能的设计思路如图4-2所示：  
![PlatON数据结构的一种可能的设计思路](./images/BoAT_Overall_Design_cn-F4-2-PlatON_Data_Structure.png)  
图 4-2 PlatON数据结构的一种可能的设计思路  
图4-2描述了PlatON的一种可能的数据结构设计思路，请注意，PlatON的两个额外地址字段应放置在数据结构的最末位置，不能破坏复用的Ethereum的数据结构的完整性。如果破坏了Ethereum的数据结构的完整性，将导致Ethereum中与该数据结构相关的实现方法不可复用。  


##### PlatONE交易接口功能实现简述
和Ethereum相比，其有差异的地方列出如下：  
+ 交易初始化  
除了Ethereum描述的初始化步骤外，PlatONE还要：
  1. 设置交易类型字段  

由前所述可以看出，PlatONE和Ethereum的差异非常的小，在设计PlatONE的数据结构及代码实现时，应考虑数据结构的继承及代码实现的复用，这样既减少代码量，也便于维护。比如在交易结构上的组成上，PlatONE的交易结构比Ethereum的交易结构多了一个交易类型字段，因此，在数据结构的设计中，一种可能的设计思路如图4-3所示：  
![PlatONE数据结构的一种可能的设计思路](./images/BoAT_Overall_Design_cn-F4-3-PlatONE_Data_Structure.png)
图 4-3 PlatONE数据结构的一种可能的设计思路  
图4-3描述了PlatONE的一种可能的数据结构设计思路，请注意，PlatONE的交易类型字段应放置在数据结构的最末位置，不能破坏复用的Ethereum的数据结构的完整性。如果破坏了Ethereum的数据结构的完整性，将导致Ethereum中与该数据结构相关的实现方法不可复用。

##### FISCO BCOS交易接口功能实现简述
和Ethereum相比，有差异的地方可以参考FISCO BCOS官方文档 https://fisco-bcos-documentation.readthedocs.io/zh_CN/latest/docs/design/protocol_description.html 。  

在设计FISCO BCOS的数据结构及代码实现时，应考虑数据结构的继承及代码实现的复用，这样既减少代码量，也便于维护。在交易结构体中新增了以下四个字段：  
  1. blockLimit交易生命周期  
  2. chainId链信息  
  3. groupId群组ID  
  4. extraData预留字段  
因此，在数据结构的设计中，一种可能的设计思路如图4-4所示：  
![FISCO BCOS数据结构的一种可能的设计思路](./images/BoAT_Overall_Design_cn-F4-4-FISCOBCOS_Data_Structure.png)  
图 4-4 FISCO BCOS数据结构的一种可能的设计思路  
图4-4描述了FISCO BCOS的一种可能的数据结构设计思路，请注意，FISCO BCOS的交易类型字段应放置在数据结构的最末位置，不能破坏复用的Ethereum的数据结构的完整性。如果破坏了Ethereum的数据结构的完整性，将导致Ethereum中与该数据结构相关的实现方法不可复用。

***注：由于FISCO BCOS在交易中增加了新的字段，所以进行RLP编码时和Ethereum并不一样，具体参考本章提供链接的RC2部分。***  

##### Fabric交易接口功能实现简述
-	钱包初始化
  该接口执行的内容包括：
  1. 为钱包结构体申请空间。
  2. 设置账户的私钥路径/索引。
  3. 设置账户的证书。
  4. 如果需要支持TLS，则设置TLS验证服务端身份时要使用的根证书。
  5. 如果TLS需要支持双向认证，还应设置客户端的私钥路径/索引及相应证书。
  6. 设置节点信息，如背书节点和排序节点的数量，地址，主机名。如果启用了TLS，主机名字段将被用来做服务端身份的验证，它和服务端TLS证书中的CN字段保持一致；如果不启用TLS，主机名将被忽略。
  7. 初始化HTTP2环境。
-	钱包反初始化
  该接口执行的内容包括：
  1. 释放为账户私钥路径/索引及证书申请的空间。
  2. 释放为TLS客户端私钥路径/索引及证书申请的空间(如果启用了TLS)。
  3. 释放为根证书申请的空间(如果启用了TLS)。
  4. 释放为背书/排序节点信息申请的空间。
  5. 反初始化HTTP2环境。
  6. 释放为钱包结构体申请的空间。
-	交易初始化
  该接口执行的内容包括：
  1. 设置交易要访问的链码的属性：路径、名称、版本。
  2. 设置发起交易的客户端的通道名称。
  3. 设置发起交易的客户端的组织名称。
-	交易反初始化
  该接口执行的内容包括：
  1. 释放为链码属性申请的空间。
  2. 释放为通道名称申请的空间。
  3. 释放为组织名称申请的空间。
-	交易参数设置
  该接口执行的内容包括：
  1. 设置交易发起时刻的时间戳。
  2. 设置交易的参数。
-	发送交易
  该接口执行的内容包括：
  1. 发送查询相关的报文。
  2. 发送调用相关的报文。


### 协议层
#### 概述
协议层位于BoAT SDK的第二层，主要实现各个区块链的协议部分。对于Ethereum系的区块链，其协议比较相似，如Ethereum、PlatON、PlatONE和FISCO BCOS。  
协议层由RPC层提供支持。RPC层描述请参考 [RPC层](#RPC层).  

#### Ethereum的协议层实现
##### raw transaction接口
协议层的raw transaction接口向接口层的交易接口“发送交易”提供服务。raw transaction向上应至少提供如下接口：
+ raw transaction异步发送    

此外，raw transaction还可选择提供如下接口：
+ raw transaction同步发送

##### raw transaction接口简述
+ raw transaction异步发送  
该接口实现了raw transaction的数据编码，如各字段RLP编码、哈希计算、签名等，并在协议层内部调用web3接口发送到区块链，不等待交易执行成功直接返回。数据编码分为指定链ID和不指定链ID两种。具体细节参照EIP-155 <https://eips.ethereum.org/EIPS/eip-155>。  
该接口执行的内容包括：
   - 如果数据编码采用不指定链ID方式：  
  1. 对交易的nonce、gasPrice、gasLimit、recipient、value、data等六个字段执行RLP编码
  2. 计算前一步骤中RLP编码的keccak-256哈希值
  3. 对前一步骤的哈希值签名，得到奇偶标识parity、r、s三个值
  4. 对交易的nonce、gasPrice、gasLimit、recipient、value、data、v、r、s等九个字段执行RLP编码，其中v = parity + 27，parity、r、s已在前一步骤中给出
  5. 调用web3接口“发送rawtransaction”发送到区块链
   - 如果数据编码采用指定链ID方式：
  1. 对交易的nonce、gasPrice、gasLimit、recipient、value、data、v、r、s等九个字段执行RLP编码，其中v = chainID，r = 0， s = 0
  2. 同“不指定链ID方式”的步骤2
  3. 同“不指定链ID方式”的步骤3
  4. 对交易的nonce、gasPrice、gasLimit、recipient、value、data、v、r、s等九个字段执行RLP编码，其中v = Chain ID * 2 + parity + 35，parity、r、s已在前一步骤中给出
  5. 同“不指定链ID方式”的步骤5
+ raw transaction同步发送  
该接口执行“rawtransaction异步发送”并等待交易成功或超时后返回。该接口执行的内容包括:
  1. 执行raw transaction异步发送
  2. 执行查询交易回执
  3. 等待交易成功或超时后返回

##### web3接口
除raw transaction接口外，协议层还应向上层提供以下web3接口：
+ web3接口初始化
+ web3接口反初始化
+ 获取区块链指定位置存储内容
+ 获取账户交易次数
+ 获取区块链gasPrice
+ 获取账户余额
+ 获取交易回执
+ 区块链无状态调用
+ 发送raw transaction  

翻阅Ethereum的RPC相关文档可以知道，Ethereum大约提供了64种RPC方法，在BoAT SDK里，我们仅实现以上几种，其原因同[钱包接口](#钱包接口)中的描述，SDK运行环境的资源是受限的，上述几种RPC方法是数据上链常用到的几种方法，如果未来客户需要其他RPC方法的实现，BoAT SDK将以客制化的方式提供。

##### web3接口简述
+ web3接口初始化  
  该接口执行的内容包括：
  1. web3接口资源申请，如RPC内容的内存空间申请，用于向区块链请求或从区块链响应的json串的内存空间的申请，响应的json串的解析结果的内存空间申请
  2. 初始化web3消息ID
  3. 执行RPC层的“RPC接口初始化”
+ web3接口反初始化  
  该接口执行的内容包括：
  1. web3接口资源的释放，如RPC内容的内存空间释放，用于向区块链请求或从区块链响应的json串的内存空间的释放，响应的json串的解析结果的内存空间释放
  2. 执行RPC层的“RPC接口反初始化”
+ 获取区块链指定位置存储内容  
  该接口执行的内容包括：
  1. web3消息ID自增
  2. 组织用于向区块链发起请求的“获取区块链指定位置存储内容”的json串报文
  3. 调用RPC方法“web3_getStorageAt”将请求报文发送给区块链
  4. 对接收到的区块链响应报文解析并返回解析结果
+ 获取账户交易次数  
  该接口执行的内容包括：
  1. web3消息ID自增
  2. 组织用于向区块链发起请求的“获取账户交易次数”的json串报文
  3. 调用RPC方法“web3_getTransactionCount”将请求报文发送给区块链
  4. 对接收到的区块链响应报文解析并返回解析结果
+ 获取区块链的gasPrice  
  该接口执行的内容包括：
  1. web3消息ID自增
  2. 组织用于向区块链发起请求的“获取区块链的gasPrice”的json串报文
  3. 调用RPC方法“web3_gasPrice”将请求报文发送给区块链
  4. 对接收到的区块链响应报文解析并返回解析结果
+ 获取账户余额  
  该接口执行的内容包括：
  1. web3消息ID自增
  2. 组织用于向区块链发起请求的“获取账户余额”的json串报文
  3. 调用RPC方法“web3_getBalance”将请求报文发送给区块链
  4. 对接收到的区块链响应报文解析并返回解析结果
+ 获取交易回执  
  该接口执行的内容包括：
  1. web3消息ID自增
  2. 组织用于向区块链发起请求的“获取交易回执”的json串报文
  3. 调用RPC方法“web3_getTransactionReceiptStatus”将请求报文发送给区块链
  4. 对接收到的区块链响应报文解析并返回解析结果
+ 区块链无状态调用  
  该接口执行的内容包括：
  1. web3消息ID自增
  2. 组织用于向区块链发起请求的“区块链无状态调用”的json串报文
  3. 调用RPC方法“web3_call”将请求报文发送给区块链
  4. 对接收到的区块链响应报文解析并返回解析结果
+ 发送raw transaction  
  该接口执行的内容包括：
  1. web3消息ID自增 
  2. 组织用于向区块链发起请求的“发送raw transaction”的json串报文
  3. 调用RPC方法“web3_sendRawTransaction”将请求报文发送给区块链
  4. 对接收到的区块链响应报文解析并返回解析结果

#### PlatON的协议层实现
PlatON的协议层实现与Ethereum的协议层完全一样，只是PlatON仅支持数据编码采用指定链ID方式。  

#### PlatONE的协议层实现
PlatONE的协议层实现与Ethereum的协议层几乎一样，其唯一的区别是：raw transaction的data字段的填充和raw transaction的RLP编码过程中，多了交易类型的编码。因为data字段的填充和对raw transaction的RLP编码，是由使用BoAT SDK的用户在调用BoAT SDK相关的API之前实现的，因此PlatONE的协议层可以复用Ethereum的协议层。  

#### FISCO BCOS的协议层实现
FISCO BCOS的协议层实现与Ethereum的协议层几乎一样，其唯一的区别是：raw transaction的RLP编码过程中，多了四个字段的编码。因为对raw transaction的RLP编码，是由使用BoAT SDK的用户在调用BoAT SDK相关的API之前实现的，因此FISCO BCOS的协议层可以复用Ethereum的协议层。

#### Fabric的协议层实现
##### Fabric协议概述
Fabric协议层主要包含提案协议和交易协议，查询的协议与提案协议相同。提案协议与交易协议分别如图4-5，图4-6所示  
![Fabric提案报文结构](./images/BoAT_Overall_Design_cn-F4-5-Fabric-Proposal.png)  
图 4-5 Fabric提案报文结构  
![Fabric交易报文结构](./images/BoAT_Overall_Design_cn-F4-6-Fabric-Transaction.png)  
图 4-6 Fabric交易报文结构  

Fabric客户端发起一笔的交易的时候，会首先向背书节点发送提案，背书节点对提案签名后返回签名数据，然后Fabric客户端连同背书节点的签名数据和交易参数按交易报文的格式组织好后发送给排序节点，排序节点校验通过后更新链的状态。详细的交易流程如图4-7所示，该图是从《hyperledger-fabricdocs master》文档中摘取的。关于Fabric更多的介绍，可以参考Fabric文档 <https://hyperledger-fabric.readthedocs.io/en/release-1.4/>  
![ Fabric交易流程](./images/BoAT_Overall_Design_cn-F4-7-Fabric-Transaction-Flow.png)  
图 4-7 Fabric交易流程
#####	Fabric协议接口实现
在Fabric报文中，协议中的各字段通过protobuf实现序列化，然后通过HTTP2协议发送出去。由前序章节可知，提案报文和交易报文有一些重复和相似的地方，可以把这些重复的部分拆分为一个子模块，以便于重用。一种可能的拆分方式列出如下：
-	channelHeader打包
-	signatureHeader打包
-	提案载荷打包
-	交易载荷打包


### RPC层
#### 概述
RPC层实现了对发送数据到区块链节点的具体链路的封装，向协议层提供服务。RPC层是发送链路具体实现的一层抽象，以使BoAT SDK更方便的移植到不同的运行环境中。

#### RPC层的可扩展设计
RPC层向下根据具体运行环境的不同，扩展出不同的发送链路实现。如有的环境提供了传输工具cURL，可以使用cURL来发送和接收报文，有的环境不支持cURL，但提供了AT指令来发送和接收报文，RPC层对这些不同的实现进行封装，向上层提供统一的接口，可以使协议层不必关心具体的发送链路，需要发送接收报文时调用统一的接口即可。RPC层对不同链路的封装也便于对更多的链路实现进行扩展。

##### RPC层对cURL的扩展
cURL是一个利用URL语法在命令行下工作的文件传输工具，支持FTP、FTPS、HTTP、HTTPS等多种通信协议。RPC层启用cURL的支持时，除了需要在SDK 初始化时候执行cURL的全局资源初始化外，还要在RPC层实现：
   1. cURL会话初始化
   2. 设置URL格式为\<protocol\>://<target name | IP>:\<port\>
   3. 配置cURL支持的协议
   4. 配置SSL证书(如果需要)
   5. 设置HTTP类型为POST
   6. 设置超时时间(响应超时、连接超时)
   7. 设置HTTP头信息
   8. 设置HTTP报文响应的回调函数和接收缓存
   9. 设置POST内容
   10. 执行RPC请求
   11. cURL资源清理和回收  



### 供应商依赖层
#### 概述
BoAT SDK会运行在不同的硬件平台上，由于各个供应商提供的多种硬件平台功能多种多样，比如有的硬件平台提供了随机数发生器的硬件实现，而有的硬件平台不仅提供随机数发生器的硬件实现，还提供TEE环境。为了更好的利用平台的资源，同时为了将软件上的变化隔离开来，BoAT SDK设计了供应商依赖层。供应商依赖层为接口层的钱包提供诸如随机数发生器、安全存储、加密签名等服务。在硬件平台能提供相应的实现时，供应商依赖层会通过供应商预留的接口调用相应的硬件功能。在硬件平台不提供相应服务时，通过纯软件实现相应功能。

#### 供应商依赖层的纯软件实现
供应商依赖层应提供一种所需要的各种服务的纯软件实现。从而使得BoAT SDK在硬件无法提供所需的相应服务时，仍然可以完整运行。供应商依赖层应以覆盖必要的硬件服务为标准，至少提供以下功能的纯软件实现：
+ 随机数生成器
+ 签名(如Ethereum使用的ECDSA)
+ 安全存储(如以加密的方式保存私钥)
+ 哈希算法(如Ethereum使用的Keccak-256)


#### TEE支持
BoAT的设计应考虑TEE环境的支持。对于有TEE环境的硬件，BoAT应可以通过少量的修改将敏感信息放到TEE环境中。为满足此目标，钱包的设计满足以下标准：
+ 钱包相关的数据结构模块化设计
+ 钱包相关的实现模块化设计
+ 钱包相关的敏感信息不在钱包以外的地方体现

### 通用工具实现
#### 概述
通用工具独立于各层单独存在，用于生成访问区块链智能合约的C语言接口，通用工具宜用脚本语言实现。通用工具包括：
+ 用于生成Ethereum智能合约C语言接口的工具
+ 用于生成PlatONE智能合约C语言接口的工具
+ 用于生成FISCO BCOS智能合约C语言接口的工具

#### 通用工具简述
##### 生成Ethereum智能合约C语言接口的工具
常用的Ethereum智能合约的编程语言是solidity，solidity编译后会生成一个描述合约接口的JSON文件，合约接口的JSON格式是由一个函数和/或事件描述的数组所给定的。一个函数的描述是一个有如下字段的JSON对象:
+ type  ："function" , "constructor" , "receive" (接收Ether的函数) or "fallback"(缺省函数)
+ name ：函数名称
+ inputs ：对象数组，每个数组对象会包含：
   - name ： 参数名称；
   - type  ： 参数的权威类型 
   - components ：用于tuple类型
+ outputs ：一个类似于 inputs 的对象数组，函数无返回值时可以被省略
+ payable ：如果函数接受Ether，为 true ；缺省为 false
+ stateMutability ：为下列值之一： pure（指定为不读取区块链状态），view（指定为不修改区块链状态）， nonpayable 和 payable（同上文payable）
+ constant ： 如果函数被指定为 pure 或 view 则为 true  

对于生成的C语言合约接口，其和JSON对象的对应关系如下：
<table>
    <tr>
        <td colspan="2">JSON对象</td>
        <td>JSON对象取值</td>
        <td>C语言合约接口</td>
    </tr>
    <tr>
        <td colspan="2">Type</td>
        <td>/</td>
        <td>/</td>
    </tr>
    <tr>
        <td colspan="2">Name</td>
        <td>/</td>
        <td>函数名称</td>
    </tr>
    <tr>
        <td rowspan="3">Inputs</td>
        <td>Name</td>
        <td>/</td>
        <td>函数参数名称</td>
    </tr>
     <tr>
        <td>Type</td>
        <td>/</td>
        <td>函数参数类型</td>
    </tr>
     <tr>
        <td>components</td>
        <td>/</td>
        <td>/</td>
    </tr>
      <tr>
        <td colspan="2">Outputs</td>
        <td>/</td>
        <td>函数返回值，因为合约返回的是一个http的报文，因此函数应按char*处理</td>
    </tr>
     <tr>
        <td colspan="2">Payable</td>
        <td>/</td>
        <td>/</td>
    </tr>
     <tr>
        <td colspan="2">stateMutability</td>
        <td>/</td>
        <td>/</td>
    </tr>
    <tr>
        <td colspan="2" rowspan="2">Constant</td>
        <td>True</td>
        <td>函数通过RPC方法“web3_call”访问区块链</td>
    </tr>
    <tr>
        <td>False</td>
        <td>函数通过RPC方法“web3_sendRawTransaction”访问区块链 </td>
    </tr>
</table>

##### 生成PlatONE智能合约C语言接口的工具
常用的PlatONE智能合约的开发语言是C++，同Ethereum一样，PlatONE的智能合约经过编译后也会生成一个描述合约接口的JSON文件。其JSON字段和Ethereum的JSON字段相同，C语言接口和JSON字段的对应关系也与Ethereum一致。

##### 生成FISCO BCOS智能合约C语言接口的工具
常用的FISCO BCOS智能合约的开发语言是solidity，具体可以参考[生成Ethereum智能合约C语言接口的工具](#生成Ethereum智能合约C语言接口的工具)。

### 实用程序
#### 概述
在BoAT SDK各个层的实现里，或多或少都会涉及到数据格式的转换、报文的编解码等。这些功能宜抽象为独立的模块，为各个层提供服务。这些功能块包括:
+ 数据格式转换工具
+ RLP编码
+ JSON编解码

#### 数据格式转换工具
在BoAT SDK的设计中，很多地方都用到了数据格式的转换，如将输入的ASCII码转为二进制格式，将区块链返回的二进制格式数据转为便于显示的ASCII码格式，合约ABI接口中涉及到的大小端转换等。为便于使用，宜将可能会用到的格式转换的函数实现放到同一个文件里。可能会用的数据转换有：
+ 将二进制流转换为十六进制字符串
+ 将十六进制字符串转换为二进制流
+ 将uint32类型的数据转换为大端
+ 将uint64类型的数据转换为大端
+ 对一串字节流做字节序转换  

除此之外，为使SDK适应更多的环境，还可以对以下函数做一层接口封装放到该文件中：  
+ 堆内存申请
+ 堆内存释放
+ 睡眠等待  

#### RLP编码
##### RLP的结构
RLP编码用于两个地方，一个是协议层组织交易报文会用到RLP编码，另一个是生成的C语言合约接口代码里可能会用到RLP编码。  
RLP编码的定义里只处理两类数据：一类是字符串，一类是列表。字符串指的是一串二进制数据，如字节数组；列表是一个嵌套递归的结构，里面可以包含字符串和列表，其结构形式如图4-8所示：  
![RLP列表的结构](./images/BoAT_Overall_Design_cn-F4-8-Structure_Of_RLP.png)  
图 4-8 RLP列表的结构  

##### RLP编码规则
RLP的编码规则描述如下：   
+ 如果一个字符串的长度是0-55字节，它的RLP编码是一个单字节前缀 + 字符串本身，单字节前缀的值是‘0x80’加上字符串的长度
+ 如果字符串的长度大于55个字节，它的RLP编码是一个单字节前缀 + “字符串长度”编码 + 字符串本身。单字节前缀的值是‘0xB7’加上“字符串长度”的字节长度
+ 如果一个列表的长度是0-55字节，它的RLP编码是一个单字节的前缀 + 列表中各元素项的RLP编码，单字节前缀的值是‘0xC0’加上列表的总长度
+ 如果一个列表总长度大于55字节，它的RLP编码是一个单字节前缀 + “列表长度”编码 + 列表中各元素项的RLP编码，单字节前缀的值是‘0xF7’加上“列表长度”的字节长度  

关于RLP编码规则更详细的描述，请参阅参考文档Rlp wiki：https://eth.wiki/en/fundamentals/rlp


##### RLP编码实现
RLP编码实现可以有多种不同的方式。由前述章节可知，RLP编码的一种可能的数据结构组成描述如图4-9所示：
![RLP编码的一种可能的数据结构](./images/BoAT_Overall_Design_cn-F4-9-Data_Structure_Of_RLP.png)  
图 4-9 RLP编码的一种可能的数据结构  

图中定义了四种类型，来表达RLP列表的嵌套递归结构，假如有一个名为List的列表对象，其包含了三个字符串对象分别为stringA，stringB，stringC，则对列表对象List执行RLP编码的一种可能的流程描述如下：
1. 初始化列表对象List
2. 初始化字符串对象StringA
3. 将StringA添加到列表对象List
4. 初始化字符串对象StringB
5. 将StringA添加到列表对象List
6. 初始化字符串对象StringC
7. 将StringA添加到列表对象List
8. 对List执行RLP编码  

综上所述，RLP的实现方法应至少包括：
+ RLP列表初始化
+ RLP字符串初始化
+ 将字符串或列表加入一个列表
+ 对字符串执行RLP编码
+ 对列表执行RLP编码
+ 替换列表中的某个字符串
+ 删除列表中的某个字符串
+ 计算RLP编码长度

#### JSON编解码
在BoAT SDK访问区块链的报文中，会涉及到JSON的编码，在区块链给BoAT SDK的响应报文中，会涉及到JSON的解码。在报文里对JSON的编解码可以用一个三方库实现：cJSON。  
cJSON是C语言编写的一个轻量级的JSON编解码器，遵循ANSI-C标准，可以适应更多的平台和编译器。cJSON移植进SDK也很方便，只需简单的将cJSON.h和cJSON.c复制到SDK，并在需要使用cJSON的地方包含头文件“cJSON.h”就可以使用它了。关于cJSON的更多描述可以参阅参考文档 cJSON<https://github.com/DaveGamble/cJSON#welcome-to-cjson>.

## 使用BoAT创建一笔区块链交易的流程
### 使用BoAT创建一笔Ethereum交易的流程
一个典型的使用BoAT创建一笔Ethereum交易的流程如图5-1所示：  
![使用BoAT创建一笔交易的流程](./images/BoAT_Overall_Design_cn-F5-1-Create_Ttransaction.png)  
图 5-1 使用BoAT创建一笔交易的流程  

其中：
+ BoAT SDK初始化：  
请参考[SDK初始化/反初始化](#SDK初始化反初始化)中关于“BoAT SDK初始化”的描述
+ 创建钱包  
请参考[钱包操作](#钱包操作)中关于“创建钱包”的描述 
+ 交易初始化  
请参考[Ethereum交易接口功能实现简述](#Ethereum交易接口功能实现简述)中关于“交易初始化”的描述
+ 设置Nonce字段
+ 设置Data字段
+ 发送交易  
请参考[Ethereum交易接口功能实现简述](#Ethereum交易接口功能实现简述)中关于“发送交易”的描述 
+ BoAT SDK反初始化  
请参考[SDK初始化/反初始化](#SDK初始化反初始化)中关于“BoAT SDK反初始化”的描述

### 使用BoAT创建一笔PlatON交易的流程
创建一笔PlatON交易的流程和Ethereum一致。相关描述请参考[使用BoAT创建一笔Ethereum交易的流程](#使用BoAT创建一笔Ethereum交易的流程)。 

### 使用BoAT创建一笔PlatONE交易的流程
创建一笔PlatONE交易的流程和Ethereum类似。PlatONE在发送交易前除了要设置Nonce字段和Data字段，还需要设置交易类型字段，其余部分流程与Ethereum一致，相关描述请参考[使用BoAT创建一笔Ethereum交易的流程](#使用BoAT创建一笔Ethereum交易的流程)。 

### 使用BoAT创建一笔FISCO BCOS交易的流程
创建一笔FISCO BCOS交易的流程和Ethereum类似。FISCO BCOS在发送交易前还需要设置交易生命周期字段、链信息/业务信息字段、群组字段和预留字段，其余部分流程与Ethereum一致，相关描述请参考[使用BoAT创建一笔Ethereum交易的流程](#使用BoAT创建一笔Ethereum交易的流程)。 


## 参考文档
[1]. cJSON <https://github.com/DaveGamble/cJSON#welcome-to-cjson>  
[2]. cURL <https://curl.haxx.se/libcurl/>  
[3]. RLP wiki <https://eth.wiki/en/fundamentals/rlp>  

