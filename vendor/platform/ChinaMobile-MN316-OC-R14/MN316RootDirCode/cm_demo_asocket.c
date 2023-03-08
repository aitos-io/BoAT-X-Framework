/**
****************************************************************************************
* @FilePath: cm_demo_asocket.c
* @Author: aitos
* @Date: 2023-02-13 11:47:49
* @LastEditors:
* @LastEditTime: 2023-02-13 11:47:49
* @Descripttion:
****************************************************************************************
*/
#include "cm_os.h"
#include "cm_asocket.h"
#include "cm_eloop.h"
#include "cm_etimer.h"
#include "cm_async_dns.h"
// #include "cm_demo_common.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include "lwip/sockets.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define MAX_ASOCKET_NUM 6
#define MAX_ASYNC_DNS_NUM 4
#define MAX_EVENT_NUM 64
#define ETIMER_NUM 10
#define STR_LEN(s) (sizeof(s) - 1)
#define STR_ITEM(s) (s), STR_LEN(s)
// #define cm_printf cm_demo_printf

static cm_eloop_handle_t s_main_eloop = NULL;
static cm_etimer_manager_t s_etimer_manager = {NULL};
static cm_etimer_t s_etimers[ETIMER_NUM];

/* 异步socket消息回调(执行环境为eloop线程) */
static void __on_asocket_event(int sock, cm_asocket_event_e event, void *user_param)
{
    (void)user_param; // UNUSED

    switch (event)
    {
    // responses
    case CM_ASOCKET_EV_CONNECT_OK:
    {
        cm_log_printf(0, "sock(%d) connect_ok\n", sock);
        break;
    }
    case CM_ASOCKET_EV_CONNECT_FAIL:
    {
        cm_log_printf(0, "sock(%d) connect_fail\n", sock);
        break;
    }

    // indications
    case CM_ASOCKET_EV_RECV_IND:
    {
        /* 取得获取接收缓存中可读的数据长度 */
        int recv_avail = 0;
        cm_asocket_ioctl(sock, FIONREAD, &recv_avail);

        /* 接收数据 */
        uint8_t recv_buf[128] = {0};
        int ret = cm_asocket_recv(sock, recv_buf, sizeof(recv_buf), 0);
        if (ret > 0)
        {
            cm_log_printf(0, "sock(%d) recv_ind: recv_avail=%d, recv_len=%d, data=%.*s\n", sock, recv_avail, ret, ret, recv_buf);
        }
        else
        {
            cm_log_printf(0, "sock(%d) recv_ind error(%d)\n", sock, errno);

            if (ENOTCONN == errno)
            {
                /* Connection closed */
                cm_log_printf(0, "sock(%d) recv_ind: Connection closed\n", sock);
            }
        }
        break;
    }
    case CM_ASOCKET_EV_SEND_IND:
        cm_log_printf(0, "sock(%d) send_ind\n", sock);
        break;
    case CM_ASOCKET_EV_ACCEPT_IND:
        cm_log_printf(0, "sock(%d) accept_ind\n", sock);
        break;
    case CM_ASOCKET_EV_ERROR_IND:
    {
        /* 获取socket错误码 */
        int sock_error = 0;
        socklen_t opt_len = sizeof(sock_error);
        cm_asocket_getsockopt(sock, SOL_SOCKET, SO_ERROR, &sock_error, &opt_len);
        cm_log_printf(0, "sock(%d) error_ind: sock_error(%d)\n", sock, sock_error);
        if (ECONNABORTED == sock_error)
        {
            /* Connection aborted */
            cm_log_printf(0, "sock(%d) error_ind: Connection aborted\n", sock);
        }
        else if (ECONNRESET == sock_error)
        {
            /* Connection reset */
            cm_log_printf(0, "sock(%d) error_ind: Connection reset\n", sock);
        }
        else if (ENOTCONN == sock_error)
        {
            /* Connection closed */
            cm_log_printf(0, "sock(%d) error_ind: Connection closed\n", sock);
        }
        break;
    }
    default:
        break;
    }
}

