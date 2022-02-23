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
@todo

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

以下例子中，生成的C语言函数均省去交易类型
#### string类型
Solidity中的string变成BCHAR *类型。  
Solidity函数：setName(string name);  
C语言函数：合约名_setName(BCHAR *name);  

示例：
```
const BCHAR *name = "Hello,World!";
合约名_setName(name);
```

#### bytes类型
Solidity中的bytes变成BUINT8 *类型和一个表示长度的BUINT32值  
Solidity函数：setData(bytes data);  
C语言函数：合约名_setData(BUINT8 *data, BUINT32 dataLen);  

示例：
```
BUINT8 data[] = {0,1,2,3,4,5};  
合约名_setData(data, 6);  
```

#### bytes[N]类型
其中N为大于0的整数。  
Solidity中的bytes[N]变成BoatFieldVariable *类型  
Solidity函数：setData(bytes[N] data);  
C语言函数：合约名_setData(BoatFieldVariable *data);  

示例：
```
BoatFieldVariable input[2];
BUINT8 data1[] = {0,1};
BUINT8 data2[] = {0,1,2,3,4,5};
input[0].field_ptr = data1;
input[0].field_len = 2;
input[1].field_ptr = data2;
input[1].field_len = 6;
合约名_setData(input); 
```

#### bytes[]类型
Solidity中的bytes[]变成BoatFieldVariable *类型和一个表示长度的BUINT32值  
Solidity函数：setData(bytes[] data);  
C语言函数：合约名_setData(BoatFieldVariable *data, BUINT32 dataLen);  

示例：
```
BoatFieldVariable input[3];
BUINT8 data1[] = {0,1};
BUINT8 data2[] = {0,1,2,3,4,5};
BUINT8 data3[] = {7,8,9,10,11,12,13,14};
input[0].field_ptr = data1;
input[0].field_len = 2;
input[1].field_ptr = data2;
input[1].field_len = 6;
input[2].field_ptr = data3;
input[2].field_len = 8;
合约名_setData(input, 3); 
```

#### string[N]类型
Solidity中的string[N]变成BoatFieldVariable *类型  
Solidity函数：setData(string[N] data);  
C语言函数：合约名_setData(BoatFieldVariable *data);  

示例：
```
BoatFieldVariable input[3];
BCHAR data1[] = {0x31, 0x32, 0x33, 0x34};
BCHAR data2[] = {'H','e','l','l','o',',','w','o','r','l','d','!'};
BCHAR data3[] = {0x39, 0x38, 0x37, 0x36, 0x35};

input[0].field_ptr = data1;
input[0].field_len = 4;
input[1].field_ptr = data2;
input[1].field_len = 12;
input[2].field_ptr = data3;
input[2].field_len = 5;
合约名_setData(input); 
```

#### string[]类型
Solidity中的string[]变成BoatFieldVariable *类型和一个表示长度的BUINT32值  
Solidity函数：setData(string[] data);  
C语言函数：合约名_setData(BoatFieldVariable *data, BUINT32 dataLen);  

示例：
```
BoatFieldVariable input[3];
BCHAR data1[] = {0x31, 0x32, 0x33, 0x34};
BCHAR data2[] = {'H','e','l','l','o',',','w','o','r','l','d','!'};
BCHAR data3[] = {0x39, 0x38, 0x37, 0x36, 0x35};

input[0].field_ptr = data1;
input[0].field_len = 4;
input[1].field_ptr = data2;
input[1].field_len = 12;
input[2].field_ptr = data3;
input[2].field_len = 5;
合约名_setData(input, 3); 
```

#### address[N]类型
Solidity中的address[N]变成BoatAddress *类型。  
Solidity函数：setData(address[N] data);  
C语言函数：合约名_setData(BoatAddress data[2]);  

示例：
```
BoatAddress data[2];
for (i = 0; i < 20; i++)
{
    data[0][i] = i;
    data[1][i] = 20 - i;
}
合约名_setData(data); 
```

#### address[]类型
Solidity中的address[]变成BoatAddress *类型和一个表示长度的BUINT32值。  
Solidity函数：setData(address[] data);  
C语言函数：合约名_setData(BoatAddress *data, BUINT32 dataLen);  

示例：  
```
BoatAddress data[2];
for (i = 0; i < 20; i++)
{
    data[0][i] = i;
    data[1][i] = 20 - i;
}
合约名_setData(data, 2); 
```

