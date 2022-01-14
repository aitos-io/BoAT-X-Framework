# Solidity接口生成工具使用说明

## 引言

### 概述

本文介绍tools文件夹下的Solidity生成各类语言的工具的接口调用说明。  
本文的预期读者为使用Solidity合约的客户。  

## 功能介绍

由于对链上部署好的Solidity合约调用需要将输入数据编码为指定的格式，编码过程非常复杂，所以提供便捷的自动转换的工具来帮助用户调用合约。具体Solidity输入编码方式参考[Solidity官方网站](https://docs.soliditylang.org/)，`Contract ABI Specification`章节

**已支持的区块链:**  
|区块链  |脚本名字                   |
|:-----  | :--------------          |
|以太坊  |eth2c.py                   |
|PlatONE  |fiscobcos2c.py           |
|FISCO-BCOS  |platoneSolidity2c.py  |

## 软件依赖


## 使用说明
通过工具生成Solidity合约的C接口文件时，需要根据相应的平台调用tools文件夹相应的合约版本，其中，`eth2c.py`，`fiscobcos2c.py`，`platoneSolidity2c.py`三个文件为相应链的合约脚本。  

### 输入文件生成

#### 从Remix获得json文件
当使用Remix编译合约时，编译完成后，不能从编译界面下面的ABI获得相关参数并手动生成JSON文件。而是应该从如下图4-1所示位置将相应的合约文件内容复制出来并手动在本地生成JSON文件。
![从Remix获得json文件](./images/BoAT_Solidity_Tool_User_Guide_cn-F4-1-Remix_Get_Json.jpg)
图 4-1 从Remix获得json文件
#### 从truffle获得json文件
使用truffle对Solidity合约编译完成后会将相应的JSON文件生成在目录`\build\contracts`下。
### 输入类型转换
**已支持的Solidity变量:**  
|Solidity变量类型  |BoAT中映射的变量类型         | 
|:-----  | :--------------          | 
|  string      |BCHAR *    |  
|  bytes       |BUINT8 *   |
|  bytes[]     |BoatFieldVariable*|
|  string[]    |BoatFieldVariable*|
|  address[]   |BoatAddress *|
|  bool[]      |BUINT8 *   |
|  uint8[]     |BUINT8 *   |
|  uint16[]    |BUINT16 *  |
|  uint32[]    |BUINT32 *  |
|  uint64[]    |BUINT64 *  |
|  uint128[]   |BUINT128 * |
|  uint256[]   |BUINT256 * |
|  int8[]      |BSINT8 *   |
|  int16[]     |BSINT16 *  |
|  int32[]     |BSINT32 *  |
|  int64[]     |BSINT64 *  |
|  int128[]    |BSINT128 * |
|  int256[]    |BSINT256 * |    
|  bytes1[]    |Bbytes1 *  |
|  bytes2[]    |Bbytes2 *  |
|  bytes3[]    |Bbytes3 *  |
|  bytes4[]    |Bbytes4 *  |
|  bytes5[]    |Bbytes5 *  |
|  bytes6[]    |Bbytes6 *  |
|  bytes7[]    |Bbytes7 *  |
|  bytes8[]    |Bbytes8 *  |    
|  bytes9[]    |Bbytes9 *  |
|  bytes10[]   |Bbytes10 * |
|  bytes11[]   |Bbytes11 * |
|  bytes12[]   |Bbytes12 * |
|  bytes13[]   |Bbytes13 * |
|  bytes14[]   |Bbytes14 * |
|  bytes15[]   |Bbytes15 * |
|  bytes16[]   |Bbytes16 * |   
|  bytes17[]   |Bbytes17 * |
|  bytes18[]   |Bbytes18 * |
|  bytes19[]   |Bbytes19 * |
|  bytes20[]   |Bbytes20 * |
|  bytes21[]   |Bbytes21 * |
|  bytes22[]   |Bbytes22 * |
|  bytes23[]   |Bbytes23 * |
|  bytes24[]   |Bbytes24 * |    
|  bytes25[]   |Bbytes25 * |
|  bytes26[]   |Bbytes26 * |
|  bytes27[]   |Bbytes27 * |
|  bytes28[]   |Bbytes28 * |
|  bytes29[]   |Bbytes29 * |
|  bytes30[]   |Bbytes30 * |
|  bytes31[]   |Bbytes31 * |
|  bytes32[]   |Bbytes32 * |
|  address     |BoatAddress|
|  bool        |BUINT8     |
|  uint8       |BUINT8     |
|  uint16      |BUINT16    |
|  uint32      |BUINT32    |
|  uint64      |BUINT64    |
|  uint128     |BUINT128   | 
|  uint256     |BUINT256   |
|  int8        |BSINT8     |
|  int16       |BSINT16    |
|  int32       |BSINT32    |
|  int64       |BSINT64    |
|  int128      |BSINT128   |
|  int256      |BSINT256   |      
|  bytes1      |Bbytes1    |
|  bytes2      |Bbytes2    |
|  bytes3      |Bbytes3    |
|  bytes4      |Bbytes4    |
|  bytes5      |Bbytes5    |
|  bytes6      |Bbytes6    |
|  bytes7      |Bbytes7    |
|  bytes8      |Bbytes8    | 
|  bytes9      |Bbytes9    |
|  bytes10     |Bbytes10   |
|  bytes11     |Bbytes11   |
|  bytes12     |Bbytes12   |
|  bytes13     |Bbytes13   |
|  bytes14     |Bbytes14   |
|  bytes15     |Bbytes15   |
|  bytes16     |Bbytes16   | 
|  bytes17     |Bbytes17   |
|  bytes18     |Bbytes18   |
|  bytes19     |Bbytes19   |
|  bytes20     |Bbytes20   |
|  bytes21     |Bbytes21   |
|  bytes22     |Bbytes22   |
|  bytes23     |Bbytes23   |
|  bytes24     |Bbytes24   |    
|  bytes25     |Bbytes25   |
|  bytes26     |Bbytes26   |
|  bytes27     |Bbytes27   |
|  bytes28     |Bbytes28   |
|  bytes29     |Bbytes29   | 
|  bytes30     |Bbytes30   |
|  bytes31     |Bbytes31   |
|  bytes32     |Bbytes32   |


