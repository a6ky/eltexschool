 /* 
  *                                  frame
  * |-----------|-----------|--------------|-----------------------|------------|
  * |  dst_mac  |  src_mac  |  ether_frame |    data (IP + UDP)    |  checksum  |
  * |-----------|-----------|--------------|-----------------------|------------|
  *
  */

#include "udp_header.h"

uint16_t checksum(uint16_t *buffer, int size)
{
    uint32_t csum = 0;
    while (size > 1) {
        csum += *(buffer++);
        size -= sizeof(uint16_t);
    }

    if (size)
        csum += *(uint16_t *) buffer;

    csum = (csum >> 16) + (csum & 0xffff);
    csum += (csum >> 16);

    return (uint16_t) (~csum);
}

uint16_t udp_cheksum(ip_header_t ip_hdr, udp_header_t udp_hdr, 
                        uint8_t *data, int data_len)
{
    char buf[IP_MAXPACKET];
    char *ptr;
    int checksum_len = 0;
    int i;

    ptr = buf;

    /*
        RFC 768:

        Checksum is the 16-bit one's complement of the one's complement sum of a
        pseudo header of information from the IP header, the UDP header, and the
        data,  padded  with zero octets  at the end (if  necessary)  to  make  a
        multiple of two octets.

        The pseudo  header  conceptually prefixed to the UDP header contains the
        source  address,  the destination  address,  the protocol,  and the  UDP
        length.   This information gives protection against misrouted datagrams.
        This checksum procedure is the same as is used in TCP.

         0      7 8     15 16    23 24    31
         +--------+--------+--------+--------+
         |          source address           |
         +--------+--------+--------+--------+
         |        destination address        |
         +--------+--------+--------+--------+
         |  zero  |protocol|   UDP length    |
         +--------+--------+--------+--------+

                           +

          0      7 8     15 16    23 24    31
         +--------+--------+--------+--------+
         |     Source      |   Destination   |
         |      Port       |      Port       |
         +--------+--------+--------+--------+
         |                 |                 |
         |     Length      |    Checksum     |
         +--------+--------+--------+--------+
         |
         |          data octets ...
         +---------------- ...
                User Datagram Header Format

    */

    memcpy(ptr, &ip_hdr.src_ip, sizeof(ip_hdr.src_ip));
    ptr += sizeof(ip_hdr.src_ip);
    checksum_len += sizeof(ip_hdr.src_ip);

    memcpy(ptr, &ip_hdr.dst_ip, sizeof(ip_hdr.dst_ip));
    ptr += sizeof(ip_hdr.dst_ip);
    checksum_len += sizeof(ip_hdr.dst_ip);

    *ptr = 0; 
    ptr++;
    checksum_len += 1;

    memcpy(ptr, &ip_hdr.protocol, sizeof(ip_hdr.protocol));
    ptr += sizeof(ip_hdr.protocol);
    checksum_len += sizeof(ip_hdr.protocol);

    memcpy(ptr, &udp_hdr.len, sizeof(udp_hdr.len));
    ptr += sizeof(udp_hdr.len);
    checksum_len += sizeof(udp_hdr.len);

    memcpy(ptr, &udp_hdr.src_port, sizeof(udp_hdr.src_port));
    ptr += sizeof(udp_hdr.src_port);
    checksum_len += sizeof(udp_hdr.src_port);

    memcpy(ptr, &udp_hdr.dst_port, sizeof(udp_hdr.dst_port));
    ptr += sizeof(udp_hdr.dst_port);
    checksum_len += sizeof(udp_hdr.dst_port);

    memcpy(ptr, &udp_hdr.len, sizeof(udp_hdr.len));
    ptr += sizeof(udp_hdr.len);
    checksum_len += sizeof(udp_hdr.len);

    *ptr = 0;
    ptr++;
    *ptr = 0;
    ptr++;
    checksum_len += 2;

    memcpy(ptr, data, data_len);
    ptr += data_len;
    checksum_len += data_len;

    for (i = 0; i < data_len % 2; i++, ptr++) {
        *ptr = 0;
        ptr++;
        checksum_len++;
    }

    return checksum((uint16_t *) buf, checksum_len); 
}

