#ifndef _frd_SNIFF_H__
#define _frd_SNIFF_H__
#define MAX_EVENTS 10
#define TIMEOUT_MS 1000
typedef struct {
	void *handle;
	int timeout;
	uint8_t ether_dhost[ETH_ALEN];
	uint8_t ether_shost[ETH_ALEN];
} frd_sniff_t;

void frd_sniff_init(char *ifname);
void frd_sniff_exit(void);
void frd_sniff_process(void *data);
#endif	//	_frd_SNIFF_H__
