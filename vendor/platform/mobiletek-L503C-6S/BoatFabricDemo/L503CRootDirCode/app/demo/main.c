#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mbtk_comm_api.h"
#include "mbtk_api.h"
#include "mbtk_os.h"
#include "boat_fabric_demo.h"
#include "mbtk_open_at.h"
#include "nl_api_sys.h"
#include "boattypes.h"


#define MBTK_TEST_STACK_SIZE    (40*1024)
int (*user_app_printf)(const char *fmt, ...);

mbtk_ostimerref time_1;
void timeout_cb(UINT32 cookie)
{
    mbtk_os_status os_status;
    mbtk_os_timer_status_struct sta;
    os_status = ol_os_get_timer_status(time_1,&sta);
    user_app_printf("sta = %d",sta.status);
    os_status = ol_os_timer_stop(time_1);
    os_status = ol_os_timer_delete(time_1);
	user_app_printf("os_status = %d",os_status);
}

extern BOAT_RESULT FabricDemo(void);

void user_data_call_demo_callback(uint8_t index, uint8_t status)
{
	user_app_printf("data_call_demo_callback enter index[%d] , status[%d]\n", index, status);
}

static int waitforNetwork(void){
    ol_regist_data_call_callback(user_data_call_demo_callback);
    if(ol_wait_network_regist(120) != mbtk_data_call_ok)
	{
		user_app_printf("mqtt_demo_wait_network ol_wait_network_regist time out\n");
		return -1;
	}
	user_app_printf("mqtt_demo_wait_network execute ol_data_call_start \n");
	// if(ol_data_call_start(mbtk_cid_index_2, mbtk_data_call_v4, "ctnet", NULL, NULL, 0) != mbtk_data_call_ok)
	if(ol_data_call_start(mbtk_cid_index_2, mbtk_data_call_v4, "UNINET", NULL, NULL, 0) != mbtk_data_call_ok)
	{
		user_app_printf("mqtt_demo_wait_network ol_data_call_start fail\n");
		return -1;
	}

	return 0;
}

void app_open_task(void *argv)
{
    uint32 cnt=0;
    char *p = NULL;

    user_app_printf("\r\n app task begin !!!\n");
    user_app_printf("waitforNetwork: %d\n", waitforNetwork());
    while(cnt++ < 3)
    {
        ol_os_task_sleep(1000);
    }
    FabricDemo();

    user_app_printf("\r\n app task end !!!\n");
}

void user_app_init(void *api_table)
{
    mbtk_taskref mbtk_test_task_ref;
    void *mbtk_test_task_stack = NULL;

    //Step 1: Initialize the API interface
    mbtk_api_init(api_table);

    user_app_printf("api init OK\r\n");

    mbtk_test_task_stack = ol_malloc(MBTK_TEST_STACK_SIZE);
    if(NULL == mbtk_test_task_stack)
    {
        user_app_printf("\r\nmbtk_test_task_stack nulllllll!!!\n");
        return;
    }

    mbtk_os_status status = ol_os_task_creat(&mbtk_test_task_ref, mbtk_test_task_stack, MBTK_TEST_STACK_SIZE, 221,"app_at_task", app_open_task, NULL);

    while(1)
    {
        user_app_printf("user_app: main \r\n");
        ol_os_task_sleep(10000);
    }

}