int main(int argc, char *argv[])
{
    struct sockaddr_ll  addrll;
    ip_header_t         ip_header;
    udp_header_t        udp_header;
    uint16_t            dgram_len;
    uint16_t            udp_len;
    uint16_t            frame_len;
    int                 rawsock_fd;
    char                message[BUF_SIZE];
    char                frame[FRAME_SIZE];
    char                ether_type[2];
    char                *data;

    u_char src_mac[6] = { SRC_MAC_0, SRC_MAC_1, SRC_MAC_2,
                          SRC_MAC_3, SRC_MAC_4, SRC_MAC_5 };

    u_char dst_mac[6] = { DST_MAC_0, DST_MAC_1, DST_MAC_2,
                          DST_MAC_3, DST_MAC_4, DST_MAC_5 };

    rawsock_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (rawsock_fd == -1)
        handle_error("socket");

    addrll.sll_family   = AF_INET;
    addrll.sll_protocol = htons(ETH_P_IP);
    addrll.sll_ifindex  = if_nametoindex(IF_NAME);
    addrll.sll_hatype   = ARPHRD_ETHER;
    addrll.sll_pkttype  = PACKET_HOST;
    addrll.sll_halen    = ETH_ALEN;

    addrll.sll_addr[0] = src_mac[0];
    addrll.sll_addr[1] = src_mac[1];
    addrll.sll_addr[2] = src_mac[2];
    addrll.sll_addr[3] = src_mac[3];
    addrll.sll_addr[4] = src_mac[4];
    addrll.sll_addr[5] = src_mac[5];

    addrll.sll_addr[6] = 0x00;
    addrll.sll_addr[7] = 0x00;

    ether_type[0] = ETH_P_IP / 256;
    ether_type[1] = ETH_P_IP % 256;

    memset(frame, 0, FRAME_SIZE);

    // устанавливаем заголовок Ethernet фрейма
    memcpy(frame, dst_mac, ETH_ALEN);
    memcpy(frame + ETH_ALEN, src_mac, ETH_ALEN);
    memcpy(frame + ETH_ALEN + ETH_ALEN, ether_type, sizeof(ether_type));

    memset(message, 0, BUF_SIZE);
    strcpy(message, "my frame");

    // общая длинна пакета
    dgram_len = sizeof(ip_header_t) + sizeof(udp_header_t) + strlen(message);
    udp_len = sizeof(udp_header_t) + strlen(message);
    frame_len = dgram_len + ETH_HEADER_LEN;

    // формируем IP заголовок
    ip_header.version   = 4;
    ip_header.ds        = 0;
    ip_header.len       = htons(dgram_len);
    ip_header.offset    = 0;
    ip_header.ttl       = 255;
    ip_header.protocol  = IPPROTO_UDP;
    ip_header.ihl       = 5;
    ip_header.checksum  = 0;
    inet_pton(AF_INET, CLIENT_ADDR, &(ip_header.src_ip));
    inet_pton(AF_INET, SERVER_ADDR, &(ip_header.dst_ip));

    // вычисляем чек-сумму IP заголовка
    ip_header.checksum = checksum((uint16_t *) &ip_header, ip_header.ihl * 4);

    // формируем UDP заголовок
    udp_header.src_port = htons(CLIENT_PORT);
    udp_header.dst_port = htons(SERVER_PORT);
    udp_header.len      = htons(udp_len);
    udp_header.checksum = 0;

    // вычисляем чек-сумму UDP заголовка
    udp_header.checksum = udp_cheksum(ip_header, udp_header, message, strlen(message));

    data = frame + ETH_HEADER_LEN;

    memcpy(data, &ip_header, sizeof(ip_header_t));
    memcpy(data + sizeof(ip_header_t), &udp_header, sizeof(udp_header_t));
    memcpy(data + sizeof(ip_header_t) + sizeof(udp_header_t), message, strlen(message));

    if (sendto(rawsock_fd, frame, frame_len, 0, (struct sockaddr *) &addrll, 
                sizeof(struct sockaddr_ll)) == -1)
            handle_error("sendto");

    if (close(rawsock_fd) == -1)
        handle_error("close");

    return 0;
}