// OPEN ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 执行环境为eloop线程(和异步socket在同一个线程,可以安全地处理socket创建和数据收发等操作) */
static void __on_eloop_cmd_OPEN_recv_event(cm_eloop_event_handle_t event, void *cb_param)
{
    /* 注销Event */
    cm_eloop_unregister_event(event);

    /* 取得参数 */
    int type = (int)cb_param;

    cm_log_printf(0, "__on_eloop_cmd_OPEN_recv_event type=%d\n", type);

    /* 处理命令 */
    int sock = -1;
    if (0 == type)
    {
        /* TCP */
        sock = cm_asocket_open(AF_INET, SOCK_STREAM, IPPROTO_TCP, __on_asocket_event, NULL);
    }
    else
    {
        /* UDP */
        sock = cm_asocket_open(AF_INET, SOCK_DGRAM, IPPROTO_UDP, __on_asocket_event, NULL);
    }
    if (-1 == sock)
    {
        cm_log_printf(0, "open failed\n");
        return;
    }
    cm_log_printf(0, "sock(%d) open\n", sock);

    // 120.27.12.119(6gforce.com):2053
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("120.27.12.119");
    server_addr.sin_port = htons(2053);

    int ret = cm_asocket_connect(sock, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0)
    {
        if (EINPROGRESS == errno)
        {
            cm_log_printf(0, "sock(%d) open request success, wait connect...\n", sock);
        }
        else
        {
            cm_log_printf(0, "sock(%d) connect failed(%d)\n", sock, errno);
        }
    }
    else if (0 == ret)
    {
        cm_log_printf(0, "sock(%d) connect_ok\n", sock);
    }
}

/* 被平台AT引擎调用, 执行环境为平台AT线程 */
static void __on_cmd_OPEN(void *param)
{
    /* 处理参数 */
    int type = atoi(((char **)param)[0]);

    cm_log_printf(0, "__on_cmd_OPEN type=%d\n", type);
    /* 注册Event(将命令封装成Event发送到eloop执行) */
    cm_eloop_event_handle_t cmd_OPEN_recv_event = cm_eloop_register_event(s_main_eloop, __on_eloop_cmd_OPEN_recv_event, (void *)type);
    /* 发送到eloop执行 */
    cm_eloop_post_event(cmd_OPEN_recv_event);
}

// CLOSE ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 执行环境为eloop线程(和异步socket在同一个线程,可以安全地处理socket创建和数据收发等操作) */
static void __on_eloop_cmd_CLOSE_recv_event(cm_eloop_event_handle_t event, void *cb_param)
{
    /* 注销Event */
    cm_eloop_unregister_event(event);

    /* 取得参数 */
    int sock = (int)cb_param;

    cm_log_printf(0, "__on_eloop_cmd_CLOSE_recv_event sock=%d\n", sock);

    /* 处理命令 */
    int ret = cm_asocket_close(sock);
    if (0 == ret)
    {
        cm_log_printf(0, "sock(%d) close success\n", sock);
    }
    else
    {
        cm_log_printf(0, "sock(%d) close error(%d)\n", sock, errno);
    }
}

/* 被平台AT引擎调用, 执行环境为平台AT线程 */
static void __on_cmd_CLOSE(void *param)
{
    /* 处理参数 */
    int sock = atoi(((char **)param)[0]);

    cm_log_printf(0, "__on_cmd_CLOSE sock=%d\n", sock);

    /* 注册Event(将命令封装成Event发送到eloop执行) */
    cm_eloop_event_handle_t cmd_CLOSE_recv_event = cm_eloop_register_event(s_main_eloop, __on_eloop_cmd_CLOSE_recv_event, (void *)sock);
    /* 发送到eloop执行 */
    cm_eloop_post_event(cmd_CLOSE_recv_event);
}

