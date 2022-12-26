/*********************************************************
*  @file    cm_vir_at_demo.c
*  @brief   ML302 OpenCPU virture AT channel example file
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2019/10/08
*  modify: XieGangLiang,2020/08/12,update examples
********************************************************/
#include "cm_main.h"

unsigned char virt_at_buffer[2048];//AT指令返回数据缓存区
int virt_at_len = 0;//AT指令返回数据长度
osThreadId_t OC_AT_TaskHandle;//OC AT任务句柄

const osMutexAttr_t at_Mutex_attr = {
  "atThreadMutex",                          // human readable mutex name
  osMutexRecursive | osMutexPrioInherit,    // attr_bits
  0,                                     // memory for control block   
  0U                                        // size for control block
};
//虚拟AT指令互斥锁定义
osMutexId_t vir_at_mutex;
osEventFlagsId_t at_task_flag; 


/**
 *  \brief 虚拟AT指令消息通知回调函数
 *  
 *  \param [in] pcu8NotifyLine URC消息数据接收指针
 *  \param [in] u32LineLen URC消息长度
 *  \param [in] eNotifyType URC消息类型
 *  \return 空
 *  
 *  \details More details
 */
static void cm_test_at_notify(const uint8_t* pcu8NotifyLine, uint32_t u32LineLen, cm_atchart_notify_e eNotifyType)
{
	char *p = NULL;
	if(eNotifyType == CM_ATCHART_RSP_EMPTY_LINE)
		return;

	cm_net_register_notify(pcu8NotifyLine,u32LineLen,eNotifyType);

	p = malloc(u32LineLen+1);
	if(p <= 0)
		return;
	memset(p,0,u32LineLen+1);
	memcpy(p,pcu8NotifyLine,u32LineLen);
	//here ,deal the notify in buffer p
	
	
	free(p);
}




/**
 *  \brief  虚拟AT指令任务
 *  
 *  \param [in] 
 *  \return NULL
 *  
 *  \details More details
 */
static void virture_at_task(void)
{
    while(1)
    {
		osEventFlagsWait(at_task_flag, 0x00000001U, osFlagsWaitAny, osWaitForever);

		memset(virt_at_buffer,0,2048);
		virt_at_len = cm_vir_at_get(virt_at_buffer, 2048);
		virt_at_buffer[virt_at_len] = 0;
		cm_sys_log("%s",virt_at_buffer);
		cm_atchart_on_recv(virt_at_buffer,virt_at_len);
    }   
}



/**
*  \brief 虚拟AT回调函数，当有AT命令返回或者URC消息时，此函数会被调用
*  
*  \param [in] param 输入参数指针变量
*  \return NULL
*  
*  \details More details
*/
static void  cm_sys_at_callback(void *param)
{
	osEventFlagsSet(at_task_flag, 0x00000001U);  
}

/**
*  \brief 虚拟AT初始化
*  
*  \param [in] 
*  \return NULL
*  
*  \details More details
*/
void boat_test_vir_at_init(void)
{
	osThreadAttr_t virat_task_attr = {0};
	at_task_flag = osEventFlagsNew(0);
	if (at_task_flag == 0)
	{
		return;
	}
	
	virat_task_attr.name  = "virat_task";
	virat_task_attr.stack_size = 2048;
	
    vir_at_mutex = osMutexNew(&at_Mutex_attr);
   // osThreadDef(OC_AT_Task, virture_at_task, osPriorityNormal, 0, 2048);
    OC_AT_TaskHandle = osThreadNew(virture_at_task,0,&virat_task_attr);
	
  cm_vir_at_init(cm_sys_at_callback);
	cm_atchart_init();
	cm_atchart_open();
	cm_atchart_set_notify_cb(cm_test_at_notify);

}