#### bool[N]类型
Solidity中的bool[N]变成BUINT8 *类型。  
Solidity函数：setData(bool[N] data);  
C语言函数：合约名_setData(BUINT8 *data);  

示例：
```
BUINT8 data[2];
data[0] = BOAT_FALSE;
data[1] = BOAT_TRUE;
合约名_setData(data); 
```

#### bool[]类型
Solidity中的bool[]变成BUINT8 *类型和一个表示长度的BUINT32值。  
Solidity函数：setData(bool[] data);  
C语言函数：合约名_setData(BUINT8 *data, BUINT32 dataLen);  

示例：  
```
BUINT8 data[3];
data[0] = BOAT_FALSE;
data[1] = BOAT_TRUE;
data[2] = BOAT_FALSE;
合约名_setData(data, 3); 
```

#### uint8[N]类型
Solidity中的uint8[N]变成BUINT8类型。  
Solidity函数：setData(uint8[N] data);  
C语言函数：合约名_setData(BUINT8 *data);  

示例：  
```
BUINT8 data[2];
data[0] = 0;
data[1] = 1;
合约名_setData(data); 
```

#### uint8[]类型
Solidity中的uint8[]变成BUINT8类型和一个表示长度的BUINT32值。  
Solidity函数：setData(uint8[] data);  
C语言函数：合约名_setData(BUINT8 *data, BUINT32 dataLen);  

示例：
```
BUINT8 data[3];
data[0] = 32;
data[1] = 10;
data[2] = 255;
合约名_setData(data, 3); 
```

#### uint16[N]类型
参考[uint8[N]类型](#uint8[n]类型)，只是需要把相应的BUINT8改为BUINT16。  

#### uint16[]类型
参考[uint8[]类型](#uint8[]类型)，只是需要把相应的BUINT8改为BUINT16。  

#### uint32[N]类型
参考[uint8[N]类型](#uint8[n]类型)，只是需要把相应的BUINT8改为BUINT32。  

#### uint32[]类型
参考[uint8[]类型](#uint8[]类型)，只是需要把相应的BUINT8改为BUINT32。  

#### uint64[N]类型
参考[uint8[N]类型](#uint8[n]类型)，只是需要把相应的BUINT8改为BUINT64。  

#### uint64[]类型
参考[uint8[]类型](#uint8[]类型)，只是需要把相应的BUINT8改为BUINT64。  

#### uint128[N]类型
Solidity中的uint128[N]变成BUINT128类型，而一个BUINT128是由BUINT8[16]组成的数组。  
Solidity函数：setData(uint128[N] data);  
C语言函数：合约名_setData(BUINT128 *data);  

示例：  
```
BUINT128 data[2];
BUINT32 i;
for (i = 0; i < 16; i++)
{
    data[0][i] = i;
    data[1][i] = (32 - i) * 2;
}
合约名_setData(data); 
```

#### uint128[]类型
Solidity中的uint128[N]变成BUINT128类型和一个表示长度的BUINT32值。，而一个BUINT128是由BUINT8[16]组成的数组。  
Solidity函数：setData(uint128[] data);  
C语言函数：合约名_setData(BUINT128 *data, BUINT32 len);  

示例：  
```
BUINT128 data[2];
BUINT32 i;
for (i = 0; i < 16; i++)
{
    data[0][i] = i;
    data[1][i] = (32 - i) * 2;
}
合约名_setData(data, 2); 
``` 

#### uint256[N]类型
Solidity中的uint256[N]变成BUINT256类型，而一个BUINT256是由BUINT8[32]组成的数组。  
Solidity函数：setData(uint256[N] data);  
C语言函数：合约名_setData(BUINT256 *data);  

示例：  
```
BUINT256 data[3];
BUINT32 i;
for (i = 0; i < 32; i++)
{
    data[0][i] = i;
    data[1][i] = (32 - i) * 2;
    data[2][i] = (16 - i) * 4;
}
合约名_setData(data); 
```

#### uint256[]类型
Solidity中的uint256[N]变成BUINT256类型和一个表示长度的BUINT32值，而一个BUINT256是由BUINT8[32]组成的数组。  
Solidity函数：setData(uint256[] data);  
C语言函数：合约名_setData(BUINT256 *data, BUINT32 len);  

示例：  
```
BUINT256 data[2];
BUINT32 i;
for (i = 0; i < 32; i++)
{
    data[0][i] = i;
    data[1][i] = (32 - i) * 2;
}
合约名_setData(data, 2); 
``` 