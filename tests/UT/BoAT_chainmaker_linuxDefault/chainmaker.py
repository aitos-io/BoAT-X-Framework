# ############## 1.pull 最新代码 ##############
import os
#from git.repo import Repo

# local_path = os.getcwd()
# repo = Repo(local_path)
# repo.git.pull()
os.chdir('../../../')
############## 2.make clean最新代码 ##############
# os.system('make clean')

############## 3.make 最新代码 ##############
#  CHAINMKER_NODE_URL="127.0.0.1:12301"\ #
os.system('make tests \
                   BOAT_TEST_USE_CHAINMAKER=1\
                   BOAT_TEST=TEST_MODE\
                   CHAINMKER_NODE_URL="121.4.178.74:12301"\
                   CHAINMKER_CHAIN_ID="chain1"\
                   CHAINMKER_ORG_ID="wx-org1.chainmaker.org"\
                   CHAINMKER_HOST_NAME="chainmaker.org"')

# # ############## 4. 运行程序 ##############
os.chdir('build/tests/BoAT_chainmaker_linuxDefault')
os.system('./boattest |tee boattest.txt')
os.chdir('../')

# ############## 5. 实现覆盖率测试 ##############
# os.system('cp ../../sdk/protocol/boatchainmaker/*.c ../sdk/protocol/boatchainmaker/')
# os.system('cp ../../sdk/protocol/boatchainmaker/protos/common/*.c ../sdk/protocol/boatchainmaker/protos/common/')
# os.system('cp ../../sdk/wallet/*.c ../sdk/wallet/')
# os.system('lcov --capture --directory ../sdk/protocol/boatchainmaker   --output-file  Boatchainmaker.info')
# os.system('lcov --capture --directory ../sdk/wallet  --output-file  BoatWallet.info')
# os.system('lcov -a Boatchainmaker.info -a BoatWallet.info -o Boatfinal.info')
# os.system('genhtml -o test_coverage_report Boatfinal.info')
# os.system('rm *.info')
# # ############## 6. 编译检查 ##############
# os.system('cppcheck --enable=warning ../../demo  ../../sdk/protocol 2> cppcheck_report.txt')
