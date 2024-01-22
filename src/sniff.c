#include "common.h"
#include "main.h"
#include "ioctl.h"
#include "sniff.h"
static frd_sniff_t self = {0, };

static void *
sniff_init(char *ifname) 
{
	char errbuf[PCAP_ERRBUF_SIZE];  /* Error string */
	struct bpf_program fp;      /* The compiled filter */
  char filter_exp[1024] = {0, };

	pcap_t *handle = pcap_open_live(ifname, BUFSIZ, 1, self.timeout, errbuf);
	if(handle == NULL)  
	{
		fprintf(stderr, "[%s][%d] errbuf = [%s]\n", __func__, __LINE__, errbuf);
		goto exit_0;
	}
	snprintf(filter_exp, 1024, "(ether[0] & 1 = 1 and (not ether src " MACLSTR ") and "
			" (ether proto 0x%x and ether dst " MACLSTR "))", MAC2STR(self.ether_shost), FRD_P_PROTO, MAC2STR(self.ether_dhost));
	//
	fprintf(stderr, "filter_exp = [%s]\n", filter_exp);
	if (pcap_compile(handle, &fp, filter_exp, 0, 0) == -1) {
		fprintf(stderr, "[%s][%d]\n", __func__, __LINE__);
		goto exit_1;
	}
	if (pcap_setfilter(handle, &fp) == -1) {
		fprintf(stderr, "[%s][%d]\n", __func__, __LINE__);
		goto exit_1;
	}
	return handle;
exit_1:
	pcap_close(handle);
exit_0:
	return NULL;
}

void
frd_sniff_init(char *ifname) {
  self.timeout = TIMEOUT_MS;
	uint8_t _d_mac[] = FRD_DEST_MAC_ADDR;
	memcpy(self.ether_dhost, _d_mac, ETH_ALEN);
	tl_IOCTL_get_hwaddr(ifname, self.ether_shost);
	self.handle = sniff_init(ifname);
}
void 
frd_sniff_exit(void) {
	if(self.handle ) 
	{
		pcap_close(self.handle);
	}
}
void
frd_sniff_send(void *buff, size_t size) {
	struct ether_header *eth_header = (struct ether_header *)malloc(sizeof(struct ether_header) + size);
	memcpy(&eth_header->ether_dhost, self.ether_dhost, ETH_ALEN);
	memcpy(&eth_header->ether_shost, self.ether_shost, ETH_ALEN);
	eth_header->ether_type = htons(FRD_P_PROTO);
	memcpy(eth_header + 1, buff, size);
	int nRet =  pcap_inject(self.handle, (uint8_t *)eth_header, sizeof(*eth_header) + size);
	fprintf(stderr, "nRet = [%d]\n", nRet);
	free(eth_header);
}
void frd_sniff_recv(unsigned char *user, const struct pcap_pkthdr *pkthdr, const unsigned char *packet) {
    // Handle packets here
    printf("Received a packet!\n");
}
void 
frd_sniff_process(void *data) {
	// Get the file descriptor for the pcap handle
	int pcap_fd = pcap_get_selectable_fd(self.handle);
	if (pcap_fd == -1) {
		fprintf(stderr, "Couldn't get file descriptor for pcap handle\n");
		goto done;
	}
	// Create an epoll instance
	int epoll_fd = epoll_create1(0);
	if (epoll_fd == -1) {
		goto done;
	}

	// Add pcap file descriptor to epoll
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = pcap_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, pcap_fd, &ev) == -1) {
		goto done;
	}

	// Main event loop with timeout
	struct epoll_event events[MAX_EVENTS];
	while (frd_main_self()->x_running) {
		int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, TIMEOUT_MS);
		if (num_events == -1) {
			break;
		} else if (num_events == 0) {
			// Timeout occurred, handle timeout logic here
			printf("Timeout occurred\n");
			frd_sniff_send("thanh", 5);
		} else {
			// Events available
			for (int i = 0; i < num_events; i++) {
				if (events[i].data.fd == pcap_fd) {
					// Packet available on pcap file descriptor
					pcap_dispatch(self.handle, 0, frd_sniff_recv, NULL);
				}
			}
		}
	}
	close(epoll_fd);
done:
	frd_main_exit();
}
