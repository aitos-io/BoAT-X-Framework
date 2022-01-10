### 编译和运行Demo
#### 准备
SDK提供基于以太坊、PlatON、PlatONE、FISCO-BCOS、Hyperledger Fabric和HW-BCS的Demo。在运行这些Demo之前，需要首先安装相应的区块链节点软件（或者有已知节点），并部署Demo所需的智能合约。  

Demo所使用的智能合约及其ABI JSON文件放置在：  

|Demo智能合约                                                 |合约ABI JSON文件                                              |用途           |
|:----------------------------------------------------------- |:------------------------------------------------------------ |:------------- | 
|\<SDKRoot\>/demo/demo_ethereum/demo_contract/StoreRead.sol   |\<SDKRoot\>/demo/demo_ethereum/demo_contract/StoreRead.json   |以太坊演示     |
|\<SDKRoot\>/demo/demo_platone/demo_contract/StoreRead.sol    |\<SDKRoot\>/demo/demo_platone/demo_contract/StoreRead.json    |PlatONE演示    |
|\<SDKRoot\>/demo/demo_fiscobcos/demo_contract/HelloWorld.sol |\<SDKRoot\>/demo/demo_fiscobcos/demo_contract/HelloWorld.json |FISCO-BCOS演示 |

在运行以太坊的Demo之前，需要安装以太坊节点模拟器ganache，以及以太坊智能合约编译部署工具truffle。  
ganache及truffle工具可以访问该网站：https://truffleframework.com  
ganache有命令行界面的ganache-cli版本，以及图形界面的Ganache版本。命令行界面的ganache-cli和图形界面的Ganache 1.x版本不会存盘，如果ganache-cli或Ganache   1.x的进程被终止，部署的合约会丢失，下次启动需要使用命令truffle migrate --reset重新部署合约，重新部署的合约地址可能会变化。图形界面的Ganache   2.x版本可以建Workspace保存状态，关闭下次重新打开该Workspace后，部署过的合约仍然在无需重新部署。  
除了使用ganache模拟器，还可以使用Ropsten等以太坊测试网络（需要申请免费的测试token）。  

在运行PlatON的Demo之前，需要安装PlatON节点。  
PlatON源码及工具可以访问该网站：https://platon.network/

在运行PlatONE的Demo之前，需要安装PlatONE节点，以及智能合约编译和部署工具。  
PlatONE源码及工具可以访问该网站：https://platone.wxblockchain.com

在运行FISCO-BCOS的Demo之前，需要安装FISCO-BCOS节点和合约部署。  
FISCO-BCOS源码及安装部署步骤可以访问该网站：https://fisco-bcos-documentation.readthedocs.io

在完成节点（或模拟器）部署后，需要分别遵照相关网站的说明，部署Demo智能合约。智能合约部署成功后，将生成合约地址。

调用智能合约的Demo C代码放置在: 

|Demo C代码                                                  |用途                   |
|:---------------------------------------------------------- |:--------------------- |
|\<SDKRoot\>/demo/demo_ethereum/demo_ethereum_storeread.c    |以太坊合约演示用例     |
|\<SDKRoot\>/demo/demo_ethereum/demo_ethereum_transfer.c     |以太坊转账演示用例     |
|\<SDKRoot\>/demo/demo_platon/demo_platon_transfer.c         |PLATON转账演示用例     |
|\<SDKRoot\>/demo/demo_platone/demo_platone_mycontract.c     |PLATONE合约演示用例    |
|\<SDKRoot\>/demo/demo_fiscobcos/demo_fiscobcos_helloworld.c |FISCO-BCOS合约演示用例 |

编译Demo之前，需要修改Demo的C代码中以下部分：
- 对于ETHEREUM、PLATON、FISCO-BCOS、PLATONE:
  1.	搜索`demoUrl`，将节点URL（含端口）填写为实际部署的节点或模拟器的IP地址和RPC端口
  2.	如果demo需使用原生私钥, 则搜索`native_demoKey`，并将客户端私钥设置为：  
        -	对于ETHEREUM，设置为ganache生成的任意一个账户的私钥  
        - 对于PlatON，无需修改Demo中的私钥
        - 对于PlatONE，无需修改Demo中的私钥
        - 对于FISCO-BCOS，设置为<FISCO-BCOS_ROOT>/console/accounts下私钥对应的原生格式私钥
  3.	如果demo需使用原生私钥, 则搜索`pkcs_demoKey`，并将客户端私钥设置为：  
        - 对于以太坊，设置为ganache生成的任意一个账户的私钥对应的PKCS格式私钥
        - 对于PlatONE，无需修改Demo中的私钥
        - 对于FISCO-BCOS，设置为<FISCO-BCOS_ROOT>/console/accounts下私钥
  4.	搜索`demoRecipientAddress`，修改为Demo合约的部署地址。