// SEND ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 执行环境为eloop线程(和异步socket在同一个线程,可以安全地处理socket创建和数据收发等操作) */
static void __on_eloop_cmd_SEND_recv_event(cm_eloop_event_handle_t event, void *cb_param)
{
    /* 注销Event */
    cm_eloop_unregister_event(event);

    /* 取得参数 */
    int sock = (int)cb_param;

    cm_log_printf(0, "__on_eloop_cmd_SEND_recv_event sock=%d\n", sock);

    /* 处理命令 */
    if (-1 != sock)
    {
        int ret = cm_asocket_send(sock, "hello", 5, 0);
        if (ret > 0)
        {
            cm_log_printf(0, "sock(%d) send len=%d\n", sock, ret);
        }
        else
        {
            cm_log_printf(0, "sock(%d) send error(%d)\n", sock, ret);
        }
    }
}

/* 被平台AT引擎调用, 执行环境为平台AT线程 */
static void __on_cmd_SEND(void *param)
{
    /* 处理参数 */
    int sock = atoi(((char **)param)[0]);

    cm_log_printf(0, "__on_cmd_SEND sock=%d\n", sock);

    /* 注册Event(将命令封装成Event发送到eloop执行) */
    cm_eloop_event_handle_t cmd_SEND_recv_event = cm_eloop_register_event(s_main_eloop, __on_eloop_cmd_SEND_recv_event, (void *)sock);
    /* 发送到eloop执行 */
    cm_eloop_post_event(cmd_SEND_recv_event);
}

// DNS ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 异步DNS事件回调函数(执行环境为eloop线程) */
static void __on_async_dns_event(int req_id, cm_async_dns_event_e event, void *cb_param,
                                 const char *host_name, const cm_async_dns_ip_addr_t *ip_addr)
{
    (void)cb_param; // UNUSED

    cm_log_printf(0, "__on_async_dns_event(%d, %d, %s)\n", req_id, event, host_name);

    switch (event)
    {
    case CM_ASYNC_DNS_RESOLVE_OK:
    {
        char ip_str[128] = "";
        if (CM_ASYNC_DNS_ADDRTYPE_IPV4 == ip_addr->type)
        {
            inet_ntop(AF_INET, &(ip_addr->u_addr.sin_addr), ip_str, sizeof(ip_str));
        }
        else if (CM_ASYNC_DNS_ADDRTYPE_IPV6 == ip_addr->type)
        {
            inet_ntop(AF_INET6, &(ip_addr->u_addr.sin6_addr), ip_str, sizeof(ip_str));
        }
        cm_log_printf(0, "dns req(%d) ok: %s->%s\n", req_id, host_name, ip_str);
        break;
    }
    case CM_ASYNC_DNS_RESOLVE_FAIL:
        cm_log_printf(0, "dns req(%d) fail: %s\n", req_id, host_name);
        break;
    default:
        break;
    }
}

/* 执行环境为eloop线程(和异步socket在同一个线程,可以安全地处理socket创建和数据收发等操作) */
static void __on_eloop_cmd_DNS_recv_event(cm_eloop_event_handle_t event, void *cb_param)
{
    /* 注销Event */
    cm_eloop_unregister_event(event);

    /* 取得参数 */
    char *host_name = (char *)cb_param;

    cm_log_printf(0, "__on_eloop_cmd_DNS_recv_event host_name=%s\n", host_name);

    /* 处理命令 */
    cm_async_dns_ip_addr_t ip_addr;
    int ret = cm_async_dns_request(host_name, CM_ASYNC_DNS_ADDRTYPE_IPV4_IPV6, &ip_addr, __on_async_dns_event, NULL);
    if (0 == ret)
    {
        char ip_str[128] = "";
        if (CM_ASYNC_DNS_ADDRTYPE_IPV4 == ip_addr.type)
        {
            inet_ntop(AF_INET, &(ip_addr.u_addr.sin_addr), ip_str, sizeof(ip_str));
        }
        else if (CM_ASYNC_DNS_ADDRTYPE_IPV6 == ip_addr.type)
        {
            inet_ntop(AF_INET6, &(ip_addr.u_addr.sin6_addr), ip_str, sizeof(ip_str));
        }
        cm_log_printf(0, "dns req ok: %s->%s\n", host_name, ip_str);
    }
    else if (ret > 0)
    {
        cm_log_printf(0, "dns req(%d) pending...\n", ret);
    }
    else
    {
        cm_log_printf(0, "dns req fail\n");
    }

    free(host_name);
}

