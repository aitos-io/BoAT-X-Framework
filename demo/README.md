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