- 对于FABRIC:  
  1. 搜索`fabric_client_demokey`，设置客户端使用的私钥
  2. 搜索`fabric_client_democert`，设置客户端私钥对应的证书
  3. 如果demo启用TLS，则搜索`fabric_org1_tlsCert`、`fabric_org2_tlsCert`、`fabric_order_tlsCert`，设置CA证书链
  4. 搜索`fabric_demo_endorser_peer0Org1_url`、`fabric_demo_endorser_peer1Org1_url`、`fabric_demo_endorser_peer0Org2_url`、`fabric_demo_endorser_peer1Org2_url`，设置背书节点、排序节点的url地址
  5. 如果demo启用TLS,则搜索`fabric_demo_endorser_peer0Org1_hostName`、`fabric_demo_endorser_peer1Org1_hostName`、`fabric_demo_endorser_peer0Org2_hostName`、`fabric_demo_endorser_peer1Org2_hostName`，设置节点的主机名称
- 对于HW-BCS:  
  1. 搜索`hw_bcs_client_demokey`，设置客户端使用的私钥
  2. 搜索`hw_bcs_client_democert`，设置客户端私钥对应的证书
  3. 如果demo启用TLS，则搜索`hw_bcs_org1_tlsCert`、`hw_bcs_org2_tlsCert`，设置CA证书链
  4. 搜索`hw_bcs_demo_endorser_peer0Org1_url`、`hw_bcs_demo_endorser_peer0Org2_url`、`hw_bcs_demo_order_url`，设置背书节点、排序节点的url地址
  5. 如果demo启用TLS,则搜索`hw_bcs_demo_endorser_peer0Org1_hostName`、`hw_bcs_demo_endorser_peer0Org2_hostName`、`hw_bcs_demo_order_hostName`，设置节点的主机名称

#### 编译Demo
在\<SDKRoot\>目录下执行以下命令编译SDK的调用Demo：
```
$make demo
```
生成的Demo程序分别位于\<SDKRoot\>/build/demo/demo_\<protocol\>/<demo_name>路径下，< protocol>可以为`ethereum` `platon` `fisco-bcos` `platone` `fabric` `hwbcs`。

### 编译中的常见问题
1.  编译中提示类似“Makefile: 120: *** 缺失分隔符。 停止”的信息。  
该问题一般是因为Makefile中的target下的命令不是以Tab（ASCII码0x09）开头引起。注意按Tab键时，文本编辑器可能将Tab字符替换为若干空格。应将文本编辑器设置为不要用空格替代Tab。

2. 编译中提示“curl/curl.h”找不到  
该问题是因为系统中未安装curl及其开发文件引起。对于在Linux发行版上做Host编译而言，注意只安装curl包不够，还需要安装其开发文件包。开发文件包在不同的Linux发行版中有不同的名称，通常会命名为类似curl-devel，或者libcurl。具体请参照所使用的Linux发行版的软件包管理工具。  

如果curl采用源码编译，且未安装到系统目录，则应在external.env中指定其搜索路径，并在链接时指定curl库所在路径。  
在交叉编译中，尤其要注意搜索路径和库应指向交叉编译环境中的头文件和库，而不应指向执行编译的Host上的路径。

3. 交叉编译链接时提示字节序、位宽或ELF格式不匹配  
该问题通常是因为交叉编译中，部分库引用了Host的库，而Obj文件则是由交叉编译产生，或者，部分库为32位，另一部分为64位。应仔细核查所有库的路径，避免Host与Target的库混合链接，或者不同位宽的库混合链接。

可以使用如下命令查看库文件是ARM版本还是x86版本，以及位宽：  
```
$file \<lib或obj文件名\>
```

4. 编译中提示可执行文件找不到，或者参数错误  
常见提示:  
'make'不是内部或外部命令，也不是可运行的程序或批处理文件。  
mkdir… 命令语法不正确。  
FIND: 参数格式不正确  

该问题一般是因为在Windows下进行编译，但未安装Cygwin，或者未在Makefile中正确配置BOAT_RM、BOAT_MKDIR、BOAT_FIND的路径。请参照[以Windows为编译环境](####以Windows为编译环境)章节安装Cygwin和配置Makefile。