/* 被平台AT引擎调用, 执行环境为平台AT线程 */
static void __on_cmd_DNS(void *param)
{
    /* 处理参数 */
    char *host_name = strdup(((char **)param)[0]);
    assert(NULL != host_name);

    cm_log_printf(0, "__on_cmd_DNS host_name=%s\n", host_name);

    /* 注册Event(将命令封装成Event发送到eloop执行) */
    cm_eloop_event_handle_t cmd_DNS_recv_event = cm_eloop_register_event(s_main_eloop, __on_eloop_cmd_DNS_recv_event, (void *)host_name);
    /* 发送到eloop执行 */
    cm_eloop_post_event(cmd_DNS_recv_event);
}

// DNS6 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 异步DNS事件回调函数(执行环境为eloop线程) */
static void __on_async_dns6_event(int req_id, cm_async_dns_event_e event, void *cb_param,
                                  const char *host_name, const cm_async_dns_ip_addr_t *ip_addr)
{
    (void)cb_param; // UNUSED

    cm_log_printf(0, "__on_async_dns6_event(%d, %d, %s)\n", req_id, event, host_name);

    switch (event)
    {
    case CM_ASYNC_DNS_RESOLVE_OK:
    {
        char ip_str[128] = "";
        if (CM_ASYNC_DNS_ADDRTYPE_IPV4 == ip_addr->type)
        {
            inet_ntop(AF_INET, &(ip_addr->u_addr.sin_addr), ip_str, sizeof(ip_str));
        }
        else if (CM_ASYNC_DNS_ADDRTYPE_IPV6 == ip_addr->type)
        {
            inet_ntop(AF_INET6, &(ip_addr->u_addr.sin6_addr), ip_str, sizeof(ip_str));
        }
        cm_log_printf(0, "dns6 req(%d) ok: %s->%s\n", req_id, host_name, ip_str);
        break;
    }
    case CM_ASYNC_DNS_RESOLVE_FAIL:
        cm_log_printf(0, "dns6 req(%d) fail: %s\n", req_id, host_name);
        break;
    default:
        break;
    }
}

/* 执行环境为eloop线程(和异步socket在同一个线程,可以安全地处理socket创建和数据收发等操作) */
static void __on_eloop_cmd_DNS6_recv_event(cm_eloop_event_handle_t event, void *cb_param)
{
    /* 注销Event */
    cm_eloop_unregister_event(event);

    /* 取得参数 */
    char *host_name = (char *)cb_param;

    cm_log_printf(0, "__on_eloop_cmd_DNS6_recv_event host_name=%s\n", host_name);

    /* 处理命令 */
    cm_async_dns_ip_addr_t ip_addr;
    int ret = cm_async_dns_request(host_name, CM_ASYNC_DNS_ADDRTYPE_IPV6_IPV4, &ip_addr, __on_async_dns6_event, NULL);
    if (0 == ret)
    {
        char ip_str[128] = "";
        if (CM_ASYNC_DNS_ADDRTYPE_IPV4 == ip_addr.type)
        {
            inet_ntop(AF_INET, &(ip_addr.u_addr.sin_addr), ip_str, sizeof(ip_str));
        }
        else if (CM_ASYNC_DNS_ADDRTYPE_IPV6 == ip_addr.type)
        {
            inet_ntop(AF_INET6, &(ip_addr.u_addr.sin6_addr), ip_str, sizeof(ip_str));
        }
        cm_log_printf(0, "dns6 req ok: %s->%s\n", host_name, ip_str);
    }
    else if (ret > 0)
    {
        cm_log_printf(0, "dns6 req(%d) pending...\n", ret);
    }
    else
    {
        cm_log_printf(0, "dns6 req fail\n");
    }

    free(host_name);
}

