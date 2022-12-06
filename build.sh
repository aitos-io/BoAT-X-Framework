###
 # @Description: 
 # @Author: aitos
 # @Date: 2022-08-19 14:33:35
 # @LastEditors: 
 # @LastEditTime: 2022-09-28 17:35:26
### 
rm -r ./build
mkdir ./build
cd ./build
cmake ../
make clean
make