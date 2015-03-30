#ifndef IN_ARP_RAW_H_INCLUDED
#define IN_ARP_RAW_H_INCLUDED

#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <netdb.h>
#include <stdio.h>
#include <netinet/if_ether.h>
#include <net/if_arp.h>
#include <sys/types.h>

#define BUF_SIZE            (65536)
#define MAX_LEN_HARD_ADR    (20)
#define MAX_LEN_PROT_ADR    (20)
#define PROT_ADDR_LEN       (16)

struct arp_req {
    unsigned char ar_sha[ETH_ALEN]; // MAC адрес исходящего устройства
    unsigned char ar_sip[PROT_ADDR_LEN];  // адрес исходящего устройства
    unsigned char ar_dha[ETH_ALEN]; // MAC адрес устройства назначения
    unsigned char ar_dip[PROT_ADDR_LEN];  // адрес устройства назначения
};

struct sockaddr_ll {
               unsigned short sll_family;   /* Always AF_PACKET */
               unsigned short sll_protocol; /* Physical layer protocol */
               int            sll_ifindex;  /* Interface number */
               unsigned short sll_hatype;   /* ARP hardware type */
               unsigned char  sll_pkttype;  /* Packet type */
               unsigned char  sll_halen;    /* Length of address */
               unsigned char  sll_addr[8];  /* Physical layer address */
           };

int send_in_arp (const char* eth_name, unsigned short int arp_type, unsigned char hw_adr_dest[ETH_ALEN], u_int32_t sin_addr);
int recv_in_arp (struct arp_req *request, unsigned short int *arp_type);

#endif // IN_ARP_RAW_H_INCLUDED
