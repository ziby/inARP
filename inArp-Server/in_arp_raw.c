#include "in_arp_raw.h"

int send_in_arp (const char* eth_name, unsigned short int arp_type, unsigned char hw_adr_dest[ETH_ALEN], u_int32_t sin_addr)
{

    int sock = socket(AF_INET, SOCK_RAW, ETH_P_ARP);

    char buffer[BUF_SIZE]; //
    memset (buffer, 0, sizeof(buffer));

    struct ifreq hw_src; //
    struct ethhdr *eth_hdr;

    strcpy (hw_src.ifr_name, eth_name);
    ioctl (sock, SIOCGIFHWADDR, &hw_src);

    eth_hdr = (struct ethhdr *) buffer;
    eth_hdr->h_proto = htons(0x0806);
    int i = 0;
    for (i = 0; i < ETH_ALEN; i++)
    {
        eth_hdr->h_source[i] = hw_src.ifr_ifru.ifru_hwaddr.sa_data[i];
        eth_hdr->h_dest[i] = hw_adr_dest[i];
    }

    struct arphdr *arp_hdr;

    arp_hdr = (struct arphdr *)(buffer + sizeof(struct ethhdr));

    arp_hdr->ar_hrd = htons (ARPHRD_ETHER);
    arp_hdr->ar_pro = htons (0x0806);
    arp_hdr->ar_hln = ETH_ALEN;
    arp_hdr->ar_pln = PROT_ADDR_LEN;
    arp_hdr->ar_op  = htons (arp_type);


    struct arp_req *arp_req_ob;  // указатель на ARP запрос.
    // располагаем ARP запрос за заголовком ARP.
    arp_req_ob = (struct arp_req *) (buffer + sizeof(struct ethhdr) + sizeof(struct arphdr));

    // копируем MAC адрес исходящего устройства.
    memcpy(&arp_req_ob->ar_sha, hw_src.ifr_ifru.ifru_hwaddr.sa_data, ETH_ALEN);
    if (arp_type == ARPOP_InREPLY) {
        memcpy(&arp_req_ob->ar_sip, &sin_addr, sizeof(u_int32_t));
    }
    else {
        memset(&arp_req_ob->ar_sip, 0, PROT_ADDR_LEN);
    }

    memcpy(&arp_req_ob->ar_dha, hw_adr_dest, ETH_ALEN);
    memset(&arp_req_ob->ar_dip, 0, PROT_ADDR_LEN);

    struct sockaddr_ll socket_address;

    socket_address.sll_family = AF_INET;
    socket_address.sll_halen = ETH_ALEN;

    for (i = 0 ; i < ETH_ALEN; i ++)
    {
        socket_address.sll_addr[i] = hw_adr_dest[i];
    }

    if (sendto  (sock, buffer, sizeof(struct ethhdr) + sizeof(struct arphdr) + sizeof(struct arp_req),
            0, &socket_address, sizeof(socket_address)) < 0) perror("sendto");

    close(sock);
    return 0;
}

int recv_in_arp (struct arp_req *request, unsigned short int *req_type)
{
    char read_sock_buf[BUF_SIZE];
    int i = 0;
    int sock = socket(AF_INET, SOCK_RAW, ETH_P_ARP);

    while(1)
    {
        int size_package = recv(sock, read_sock_buf, sizeof(read_sock_buf), 0);

        if (size_package < 0) {
            return 0;
        }

        struct ethhdr *eth_hdr = (struct ethhdr *) read_sock_buf;
        if (eth_hdr->h_proto != 0x0806 ) continue;
        break;
    }

    struct arphdr *arp_hdr = (struct arphdr *) (read_sock_buf + sizeof(struct ethhdr));
    *req_type = arp_hdr->ar_op;
    struct arp_req *arp_req_ob = (struct arp_req *) (read_sock_buf + sizeof(struct ethhdr) + sizeof(struct arphdr));
    memcpy(request, arp_req_ob, sizeof(struct arphdr));

    close (sock);

    return 0;
}
