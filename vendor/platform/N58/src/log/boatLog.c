#include <stdio.h>
#include <stdarg.h>

#include "nwy_osi_api.h"
#include "nwy_network.h"
#include "nwy_http.h"
#include "nwy_usb_serial.h"

#define BOAT_NWY_EXT_SIO_RX_MAX          (2*1024)

void BoatPrintf(char* fmt, ...)
{
    static char    echo_str[BOAT_NWY_EXT_SIO_RX_MAX];
    static nwy_osiMutex_t *echo_mutex = NULL;
    va_list     a;
    int i, size;

    if(NULL == echo_mutex)
        echo_mutex = nwy_create_mutex();
    if(NULL == echo_mutex)
        return;
    nwy_lock_mutex(echo_mutex, 0); 
    va_start(a, fmt);
    vsnprintf(echo_str, BOAT_NWY_EXT_SIO_RX_MAX, fmt, a);
    va_end(a);
    size = strlen((char *)echo_str);
    i = 0;
    while(1)
    {
        int tx_size;
  
        tx_size = nwy_usb_serial_send((char *)echo_str + i, size - i);
        if(tx_size <= 0)
            break;
        i += tx_size;
        
        if((i < size))
            nwy_sleep(10);
        else
            break;
    }
    nwy_unlock_mutex(echo_mutex);
}