/* 被平台AT引擎调用, 执行环境为平台AT线程 */
static void __on_cmd_DNS6(void *param)
{
    /* 处理参数 */
    char *host_name = strdup(((char **)param)[0]);
    assert(NULL != host_name);

    cm_log_printf(0, "__on_cmd_DNS6 host_name=%s\n", host_name);

    /* 注册Event(将命令封装成Event发送到eloop执行) */
    cm_eloop_event_handle_t cmd_DNS6_recv_event = cm_eloop_register_event(s_main_eloop, __on_eloop_cmd_DNS6_recv_event, (void *)host_name);
    /* 发送到eloop执行 */
    cm_eloop_post_event(cmd_DNS6_recv_event);
}

// DNS_ALL /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 异步DNS_ALL事件回调函数(执行环境为eloop线程) */
static void __on_async_dns_all_event(int req_id, cm_async_dns_event_e event, void *cb_param,
                                     const char *host_name, const cm_async_dns_ip_addr_t *ip_addr)
{
    (void)cb_param; // UNUSED

    cm_log_printf(0, "__on_async_dns_all_event(%d, %d, %s)\n", req_id, event, host_name);

    switch (event)
    {
    case CM_ASYNC_DNS_RESOLVE_OK:
    {
        cm_log_printf(0, "dns_all req(%d) ok: %s\n", req_id, host_name);

        char ip_str[128] = "";
        uint8_t addr_num = cm_async_dns_get_result_addr_num(ip_addr);
        uint8_t i = 0;
        for (; i < addr_num; ++i)
        {
            if (CM_ASYNC_DNS_ADDRTYPE_IPV4 == ip_addr[i].type)
            {
                inet_ntop(AF_INET, &(ip_addr[i].u_addr.sin_addr), ip_str, sizeof(ip_str));
            }
            else if (CM_ASYNC_DNS_ADDRTYPE_IPV6 == ip_addr[i].type)
            {
                inet_ntop(AF_INET6, &(ip_addr[i].u_addr.sin6_addr), ip_str, sizeof(ip_str));
            }
            cm_log_printf(0, "%s\n", ip_str);
        }
        break;
    }
    case CM_ASYNC_DNS_RESOLVE_FAIL:
        cm_log_printf(0, "dns_all req(%d) fail: %s\n", req_id, host_name);
        break;
    default:
        break;
    }
}

/* 执行环境为eloop线程(和异步socket在同一个线程,可以安全地处理socket创建和数据收发等操作) */
static void __on_eloop_cmd_DNS_ALL_recv_event(cm_eloop_event_handle_t event, void *cb_param)
{
    /* 注销Event */
    cm_eloop_unregister_event(event);

    /* 取得参数 */
    char *host_name = (char *)cb_param;

    cm_log_printf(0, "__on_eloop_cmd_DNS_ALL_recv_event host_name=%s\n", host_name);

    /* 处理命令 */
    cm_async_dns_ip_addr_t ip_addr[4];
    int ret = cm_async_dns_req_all(host_name, CM_ASYNC_DNS_ADDRTYPE_IPV6_IPV4, ip_addr, ARRAY_SIZE(ip_addr), __on_async_dns_all_event, NULL);
    if (0 == ret)
    {
        cm_log_printf(0, "dns_all req ok: %s\n", host_name);

        char ip_str[128] = "";
        uint8_t addr_num = cm_async_dns_get_result_addr_num(ip_addr);
        uint8_t i = 0;
        for (; i < addr_num; ++i)
        {
            if (CM_ASYNC_DNS_ADDRTYPE_IPV4 == ip_addr[i].type)
            {
                inet_ntop(AF_INET, &(ip_addr[i].u_addr.sin_addr), ip_str, sizeof(ip_str));
            }
            else if (CM_ASYNC_DNS_ADDRTYPE_IPV6 == ip_addr[i].type)
            {
                inet_ntop(AF_INET6, &(ip_addr[i].u_addr.sin6_addr), ip_str, sizeof(ip_str));
            }
            cm_log_printf(0, "%s\n", ip_str);
        }
    }
    else if (ret > 0)
    {
        cm_log_printf(0, "dns_all req(%d) pending...\n", ret);
    }
    else
    {
        cm_log_printf(0, "dns_all req fail\n");
    }

    free(host_name);
}

