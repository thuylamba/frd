#ifndef _tl_IOCTL_H__
#define	_tl_IOCTL_H__
int tl_IOCTL_get_index(const char *ifname, uint32_t *ifindex);
int tl_IOCTL_get_hwaddr(const char *ifname, uint8_t *ifmac);
int tl_IOCTL_get_state(const char *ifname,  int *state);
int tl_IOCTL_set_state(const char *ifname, int enable);
#endif //	_tl_IOCTL_H__
