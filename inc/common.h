#ifndef __frd_COMMMON_H__
#define __frd_COMMMON_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <fcntl.h>
#include <signal.h>
#include <arpa/inet.h>
#include <net/route.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include <errno.h>
#include <pthread.h>
#include <pcap.h>
#include <sys/epoll.h>
//
#ifndef TRUE
#define TRUE (1)
#endif  // TRUE
#ifndef FALSE
#define FALSE (0)
#endif  // FALSE

#define FRD_DEST_MAC_ADDR {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}
#define FRD_P_PROTO	0x894C
#define MACLSTR	"%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#endif	//	__frd_COMMMON_H__