/* 被平台AT引擎调用, 执行环境为平台AT线程 */
static void __on_cmd_DNS_ALL(void *param)
{
    /* 处理参数 */
    char *host_name = strdup(((char **)param)[0]);
    assert(NULL != host_name);

    cm_log_printf(0, "__on_cmd_DNS_ALL host_name=%s\n", host_name);

    /* 注册Event(将命令封装成Event发送到eloop执行) */
    cm_eloop_event_handle_t cmd_DNS_ALL_recv_event = cm_eloop_register_event(s_main_eloop, __on_eloop_cmd_DNS_ALL_recv_event, (void *)host_name);
    /* 发送到eloop执行 */
    cm_eloop_post_event(cmd_DNS_ALL_recv_event);
}

// ETIMER_START ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* timer回调函数(执行环境为eloop线程) */
static void __on_cm_etimer_timeout(cm_etimer_t *timer, void *cb_param)
{
    uint32_t timer_id = (uint32_t)cb_param;
    cm_log_printf(0, "__on_cm_etimer_timeout(%p, %p) timer_id=%u\n", (uint32_t)timer, cb_param, timer_id);
    cm_log_printf(0, "timer(%u) timeout\n", timer_id);
}

/* 初始化所有定时器(执行环境为eloop线程) */
static void __cm_etimer_init_all()
{
    cm_log_printf(0, "__cm_etimer_init_all() ETIMER_NUM=%d\n", ETIMER_NUM);

    int i = 0;
    for (i = 0; i < ETIMER_NUM; ++i)
    {
        cm_etimer_init(&s_etimers[i], __on_cm_etimer_timeout, (void *)i);
    }
}

/* ETIMER_START命令参数 */
typedef struct
{
    uint32_t timer_id;
    uint32_t millsec;
} etimer_start_param_t;

/* 执行环境为eloop线程(和异步socket在同一个线程,可以安全地处理socket创建和数据收发等操作) */
static void __on_eloop_cmd_ETIMER_START_recv_event(cm_eloop_event_handle_t event, void *cb_param)
{
    /* 注销Event */
    cm_eloop_unregister_event(event);

    /* 取得参数 */
    etimer_start_param_t *start_param = (etimer_start_param_t *)cb_param;

    cm_log_printf(0, "__on_eloop_cmd_ETIMER_START_recv_event timer_id=%u, millsec=%u\n", start_param->timer_id, start_param->millsec);

    /* 取得定时器对象 */
    cm_etimer_t *timer = &s_etimers[start_param->timer_id];
    /* 启动定时器 */
    int32_t ret = cm_etimer_start(&s_etimer_manager, timer, start_param->millsec);
    assert(0 == ret);

    cm_log_printf(0, "timer start ok: timer_id=%u, millsec=%u\n", start_param->timer_id, start_param->millsec);

    free(start_param);
}

/* 被平台AT引擎调用, 执行环境为平台AT线程 */
static void __on_cmd_ETIMER_START(void *param)
{
    /* 处理参数 */
    int timer_id = atoi(((char **)param)[0]);
    int millsec = atoi(((char **)param)[1]);

    cm_log_printf(0, "__on_cmd_ETIMER_START timer_id=%d, millsec=%d\n", timer_id, millsec);

    if (timer_id < 0 || timer_id >= ETIMER_NUM)
    {
        cm_log_printf(0, "__on_cmd_ETIMER_START timer_id invalid!\n");
        return;
    }

    if (millsec < 0)
    {
        cm_log_printf(0, "__on_cmd_ETIMER_START millsec invalid!\n");
        return;
    }

    etimer_start_param_t *start_param = (etimer_start_param_t *)malloc(sizeof(etimer_start_param_t));
    assert(NULL != start_param);
    start_param->timer_id = (uint32_t)timer_id;
    start_param->millsec = (uint32_t)millsec;

    /* 注册Event(将命令封装成Event发送到eloop执行) */
    cm_eloop_event_handle_t cmd_ETIMER_START_recv_event = cm_eloop_register_event(s_main_eloop, __on_eloop_cmd_ETIMER_START_recv_event, (void *)start_param);
    /* 发送到eloop执行 */
    cm_eloop_post_event(cmd_ETIMER_START_recv_event);
}

