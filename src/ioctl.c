#include "common.h"
#include "main.h"
static int
tl_ioctl(u_long req, caddr_t data) {
	int sock;
	int ret;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0) {
		return -1;
	}
	if((ret = ioctl(sock, req, data)) < 0) {
		close(sock);
		return -1;
	}
	close(sock);
	return 0;
}
int
tl_IOCTL_get_index(const char *ifname, uint32_t *ifindex) {
	struct ifreq ifreq;
        int ret;
        strncpy (ifreq.ifr_name, ifname, IFNAMSIZ);
        ifreq.ifr_addr.sa_family = AF_INET;
        ret = tl_ioctl(SIOCGIFINDEX, (caddr_t) &ifreq);
        if(ret < 0) {
                return -1;
        }
        *ifindex = ifreq.ifr_ifindex;
        return 0;
}

int
tl_IOCTL_get_hwaddr(const char *ifname, uint8_t *ifmac) {
	struct ifreq ifreq;
	int ret;
	strncpy (ifreq.ifr_name, ifname, IFNAMSIZ);
	ifreq.ifr_addr.sa_family = AF_INET;
	ret = tl_ioctl(SIOCGIFHWADDR, (caddr_t) &ifreq);
	if(ret < 0) {
		return -1;
	}
	memcpy(ifmac, ifreq.ifr_hwaddr.sa_data, ETH_ALEN);
	return 0;
}

// up/down
int
tl_IOCTL_get_state(const char *ifname,  int *state) {
        struct ifreq ifreq;
        int ret;
        strncpy (ifreq.ifr_name, ifname, IFNAMSIZ);
        ifreq.ifr_addr.sa_family = AF_INET;
	ret = tl_ioctl(SIOCGIFFLAGS, (caddr_t) &ifreq);
	if(ret < 0) {
		return -1;
	}
	if(ifreq.ifr_flags & IFF_UP ) {
		*state = TRUE;
	} else {
		*state = FALSE;
	}
	return 0;
}

int
tl_IOCTL_set_state(const char *ifname, int enable) {
	struct ifreq ifreq;
	int ret;
	strncpy (ifreq.ifr_name, ifname, IFNAMSIZ);
	ifreq.ifr_addr.sa_family = AF_INET;
	ret = tl_ioctl(SIOCGIFFLAGS, (caddr_t) &ifreq);
	if(ret < 0) {
		return -1;
	}
	if(enable) {
        	ifreq.ifr_flags |= ( IFF_UP | IFF_PROMISC | IFF_RUNNING );
	} else {
		ifreq.ifr_flags &= ~(IFF_PROMISC | IFF_RUNNING | IFF_UP);
	}
	ret = tl_ioctl(SIOCSIFFLAGS, (caddr_t) &ifreq);
	if(ret < 0) {
                return -1;
        }
	return 0;
}
