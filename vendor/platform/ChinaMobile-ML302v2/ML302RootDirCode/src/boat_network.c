/*********************************************************
*  @file    cm_network_demo.c
*  @brief   ML302 OpenCPU network example file
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2019/10/15
********************************************************/
#include "cm_main.h"
#include "cm_net_register.h"

struct   in_addr test_remote_addr;
uint16_t test_remote_port;
char test_msg[] = "network test\n";
char test_rx_buf[100];
int cm_global_socket_id = -1;
osThreadId_t cm_socket_TaskHandle = 0;
unsigned char cm_socket_buf[1024] = {0};

/*
网络初始化配置
*/
static bool bNetAutoRegNeedReinit = 0; /* 标识自动网络注册需要初始化 */

//自动注网模块需要添加ATCHART模块才能生效
static int net_ready = 0;
//网络自注册接口回调函数，通知网络状态变化
static void network_state_change(cm_network_state_e state)
{
    cm_sys_log("[NET_REGISTER]:state:%d\n",state);
    switch(state)
    {
        //自动注网模块初始化
        case CM_NETWORK_STATE_INIT:
            cm_sys_log("[NET_REGISTER]:Module init\n");
            break;
        //无SIM卡
        case CM_NETWORK_STATE_NO_SIM:
            cm_sys_log("[NET_REGISTER]:No SIM\n");
            break;
        //SIM卡识别成功
        case CM_NETWORK_STATE_SIM_READY:
            cm_sys_log("[NET_REGISTER]:SIM ready\n");
            break;
        //正在附着网络，CEREG:0,2
        case CM_NETWORK_STATE_ATTACHING:
            cm_sys_log("[NET_REGISTER]:Attaching\n");
            break;
        //网络附着失败，CEREG:0,0
        case CM_NETWORK_STATE_ATTACH_FAIL:
            cm_sys_log("[NET_REGISTER]:Attach fail\n");
            break;
        //网络附着成功，CEREG:0,1
        case CM_NETWORK_STATE_ATTACH_SUCCESS:
            cm_sys_log("[NET_REGISTER]:Attach success\n");
            break;
        //正在激活PDP
        case CM_NETWORK_STATE_PDP_ACTIVATING:
            cm_sys_log("[NET_REGISTER]:PDP activating\n");
            break;
        //PDP激活失败
        case CM_NETWORK_STATE_PDP_NOT_READY:
            cm_sys_log("[NET_REGISTER]:PDP not ready\n");
            break;
        //PDP激活成功
        case CM_NETWORK_STATE_PDP_READY:
            net_ready = 1;
            cm_sys_log("[NET_REGISTER]:PDP ready\n");
            break;
        default:
            cm_sys_log("[NET_REGISTER]:Module stop\n");
            
    }
}
/**
 *  \brief 网络自注册模块初始化
 *  
 *  \return 空
 *  
 *  \details More details
 */
static void cm_test_net_register_init(void)
{
    //网络配置结构体
    cm_network_param_t network_param;
    
    //填写APN等信息，
    network_param.cid = 1;
    network_param.apn = "CMNET";
    network_param.pdp_type = "IP";
    network_param.use_auth = 0;

    
    //初始化
    cm_net_register_init(); 
    //网络参数配置
    cm_net_register_config(&network_param);
    //设置网络状态变化时的回调函数
    cm_net_register_set_callback(network_state_change);
    //启动网络自注册模块
    cm_net_register_start();
}
/**
 *  \brief 等待网络注册成功
 *  
 *  \return 0：成功 -1：超时失败
 *  
 *  \details More details
 */
static int cm_test_wait_for_network(void)
{
    //等待时间
    int retry_count = 50*60; 
    //将成功标志设为0
    net_ready = 0;
    //手动触发网络注册模块更新状态，确保获取到最新网络状态
    cm_net_register_update_status();
    //等待一段时间，当超过时间，network_state_change回调函数仍没有返回CM_NETWORK_STATE_PDP_READY，说明网络注册可能遇到问题。
    while((net_ready == 0)&&(retry_count>0))
    {
        osDelay(1);
        retry_count--;
    }
    if(net_ready != 0) //网络注册成功
    {
        cm_sys_log("network ready");
        return 0;
    }
    else //网络注册尚未成功
    {
        cm_sys_log("network not ready");
        return -1;
    }
}

/**
 *  \brief 网络注册配置
 *  
 *  \return 空
 *  
 *  \details More details   
 */
void boat_test_network_config(void)
{

    //采用ATCHART模块实现的网络自注册方式，实现自动注网
    cm_test_net_register_init();
    cm_test_wait_for_network();
}