// ETIMER_STOP ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 执行环境为eloop线程(和异步socket在同一个线程,可以安全地处理socket创建和数据收发等操作) */
static void __on_eloop_cmd_ETIMER_STOP_recv_event(cm_eloop_event_handle_t event, void *cb_param)
{
    /* 注销Event */
    cm_eloop_unregister_event(event);

    /* 取得参数 */
    uint32_t timer_id = (uint32_t)cb_param;

    cm_log_printf(0, "__on_eloop_cmd_ETIMER_STOP_recv_event timer_id=%u\n", timer_id);

    /* 取得定时器对象 */
    cm_etimer_t *timer = &s_etimers[timer_id];

    /* 停止定时器 */
    int ret = cm_etimer_stop(timer);
    if (0 == ret)
    {
        cm_log_printf(0, "timer stop ok: timer_id=%u\n", timer_id);
    }
    else if (1 == ret)
    {
        cm_log_printf(0, "timer already stop: timer_id=%u\n", timer_id);
    }
    else
    {
        cm_log_printf(0, "timer stop failed: timer_id=%u\n", timer_id);
    }
}

/* 被平台AT引擎调用, 执行环境为平台AT线程 */
static void __on_cmd_ETIMER_STOP(void *param)
{
    /* 处理参数 */
    int timer_id = atoi(((char **)param)[0]);

    cm_log_printf(0, "__on_cmd_ETIMER_STOP timer_id=%d\n", timer_id);

    if (timer_id < 0 || timer_id >= ETIMER_NUM)
    {
        cm_log_printf(0, "__on_cmd_ETIMER_STOP timer_id invalid!\n");
        return;
    }

    /* 注册Event(将命令封装成Event发送到eloop执行) */
    cm_eloop_event_handle_t cmd_ETIMER_STOP_recv_event = cm_eloop_register_event(s_main_eloop, __on_eloop_cmd_ETIMER_STOP_recv_event, (void *)timer_id);
    /* 发送到eloop执行 */
    cm_eloop_post_event(cmd_ETIMER_STOP_recv_event);
}

