#include <stdio.h>
#include <stdlib.h>
#include "in_arp_raw.h"

int main()
{
    unsigned char hw_dest_addr[ETH_ALEN];
    hw_dest_addr[0] = 0xB3;
    hw_dest_addr[1] = 0x02;
    hw_dest_addr[2] = 0xC2;
    hw_dest_addr[3] = 0xFF;
    hw_dest_addr[4] = 0x11;
    hw_dest_addr[5] = 0x76;
    send_in_arp("eth0", ARPOP_InREQUEST, hw_dest_addr, 0);
    perror("send");

    struct arp_req request;
    unsigned short int arp_type;

    while (1)
    {
        recv_in_arp (&request, &arp_type);
        printf("%d", request.ar_sip);
    }
    return 0;
}
