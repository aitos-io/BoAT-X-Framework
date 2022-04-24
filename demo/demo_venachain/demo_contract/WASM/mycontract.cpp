//auto create contract
#include <stdlib.h>
#include <string.h>
#include <string>
#include <bcwasm/bcwasm.hpp>

namespace demo {
class my_contract : public bcwasm::Contract
{
    public:
    my_contract(){}

    /// 实现父类: bcwasm::Contract 的虚函数
    /// 该函数在合约首次发布时执行，仅调用一次
    void init() 
    {
        bcwasm::println("init success...");
    }
    /// 定义Event.
    /// BCWASM_EVENT(eventName, arguments...)
    BCWASM_EVENT(setName, const char *)
    
    public:
    void setName(const char *msg)
    {
        // 定义状态变量
        bcwasm::setState("NAME_KEY", std::string(msg));
        // 日志输出
        // 事件返回
        BCWASM_EMIT_EVENT(setName, "std::string(msg)");
    }
    const char* getName() const 
    {
        std::string value;
        bcwasm::getState("NAME_KEY", value);
        // 读取合约数据并返回
        return value.c_str();
    }
};
}
// 此处定义的函数会生成ABI文件供外部调用
BCWASM_ABI(demo::my_contract, setName)
BCWASM_ABI(demo::my_contract, getName)

            
//bcwasm autogen begin
extern "C" { 
void setName(const char * msg) {
demo::my_contract my_contract_bcwasm;
my_contract_bcwasm.setName(msg);
}
const char * getName() {
demo::my_contract my_contract_bcwasm;
return my_contract_bcwasm.getName();
}
void init() {
demo::my_contract my_contract_bcwasm;
my_contract_bcwasm.init();
}

}
//bcwasm autogen end