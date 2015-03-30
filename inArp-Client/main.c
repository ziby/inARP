#include <stdio.h>
#include <stdlib.h>
#include "in_arp_raw.h"

int main()
{

    struct arp_req request;
    unsigned short int arp_type;

    while (1)
    {
        recv_in_arp (&request, &arp_type);
        struct ifreq ifr;
        int sock = socket(AF_INET, SOCK_DGRAM, 0);

        /* I want to get an IPv4 IP address */
        ifr.ifr_addr.sa_family = AF_INET;

        if (arp_type != ARPOP_InREQUEST) continue;

        strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
        ioctl(sock, SIOCGIFADDR, &ifr);

        close(sock);

        printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

        u_int32_t sin_addr = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr;

        send_in_arp("eth0", ARPOP_InREPLY, request.ar_sha, sin_addr);

    }
    return 0;
}