/* eloop执行线程 */
static void __eloop_thread_proc(void *argument)
{
    (void)argument; // UNUSED

    cm_log_printf(0, "__eloop_thread_proc\n");

    /* 初始化所有定时器 */
    __cm_etimer_init_all();

    /* 启动事件循环 */
    while (1)
    {
        uint32_t wait_time = cm_etimer_get_wait_time(&s_etimer_manager);
        int ret = cm_eloop_wait_event(s_main_eloop, wait_time);
        if (ret < 0)
        {
            break;
        }
        cm_etimer_run(&s_etimer_manager);
    }

    /* 删除事件循环 */
    cm_eloop_delete(s_main_eloop);

    /* FreeRTOS线程退出需要的处理 */
    /* Tasks must not attempt to return from their implementing
    function or otherwise exit.  In newer FreeRTOS port
    attempting to do so will result in an configASSERT() being
    called if it is defined.  If it is necessary for a task to
    exit then have the task call vTaskDelete( NULL ) to ensure
    its exit is clean. */
    osThreadExit();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 初始化eloop测试模块 */
void cm_test_asocket_init(void)
{
    cm_log_printf(0, "cm_test_asocket_init\n");

    /* 初始化Event Loop模块 */
    cm_eloop_init();

    /* 初始化定时器管理器 */
    cm_etimer_manager_init(&s_etimer_manager);

    /* 创建事件循环 */
    s_main_eloop = cm_eloop_create(MAX_EVENT_NUM);

    /* 初始化异步socket模块 */
    cm_asocket_init(MAX_ASOCKET_NUM, s_main_eloop);

    /* 初始化异步DNS模块 */
    cm_async_dns_init(MAX_ASYNC_DNS_NUM, s_main_eloop);

    /* 创建eloop执行线程 */
    osThreadAttr_t thread_attr = {0};
    thread_attr.name = "demo_eloop_main";
    thread_attr.stack_size = 2048;
    osThreadNew(__eloop_thread_proc, NULL, &thread_attr);
}

/* 执行eloop测试命令 */
void cm_test_asocket_cmd(unsigned char **cmd, int len)
{
    cm_log_printf(0, "cm_test_asocket_cmd len=%d\n", len);

    /* CM:ASOCKET:[CMD]... */
    if (len < 3)
    {
        cm_log_printf(0, "invalid param\n");
        return;
    }

    cm_log_printf(0, "cmd[0]=%s,cmd[1]=%s,cmd[2]=%s\n", cmd[0], cmd[1], cmd[2]);

    const char *sub_cmd = (const char *)cmd[2];
    if (strncmp(sub_cmd, STR_ITEM("OPEN")) == 0)
    {
        /* CM:ASOCKET:OPEN:[type]... */
        if (len < 4)
        {
            cm_log_printf(0, "invalid param\n");
            return;
        }
        __on_cmd_OPEN(cmd + 3);
    }
    else if (strncmp(sub_cmd, STR_ITEM("CLOSE")) == 0)
    {
        /* CM:ASOCKET:CLOSE:[sock]... */
        if (len < 4)
        {
            cm_log_printf(0, "invalid param\n");
            return;
        }
        __on_cmd_CLOSE(cmd + 3);
    }
    else if (strncmp(sub_cmd, STR_ITEM("SEND")) == 0)
    {
        /* CM:ASOCKET:SEND:[sock]... */
        if (len < 4)
        {
            cm_log_printf(0, "invalid param\n");
            return;
        }
        __on_cmd_SEND(cmd + 3);
    }
    else if (strncmp(sub_cmd, STR_ITEM("DNS_ALL")) == 0)
    {
        /* CM:ASOCKET:DNS_ALL:[host_name]... */
        if (len < 4)
        {
            cm_log_printf(0, "invalid param\n");
            return;
        }
        __on_cmd_DNS_ALL(cmd + 3);
    }
    else if (strncmp(sub_cmd, STR_ITEM("DNS6")) == 0)
    {
        /* CM:ASOCKET:DNS6:[host_name]... */
        if (len < 4)
        {
            cm_log_printf(0, "invalid param\n");
            return;
        }
        __on_cmd_DNS6(cmd + 3);
    }
    else if (strncmp(sub_cmd, STR_ITEM("DNS")) == 0)
    {
        /* CM:ASOCKET:DNS:[host_name]... */
        if (len < 4)
        {
            cm_log_printf(0, "invalid param\n");
            return;
        }
        __on_cmd_DNS(cmd + 3);
    }
    else if (strncmp(sub_cmd, STR_ITEM("ETIMER_START")) == 0)
    {
        /* CM:ASOCKET:ETIMER_START:[id]:[millsec]... */
        if (len < 5)
        {
            cm_log_printf(0, "invalid param\n");
            return;
        }
        __on_cmd_ETIMER_START(cmd + 3);
    }
    else if (strncmp(sub_cmd, STR_ITEM("ETIMER_STOP")) == 0)
    {
        /* CM:ASOCKET:ETIMER_STOP:[id]... */
        if (len < 4)
        {
            cm_log_printf(0, "invalid param\n");
            return;
        }
        __on_cmd_ETIMER_STOP(cmd + 3);
    }
}

/* 适配Demo框架命令 */
int test_asocket_init(void)
{
    cm_log_printf(0, "test_asocket_init\n");
    cm_test_asocket_init();
    return 0;
}
